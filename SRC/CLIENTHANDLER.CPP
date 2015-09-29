#include "clienthandler.h"
#include "constantes.h"
#include "firmware.h"
#include "string.h"
#include <iostream>
using namespace std;

void ClientHandler::stop(){
	pthread_cancel(hilo);
}
void* ClientHandler::main(){
	// Creo una instancia de firmware, para traducir a binario 
	// los registros S19 que van llegando del cliente
	Firmware f(nomArchBin);
	if(f){
		string buffer = "";
		while(buffer != Constantes::FIN && buffer != Constantes::ERROR){
			int bytesRecibidos = pSockCliente->recibir(buffer, Constantes::TAM_BUFFER);
			if (bytesRecibidos>0 && buffer!= Constantes::FIN){
				int error = 0;
				if (buffer != Constantes::ERROR)
					error = f.procesarLinea(buffer);
					// Si el archivo presenta algún error, se aborta la
					// transmisión. 
					if (error != 0){
						f.eliminarArchSalida();
						status = error;
						avisarErrorAThreadPadre(error);
						pSockCliente->enviar(Constantes::FIN);
						break;
					}
				if (buffer == Constantes::FIN)
					f.eliminarArchSalida();
				// Confirmo la recepción
				pSockCliente->enviar(Constantes::OK);
			}
		}
	}else{
		status = 1; // Error de archivos
		pSockCliente->enviar(Constantes::ERROR);
	}
	return &status;
}
void ClientHandler::avisarErrorAThreadPadre(int error){
	pThreadPadre->setStatus(error);
}
