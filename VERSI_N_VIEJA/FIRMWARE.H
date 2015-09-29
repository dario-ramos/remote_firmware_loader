#ifndef FIRMWARE_H
#define FIRMWARE_H
#include <string>
#include <fstream>
using namespace std;

class Firmware{
	int numReg;
	ofstream streamBin;
	const char* nomArchBin;
public:
	Firmware(const char* nomArch) : numReg(1), streamBin(nomArch, ios::binary & ios::app),
		 nomArchBin(nomArch) {}
	~Firmware() { streamBin.close(); }
	int procesarLinea(string& buffer); // MAL : const
	operator bool () const{ return streamBin.good(); }
	void eliminarArchSalida() { remove(nomArchBin); }
private:
	int obtenerLargo(const string& reg, unsigned char& checksum);
	unsigned char decodificar(char byte1, char byte2);
	unsigned char digASCIIADecimal(char byte);
	unsigned int obtenerDir(const string& reg, unsigned char& checksum);
	int procesarDatos(const string& reg, int largo, unsigned int direccion,
			  unsigned char& checksum);
};

#endif
