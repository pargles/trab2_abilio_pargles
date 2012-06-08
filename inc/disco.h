#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;
/*
        @brief Classe responsável pela entrada e saida de dados assim como a leitura da tabela de entradas.
        @author pargles and abilio
 */
class disco {
private:
    string nomeArquivo;
    FILE* Reader;

public:
    disco(string enderecoArquivo);
    void setNomeArquivo(string nomeDisco);
    string getNomeArquivo();
    void fecharReader();
    void listarConteudo();

};



