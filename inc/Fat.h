#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>//Stream class to both read and write from/to files in C++
using namespace std;
/**
 *@file Fat.h
 *@brief Classe responsável pela entrada e saida de dados assim como a leitura da tabela de entradas.
 *
 *@author Pargles
 *@author Abilio
 */

/* 0-2 	    Assembly code instructions to jump to boot code (mandatory in bootable partition)
 * 3-10 	OEM name in ASCII
 * 11-12 	Bytes per sector (512, 1024, 2048, or 4096)
 * 13 	    Sectors per cluster (Must be a power of 2 and cluster size must be <=32 KB)
 * 14-15 	Size of reserved area, in sectors
 * 16 	    Number of FATs (usually 2)
 * 17-18 	Maximum number of files in the root directory (FAT12/16; 0 for FAT32)
 * 19-20 	Number of sectors in the file system; if 2 B is not large enough, set to 0 and use 4 B value in bytes 32-35 below
 * 21 	    Media type (0xf0=removable disk, 0xf8=fixed disk)
 * 22-23 	Size of each FAT, in sectors, for FAT12/16; 0 for FAT32
 * 24-25 	Sectors per track in storage device
 * 26-27 	Number of heads in storage device
 * 28-31 	Number of sectors before the start partition
 * 32-35 	Number of sectors in the file system; this field will be 0 if the 2B field above (bytes 19-20) is non-zero
 */

class Fat {
private:
    string nomeArquivo;
    int bytesPorSetor;//usually 512 bytes
    int setoresPorBloco;
    int quantidadeSetoresCabecalho;
    int numeroDeFATs;//usually 2
    int quantidadeSetoresFat;//tamanho da fat em setores
    int tamanhoCadaBloco;
    int numeroMaximoArquivos;
    ifstream Reader;//arquivo em c++
    int posicaoFat1;
    int posicaoFat2;
    int posicaoDiretorioArquivos;
    int posicaoInicialDados;
    vector <unsigned short int> vetorDeEntradasFat1;//short int tem 16 bits, vamos precisar de apenas 12 bits (FAT12)
    vector <unsigned short int> vetorDeEntradasFat2;

public:
    Fat(string enderecoArquivo);
    void setNomeArquivo(string nomeDisco);
    string getNomeArquivo();
    void fecharReader();
    void listarBloco(int numeroBloco);
    void inicializarInformacoesCabecalho();
    void listarInformacoesCabecalho();
    vector <unsigned short int> adicionarEntradas(int posicaoInicialFat, vector <unsigned short int> vetor);
    void listarEntradasFAT();
    void inserirEntradasFat();
    unsigned short int lerBytes(int posicaoInicial, int quantosBytes);
    void diferenciarFATs();
    void listarBlocosLivres();
    void listarBlocosLivresComDados();
    bool eBlocoComDados(int numeroBloco);
    void listarTabelaDiretorios();
    void imprimirArquivoCompleto(int bloco);
    void fliparBitsFAT2();
};



