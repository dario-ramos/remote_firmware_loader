#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
#include "thread.h"
#include "socket.h"

class ClientHandler : public Thread{
	Socket* pSockCliente;
	const char* nomArchBin;
	Thread* pThreadPadre; // O sea, al servidor MAL : No es necesario
public:
	ClientHandler(Socket* s, const char* nomArch, Thread* pThread) : Thread(),
		pSockCliente(s), nomArchBin(nomArch), pThreadPadre(pThread) {}
	void stop();
	void main();
	void avisarErrorAThreadPadre(int error); // MAL : Los threads pueden devolver estados
	~ClientHandler() {}
};

#endif
