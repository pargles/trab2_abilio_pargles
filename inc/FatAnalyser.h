#pragma once

#include "../inc/Parser.h"

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
    void verificarParticoesFat(string particaoA, string parlticaoB);
    void imprimiListaDeBlocosLivres();
    void imprimirListaDeBlocosComDados();
    void lerBloco(int bloco);

public:

    FatAnalyser(Parser *parse);
    void run();

};
