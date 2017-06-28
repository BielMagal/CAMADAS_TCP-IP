#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
//Constantes de entrada
#define PORTA argv[2]
#define FILESERVER argv[3]
using namespace std;
ifstream openfile;
ofstream resposta;
FILE *mensagemfile, *openfile2;

int main(int argc, char **argv) {
	char resultcode[50], linha[300] = "", result[1024] = "", metodo[20], ip[20], porta[20], hostname[40], fileserver[50];
    openfile.open ("./mensagem", std::ios::in);
    openfile >> metodo;
    openfile >> ip;
    openfile >> porta;
    openfile >> hostname;
    openfile >> fileserver;
    openfile.close();


    cout << "APLICACAO - Requisição para o arquivo " << fileserver << ".html ...\n";
	/***Le arquivo html no servidor**/
	string caminho = string("./") + string(fileserver) + string(".html");
    openfile.open (caminho.c_str(), std::ios::in);
	if (openfile.is_open()) {
    	cout << "APLICACAO - Arquivo " << fileserver << ".html encontrado...\n";
	    strcpy(resultcode, "200 OK");
	} else {
    	cout << "APLICACAO - Arquivo " << fileserver << ".html nao encontrado...\n";
	    strcpy(resultcode, "404 Not Found");
    	openfile.open ("404.html", std::ios::in);
	}

	system("rm mensagem");

    cout << "APLICACAO - Enviando resposta...\n";
	resposta.open ("./mensagem", std::ios::app);
	resposta << "HTTP/1.1 " << resultcode << endl;
	resposta << "Location: http://" << ip << ":" << porta << endl;
	resposta << "Date: " << __DATE__ << " " << __TIME__ << endl;
	resposta << "Server: Apache/2.2.22" << endl;
	resposta << "Content-Type: text/html" << endl;
	resposta << "Connection: close" << endl;
	char c = openfile.get();;
	while (openfile.good())
		{
			resposta << c;
			c = openfile.get();
		};

	openfile.close();
	resposta.close();
    return 0;
}
