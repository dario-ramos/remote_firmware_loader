#include "cliente.h"
#include "socket.h"
#include <string>
#include <fstream>
#define TAMBUFFER 1024

void Cliente::stop(){
	;
}
void Cliente::main(){
	if(sockCli){
		ifstream streamEntrada;
		streamEntrada.open(nomArchS19, ios::in);
		if (streamEntrada.good()){
			string bufferOut, bufferIn;
			// MAL : while(!f.eof && f.good())
			for(getline(streamEntrada, bufferOut); !streamEntrada.eof(); getline(streamEntrada,bufferOut)){ 
				// Si estoy enviando el último registro, el servidor debe saberlo.
				// Para ello, le agrego una marca especial al final (U).
				char c = streamEntrada.peek();
				if (c== EOF) bufferOut.append("U");
				sockCli.enviar(bufferOut); // MAL : Verificar que se envía todo
				// Espero confirmación de llegada
				sockCli.recibir(bufferIn, TAMBUFFER);
				if (bufferIn != "OK") break;
			}
			sockCli.enviar("FIN"); // MAL : "Hardcodeos"
			streamEntrada.close();
		}else{
			status = 1; // Error de archivos
			sockCli.enviar("1"); // Le aviso al servidor que deje de escuchar
		}
	}else{
		status = 5; // Error de sockets
		sockCli.enviar("FIN"); // Le aviso al servidor que deje de escuchar
	}
}
