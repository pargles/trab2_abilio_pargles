#include "../inc/Parser.h"

/**
	@brief Inicializa o Parser com valores default.
	@author pargles
*/
Parser::Parser() {
    this->vf = 1;//default apenas verifica as particoes
    this->bd = 0;
    this->bl = 0;
    this->lb = 0;
    this->numeroBloco=0;
    this->particaoA ;
    this->particaoB ;
}

/**
	@brief Retorna 1 se é para verificar duas fats ou 0 c.c.
	@return int Verifica particoes Fat
	@author pargles
*/
int Parser::verificaFats() {
	return this->vf;
}

/**
        @brief Retorna 1 se é para listar os blocos livres ou 0 caso contrario.
        @return int Lista blocos livres
        @author pargles
 */
int Parser::blocosLivres() {
    return this->bl;
}

/**
        @brief Retorna 1 se é para listar os blocos que contem dados ou 0 c.c.
        @return int Verifica particoes Fat
        @author pargles
 */
int Parser::blocoComDados() {
    return this->bl;
}

/**
        @brief Retorna 1 se é para ler um determinado bloco ou 0 c.c.
        @return int ler determinado bloco
        @author pargles
 */
int Parser::lerBloco() {
    return this->lb;
}

/**
        @brief Retorna o numero do bloco a ser lido.
        @return int numero do bloco a ser lido
        @author pargles
 */
int Parser::getNumeroDoBloco() {
    return this->numeroBloco;
}

/**
        @brief Retorna o nome da particao A.
        @return string Particao A
        @author pargles
 */
string Parser::getParticaoA() {
    return this->particaoA;
}

/**
        @brief Retorna o nome da particao B.
        @return string Particao B
        @author pargles
 */
string Parser::getParticaoB() {
    return this->particaoB;
}


/**
	@brief seta o numero do bloco a ser listado.
	@param numero numero do bloco
	@author pargles
*/
void Parser::setNumeroDoBloco(int numeroDoBloco) {
	this->numeroBloco = numeroBloco;
}


/**
	@brief seta a particao A.
	@param particao particaoA
	@author pargles
*/
void Parser::setParticaoA(string Particao) {
	this->particaoA = Particao;
}

/**
	@brief seta a particao B.
	@param particao particaoB
	@author pargles
*/
void Parser::setParticaoB(string Particao) {
	this->particaoB = Particao;
}


/**
	@brief Analiza os argumentos passados na chamada do programa e seta os valores passados por eles.
	@param Arguments Lista dos argumentos passados na chamada do programa
	@author Pargles
*/
void Parser::parse(vector<string> Arguments) {
	for(unsigned int i = 0; i < Arguments.size(); i++)
		if(Arguments[i].compare("-vf") == 0)
                {
                    this->vf = 1;//1.1
                    this->setParticaoA(Arguments[i+1]);
                    this->setParticaoA(Arguments[i+2]);
                }
		else if(Arguments[i].compare("-bl") == 0)
			this->bl = 2;//1.2
		else if(Arguments[i].compare("-bd") == 0)
			this->bd = 3;//1.3
		else if(Arguments[i].compare("-lb") == 0)
                {
			this->lb = 4;//1.4
                        this->numeroBloco = atoi(Arguments[i+1].c_str());
                }
}

