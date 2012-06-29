#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

using namespace std;

/**
	@brief Responsável por separar os parametros que vao ser informados quando o programa for chamado.
	@author pargles and abilio
*/ 
class Parser {

	private:
            bool vf;//1.1
            bool bl;//1.2
            bool bd;//1.3
            bool lb;//1.4
            bool la;
            bool inf;
            bool tab;
            int numeroBloco;//1.4
            int numeroBlocoArquivo;
            string enderecoSistemaArquivos;

	public:
		Parser();

                bool verificaFats();//1.1
                bool blocosLivres();//1.2
                bool blocoComDados();//1.3
                bool lerBloco();//1.4
                bool listarArquivo();
                bool listarInformacoes();
                bool listarTabelaDiretorios();

                int getNumeroDoBloco();
                int getNumeroBlocoArquivo();
                string getEnderecoSistemaArquivos();

                void setNumeroDoBloco(int numeroBloco);
                void setEnderecoSistemaArquivos(string Particao);
		
		void parse(vector<string> Arguments);

};
