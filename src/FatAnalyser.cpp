#include "../inc/FatAnalyser.h"

/**
	@brief Inicializa os atributos da classe FatAnalyser com valores default.
 *      e recebe o parse como parametro
 *      @param Parser contendo os argumentos selecionados
	@author pargles and abilio
*/
FatAnalyser::FatAnalyser(Parser *parse) {
    this->parser = parse;
    string diretorio = "/home/pargles/Documentos/disco";
    this->fat1 = new Fat(diretorio);

}

/**
	@brief executa o analisador de fat 
	@author pargles and abilio
*/
void FatAnalyser::run() {

    if(parser->verificaFats())
        verificarParticoesFat(parser->getParticaoA(), parser->getParticaoB());
        //fat1->listarConteudo();
        //fat1->listarClusterInicial();
        //fat1->listarCluster(0);
    fat1->listarInformacoesCabecalho();
    fat1->listarFAT();
    fat1->adicionarEntradas();
        fat1->fecharReader();  
    if(parser->blocosLivres())
        imprimiListaDeBlocosLivres();
    if(parser->blocoComDados())
        imprimirListaDeBlocosComDados();
    if(parser->lerBloco())
        lerBloco(parser->getNumeroDoBloco());
}


/**
	@brief 1.1 Verificar FATs
 *      Verificar se as duas FATs são iguais – caso não sejam, imprimir uma lista
 *      de diferenças no seguinte formato, com uma linha
        para cada diferença:
 *      @param strings particaoA e particaoB
	@author pargles and abilio
*/
void FatAnalyser::verificarParticoesFat(string particaoA, string parlticaoB) {

    cout << "DIF <item>:<fat1>,<fat2>";

}

/**
	@brief 1.2 Imprimir lista de blocos livres
 *      Imprime os índices de todos os blocos que estão livres (ou seja, não são apontados pela FAT) em uma única linha.
	@author pargles and abilio
*/
void FatAnalyser::imprimiListaDeBlocosLivres() {

    cout << "LIVRE 1,5,6,7";

}

/**
	@brief 1.3 Imprimir lista de blocos livres com dados
 *      Parâmetro na linha de comando: -bd
        Imprime os índices de todos os blocos que estão livres e que tem conteúdo diferente de zeros,
 *      em uma única linha.
	@author pargles and abilio
*/
void FatAnalyser::imprimirListaDeBlocosComDados() {

    cout << "REMOVIDOS 1,5,6,7";

}

/**
	@brief 1.4 Ler um determinado bloco
 *      Parâmetro na linha de comando: -lb <numero do bloco>
        Imprime o bloco para a saída padrão sem formatação.
 *      @param int pósicao do bloco a ser lido
	@author pargles and abilio
*/
void FatAnalyser::lerBloco(int bloco) {

    cout << "asdqwe24534aqw1387";

}
