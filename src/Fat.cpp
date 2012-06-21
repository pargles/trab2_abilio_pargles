#include <vector>

#include "../inc/Fat.h"

/**
	@brief Inicializa os atributos do disco.
	@author pargles and abilio
*/
Fat::Fat(string enderecoArquivo) {
    this->bytesPorSetor = 512;//TODO, obter do arquivo essa informacao
    this->nomeArquivo = enderecoArquivo;
    this->posicaoFat1 = 0x0200;//TODO, obter do arquivo essa informacao
    this->posicaoFat2 = 0x0800;//TODO, obter do arquivo essa informacao
    this->Reader.open(nomeArquivo.c_str(),ios::in|ios::binary);//Open in binary mode or read.
    if(this->Reader == NULL)
    {
        cout << "nao foi possivel abrir o arquivo: " << nomeArquivo;
        cout << "\n voce precisa definir o endereco do sistema de arquivos atraves de -in nome_arquivo \n";
        exit(1);
    }

}

/**
	@brief seta o endereco do disco.
	@param string contendo o endereco do disco
	@author pargles and abilio
*/
void Fat::setNomeArquivo(string enderecoDisco)
{
    this->nomeArquivo = enderecoDisco;
}

/**
	@brief retorna o endereco do disco
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

/**
        @brief fecha a stream de dados que acessava o disco
 *      http://www.cplusplus.com/doc/tutorial/files/
        @author pargles and abilio
 */
void Fat::listarClusterInicial() {
    char *byte = new char[1];
    for (int i = 0; i < bytesPorSetor; i++) {
        Reader.read(byte, 1);//le byte por byte do arquivo 1 e um byte
        cout << byte;
    }
}

/*metodo apenas para debug
 */
void Fat::listarCluster(int numeroCluster)
{
    char *byte = new char[1];
    Reader.seekg(numeroCluster*bytesPorSetor, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    for (register int i = 0; i < bytesPorSetor; i++) {
        Reader.read(byte, 1);//le byte por byte do arquivo 1 e um byte
        cout << byte;
    }

}

/* do arquivo
 */
void Fat::listarInformacoesCabecalho()
{
    char *vetor = new char[1];
    unsigned char temp1;
    this->bytesPorSetor = lerDoisBytes(11);
    Reader.seekg(13, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    Reader.read(vetor, 1);
    temp1 = vetor[0];
    this->setoresPorCluster = temp1;
    cout << "Bytes per sector: "<< bytesPorSetor<<endl;
    cout << "Sectors per cluster: "<< setoresPorCluster<<endl;
    Reader.seekg(54, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    Reader.read(vetor, 7);
    cout << "File system type level: "<< vetor<<endl;

}

unsigned short int Fat::lerDoisBytes(int posicaoInicial)
{
    char *byte = new char[1];
    unsigned char temp1;unsigned char temp2;
    unsigned short int palavra;
    Reader.seekg(posicaoInicial, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    Reader.read(byte, 1); temp1 = byte[0];
    Reader.read(byte, 1); temp2 = byte[0];
    palavra = temp2;
    palavra = palavra << 8;//abre espaco para concatenar a proxima palavra (8 bits)
    palavra = palavra | temp1;
    return palavra;
}

/* metodo temporario para listar a tabela FAT1
 * posteriormente mudar para metodo que le uma tabela
 * e armazena suas entradas em um vetor de inteiros de 12 bits
 */
void Fat::listarFAT()
{
    char *byte = new char[1];
    Reader.seekg(0x0200, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    for (int i = 0; i < bytesPorSetor*10; i++) {//9 pois 1-9 	0x0200-0x13ff 	File Allocation Table (primary)
        Reader.read(byte, 1);//le byte por byte do arquivo 1 e um byte
        cout << byte;
    }
}

/**
*	@brief chama duas vezes o metodo para inserir em um vetor
 *      as entradas da fat, primeiro preenche fat1 e depois a fat2;
 *      metodo testado e funcionado
*	@author pargles and abilio
*/
void Fat::inserirEntradasFat()
{
    this->vetorDeEntradasFat1 = this->adicionarEntradas(this->posicaoFat1,this->vetorDeEntradasFat1);
    this->vetorDeEntradasFat2 = this->adicionarEntradas(this->posicaoFat2,this->vetorDeEntradasFat2);
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
    unsigned short int entrada12bits;//512bytes*10 = 0x1400, inicio da segunda FAT
    register int i;
    for(i =0;i<bytesPorSetor*11;i+=3)//le tres bytes por vez, ou seja, 24 bits por vez, ou seja, duas entradas de 12 bits pro vez
    {
        entrada12bits = 0;
        quatrobitsA=0;
        quatrobitsB=0;
        Reader.read(palavras, 3);//le 3 bytes por vez, colocando cada  byte em cada posicao do vetor palavras
        a = palavras[0];
        b = palavras[1];
        c = palavras[2];
        quatrobitsA = palavras[1]&240;//240= 1111 0000 o & vai pegar so os ultimos 4 bits da palavra do meio
        quatrobitsA= quatrobitsA>>4;//coloca os bits a direita para concatenas com entrada12bits
        quatrobitsB = palavras[1]&15;//15 = 0000 1111 o & vai pegar so os primeiros 4 bits da palavra do meio
        entrada12bits = quatrobitsB;
        entrada12bits = entrada12bits << 8;
        entrada12bits = entrada12bits | palavras[0];
        entrada12bits = entrada12bits << 4; entrada12bits = entrada12bits >> 4;//zera os quatro bits mais a esquerda
        vetor.push_back(entrada12bits);
        entrada12bits = 0;//zera tudo para formar a segunda palavra de 12 bits
        entrada12bits = palavras[2];
        entrada12bits = entrada12bits << 4;
        entrada12bits = entrada12bits | quatrobitsA;
        entrada12bits = entrada12bits << 4; entrada12bits = entrada12bits >> 4;//zera os quatro bits mais a esquerda
        vetor.push_back(entrada12bits);//insere no vetor a segunda entrada gerada
    }
    return vetor;
}

void Fat::listarEntradasFAT()
{
    int byte =0x200;
    cout << "ENTRADA  |     FAT1       | FAT2        |"<<endl;
    for(register int i = 0 ;i<vetorDeEntradasFat1.size();i++)
    {
        byte+=3;
        cout << i << " :            " << vetorDeEntradasFat1[i]<< "          ";
        cout << vetorDeEntradasFat2[i]<<endl;
        //getchar();
        //cout<< " hex: " << hex << vetorDeEntradas[i]<<endl;
    }
    cout<<"terminou no endereco: "<<hex<<byte<<endl;
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
            cout << "DIF <" << i << ">:<" << vetorDeEntradasFat1[i] << ">,<" << vetorDeEntradasFat2[i]<< ">\n";
        }
    }
}

/**
*	@brief metodo que compara os dois vetores de entradas das fats
*       e indica as diferencas caso existam
*	@author pargles and abilio
*/
void Fat::listarBlocosLivres()
{
    cout << "LIVRE ";
    for(register int i = 0 ;i<vetorDeEntradasFat1.size();i++)
    {        
        if(vetorDeEntradasFat1[i]== 0)//se o indice contem 0 e porque a FAT nao esta apontando
        {
            cout << i << ",";
        }
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

}

