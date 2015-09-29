#ifndef SERVIDOR_H
#define SERVIDOR_H
#include "thread.h"
#include "socket.h"
#include "clienthandler.h"
#include "constantes.h"
#include <vector>
typedef vector<ClientHandler*>::const_iterator iteradorARefsHDA;
typedef vector<Socket*>::const_iterator iteradorARefsSC;

class Servidor : public Thread{
	Socket sockServ;
	const string nomArchBin;
	// Si se llama a stop y el servidor sólo está esperando una conexión,
	// se puede terminar el thread. Este atributo permite decidirlo.
	bool esperandoConexion;
	const string nomServidor;
	// Mantengo referencias a todos los manejadores de clientes y sockets clientes
	// porque sus instancias se crean dinámicamente y debe limpiarse el heap al terminar
	vector<ClientHandler*> refsAHilosDeAtencion;
	vector<Socket*> refsASocketsClientes;
public:
	Servidor(const string& nomArch, Socket::port_type port, const string& nom) :
		Thread(), sockServ(port, nom), nomArchBin(nomArch), esperandoConexion(false),
		nomServidor(nom) { }
	void stop();
	void* main();
	~Servidor();
	const string& getNomServidor() { return nomServidor; }
};

#endif
