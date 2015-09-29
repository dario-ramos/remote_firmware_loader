#ifndef SERVIDOR_H
#define SERVIDOR_H
#include "thread.h"
#include "socket.h"
#include "clienthandler.h"
#include <vector>

class Servidor : public Thread{
	Socket sockServ;
	const char* nomArchBin;
	// Si se llama a stop y el servidor sólo está esperando una conexión,
	// se puede terminar el thread. Este atributo permite decidirlo.
	bool esperandoConexion;
	const char* nomServidor;
	// Mantengo referencias a todos los manejadores de clientes y sockets clientes
	// porque sus instancias se crean dinámicamente y debe limpiarse el heap al terminar
	vector<ClientHandler*> refsAHilosDeAtencion;
	vector<Socket*> refsASocketsClientes; // MAL : NO
public:
	Servidor(const char* nomArch, Socket::port_type port, const char* nom) :
		Thread(), sockServ(port, nom), nomArchBin(nomArch), esperandoConexion(false),
		nomServidor(nom) { }
	void stop();
	void main();
	~Servidor();
	const char* getNomServidor() { return nomServidor; }
};

#endif
