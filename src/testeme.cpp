#include "../inc/Parser.h"
#include "../inc/FatAnalyser.h"
#include <iostream>

/*
 * @brief trabalho 2 de SO disponivel em: https://docs.google.com/a/inf.ufpel.edu.br/viewer?a=v&pid=sites&srcid=aW5mLnVmcGVsLmVkdS5icnxwaWxsYXxneDo1YjgxMTE3MjFhY2U1OTA4
 * @author pargles and abilio
 * File:   testeme.cpp
 * Created on 7 de Junho de 2012, 11:02
 */

int main(int argc, char * argv[]) {
	Parser * comandos = new Parser();
	FatAnalyser * fatAnalyzer;

	vector<string> args; //coloca em um vetor todos os argumentos para posteriormente passar para o parcer
	
	for(int i = 0; i < argc; i++) // le cada argumento e coloca no vetor de string
		args.push_back(argv[i]);
		
	comandos->parse(args); // chama o parser passando o vetor com os argumentos
	
	cout << "Parametros" << endl;
	cout << endl;//pula linha
	
	cout << "Verficar Fats: " << ((comandos->verificaFats()) ? "sim":"nao") << endl;
	cout << "Imprimir Blocos Livres: " << (comandos->blocosLivres() == 1 ? "sim":"nao") << endl;
	cout << "Imprimir Blocos Com Dados: " << (comandos->blocoComDados() == 1 ? "sim":"nao") << endl;
	cout << "Ler Determinado Bloco: " << (comandos->lerBloco() == 1 ? "sim":"nao") << endl;
	cout << "Bloco a ser listado: " << (comandos->lerBloco() == 1 ? "10":"----") << endl;//procurar como funciona funcao atoa
	cout << endl;
	
	fatAnalyzer = new FatAnalyser(comandos);
	
	fatAnalyzer->run();
	
	return 0;
}
