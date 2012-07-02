#include "../inc/Parser.h"
#include "../inc/FatAnalyser.h"
#include <iostream>

/*
 * @author pargles and abilio
 * File:   testeme.cpp
 * Created on 7 de Junho de 2012, 11:02
 *
 * Trabalho 2 de SO desenvolvido como especificado em:
 * https://docs.google.com/a/inf.ufpel.edu.br/viewer?a=v&pid=sites&srcid=aW5mLnVmcGVsLmVkdS5icnxwaWxsYXxneDo1YjgxMTE3MjFhY2U1OTA4
 *
 * Criação de disco
 * 		dd if=/dev/zero of=disco bs=1M count=2
 * 		mkfs.vfat -F12 disco
 *
 * Comando mount:
 * 		sudo mount -o loop,uid=1000,gid=1000 disco ponto_montagem
 *
 * Flags para o comando mout:
 * http://gnulinuxbr.com/2009/11/25/comando-mount/
 *
 * A opção -o pode utilizar uma lista de opções separadas por vírgula, as principais são:
 *   para verificar seu uid e seu gid: ps -p $$ -o "gid uid"
 *
 * Restrições de Partições FAT12:
 *   12 bits cada entrada
 *   12 caracteres para o nome de cada arquivo, 8 para nome 1 para o ponto e 3 para extensao
 *   2^12 clusters  = 4.096 (4K) clusters ou unidades de alocação, grupo de setores do disco
 *   Cluster sizes 512b to 4k
 *   Os volumes FAT não diferenciam maiúsculas de minúsculas.
 *
 * Referências utilizadas no trabalho:
 *
 * http://linux.about.com/od/embedded/l/blcmdl8_mkfsvfa.htm
 *
 * Bless, visualizador de arquivos binarios:
 * http://home.gna.org/bless/downloads.html
 *
 * Artigo sobre discos:
 * http://www.clubedohardware.com.br/artigos/Limites-de-Capacidade-dos-Discos-Rigidos/1420/3
 *
 * Trabalho de SO similar, porem cobra a parte dos arquivos:
 * http://asc.di.fct.unl.pt/~vad/SO/SO-0708/trab-fat.html
 *
 * The FAT File System
 * http://www.c-jump.com/CIS24/Slides/FAT/lecture.html
 *
 * Wiki:
 * http://en.wikipedia.org/wiki/File_Allocation_Table
 *
 * C++:
 * http://www.cplusplus.com/doc/tutorial/files/
 */



/**
 *@file testeme.cpp
 *@brief Inicializa o funcionamento do programa
 *		 Faz a leitura dos argumentos e a passagem
 *		 deste para o parser
 *
 *@param Argumento da linha de comando
 *
 *@author Pargles
 *@author Abilio
 */
int main(int argc, char * argv[]) {
	Parser *comandos = new Parser();
	FatAnalyser *fatAnalyzer;

	/*
	 *Coloca em um vetor todos os argumentos
	 *   a serem passados para o parser
	 */
	vector<string> args;

	/*
	 *Adiciona cada entrada no vetor de strings
	 */
	for (int i = 0; i < argc; i++)
		args.push_back(argv[i]);

        	/*
	 *Faz a chamado do parser passando para ele
	 * 	os argumento do vetor
	 *Caso não tenha argumento insere a opção Help
	 */
        if (args.size()==1) {
		args.push_back("-help");
                comandos->parse(args);
                exit(1);
	}
	comandos->parse(args);

	fatAnalyzer = new FatAnalyser(comandos);
	fatAnalyzer->run();
	return 0;
}



/*!
* \mainpage Trabalho 2 SO
*Trabalho desenvolvido para a disciplina de Sistemas Operacionais I
*        Professor: Maurício Lima Pilla
*
*Alunos: Abilio Parada
*		Pargles Dall'Oglio
*
* \section Observações
*OBS 1: O Parser nao trata argumentos invalidos, segue apenas a especificao definida em :
*https://docs.google.com/a/inf.ufpel.edu.br/viewer?a=v&pid=sites&srcid=aW5mLnVmcGVsLmVkdS5icnxwaWxsYXxneDo1YjgxMTE3MjFhY2U1OTA4
*
*OBS 2: Como não foi definido o sistema de arquivos default na especificao do trabalho, a flag -in foi definada para passar por
*parametro o endereco do sistema de arquivos.
*
*OBS 3: a flag -help foi definida para listar todas as opcoes validas.
*
* \section Teste
* Para certificação do correto funcionamento do programa desenvolvido foram realizados os seguintes testes
*	Testes realizados com discos de 2MB, 1,5MB e de 1MB.
*
*	Para os testes foram criados 6 arquivos, contendo texto puro, de tamanhos variados, sendo:
*	Arq1 - 1.6 KB
*	Arq2 - 2.2 KB
*	Arq3 - 4.1 KB
*	Arq4 - 7.8 KB
*	Arq5 - 8.5 KB
*	Arq6 - 10.4 KB
*
*	Foram realizados testes nos disco para cada flag (vf, bl, bd, lb n, la n, tab),
*	testados com discos criados nas seguintes formas:
*
*	Discos de 2M:
*	 Vazio
*	 Lotado (256 Arquivos)
*	 Arq1
*     Todos (Arq1, Arq2, Arq3, Arq4, Arq5, Arq6)
*     Todos Apagados ~(Arq1, Arq2, Arq3, Arq4, Arq5, Arq6)
*     Arq1 e Arq2 e Arq3
*     Todos Apagados exceto Arq2 e Arq3
*     Todos Apagados exceto Arq1
*     Todos Apagados exceto Arq4
*     Todos Apagados exceto Arq6
*
*	Discos de 1,5M:
*	 Vazio
*	 Arq1 e Arq2 e Arq3
*     Todos (Arq1, Arq2, Arq3, Arq4, Arq5, Arq6)
*     Todos Apagados ~(Arq1, Arq2, Arq3, Arq4, Arq5, Arq6)
*     Todos Apagados exceto Arq3
*     Arq6
*
*	Discos de 1M:
*	 Vazio
*	 Arq3 e Arq4 e Arq1
*     Todos (Arq1, Arq2, Arq3, Arq4, Arq5, Arq6)
*     Todos Apagados ~(Arq1, Arq2, Arq3, Arq4, Arq5, Arq6)
*     Todos Apagados exceto Arq3
*     Arq6
* \section Referências
*
* [1] "The Fat File Systems", diponível em : <http://www.c-jump.com/CIS24/Slides/FAT/lecture.html>
* [2] "Input/Output with files", disponível em : <http://www.cplusplus.com/doc/tutorial/files/>
*
*/
