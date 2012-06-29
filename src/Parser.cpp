#include "../inc/Parser.h"
#include <iostream>
/**
	@brief Inicializa o Parser com valores default.
	@author pargles
*/
Parser::Parser() {
    this->vf = false;
    this->bd = false;
    this->bl = false;
    this->lb = false;
    this->la = false;
    this->inf = false;
    this->tab = false;
    this->numeroBloco=0;
    this->numeroBlocoArquivo=0;
}

/**
	@brief Retorna true se é para verificar duas fats ou 0 c.c.
	@return int Verifica particoes Fat
	@author pargles
*/
bool Parser::verificaFats() {
	return this->vf;
}

/**
        @brief Retorna true se é para listar os blocos livres ou 0 caso contrario.
        @return int Lista blocos livres
        @author pargles
 */
bool Parser::blocosLivres() {
    return this->bl;
}

/**
        @brief Retorna true se é para listar os blocos que contem dados ou 0 c.c.
        @return int Verifica particoes Fat
        @author pargles
 */
bool Parser::blocoComDados() {
    return this->bd;
}

/**
        @brief Retorna true se é para ler um determinado bloco ou 0 c.c.
        @return int ler determinado bloco
        @author pargles
 */
bool Parser::lerBloco() {
    return this->lb;
}

/**
        @brief Retorna true se é para listar um arquivo ou 0 c.c.
        @return true para listar e false c.c.
        @author pargles
 */
bool Parser::listarArquivo()
{
    return this->la;
}

/**
        @brief Retorna true se é para listar as informacoes do sistema de arquivos ou 0 c.c.
        @return true para mostrar as informacoes e false c.c.
        @author pargles
 */
bool Parser::listarInformacoes()
{
    return this->inf;
}

/**
        @brief Retorna true se é para listar a tabela de diretorios.
        @return true para mostrar as informacoes e false c.c.
        @author abilio and pargles
 */
bool Parser::listarTabelaDiretorios()
{
    return this->tab;
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
        @brief Retorna o numero do bloco inicial de um arquivo para ser listado.
        @return int numero do bloco a ser lido
        @author pargles
 */
int Parser::getNumeroBlocoArquivo() {
    return this->numeroBlocoArquivo;
}

/**
        @brief Retorna o endereco do sistema de arquivos.
        @return string Particao A
        @author pargles
 */
string Parser::getEnderecoSistemaArquivos() {
    return this->enderecoSistemaArquivos;
}

/**
	@brief seta o numero do bloco a ser listado.
	@param numero numero do bloco
	@author pargles
*/
void Parser::setNumeroDoBloco(int numeroDoBloco) {
	this->numeroBloco = numeroDoBloco;
}


/**
	@brief seta o endereco onde se localiza o sistema de arquivos.
	@param particao particaoA
	@author pargles
*/
void Parser::setEnderecoSistemaArquivos(string Particao) {
	this->enderecoSistemaArquivos = Particao;
}

/**
	@brief Analiza os argumentos passados na chamada do programa e seta os valores passados por eles.
	@param Arguments Lista dos argumentos passados na chamada do programa
	@author Pargles
*/
void Parser::parse(vector<string> Arguments) {
	for(register int i = 0; i < Arguments.size(); i++)
		if(Arguments[i].compare("-vf") == 0)
                    this->vf = true;//1.1
                else if(Arguments[i].compare("-in") == 0)
                    this->setEnderecoSistemaArquivos(Arguments[i+1]);
		else if(Arguments[i].compare("-bl") == 0)
			this->bl = true;//1.2
		else if(Arguments[i].compare("-bd") == 0)
			this->bd = true;//1.3
 		else if(Arguments[i].compare("-lb") == 0)
                {
                    this->lb = true;//1.4
                    this->numeroBloco = atoi(Arguments[i+1].c_str());
                }
                else if(Arguments[i].compare("-inf") == 0)
                    this->inf = true;
                else if(Arguments[i].compare("-la") == 0)
                {
                    this->la = true;
                    this->numeroBlocoArquivo = atoi(Arguments[i+1].c_str());
                }
                else if(Arguments[i].compare("-tab") == 0)
                    this->tab = true;
                else if(Arguments[i].compare("-help")==0)
                {
                    cout << "-in nome_arquivo , use para especificar o nome da particao \n";
                    cout << "-inf , use para mostrar as informacoes do sistema de arquivos \n";
                    cout << "-bl , use para listar os blocos livres \n";
                    cout << "-bd , use para listar os indices de todos os blocos livres e com conteudo \n";
                    cout << "-lb numero_bloco , use para imprimir um bloco sem formatacao \n";
                    cout << "-vf , use para verficar as diferencas entre as tabelas FAT1 e FAT2 \n";
                    cout << "-la numero_bloco, use para listar todo o arquivo que comeca no bloco (numero_bloco)\n";
                }
}

