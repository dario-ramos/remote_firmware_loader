#include "clienthandler.h"
#include "firmware.h"
#include "string.h"
#include "mutex.h"
#include "locking.h"
#define TAMBUFFER 1024
#include <iostream>
using namespace std;

// Para sincronizar el acceso al archivo de salida.
Mutex m;

void ClientHandler::stop(){
	pthread_cancel(hilo);
}
void ClientHandler::main(){
	Locking lock(m); // MAL: Se lockea todo el método
	// Creo una instancia de firmware, para traducir a binario 
	// los registros S19 que van llegando del cliente
	Firmware f(nomArchBin);
	if(f){
		string buffer = "";
		while(buffer != "FIN" && buffer !="1"){
			int bytesRecibidos = pSockCliente->recibir(buffer, TAMBUFFER);
			if (bytesRecibidos>0 && buffer!= "FIN"){
				int error = 0;
				if (buffer !="1")
					error = f.procesarLinea(buffer);
					// Si el archivo presenta algún error, se aborta la
					// transmisión. 
					if (error != 0){
						f.eliminarArchSalida();
						status = error;
						avisarErrorAThreadPadre(error);
						pSockCliente->enviar("FIN");
						break;
					}
				if (buffer == "1")
					f.eliminarArchSalida();
				// Confirmo la recepción
				pSockCliente->enviar("OK");
			}
		}
	}else{
		status = 1; // Error de archivos
		pSockCliente->enviar("1");
	}
}
void ClientHandler::avisarErrorAThreadPadre(int error){
	pThreadPadre->setStatus(error);
}
