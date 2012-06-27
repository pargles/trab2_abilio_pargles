#include "../inc/Parser.h"
#include "../inc/FatAnalyser.h"
#include <iostream>

/*
 * @brief trabalho 2 de SO disponivel em:
 * https://docs.google.com/a/inf.ufpel.edu.br/viewer?a=v&pid=sites&srcid=aW5mLnVmcGVsLmVkdS5icnxwaWxsYXxneDo1YjgxMTE3MjFhY2U1OTA4
 * dd if=/dev/zero of=disco bs=1M count=2
 * Será feita cópia da partição /dev/zero sendo criado um arquivo disco no diretorio corrente
 * bs significa block size
 * count é quantas vezes ele conta o bs
 * mkfs.vfat -F12 disco
 * http://linux.about.com/od/embedded/l/blcmdl8_mkfsvfa.htm
 * para visualizar arquivos binarios pode-se utilizar o bless:
 * http://home.gna.org/bless/downloads.html
 * flags do comando mount:
 * http://gnulinuxbr.com/2009/11/25/comando-mount/
 * sudo mount -o loop,uid=1000,gid=1000 disco ponto_montagem
 * A opção -o pode utilizar uma lista de opções separadas por vírgula, das quais, as principais são:
 * the thing that the loopback device points to does not have to be a separate drive, but it can be a file!
 * para verificar seu uid e seu gid digite na linha de comando: ps -p $$ -o "gid uid"
 * RESTRICOES DE PARTICOES FAT12:
 * - 12 bits cada entrada
 * - 16MB tamanho maximo de arquivo
 * - 12 caracters para o nome de cada arwquivo, 8 para nome 1 para o ponto e 3 para extensao
 * - 2^12 clusters  = 4.096 (4K) clusters ou unidades de alocação ( que são um um grupo de setores (do disco))
 * - cluster sizes 512b to 4k
 * - 2GB tamanho maximo da particao, por isso ainda usado em pen-drives cartoes ssd etc.
 * - Os volumes FAT não diferenciam maiúsculas de minúsculas.
 * artigo sobre discos que fala algo sobre a fat:
 * http://www.clubedohardware.com.br/artigos/Limites-de-Capacidade-dos-Discos-Rigidos/1420/3
 * trabalho de SO similar, porem cobra a parte dos arquivos:
 * http://asc.di.fct.unl.pt/~vad/SO/SO-0708/trab-fat.html
 * muito bom: The FAT File System
 * http://www.c-jump.com/CIS24/Slides/FAT/lecture.html
 * tem alguma coisa na wiki tbem:
 * http://en.wikipedia.org/wiki/File_Allocation_Table
 * otimo site para aprender c++:
 * http://www.cplusplus.com/doc/tutorial/files/
 * @author pargles and abilio
 * File:   testeme.cpp
 * Created on 7 de Junho de 2012, 11:02
 */

void printaItensSelecionados(Parser *argumentos) {
    cout << "Parametros" << endl;
    cout << endl; //pula linha
    cout << "Verficar Fats: " << ((argumentos->verificaFats()) ? "sim" : "nao") << endl;
    cout << "Imprimir Blocos Livres: " << (argumentos->blocosLivres() == 1 ? "sim" : "nao") << endl;
    cout << "Imprimir Blocos Com Dados: " << (argumentos->blocoComDados() == 1 ? "sim" : "nao") << endl;
    cout << "Ler Determinado Bloco: " << (argumentos->lerBloco() == 1 ? "sim" : "nao") << endl;
    cout << "Bloco a ser listado: " << (argumentos->lerBloco() == 1 ? argumentos->getNumeroDoBloco() : -1) << endl; //<<<====procurar como funciona funcao atoa
    cout << endl;
}

//-in /home/pargles/Documentos/disco -inf -vf -bl -la 12
int main(int argc, char * argv[]) {
	Parser *comandos = new Parser();
	FatAnalyser *fatAnalyzer;

	vector<string> args; //coloca em um vetor todos os argumentos para posteriormente passar para o parcer
	
	for(int i = 0; i < argc; i++) // le cada argumento e coloca no vetor de string
		args.push_back(argv[i]); //adiciona elemento no final
		
	comandos->parse(args); // chama o parser passando o vetor com os argumentos
        if(comandos->listarInformacoes())
        {
            printaItensSelecionados(comandos);
        }
	fatAnalyzer = new FatAnalyser(comandos);
	fatAnalyzer->run();
	return 0;
}
