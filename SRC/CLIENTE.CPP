#include "cliente.h"
#include "socket.h"
#include <string>
#include <fstream>
#include "constantes.h"

void Cliente::stop(){
	;
}
void* Cliente::main(){
	if(sockCli){
		ifstream streamEntrada;
		streamEntrada.open(nomArchS19.c_str(), ios::in);
		if (streamEntrada.good()){
			string bufferOut, bufferIn;
			while(!streamEntrada.eof() && streamEntrada.good()){
				getline(streamEntrada, bufferOut);
				// Si estoy enviando el último registro, el servidor debe saberlo.
				// Para ello, le agrego una marca especial al final (U).
				char c = streamEntrada.peek();
				if (c== EOF) bufferOut.append(Constantes::MARCA_ULT_REG);
				unsigned int tamMensaje = bufferOut.size();
				unsigned int bytesEnviados = 0;
				// Itero hasta enviar el mensaje completo
				while(bytesEnviados < tamMensaje){
					bytesEnviados += sockCli.enviar(bufferOut);
					// Espero confirmación de llegada
					sockCli.recibir(bufferIn, Constantes::TAM_BUFFER);
					if (bufferIn != Constantes::OK) break;
				}
				if (bufferIn != Constantes::OK) break;
			}
			sockCli.enviar(Constantes::FIN);
			streamEntrada.close();
		}else{
			status = 1; // Error de archivos
			sockCli.enviar(Constantes::ERROR); // Le aviso al servidor que deje de escuchar
		}
	}else{
		status = 5; // Error de sockets
		sockCli.enviar(Constantes::ERROR); // Le aviso al servidor que deje de escuchar
	}
	return &status;
}
