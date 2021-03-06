#include "servidor.h"
#include "socket.h"
#include "clienthandler.h"

void Servidor::stop(){
	//Me aseguro de terminar de atender todos los clientes
	for(unsigned int i=0; i<refsAHilosDeAtencion.size(); i++)
		refsAHilosDeAtencion[i]->unir();
	// Corto el loop que espera conexiones
	corriendo = false;
	// Pese a haber cortado el loop, puede que el thread se haya quedado
	// bloqueado con un accept. En ese caso, se lo puede terminar, ya que
	// si se llam� a este m�todo es porque no hay m�s clientes para atender.
	if (esperandoConexion) pthread_cancel(hilo); // MAL : Encapsular en pthread o bien s�lo cerrar el socket
}

void Servidor::main(){
	if(sockServ){
		
		while(corriendo){
			esperandoConexion = true; // MAL : Innecesario
			Socket* atendido = sockServ.aceptar();
			esperandoConexion = false;
			refsASocketsClientes.push_back(atendido);
			if (*atendido){
				ClientHandler* ch = new ClientHandler(atendido, nomArchBin, this);
				refsAHilosDeAtencion.push_back(ch);
				ch->run();
			}else{
				status = 5; // Error de sockets
				atendido->enviar("5"); // MAL: Hardcodeo
			}
		}
	}else status = 5;
}
Servidor::~Servidor(){
	// Limpio el heap (en este caso, por completo: no hay m�s objetos
	// creados din�micamente que ClientHandlers y Sockets atendidos.):
	for(unsigned int i=0; i<refsAHilosDeAtencion.size(); i++) // MAL : Iterator
		delete refsAHilosDeAtencion[i];
	for(unsigned int i=0; i<refsASocketsClientes.size(); i++)
		delete refsASocketsClientes[i];
}
