#pragma once

#include "../inc/Parser.h"
#include "../inc/Fat.h"
#include <iostream>
#include <string>
#include <cmath>

/**
 *@file FatAnalyser.h
 *@brief Contem os metodos para analisar as particoes FAT12
 *
 *@author Pargles
 *@author Abilio
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
