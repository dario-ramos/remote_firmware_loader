#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>

class Thread{
protected:
	pthread_t hilo;
	bool corriendo;
	int status; // Para reportar errores surgidos en el thread
public:
	Thread() : corriendo(false), status(0) {}
	// Lanza el thread
	virtual void run(){
		if (pthread_create(&hilo, NULL, static_run, this) == -1) 
			status = 6;
		else corriendo = true;
	}
	// Pide al thread que finalice sus tareas lo antes posible
	virtual void stop() = 0;
	// Indica si el thread está corriendo o no
	virtual bool running() const { return corriendo; }
	// Espera a que el thread termine realmente
	virtual void unir() { pthread_join(hilo, NULL); }
	// Espera que el thread termine y lo libera
	virtual ~Thread() {
		pthread_join(hilo, NULL);
		pthread_kill(hilo, 0);
	}
	// Duerme el thread actual una cierta cantidad de milisegundos
	void sleep(unsigned long msecs) {
	//No está implementado en pthread.h, y como no lo uso no
	//lo implementé
	}
	int getStatus() { return status; }
	int setStatus(int nuevoStatus) { status = nuevoStatus; }
protected:
	// Para que las subclases redefinan
	virtual void main() = 0;
private:
	// Ejecuta el thread (su argumento siempre será this)
	static void* static_run(void* arg){
		(static_cast<Thread*>(arg))->main();
		return NULL;
	}
	// Constructor de copia y operator= ocultos
	Thread(const Thread&);
	Thread& operator=(const Thread&);
};

#endif
