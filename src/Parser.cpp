#include "../inc/Parser.h"
#include <iostream>
/**
 *@file Parser.cpp
 *@brief Inicializa o Parser com valores default.
 *
 *@author Pargles
 *@author Abilio
 */
Parser::Parser() {
	this->vf = false;
	this->bd = false;
	this->bl = false;
	this->lb = false;
	this->la = false;
	this->inf = false;
	this->tab = false;
	this->numeroBloco = 0;
	this->numeroBlocoArquivo = 0;
}

/**
 *@file Parser.cpp
 *@brief Retorna true se é para verificar as duas fats
 *@return bool
 *
 *@author Pargles
 *@author Abilio
 */
bool Parser::verificaFats() {
	return this->vf;
}

/**
 *@file Parser.cpp
 *@brief Retorna true se é para listar os blocos livres
 *@return bool
 *
 *@author Pargles
 *@author Abilio
 */
bool Parser::blocosLivres() {
	return this->bl;
}

/**
 *@file Parser.cpp
 *@brief Retorna true se é para listar os blocos que contem dados
 *@return bool
 *
 *@author Pargles
 *@author Abilio
 */
bool Parser::blocoComDados() {
	return this->bd;
}

/**
 *@file Parser.cpp
 *@brief Retorna true se é para ler um determinado bloco
 *@return bool
 *
 *@author Pargles
 *@author Abilio
 */
bool Parser::lerBloco() {
	return this->lb;
}

/**
 *@file Parser.cpp
 *@brief Retorna true se é para listar um arquivo
 *@return bool
 *
 *@author Pargles
 *@author Abilio
 */
bool Parser::listarArquivo() {
	return this->la;
}

/**
 *@file Parser.cpp
 *@brief Retorna true se é para listar as informacoes
 *		 do sistema de arquivos
 *@return bool
 *
 *@author Pargles
 *@author Abilio
 */
bool Parser::listarInformacoes() {
	return this->inf;
}

/**
 *@file Parser.cpp
 *@brief Retorna true se é para listar a tabela de diretorios
 *@return bool
 *
 *@author Pargles
 *@author Abilio
 */
bool Parser::listarTabelaDiretorios() {
	return this->tab;
}

/**
 *@file Parser.cpp
 *@brief Retorna o numero do bloco a ser lido.
 *@return int
 *
 *@author Pargles
 *@author Abilio
 */
int Parser::getNumeroDoBloco() {
	return this->numeroBloco;
}

/**
 *@file Parser.cpp
 *@brief Retorna o numero do bloco inicial de um arquivo a ser listado.
 *@return int
 *
 *@author Pargles
 *@author Abilio
 */
int Parser::getNumeroBlocoArquivo() {
	return this->numeroBlocoArquivo;
}

/**
 *@file Parser.cpp
 *@brief Retorna o endereco do sistema de arquivos.
 *@return string (Particao A)
 *
 *@author Pargles
 *@author Abilio
 */
string Parser::getEnderecoSistemaArquivos() {
	return this->enderecoSistemaArquivos;
}

/**
 *@file Parser.cpp
 *@brief Seta o numero do bloco a ser listado.
 *@param int (numero do bloco)
 *
 *@author Pargles
 *@author Abilio
 */
void Parser::setNumeroDoBloco(int numeroDoBloco) {
	this->numeroBloco = numeroDoBloco;
}

/**
 *@file Parser.cpp
 *@brief Seta o endereco onde se localiza o sistema de arquivos.
 *@param string particao
 *
 *@author Pargles
 *@author Abilio
 */
void Parser::setEnderecoSistemaArquivos(string Particao) {
	this->enderecoSistemaArquivos = Particao;
}

/**
 *@file Parser.cpp
 *@brief Analiza os argumentos passados na chamada do programa e seta os valores passados por eles.
 *@param vector<string> Arguments (Lista dos argumentos passados na chamada do programa)
 *
 *@author Pargles
 *@author Abilio
 */
void Parser::parse(vector<string> Arguments) {
	for (register int i = 0; i < Arguments.size(); i++)
		if (Arguments[i].compare("-vf") == 0)
			this->vf = true; //1.1
		else if (Arguments[i].compare("-in") == 0)
			this->setEnderecoSistemaArquivos(Arguments[i + 1]);
		else if (Arguments[i].compare("-bl") == 0)
			this->bl = true; //1.2
		else if (Arguments[i].compare("-bd") == 0)
			this->bd = true; //1.3
		else if (Arguments[i].compare("-lb") == 0) {
			this->lb = true; //1.4
			this->numeroBloco = atoi(Arguments[i + 1].c_str());
		} else if (Arguments[i].compare("-inf") == 0)
			this->inf = true;
		else if (Arguments[i].compare("-la") == 0) {
			this->la = true;
			this->numeroBlocoArquivo = atoi(Arguments[i + 1].c_str());
		} else if (Arguments[i].compare("-tab") == 0)
			this->tab = true;
		else if (Arguments[i].compare("-help") == 0) {
			cout << "-in nome_arquivo: Especificar o Nome da Particao \n";
			cout << "-inf: Mostrar as Informacoes do Sistema de Arquivos \n";
			cout << "-bl: Listar os Blocos Livres \n";
			cout << "-bd: Listar os Indices dos Blocos Livres e com Conteudo \n";
			cout << "-lb numero_bloco: Imprimir um bloco sem formatacao \n";
			cout << "-vf: Verficar as Diferencas entre as Tabelas FAT1 e FAT2 \n";
			cout << "-la numero_bloco: Listar todo o Arquivo que comeca no Bloco (numero_bloco)\n";
			cout << "-tab: Listar os Arquivos Presentes e Deletados\n";
		}
}

