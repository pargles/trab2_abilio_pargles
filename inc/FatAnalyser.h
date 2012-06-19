#pragma once

#include "../inc/Parser.h"
#include "../inc/Fat.h"
#include <iostream>
#include <string>
#include <cmath>

/**
        @brief Contem os metodos para analisar as particoes FAT12
        @author pargles and abilio
 */
class FatAnalyser {
private:
    Parser *parser;
    Fat *FAT;
    void verificarParticoesFat();
    void imprimiListaDeBlocosLivres();
    void imprimirListaDeBlocosComDados();
    void lerBloco(int bloco);
    void listarArquivo(int bloco);

public:

    FatAnalyser(Parser *parse);
    void run();

};
