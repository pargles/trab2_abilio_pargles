#include <vector>

#include "../inc/Fat.h"

/**
	@brief Inicializa os atributos do disco.
	@author pargles and abilio
*/
Fat::Fat(string enderecoArquivo) {
    this->bytesPorSetor = 512;//default setor de 512 bytes
    this->nomeArquivo = enderecoArquivo;
    //this->Reader.open() = fopen(nomeArquivo.c_str(), "rb");//rb = read bytes
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

/*aletar esse metodo para setar todas as informacoes da fat
 * chamar esse metodo no construtor depois d ter recebido o nome
 * do arquivo
 */
void Fat::listarInformacoesCabecalho()
{
    char *byte = new char[1];
    Reader.seekg(11, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    Reader.read(byte, 2);
    cout << "Bytes per sector: "<< byte<<endl;
    Reader.seekg(13, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    Reader.read(byte, 1);
    cout << "Sectors per cluster: "<< byte<<endl;
    Reader.seekg(16, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    Reader.read(byte, 1);
    cout << "Number of FATs: "<< byte<<endl;
    Reader.seekg(54, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    Reader.read(byte, 7);
    cout << "File system type level: "<< byte<<endl;

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

void Fat::adicionarEntradas()
{
    char *palavras = new char[3];
    unsigned char quatrobitsA;
    unsigned char quatrobitsB;
    Reader.seekg(0x0200, ios::beg);//ios::beg e para dar o seek a partir do inicio do arquivo
    unsigned short int entrada12bits;//512bytes*10 = 0x1400, inicio da segunda FAT
    register int i;
    for(i =0;i<bytesPorSetor*11;i+=3)//le tres bytes por vez, ou seja, 24 bits por vez, ou seja, duas entradas de 12 bits pro vez
    {
        entrada12bits = 0;
        quatrobitsA=0;
        quatrobitsB=0;
        Reader.read(palavras, 3);//le 3 bytes por vez, colocando cada  byte em cada posicao do vetor palavras
        quatrobitsA = palavras[1]&240;//240= 1111 0000 o & vai pegar so os ultimos 4 bits da palavra do meio
        quatrobitsA= quatrobitsA>>4;//coloca os bits a direita para concatenas com entrada12bits
        quatrobitsB = palavras[1]&15;//15 = 0000 1111 o & vai pegar so os primeiros 4 bits da palavra do meio

        entrada12bits = quatrobitsB;
        entrada12bits = entrada12bits << 8;
        entrada12bits = entrada12bits | palavras[0];

        //entrada12bits = palavras[0];//recebe os 8 bits da primeira palavra, fica faltando 4 bits
        //entrada12bits = entrada12bits << 4;//abre espaco para colocar os quatro bits que faltam
        //entrada12bits = entrada12bits | quatrobitsA;//o | faz a concatenacao entre as duas palavras, formando os 12 bits da entrada
        entrada12bits = entrada12bits << 4; entrada12bits = entrada12bits >> 4;//zera os quatro bits mais a esquerda
        vetorDeEntradas.push_back(entrada12bits);
        entrada12bits = 0;//zera tudo para formar a segunda palavra de 12 bits
        
        entrada12bits = palavras[2];
        entrada12bits = entrada12bits << 4;
        entrada12bits = entrada12bits | quatrobitsA;

        //entrada12bits = entrada12bits | quatrobitsB;//pega os 4 bits que restaram da palavra do meio e concatena
        //entrada12bits = entrada12bits << 8;//abre espaco para inserir a ultima palavra de 8 bits
        //entrada12bits = entrada12bits | palavras[2]; // concatena os 4 bits com os 8 bits formando os 12 bits da proxima entrada
        entrada12bits = entrada12bits << 4; entrada12bits = entrada12bits >> 4;//zera os quatro bits mais a esquerda
        vetorDeEntradas.push_back(entrada12bits);//insere no vetor a segunda entrada gerada
    }
}

void Fat::listarEntradasFAT()
{
    for(register int i = 0 ;i<vetorDeEntradas.size();i++)
    {
        cout << i << " : " << vetorDeEntradas[i]<<endl;
        getchar();
        //cout<< " hex: " << hex << vetorDeEntradas[i]<<endl;
    }
}

