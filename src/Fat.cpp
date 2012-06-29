#include <vector>

#include "../inc/Fat.h"

/**
	@brief Inicializa os atributos do disco.
 *      Modos de abertura               Descrição
        ios::in            Abre para leitura (default de ifstream).
        ios::out           Abre para gravação (default de ofstream),
        ios::ate           Abre e posiciona no final do arquivo.
        ios::app           Grava a partir do fim do arquivo
        ios::trunc         Abre e apaga todo o conteúdo do arquivo
        ios::nocreate      Erro de abertura se o arquivo não existe
        ios::noreplace     Erro de abertura se o arquivo existir
        ios::binary        Abre em binário (default é texto)
	@author pargles and abilio
*/
Fat::Fat(string enderecoArquivo) {
    this->nomeArquivo = enderecoArquivo;
    this->Reader.open(nomeArquivo.c_str(),ios::in|ios::binary);//Open in binary mode or read.
    if(this->Reader == NULL)
    {
        cerr << "nao foi possivel abrir o arquivo: " << nomeArquivo;
        cerr << "\n voce precisa definir o endereco do Sistema de Arquivos atraves de -in nome_arquivo \n";
        exit(1);
    }
    inicializarInformacoesCabecalho();//inicializa todas as informacoes da fat, como tamanho do setor, tamanho bloco etc.

}


/**
        @brief metodo que inicializa todos os parametros do sistema de arquivos
 *      como por exemplo bytes por setor, setores por bloco etc.
 *      http://www.cplusplus.com/doc/tutorial/files/
 *      @author pargles and abilio
 */
void Fat::inicializarInformacoesCabecalho()
{
    bytesPorSetor = lerBytes(11,2);
    setoresPorBloco = lerBytes(13,1);
    quantidadeSetoresCabecalho = lerBytes(14,2);
    numeroDeFATs = lerBytes(16,1);
    numeroMaximoArquivos = lerBytes(17,2);
    quantidadeSetoresFat = lerBytes(22,2);
    tamanhoCadaBloco = bytesPorSetor*setoresPorBloco;
    posicaoFat1 = quantidadeSetoresCabecalho*bytesPorSetor;//depois do cabecalho inicia a Fat1
    posicaoFat2 = posicaoFat1 + quantidadeSetoresFat*bytesPorSetor;//depois da Fat1 inicia a Fat2
    posicaoDiretorioArquivos = posicaoFat2+quantidadeSetoresFat*bytesPorSetor;//diretorio de arquivos inicia depois da fat2
    posicaoInicialDados = posicaoDiretorioArquivos + numeroMaximoArquivos*32;//cada entrada da tabela de diretorios tem 32 bytes
}

void Fat::listarInformacoesCabecalho()
{
    cout << "Numero maximo de arquivos: "<<dec<<numeroMaximoArquivos<<endl;
    cout << "Bytes por setor: "<<dec<< bytesPorSetor<<endl;
    cout << "Setores por bloco: "<<dec<< setoresPorBloco<<endl;
    cout << "Setores para o cabecalho: "<<dec<<quantidadeSetoresCabecalho<<endl;
    cout << "Numero de FATs: "<<dec<< numeroDeFATs<<endl;
    cout << "Quantidade de setores de cada FAT: "<<dec<< quantidadeSetoresFat<<endl;
    cout << "Posicao Fat1 (hexadecimal): ";
    cout << hex << posicaoFat1 << endl;
    cout << "Posicao Fat2 (hexadecimal): ";
    cout << hex << posicaoFat2 << endl;
    cout << "Posicao Diretorio de Arquivos (hexadecimal): ";
    cout << hex << posicaoDiretorioArquivos << endl;
    cout << "Posicao Inicial Dados (hexadecimal): ";
    cout << hex << posicaoInicialDados << endl;
}

/**
        @brief metodo que lista um bloco passado por parametro
 *      http://www.cplusplus.com/doc/tutorial/files/
 *      @param numero do bloco a ser listado
 *      @author pargles and abilio
 */
void Fat::listarBloco(int numeroBloco)
{
    char *byte = new char[1];
    unsigned char temp;
    if(numeroBloco<=1){cerr<<"Os blocos 0 e 1 nao são enderecaveis";exit(1);}
    Reader.seekg((numeroBloco-2)*tamanhoCadaBloco + posicaoInicialDados, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    for (register int i = 0; i < tamanhoCadaBloco; i++) {
        Reader.read(byte, 1);//le byte por byte do arquivo 1 e um byte
        temp = byte[0];
        cout <<hex<<temp;
    }

}

/**
*	@brief metodo que funciona apenas para ler ate dois bytes
 *      recebe a posicao inicial da leitura e retorna o valor lido
 *      caso sejam necessarios ler dois bytes ele faz a mascara
 *      para gerar o valor corretamente
 *      as entradas da fat, primeiro preenche fat1 e depois a fat2;
 *      metodo testado e funcionado
 *      @param posicao onde se encontra o primeiro byte a ser lido
 *      e quantidade de bytes a serem lidos ( 1 ou 2)
 *      @return os bytes lidos em um short int 
*	@author pargles and abilio
*/
unsigned short int Fat::lerBytes(int posicaoInicial, int quantosBytes)
{
    char *byte = new char[1];
    unsigned char temp1;unsigned char temp2;
    unsigned short int palavra;
    Reader.seekg(posicaoInicial, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    Reader.read(byte, 1); temp1 = byte[0];
    if(quantosBytes==1){return temp1;}
    Reader.read(byte, 1); temp2 = byte[0];
    palavra = temp2;
    palavra = palavra << 8;//abre espaco para concatenar a proxima palavra (8 bits)
    palavra = palavra | temp1;
    return palavra;
}

/**
*	@brief chama duas vezes o metodo para inserir em um vetor
 *      as entradas da fat, primeiro preenche fat1 e depois a fat2;
 *      metodo testado e funcionado
*	@author pargles and abilio
*/
void Fat::inserirEntradasFat()
{
    vetorDeEntradasFat1 = adicionarEntradas(posicaoFat1,vetorDeEntradasFat1);
    if(numeroDeFATs == 2){vetorDeEntradasFat2 = adicionarEntradas(posicaoFat2,vetorDeEntradasFat2);}
}

/**
*	@brief adiciona todas as entradas da fat em um vetor
 *      metodo testado e funcionado
*	@param posicao onde inicia a fat e o vetor para inserir as entradas
*	@author pargles and abilio
*/
 vector <unsigned short int> Fat::adicionarEntradas(int posicaoInicialFat, vector <unsigned short int> vetor)
{
    char *palavras = new char[3];
    unsigned char quatrobitsA;
    unsigned char quatrobitsB; unsigned char a; unsigned  char b; unsigned char c;
    Reader.seekg(posicaoInicialFat, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    unsigned short int entrada12bits;
    register int i;
    for(i =0;i<bytesPorSetor*quantidadeSetoresFat;i+=3)//le tres bytes por vez, ou seja, 24 bits por vez, ou seja, duas entradas de 12 bits pro vez
    {
        entrada12bits = 0;
        quatrobitsA=0;
        quatrobitsB=0;
        Reader.read(palavras, 3);//le 3 bytes por vez, colocando cada  byte em cada posicao do vetor palavras
        quatrobitsA = palavras[1]&240;//240= 1111 0000 o & vai pegar so os ultimos 4 bits da palavra do meio
        quatrobitsA= quatrobitsA>>4;//coloca os bits a direita para concatenas com entrada12bits
        quatrobitsB = palavras[1]&15;//15 = 0000 1111 o & vai pegar so os primeiros 4 bits da palavra do meio
        entrada12bits = quatrobitsB;
        entrada12bits = entrada12bits << 8;//abre espaco para o proximo byte (oito bits)
        entrada12bits = entrada12bits | palavras[0];
        entrada12bits = entrada12bits << 4; entrada12bits = entrada12bits >> 4;//zera os quatro bits mais a esquerda
        vetor.push_back(entrada12bits);
        entrada12bits = 0;//zera tudo para formar a segunda palavra de 12 bits
        entrada12bits = palavras[2];
        entrada12bits = entrada12bits << 4;//abre espaco para os 4 bits restantes, necessarios para formar a entrada de 12 bits
        entrada12bits = entrada12bits | quatrobitsA;
        entrada12bits = entrada12bits << 4; entrada12bits = entrada12bits >> 4;//zera os quatro bits mais a esquerda
        vetor.push_back(entrada12bits);//insere no vetor a segunda entrada gerada
    }
    return vetor;
}


/**
*	@brief metodo que lista todas as entradas da FAT1 e FAT2
*	@author pargles and abilio
*/
void Fat::listarEntradasFAT()
{
    cout << "ENTRADA  |     FAT1       | FAT2        |"<<endl;
    for(register int i = 0 ;i<this->vetorDeEntradasFat1.size();i++)
    {
        cout << i <<dec<< " :            " << vetorDeEntradasFat1[i]<< "          ";
        cout << vetorDeEntradasFat2[i]<<endl;
    }
}

/**
*	@brief metodo que compara os dois vetores de entradas das fats
*       e indica as diferencas caso existam
*	@author pargles and abilio
*/
void Fat::diferenciarFATs()
{
    for(register int i = 0 ;i<vetorDeEntradasFat1.size();i++)
    {
        if(vetorDeEntradasFat1[i]!=vetorDeEntradasFat2[i])
        {
            cout << "DIF <" <<dec <<i <<dec << ">:<" << vetorDeEntradasFat1[i] << ">,<" << vetorDeEntradasFat2[i]<< ">\n";
        }
    }
}

/**
*	@brief metodo que percorre as entradas da FAT
*       informando os bloco livres (entrada da FAT = 0)
*	@author pargles and abilio
*/
void Fat::listarBlocosLivres()
{
    bool temVirgula = false;
    cout << "LIVRE ";
    for(register int i = 0 ;i<vetorDeEntradasFat1.size();i++)
    {        
        if(vetorDeEntradasFat1[i]== 0)//se o indice contem 0 e porque a FAT nao esta apontando
        {
            cout << i <<dec<< ",";
            temVirgula = true;
        }
    }
    if(temVirgula)//testa se ja colocou alguma virgula na saida
    {
        cout<<'\b'<<" ";//tira a virgula da saida, apenas para ficar como a definicao do trabalho
    }
    cout << endl;
}


/**
*	@brief metodo que percorre as entradas da FAT e informa quais blocos
*       estao livres mas contem dados (quando a entrada da FAT tiver 0 o bloco esta livre
 *      mas pode conter dados de um arquivo anterior)
*	@author pargles and abilio
*/
void Fat::listarBlocosLivresComDados()
{
    cout << "REMOVIDOS ";
    bool temVirgula=false;
    for(register int i = 0 ;i<vetorDeEntradasFat1.size();i++)
    {
        if(vetorDeEntradasFat1[i]== 0)//se o indice contem valor igual a 0 e porque a FAT nao esta apontando para arquivo
        {
            if(eBlocoComDados(i))//mas o bloco vazio pode ter sido apagado e os dados continuam la
            {
                cout << i <<dec<< ",";//se o bloco contem dados e pq o arquivo foi excluido e a FAT nao aponta mais
                temVirgula =true;
            }
        }
    }
    if(temVirgula)//testa se ja colocou alguma virgula na saida
    {
        cout<<'\b'<<" ";//tira a virgula da saida, apenas para ficar como a definicao do trabalho
    }
    cout << endl;
}


/**
*	@brief vai percorrendo um bloco passado por parametro ate achar
 *      algum char, se nao encontrar nada o bloco nao contem dados e retorna true
 *      @param numero do bloco a ser verificado se contem ou nao dados
 *      @return true se o bloco contem dados, false c.c.
*	@author pargles and abilio
*/
bool Fat::eBlocoComDados(int numeroBloco)
{
    char *byte = new char[1];
    unsigned char temp;
    if(numeroBloco<=1){cerr<<"Os blocos 0 e 1 nao são enderecaveis";exit(1);}
    Reader.seekg((numeroBloco-2)*tamanhoCadaBloco + posicaoInicialDados, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    for (register int i = 0; i < tamanhoCadaBloco; i++) {
        Reader.read(byte, 1);//le byte por byte do arquivo 1 e um byte
        temp = byte[0];
        if(temp!=0){return true;}//tem algum dado
    }
}

void Fat::imprimirArquivoCompleto(int bloco)
{
    int blocoAtual = bloco;
    while(blocoAtual != 0xfff)//0xfff = 4095, termino do arquivo
    {
        listarBloco(blocoAtual);
        blocoAtual = vetorDeEntradasFat1.at(blocoAtual);
    }
    
}

// METODO AINDA NAO VALIDADO. //TODO, verificar o correto funcionamento
void Fat::listarTabelaDiretorios()
{
	char *palavras = new char[64];//cada entrada do diretorio contem 64 bytes
	    unsigned short int endereco;
	    unsigned char deletado = 0xe5;
	    register int i;//contador do laco for
	    Reader.seekg(this->posicaoDiretorioArquivos, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
	    cout << "nº"<<"  Nome   "<<"      Bloco inicial"<<endl;
	    for(i =0;i<numeroMaximoArquivos/2;i++)
	    {
	        Reader.read(palavras, 64);//le 3 bytes por vez, colocando cada  byte em cada posicao do vetor palavras
	        if(!palavras[0]==0)//entrada esta vazia, nao precisa printar
	        {
	            cout <<dec<< i+1;
	            if(palavras[0]&146)//0xe5 = 229 , arquivo deletado, mas ainda esta com as informacoes no bloco
	            {
	                cout << " deletado ";
	            }
	            else
	            {
	                cout << " "<<palavras[32] << palavras[33] << palavras[34]<< palavras[35]<< palavras[36]<<palavras[37];
	                cout << palavras[38] << palavras[39] <<"."<< palavras[40]<< palavras[41]<< palavras[42];
	            }
	            endereco = palavras[59];
	            endereco = endereco << 8;//abre espaco para concatenar a proxima palavra (8 bits)
	            endereco = endereco | palavras[58];
	            cout <<"             "<< endereco<<endl;
	        }
	    }

}

/**
*	@brief metodo que insere erros na FAT 2, apenas para testar as difenreças
 *      entre a FAT 1 e a Fat 2
 *      Função                 Descrição
*       seekg( )-Movimenta a posição atual de leitura (get)
*       seekp( )-Movimenta a posição atual de gravação (put)
*       tellg( )-Retorna a posição atual de leitura (em bytes), a partir do início do arquivo
*       tellp( )-Retorna a posição atual de gravação (em bytes), a partir do início do arquivo

 *      http://www.cplusplus.com/doc/tutorial/files/
*	@author pargles and abilio
*/
void Fat::fliparBitsFAT2()
{
    fstream temp;//arquivo em c++
    temp.open(nomeArquivo.c_str(),  ios::out | ios::in|ios::binary);//Open in binary mode or read.
    if (!temp.is_open()) { cerr << "Problema em abrir arquivo para flipar Fat2"; }
    int posicaoInicialFat2 = posicaoFat2;
    temp.seekp(posicaoInicialFat2+15, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    char *erro = new char[1];
    erro[0] = 'a';
    temp.write(erro,1);//1 byte
    temp.seekp(posicaoInicialFat2+23, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    erro[0] = '1';
    temp.write(erro,1);//1 byte
    temp.seekp(posicaoInicialFat2+45, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    erro[0] = 'e';
    temp.write(erro,1);//1 byte
    temp.seekp(posicaoInicialFat2+204, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    erro[0] = '5';
    temp.write(erro,1);//1 byte
    temp.seekp(posicaoInicialFat2+604, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    erro[0] = '9';
    temp.write(erro,1);//1 byte
    temp.close();

}


/**
	@brief seta o endereco do sistema de arquivos.
	@param string contendo o endereco do disco
	@author pargles and abilio
*/
void Fat::setNomeArquivo(string enderecoDisco)
{
    this->nomeArquivo = enderecoDisco;
}

/**
	@brief retorna o endereco do sistema de arquivos
	@return string contendo o endereco do disco
	@author pargles and abilio
*/
string Fat::getNomeArquivo()
{
    return this->nomeArquivo;
}

/**
	@brief fecha a stream de dados que acessava o disco
	@author pargles and abilio
*/
void Fat::fecharReader()
{
    this->Reader.close();
}
