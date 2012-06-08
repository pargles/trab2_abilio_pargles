#include "../inc/disco.h"

/**
	@brief Inicializa os atributos do disco.
	@author pargles and abilio
*/
disco::disco(string enderecoArquivo) {
    this->nomeArquivo = enderecoArquivo;
    this->Reader = fopen(nomeArquivo.c_str(), "rb");//rb = read bytes
    if(this->Reader == NULL)
    {
        cout << "nao foi possivel abrir o arquivo: " << nomeArquivo;
        exit(1);
    }

}

/**
	@brief seta o endereco do disco.
	@param string contendo o endereco do disco
	@author pargles and abilio
*/
void disco::setNomeArquivo(string enderecoDisco)
{
    this->nomeArquivo = enderecoDisco;
}

/**
	@brief retorna o endereco do disco
	@return string contendo o endereco do disco
	@author pargles and abilio
*/
string disco::getNomeArquivo()
{
    return this->nomeArquivo;
}

/**
	@brief fecha a stream de dados que acessava o disco
	@author pargles and abilio
*/
void disco::fecharReader()
{
    fclose(this->Reader);
}

/**
	@brief fecha a stream de dados que acessava o disco
	@author pargles and abilio
*/
void disco::listarConteudo()
{
    char byte;
    while(byte=(fgetc(this->Reader))!=EOF)//retorna EOF caso tenha terminado o arquivo
    {
        cout << byte;
    }
}

