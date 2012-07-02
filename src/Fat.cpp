#include <vector>
#include <math.h>//para a funcao fmode(retorna o modulo de um float)
#include "../inc/Fat.h"

/**
 *@file Fat.cpp
 *@brief Inicializa os atributos do disco.
 * Modos de abertura    Descrição
 *   ios::in            Abre para leitura (default de ifstream).
 *   ios::out           Abre para gravação (default de ofstream),
 *   ios::ate           Abre e posiciona no final do arquivo.
 *   ios::app           Grava a partir do fim do arquivo
 *   ios::trunc         Abre e apaga todo o conteúdo do arquivo
 *   ios::nocreate      Erro de abertura se o arquivo não existe
 *   ios::noreplace     Erro de abertura se o arquivo existir
 *   ios::binary        Abre em binário (default é texto)
 *
 *@param string enderecoArquivo
 *
 *@author Pargles
 *@author Abilio
 */
Fat::Fat(string enderecoArquivo) {
	this->nomeArquivo = enderecoArquivo;
	/*
	 * Open in binary mode or read.
	 */
	this->Reader.open(nomeArquivo.c_str(), ios::in | ios::binary);
	if (this->Reader == NULL) {
		cerr << "Nao foi Possivel Abrir o Arquivo: " << nomeArquivo;
		cerr << "\nVoce precisa Definir o Endereco do Sistema de Arquivos Atraves de -in nome_arquivo \n";
		exit(1);
	}
	/*
	 * Inicializa as Informacoes da FAT
	 *	 como tamanho do setor, tamanho bloco etc.
	 */
	inicializarInformacoesCabecalho();
}

/**
 *@file Fat.cpp
 *@brief Inicializa todos os parametros do sistema de arquivos
 *       como por exemplo bytes por setor, setores por bloco etc.
 *       http://www.cplusplus.com/doc/tutorial/files/
 *
 *@param string enderecoArquivo
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::inicializarInformacoesCabecalho() {
	bytesPorSetor = lerBytes(11, 2);
	setoresPorBloco = lerBytes(13, 1);
	quantidadeSetoresCabecalho = lerBytes(14, 2);
	numeroDeFATs = lerBytes(16, 1);
	numeroMaximoArquivos = lerBytes(17, 2);
	quantidadeSetoresFat = lerBytes(22, 2);
	tamanhoCadaBloco = bytesPorSetor * setoresPorBloco;
	/*
	 * Depois do cabecalho inicia a Fat1 (posicaoFat1)
	 */
	posicaoFat1 = quantidadeSetoresCabecalho * bytesPorSetor;
	/*
	 * Depois da Fat1 inicia a Fat2 (posicaoFat2)
	 */
	posicaoFat2 = posicaoFat1 + quantidadeSetoresFat * bytesPorSetor;
	/*
	 * Diretorio de arquivos inicia depois da Fat2
	 */
	posicaoDiretorioArquivos = posicaoFat2 + quantidadeSetoresFat * bytesPorSetor;
	/*
	 * Cada entrada da tabela de diretorios tem 32 bytes
	 */
	posicaoInicialDados = posicaoDiretorioArquivos + numeroMaximoArquivos * 32;
}

/**
 *@file Fat.cpp
 *@brief Lista as informações dos Cabeçalhos
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::listarInformacoesCabecalho() {
	cout << "Numero Maximo de Arquivos: " << dec << numeroMaximoArquivos << endl;
	cout << "Bytes por Setor: " << dec << bytesPorSetor << endl;
	cout << "Setores por Bloco: " << dec << setoresPorBloco << endl;
	cout << "Setores para o Cabecalho: " << dec << quantidadeSetoresCabecalho << endl;
	cout << "Numero de FATs: " << dec << numeroDeFATs << endl;
	cout << "Quantidade de Setores de cada FAT: " << dec << quantidadeSetoresFat << endl;
	cout << "Posicao da FAT1 (hexadecimal): ";
	cout << hex << posicaoFat1 << endl;
	cout << "Posicao da FAT2 (hexadecimal): ";
	cout << hex << posicaoFat2 << endl;
	cout << "Posicao Diretorio de Arquivos (hexadecimal): ";
	cout << hex << posicaoDiretorioArquivos << endl;
	cout << "Posicao Inicial Dados (hexadecimal): ";
	cout << hex << posicaoInicialDados << endl;
}

/**
 *@file Fat.cpp
 *@brief Lista um determinado bloco indicado no parametro
 *       http://www.cplusplus.com/doc/tutorial/files/
 *@param int (numero do bloco a ser listado)
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::listarBloco(int numeroBloco) {
	char *byte = new char[1];
	unsigned char temp;
	if (numeroBloco <= 1) {
		cerr << "Os blocos 0 e 1 nao são enderecaveis";
		exit(1);
	}
	//Primeiro bloco Enderecavel Inicia em 2
	numeroBloco -= 2;
	//ios::beg dar o seek a partir do Inicio do Arquivo
	Reader.seekg(numeroBloco * tamanhoCadaBloco + posicaoInicialDados, ios::beg);
	for (register int i = 0; i < tamanhoCadaBloco; i++) {
		//Le cada Byte printa
		Reader.read(byte, 1);
		temp = byte[0];
		cout << hex << temp;
	}
}

/**
 *@file Fat.cpp
 *@brief Realiza a leitura de ate dois bytes
 *       Recebe a posicao inicial da leitura e retorna o valor lido
 *       Realiza mascara caso sejam necessarios ler dois bytes
 *          para gerar o valor corretamente
 *       As entradas da fat, primeiro preenche FAT1 e depois a FAT2;
 *
 *@param int (posicao onde se encontra o primeiro byte a ser lido)
 *@param int (quantidade de bytes a serem lidos ( 1 ou 2))
 *
 *@return os bytes lidos em um short int
 *
 *@author Pargles
 *@author Abilio
 */
unsigned short int Fat::lerBytes(int posicaoInicial, int quantosBytes) {
	char *byte = new char[1];
	unsigned char temp1;
	unsigned char temp2;
	unsigned short int palavra;
	//ios::beg dar o seek a partir do inicio do arquivo
	Reader.seekg(posicaoInicial, ios::beg);
	Reader.read(byte, 1);
	temp1 = byte[0];
	if (quantosBytes == 1) {
		return temp1;
	}
	Reader.read(byte, 1);
	temp2 = byte[0];
	palavra = temp2;
	//Abre espaco para concatenar a proxima palavra (8 bits)
	palavra = palavra << 8;
	palavra = palavra | temp1;
	return palavra;
}

/**
 *@file Fat.cpp
 *@brief Preenche a FAT1 e depois a FAT2
 *	     Para realização completa da operação o método deve
 *	     ser chamado duas vezes
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::inserirEntradasFat() {
	vetorDeEntradasFat1 = adicionarEntradas(posicaoFat1, vetorDeEntradasFat1);
	if (numeroDeFATs == 2) {
		vetorDeEntradasFat2 = adicionarEntradas(posicaoFat2,vetorDeEntradasFat2);
	}
}

/**
 *@file Fat.cpp
 *@brief Adiciona todas as entradas da FAT em um vetor
 *
 *@param int (posicao onde inicia a fat)
 *@param vector<unsigned short int> (vetor para inserir as entradas)
 *
 *@author Pargles
 *@author Abilio
 */
vector<unsigned short int> Fat::adicionarEntradas(int posicaoInicialFat, vector<unsigned short int> vetor) {
	char *palavras = new char[3];
	unsigned char quatrobitsA;
	unsigned char quatrobitsB;
	int quantidadeDeEntradasFat = bytesPorSetor * quantidadeSetoresFat;
	/*
	 *Cada entrada necessita 12 bits = 1,5 byte
	 *Logo pode nao dar um numero inteiro de entradas,
	 *     exemplo: sistema de arquivo de 1MB
	 */
	while (fmod(quantidadeDeEntradasFat, 1.5) != 0.0)
	{
		quantidadeDeEntradasFat--;
	}
	//Atualiza a Quantidade de Entradas validas que a FAT vai ter
	quantidadeDeEntradasFat = quantidadeDeEntradasFat / 1.5;
	//ios::beg e para dar o seek a partir do inicio do arquivo
	Reader.seekg(posicaoInicialFat, ios::beg);
	unsigned short int entrada12bits;
	register int i;
	//Divide por dois pois inserimos no vetor de entradas duas palavras por vez
	for (i = 0; i < quantidadeDeEntradasFat / 2; i++){
		entrada12bits = 0;
		quatrobitsA = 0;
		quatrobitsB = 0;
		//Le 3 bytes por vez, colocando cada byte na posicao do vetor palavras
		Reader.read(palavras, 3);
		//240 = 1111 0000 o & vai pegar so os ultimos 4 bits da palavra do meio
		quatrobitsA = palavras[1] & 240;
		//coloca os bits a direita para concatenar com as entrada12bits
		quatrobitsA = quatrobitsA >> 4;
		//15 = 0000 1111 o & vai pegar so os primeiros 4 bits da palavra do meio
		quatrobitsB = palavras[1] & 15;
		entrada12bits = quatrobitsB;
		//abre espaco para o proximo byte (oito bits)
		entrada12bits = entrada12bits << 8;
		entrada12bits = entrada12bits | palavras[0];
		entrada12bits = entrada12bits << 4;
		//zera os quatro bits mais a esquerda
		entrada12bits = entrada12bits >> 4;
		vetor.push_back(entrada12bits);
		//zera tudo para formar a segunda palavra de 12 bits
		entrada12bits = 0;
		entrada12bits = palavras[2];
		//abre espaco para os 4 bits restantes, necessarios para formar a entrada de 12 bits
		entrada12bits = entrada12bits << 4;
		entrada12bits = entrada12bits | quatrobitsA;
		entrada12bits = entrada12bits << 4;
		//zera os quatro bits mais a esquerda
		entrada12bits = entrada12bits >> 4;
		//insere no vetor a segunda entrada gerada
		vetor.push_back(entrada12bits);
	}
	return vetor;
}

/**
 *@file Fat.cpp
 *@brief Lista todas as entradas da FAT1 e FAT2
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::listarEntradasFAT() {
	cout << "ENTRADA  |     FAT1       | FAT2        |" << endl;
	for (register int i = 0; i < this->vetorDeEntradasFat1.size(); i++){
		cout << i << dec << " :            " << vetorDeEntradasFat1[i] << "          ";
		cout << vetorDeEntradasFat2[i] << endl;
	}
}

/**
 *@file Fat.cpp
 *@brief Compara os dois vetores de entradas das FATs
 *       e indica as diferencas caso existam
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::diferenciarFATs() {
	for (register int i = 0; i < vetorDeEntradasFat1.size(); i++) {
		if (vetorDeEntradasFat1[i] != vetorDeEntradasFat2[i]) {
			cout << "DIF <" << dec << i << dec << ">:<"
				 << vetorDeEntradasFat1[i] << ">,<" << vetorDeEntradasFat2[i]
				 << ">\n";
		}
	}
}

/**
 *@file Fat.cpp
 *@brief Percorre as entradas da FAT informando os bloco livres
 *       (entrada da FAT = 0)
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::listarBlocosLivres() {
	bool temVirgula = false;
	cout << "LIVRE ";
	for (register int i = 0; i < vetorDeEntradasFat1.size(); i++) {
		//Se o indice contem 0 e porque a FAT nao esta apontando
		if (vetorDeEntradasFat1[i] == 0){
			cout << i << dec << ",";
			temVirgula = true;
		}
	}
	//Testa se ja colocou alguma virgula na saida
	if (temVirgula){
		//tira a virgula da saida, apenas para ficar como a definicao do trabalho
		cout << '\b' << " ";
	}
	cout << endl;
}

/**
 *@file Fat.cpp
 *@brief Percorre as entradas da FAT e informa quais blocos estao livres mas contem dados
 *       (quando a entrada da FAT tiver 0 o bloco esta livre
 *        mas pode conter dados de um arquivo anterior)
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::listarBlocosLivresComDados() {
	cout << "REMOVIDOS ";
	bool temVirgula = false;
	for (register int i = 0; i < vetorDeEntradasFat1.size(); i++) {
		//se o indice contem valor igual a 0 e porque a FAT nao esta apontando para arquivo
		if (vetorDeEntradasFat1[i] == 0){
			//mas o bloco vazio pode ter sido apagado e os dados continuam la
			if (eBlocoComDados(i)) {
				//se o bloco contem dados e pq o arquivo foi excluido e a FAT nao aponta mais
				cout << i << dec << ",";
				temVirgula = true;
			}
		}
	}
	//testa se ja colocou alguma virgula na saida
	if (temVirgula) {
		//tira a virgula da saida, apenas para ficar como a definicao do trabalho
		cout << '\b' << " ";
	}
	cout << endl;
}

/**
 *@file Fat.cpp
 *@brief Percorre um bloco passado por parametro ate achar algum char,
 *       se nao encontrar nada o bloco nao contem dados e retorna true
 *
 *@param int (numero do bloco a ser verificado se contem ou nao dados)
 *
 *@return bool true se o bloco contem dados
 *
 *@author Pargles
 *@author Abilio
 */
bool Fat::eBlocoComDados(int numeroBloco) {
	char *byte = new char[1];
	unsigned char temp;
	if (numeroBloco <= 1) {
		cerr << "Os blocos 0 e 1 nao são enderecaveis";
		exit(1);
	}
	//ios::beg e para dar o seek a partir do inicio do arquivo
	Reader.seekg((numeroBloco - 2) * tamanhoCadaBloco + posicaoInicialDados,ios::beg);
	for (register int i = 0; i < tamanhoCadaBloco; i++) {
		//le byte por byte do arquivo 1 e um byte
		Reader.read(byte, 1);
		temp = byte[0];
		if (temp != 0) {
			//Tem algum dado
			return true;
		}
	}
}

/**
 *@file Fat.cpp
 *@brief Imprime um arquivo sem formatacao a partir de um bloco inicial,
 *       o bloco onde inicia cada arquivo pode ser exibido atraves da flag -tab
 *
 *@param int (numero do bloco inicial do arquivo)
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::imprimirArquivoCompleto(int bloco) {
	int blocoAtual = bloco;
	//0xfff = 4095, termino do arquivo
	while (blocoAtual != 0xfff)
	{
		listarBloco(blocoAtual);
		blocoAtual = vetorDeEntradasFat1.at(blocoAtual);
	}

}

/**
 *@file Fat.cpp
 *@brief Lista a tabela de diretorios
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::listarTabelaDiretorios() {
	char *palavras = new char[64]; //cada entrada do diretorio contem 64 bytes
	unsigned short int endereco;
	register int i; //contador do laco for
	//ios::beg e para dar o seek a partir do inicio do arquivo
	Reader.seekg(this->posicaoDiretorioArquivos, ios::beg);
	cout << "nº" << "  Nome   " << "      Bloco inicial" << endl;
	for (i = 0; i < numeroMaximoArquivos / 2; i++) {
		//le 3 bytes por vez, colocando cada  byte em cada posicao do vetor palavras
		Reader.read(palavras, 64);
		//entrada esta vazia, nao precisa printar
		if (!palavras[0] == 0) {
			cout << dec << i + 1;
			//0xe5 = 229 , arquivo deletado, mas ainda esta com as informacoes no bloco
			if (palavras[0] & 146) {
				cout << " deletado ";
			} else {
				cout << " " << palavras[32] << palavras[33] << palavras[34]
					 << palavras[35] << palavras[36] << palavras[37];
				cout << palavras[38] << palavras[39] << "." << palavras[40]
					 << palavras[41] << palavras[42];
			}
			endereco = palavras[59];
			//abre espaco para concatenar a proxima palavra (8 bits)
			endereco = endereco << 8;
			endereco = endereco | palavras[58];
			cout << "             " << endereco << endl;
		}
	}

}

/**
 *@file Fat.cpp
 *@brief Insere erros na FAT 2, apenas para testar as difenreças
 *      entre a FAT 1 e a Fat 2
 *      Função                 Descrição
 *      seekg( )-Movimenta a posição atual de leitura (get)
 *      seekp( )-Movimenta a posição atual de gravação (put)
 *      tellg( )-Retorna a posição atual de leitura (em bytes), a partir do início do arquivo
 *      tellp( )-Retorna a posição atual de gravação (em bytes), a partir do início do arquivo
 *      http://www.cplusplus.com/doc/tutorial/files/
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::fliparBitsFAT2() {
	fstream temp; //arquivo em c++
	temp.open(nomeArquivo.c_str(), ios::out | ios::in | ios::binary); //Open in binary mode or read.
	if (!temp.is_open()) {
		cerr << "Problema em abrir arquivo para flipar Fat2";
	}
	int posicaoInicialFat2 = posicaoFat2;
	//ios::beg e para dar o seek a partir do inicio do arquivo
	temp.seekp(posicaoInicialFat2 + 15, ios::beg);
	char *erro = new char[1];
	erro[0] = 'a';
	temp.write(erro, 1); //1 byte
	//ios::beg e para dar o seek a partir do inicio do arquivo
	temp.seekp(posicaoInicialFat2 + 23, ios::beg);
	erro[0] = '1';
	temp.write(erro, 1); //1 byte
	//ios::beg e para dar o seek a partir do inicio do arquivo
	temp.seekp(posicaoInicialFat2 + 45, ios::beg);
	erro[0] = 'e';
	temp.write(erro, 1); //1 byte
	//ios::beg e para dar o seek a partir do inicio do arquivo
	temp.seekp(posicaoInicialFat2 + 204, ios::beg);
	erro[0] = '5';
	temp.write(erro, 1); //1 byte
	//ios::beg e para dar o seek a partir do inicio do arquivo
	temp.seekp(posicaoInicialFat2 + 604, ios::beg);
	erro[0] = '9';
	temp.write(erro, 1); //1 byte
	temp.close();

}

/**
 *@file Fat.cpp
 *@brief Seta o endereco do sistema de arquivos
 *
 *@param string (contendo o endereco do disco)
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::setNomeArquivo(string enderecoDisco) {
	this->nomeArquivo = enderecoDisco;
}

/**
 *@file Fat.cpp
 *@brief Retorna o endereco do sistema de arquivos
 *
 *@return string (contendo o endereco do disco)
 *
 *@author Pargles
 *@author Abilio
 */
string Fat::getNomeArquivo() {
	return this->nomeArquivo;
}

/**
 *@file Fat.cpp
 *@brief Fecha a stream de dados do disco
 *
 *@author Pargles
 *@author Abilio
 */
void Fat::fecharReader() {
	this->Reader.close();
}
