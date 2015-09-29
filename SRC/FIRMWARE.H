#ifndef FIRMWARE_H
#define FIRMWARE_H
#include <string>
#include <fstream>
#include "mutex.h"
#include "locking.h"
using namespace std;

class Firmware{
	int numReg;
	ofstream streamBin;
	const string nomArchBin;
	static Mutex mutex;
public:
	Firmware(const string& nomArch) : numReg(1), streamBin(nomArch.c_str(), ios::binary & ios::app),
		 nomArchBin(nomArch) {
		  streamBin.close(); //Lo dejo cerrado porque voy a abrir y cerrar cada vez que escriba
	}
	~Firmware() { streamBin.close(); }
	int procesarLinea(string& buffer);
	operator bool () const{ return streamBin.good(); }
	void eliminarArchSalida() {
		// Dado que voy a acceder al archivo, debo sincronizarlo
		Locking lock(mutex);
		remove(nomArchBin.c_str());
	}
private:
	int obtenerLargo(const string& reg, unsigned char& checksum);
	unsigned char decodificar(char byte1, char byte2);
	unsigned char digASCIIADecimal(char byte);
	unsigned int obtenerDir(const string& reg, unsigned char& checksum);
	int procesarDatos(const string& reg, int largo, unsigned int direccion,
			  unsigned char& checksum);
};

#endif
