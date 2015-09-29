#ifndef CLIENTE_H
#define CLIENTE_H
#include "thread.h"
#include "socket.h"

class Cliente : public Thread{
	Socket sockCli;
	string nomArchS19;
	
public:
	Cliente(string nomArch, string hostname, 
		Socket::port_type port) : Thread(), sockCli(hostname, port), nomArchS19(nomArch){}
	void stop();
	void* main();
	~Cliente() {}
};

#endif
