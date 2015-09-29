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
	// si se llamó a este método es porque no hay más clientes para atender.
	sockServ.~Socket();
}

void* Servidor::main(){
	if(sockServ){
		
		while(corriendo){
			Socket* atendido = sockServ.aceptar();
			if (corriendo){
				refsASocketsClientes.push_back(atendido);
				if (*atendido){
					ClientHandler* ch = new ClientHandler(atendido, nomArchBin, this);
					refsAHilosDeAtencion.push_back(ch);
					ch->run();
				}else{
					status = 5; // Error de sockets
					atendido->enviar(Constantes::ERROR);
				}
			}
		}
	}else status = 5;
	return &status;
}
Servidor::~Servidor(){
	// Limpio el heap (en este caso, por completo: no hay más objetos
	// creados dinámicamente que ClientHandlers y Sockets atendidos.):
	for(iteradorARefsHDA it = refsAHilosDeAtencion.begin(); 
	    it != refsAHilosDeAtencion.end() ; it++)
		delete (*it);
	for(iteradorARefsSC it = refsASocketsClientes.begin();
	    it != refsASocketsClientes.end() ; it++)
		delete (*it);
}
