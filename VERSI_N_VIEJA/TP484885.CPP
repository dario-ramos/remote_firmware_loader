#include <iostream>
#include "cliente.h"
#include "servidor.h"
#define PUERTO 3550
using namespace std;

static void reportarError(int codError){
	switch(codError) {
		case 1: cout << "Error de Acceso a Archivos" << endl;
			break;
		case 2: cout << "Error de Checksum" << endl;
			break;
		case 3: cout << "Error de Formato" << endl;
			break;
		case 5: cout << "Error de Sockets" << endl;
			break;
		case 6: cout << "Error al Crear Thread" << endl;
			break;
	}
}
int main(int argc, char* argv[]){
	// Validación de parámetros
	if (argc != 3) {
		cout << "Error de sintaxis" << endl;
		return 4;
	}
	Servidor s(argv[2], PUERTO, "localhost");
	Cliente c(argv[1], s.getNomServidor(), PUERTO);
	s.run();
	c.run();
	c.unir();
	s.stop();
	s.unir();
	if (c.getStatus() != OK){
		reportarError(c.getStatus());
		return c.getStatus();
	}
	if (s.getStatus() != OK){
		reportarError(s.getStatus());
		return s.getStatus();
	}
	cout << "Ejecucion Exitosa" << endl;
	return 0;
}
