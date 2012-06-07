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
            int vf;//1.1
            int bl;//1.2
            int bd;//1.3
            int lb;//1.4
            int numeroBloco;//1.4
            string particaoA;
            string particaoB;

	public:
		Parser();

                int verificaFats();//1.1
                int blocosLivres();//1.2
                int blocoComDados();//1.3
                int lerBloco();//1.4

                int getNumeroDoBloco();
                string getParticaoA();
                string getParticaoB();

                void setNumeroDoBloco(int numeroBloco);
                void setParticaoA(string Particao);
                void setParticaoB(string Particao);
		
		void parse(vector<string> Arguments);

};
