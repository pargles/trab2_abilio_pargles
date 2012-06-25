#include "../inc/FatAnalyser.h"

/**
	@brief Inicializa os atributos da classe FatAnalyser com valores default.
 *      e recebe o parse como parametro
 *      @param Parser contendo os argumentos selecionados
	@author pargles and abilio
*/
FatAnalyser::FatAnalyser(Parser *parse) {
    this->parser = parse;
    this->FAT = new Fat(parse->getEnderecoSistemaArquivos());

}
/**
        @brief executa o analisador de fat
        @author pargles and abilio
 */
void FatAnalyser::run() {
    FAT->inserirEntradasFat();//apenas o metodo ler bloco nao precisa das entradas, portando elas
    //sao alocadas em vetores antes de iniciar a execucao do programa
    FAT->listarEntradasFAT();
    FAT->listarBloco(3);
    //FAT->fliparBitsFAT2();
    if(parser->listarInformacoes())
        FAT->listarInformacoesCabecalho();
    if (parser->verificaFats())
        verificarParticoesFat();
    if (parser->blocosLivres())
        imprimiListaDeBlocosLivres();
    if (parser->blocoComDados())
        imprimirListaDeBlocosComDados();
    if (parser->lerBloco())
        lerBloco(parser->getNumeroDoBloco());
    if (parser->listarArquivo())
        listarArquivo(parser->getNumeroDoBloco());
    FAT->fecharReader();
}


/**
	@brief 1.1 Verificar FATs
 *      Verificar se as duas FATs são iguais – caso não sejam, imprimir uma lista
 *      de diferenças no seguinte formato, com uma linha
        para cada diferença:
 *      @param strings particaoA e particaoB
	@author pargles and abilio
*/
void FatAnalyser::verificarParticoesFat() {
    //FAT->listarEntradasFAT();
    //FAT->fliparBitsFAT2();//TODO, desativar esse metodo na versao final, e so pra teste
    FAT->diferenciarFATs();

}

/**
	@brief 1.2 Imprimir lista de blocos livres
 *      Imprime os índices de todos os blocos que estão livres (ou seja, não são apontados pela FAT) em uma única linha.
	@author pargles and abilio
*/
void FatAnalyser::imprimiListaDeBlocosLivres() {
    FAT->listarBlocosLivres();
}

/**
	@brief 1.3 Imprimir lista de blocos livres com dados
 *      Parâmetro na linha de comando: -bd
        Imprime os índices de todos os blocos que estão livres e que tem conteúdo diferente de zeros,
 *      em uma única linha.
	@author pargles and abilio
*/
void FatAnalyser::imprimirListaDeBlocosComDados() {

    cout << "REMOVIDOS 1,5,6,7\n";

}

/**
	@brief 1.4 Ler um determinado bloco
 *      Parâmetro na linha de comando: -lb <numero do bloco>
        Imprime o bloco para a saída padrão sem formatação.
 *      @param int pósicao do bloco a ser lido
	@author pargles and abilio
*/
void FatAnalyser::lerBloco(int bloco) {

    FAT->listarBloco(bloco);

}

/**
	@brief listar todo um arquivo que inicia no bloco X
 *      Parâmetro na linha de comando: -lb <numero do bloco>
        Imprime o bloco para a saída padrão sem formatação.
 *      @param int posicao inicial do bloco a ser listado
	@author pargles and abilio
*/
void FatAnalyser::listarArquivo(int bloco)
{
    FAT->listarTabelaDiretorios();
    //FAT->imprimirArquivoCompleto(bloco);
}
