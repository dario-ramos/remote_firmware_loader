#include "socket.h"

struct sockaddr_in Socket::getipa(const string& hostname, int port){
		struct sockaddr_in ipa;
		ipa.sin_family = AF_INET;
		ipa.sin_port = htons(port);
	
		struct hostent* localhost = gethostbyname(hostname.c_str());
		if(!localhost){
			valido = false;
			return ipa;
		}
		char* addr = localhost->h_addr_list[0];
		memcpy(&ipa.sin_addr.s_addr, addr, sizeof addr);
	
		return ipa;
}
Socket::Socket(const string& host, port_type port) : valido(true){
        struct protoent* tcp;
        tcp = getprotobyname("tcp");
        sockFd = socket(PF_INET, SOCK_STREAM, tcp->p_proto);
        if(sockFd == -1) valido = false;
        struct sockaddr_in isa = getipa(host.c_str(), port);
        if(connect(sockFd, (struct sockaddr*)&isa, sizeof isa) == -1)
                valido = false;
}
Socket::Socket(port_type port, const string& nom, int backlog) : valido(true){
        struct protoent* tcp;
        tcp = getprotobyname("tcp");
        sockFd = socket(PF_INET, SOCK_STREAM, tcp->p_proto);	
        if(sockFd == -1) valido = false;
        struct sockaddr_in isa = getipa(nom, port);
        if(bind(sockFd, (struct sockaddr*)&isa, sizeof isa) == -1)
            valido = false;
        if(listen(sockFd, backlog) == -1)
		    valido = false;
}
int Socket::enviar(const string& buf){
	int numBytes;
	if ( (numBytes = send(sockFd, buf.c_str(), (unsigned int)buf.size()+1, MSG_NOSIGNAL)) == -1){
		valido = false;
		return -1;
	}
	return numBytes;
}
int Socket::recibir(string& buf, size_t longitud){
	char *buffer;
	buffer = (char*)malloc(longitud*sizeof(char));
	int bytesEnviados = recv(this->sockFd,buffer,longitud,0);
	string temp(buffer,longitud);
	buf = temp;
	buf = buf.substr(0, bytesEnviados-1);
	free(buffer);
	return bytesEnviados;
}
Socket* Socket:: aceptar(){
	return new Socket(sockFd);
}
Socket::operator bool () const{
	return valido;
}
Socket::~Socket(){
	shutdown(sockFd, SHUT_RDWR);
	close(sockFd);
}
Socket::Socket(int fd) : valido(true){
	sockFd = accept(fd, NULL, NULL);
	if(sockFd == -1) valido = false;
}

