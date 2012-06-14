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
            int numeroBloco;//1.4
            string particaoA;
            string particaoB;

	public:
		Parser();

                bool verificaFats();//1.1
                bool blocosLivres();//1.2
                bool blocoComDados();//1.3
                bool lerBloco();//1.4

                int getNumeroDoBloco();
                string getParticaoA();
                string getParticaoB();

                void setNumeroDoBloco(int numeroBloco);
                void setParticaoA(string Particao);
                void setParticaoB(string Particao);
		
		void parse(vector<string> Arguments);

};
