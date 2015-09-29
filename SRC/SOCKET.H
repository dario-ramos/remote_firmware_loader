#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <arpa/inet.h>
#include <netdb.h>

#define BACKLOG 5

using namespace std;

class Socket{
	int sockFd; // Descriptor/handler
	struct sockaddr_in dirSock; // Información sobre el socket
	bool valido; // Indica si el socket está en condiciones de usarse
public:
	typedef unsigned short port_type;
	// Crea un socket que se conecta al servidor 'host', puerto 'port'
	Socket(const string& host, port_type port);
	// Crea un socket que escucha conexiones en el puerto 'port', y
	// le da un nombre específico.
	explicit Socket(port_type port, const string& nom,  int backlog = BACKLOG);
	// Envía datos por el socket
	int enviar(const string& buf);
	// Recibe datos del socket
	int recibir(string& buf, size_t len);
	// Acepta una nueva conexión
	Socket* aceptar();
	// Para chequear si el socket es válido
	operator bool () const;
	// Cierra y libera el socket
	~Socket();
private:
	// Constructor de copia y operator= ocultos (por seguridad)
	Socket(const Socket&);
	Socket& operator=(const Socket&);
	// Crea un socket a partir de un file descriptor (para el aceptar)
	Socket(int fd);
	// Auxiliar: Rellena la estructura con los datos recibidos. Si no
	// logra establecer el nombre del servidor, invalida el socket actual
	struct sockaddr_in getipa(const string& hostname, int port);
};

#endif

