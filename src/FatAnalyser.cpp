#include "../inc/FatAnalyser.h"

/**
 *@file FatAnalyser.cpp
 *@brief Inicializa os atributos da classe FatAnalyser com valores default.
 *
 *@param Parser (contendo os argumentos selecionados)
 *
 *@author Pargles
 *@author Abilio
 */
FatAnalyser::FatAnalyser(Parser *parse) {
    this->parser = parse;
    this->FAT = new Fat(parse->getEnderecoSistemaArquivos());

}
/**
 *@file FatAnalyser.cpp
 *@brief Executa o analisador de fat
 *
 *@author Pargles
 *@author Abilio
 */
void FatAnalyser::run() {
	/*
	 * Apenas le bloco nao precisa das entradas, os outros 3 metodos necessitam
	 * portando as entradas sao alocadas em vetores antes de iniciar a execucao do programa
	 */
    FAT->inserirEntradasFat();
    if(parser->listarInformacoes())
        FAT->listarInformacoesCabecalho();
    if(parser->listarTabelaDiretorios())
        FAT->listarTabelaDiretorios();
    if (parser->verificaFats())
        verificarParticoesFat();
    if (parser->blocosLivres())
        imprimiListaDeBlocosLivres();
    if (parser->lerBloco())
        lerBloco(parser->getNumeroDoBloco());
    if (parser->listarArquivo())
        listarArquivo(parser->getNumeroBlocoArquivo());
    if (parser->blocoComDados())
        imprimirListaDeBlocosComDados();
    FAT->fecharReader();
}


/**
 *@file FatAnalyser.cpp
 *@brief 1.1 Verificar FATs
 *      Verificar se as duas FATs são iguais – caso não sejam, imprimir uma lista
 *      de diferenças no seguinte formato, com uma linha para cada diferença:
 *
 *@author Pargles
 *@author Abilio
 */
void FatAnalyser::verificarParticoesFat() {
    //FAT->fliparBitsFAT2();//TODO, desativar esse metodo na versao final, e so pra teste
    FAT->diferenciarFATs();
}

/**
 *@file FatAnalyser.cpp
 *@brief 1.2 Imprimir lista de blocos livres
 *      Imprime os índices de todos os blocos que estão livres
 *      (ou seja, não são apontados pela FAT) em uma única linha.
 *
 *@author Pargles
 *@author Abilio
 */
void FatAnalyser::imprimiListaDeBlocosLivres() {
    FAT->listarBlocosLivres();
}

/**
 *@file FatAnalyser.cpp
 *@brief 1.3 Imprimir lista de blocos livres com dados
 *      Imprime os índices de todos os blocos que estão livres e que tem conteúdo diferente de zeros,
 *      em uma única linha.
 *
 *@author Pargles
 *@author Abilio
 */
void FatAnalyser::imprimirListaDeBlocosComDados() {
    FAT->listarBlocosLivresComDados();
}

/**
 *@file FatAnalyser.cpp
 *@brief 1.4 Ler um determinado bloco
 *      Parâmetro na linha de comando: -lb <numero do bloco>
 *      Imprime o bloco para a saída padrão sem formatação.
 *
 *@param int (posicao do bloco a ser lido)
 *
 *@author Pargles
 *@author Abilio
 */
void FatAnalyser::lerBloco(int bloco) {
    FAT->listarBloco(bloco);
}

/**
 *@file FatAnalyser.cpp
 *@brief Listar todo um arquivo que inicia no bloco X
 *       Parâmetro na linha de comando: -lb <numero do bloco>
 *       Imprime o bloco para a saída padrão sem formatação.
 *
 *@param int posicao inicial do bloco a ser listado
 *
 *@author Pargles
 *@author Abilio
 */
void FatAnalyser::listarArquivo(int bloco){
    FAT->imprimirArquivoCompleto(bloco);
}
