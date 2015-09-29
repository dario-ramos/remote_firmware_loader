#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
#include "thread.h"
#include "socket.h"

class ClientHandler : public Thread{
	Socket* pSockCliente;
	const string nomArchBin;
	Thread* pThreadPadre; // O sea, al servidor
public:
	ClientHandler(Socket* s, const string& nomArch, Thread* pThread) : Thread(),
		pSockCliente(s), nomArchBin(nomArch), pThreadPadre(pThread) {}
	void stop();
	void* main();
	void avisarErrorAThreadPadre(int error);
	~ClientHandler() {}
};

#endif
