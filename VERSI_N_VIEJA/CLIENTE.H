#ifndef CLIENTE_H
#define CLIENTE_H
#include "thread.h"
#include "socket.h"
#define OK 0

class Cliente : public Thread{
	Socket sockCli;
	const char* nomArchS19; //MAL : Usar string
public:
	Cliente(const char* nomArch, const char* hostname,  //MAL : Usar string
		Socket::port_type port) : Thread(), sockCli(hostname, port), nomArchS19(nomArch) {}
	void stop();
	void main();
	~Cliente() {}
};

#endif
