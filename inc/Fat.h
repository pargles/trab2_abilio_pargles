#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>//Stream class to both read and write from/to files in C++
using namespace std;
/*
        @brief Classe responsável pela entrada e saida de dados assim como a leitura da tabela de entradas.
        @author pargles and abilio
 */
/*0-2 	Assembly code instructions to jump to boot code (mandatory in bootable partition)
* 3-10 	OEM name in ASCII
* 11-12 	Bytes per sector (512, 1024, 2048, or 4096)
* 13 	Sectors per cluster (Must be a power of 2 and cluster size must be <=32 KB)
* 14-15 	Size of reserved area, in sectors
* 16 	Number of FATs (usually 2)
* 17-18 	Maximum number of files in the root directory (FAT12/16; 0 for FAT32)
* 19-20 	Number of sectors in the file system; if 2 B is not large enough, set to 0 and use 4 B value in bytes 32-35 below
* 21 	Media type (0xf0=removable disk, 0xf8=fixed disk)
* 22-23 	Size of each FAT, in sectors, for FAT12/16; 0 for FAT32
* 24-25 	Sectors per track in storage device
* 26-27 	Number of heads in storage device
* 28-31 	Number of sectors before the start partition
* 32-35 	Number of sectors in the file system; this field will be 0 if the 2B field above (bytes 19-20) is non-zero
 */
class Fat {
private:
    string nomeArquivo;
    int bytesPorSetor;//default 512 bytes
    int setoresPorCluster;
    int numeroDeFATs;//usually 2
    ifstream Reader;//arquivo em c++
    int posicaoFat1;
    int posicaoFat2;
    vector <unsigned short int> vetorDeEntradasFat1;//short int tem 16 bits, vamos precisar de apenas 12 bits (FAT12)
    vector <unsigned short int> vetorDeEntradasFat2;

public:
    Fat(string enderecoArquivo);
    void setNomeArquivo(string nomeDisco);
    string getNomeArquivo();
    void fecharReader();
    void listarClusterInicial();
    void listarCluster(int numeroCluster);
    void listarInformacoesCabecalho();
    void listarFAT();
    vector <unsigned short int> adicionarEntradas(int posicaoInicialFat, vector <unsigned short int> vetor);
    void listarEntradasFAT();
    void inserirEntradasFat();
    unsigned short int lerDoisBytes(int posicaoInicial);
    void diferenciarFATs();
    void listarBlocosLivres();
    bool eBlocoComDados(int numeroBloco);
};



