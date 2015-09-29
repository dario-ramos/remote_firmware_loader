#include "firmware.h"
#include <ctype.h>

int Firmware::procesarLinea(string& buffer){
	bool ultimo = false;
	// Si es el último, debo asegurarme de que sea un footer. Si no
	// lo es : error de formato. Si lo es, debo quitarle la marca de
	// último para procesarlo bien.
	if ( buffer[buffer.size()-1] == 'U' ){
		ultimo = true;
		if (buffer[1] != '9') return 3;
		else buffer.erase((buffer.size()-1));
	}	
	bool ignorar;
	int largo;
     	char c, tipoReg;
     	unsigned char checksum; /* Es unsigned para sumar módulo 256 */
     	unsigned int direccion; /* La dirección es de 2 bytes->varía entre
                                 * 0 y (2^16)-1 = 65535 */
     	ignorar = false;
     	checksum = 0;
     	/* Detecto líneas en blanco y espacios al principio. */
	c = buffer[0];
	if (c == '\n' || c == ' ' || buffer == "") { return 3; }
	/* Si no es S0, S1 o S9 : Lo ignoro*/
	else if (c != 'S') ignorar = true; 
	else{
		tipoReg = buffer[1];
		/* Si es un header y no está al principio : error de formato */
		if ((tipoReg == '0') && (numReg != 1)) return 3;
		if (tipoReg != '0' && tipoReg != '1' && tipoReg != '9') ignorar = true;
		/* Si es un footer y no es el último registro: error de formato */
		if (tipoReg == '9' && !ultimo) return 3;
	}          
	if(!ignorar){ 
		/* Obtengo el largo, que varía entre 0x00 y 0xFF. De paso, hay que sumar-
		* lo al checksum. */
		largo = obtenerLargo(buffer, checksum);
		/* Ahora voy a obtener la dirección */
		direccion = obtenerDir(buffer, checksum);
		/* Proceso el campo de datos */
		int error = procesarDatos(buffer, largo, direccion, checksum);
		if (error != 0) return error;
	}
	numReg++; 
	return 0;
}
int Firmware::obtenerLargo(const string& reg, unsigned char& checksum){
	char c1, c2;
	c1 = reg[2];
	c2 = reg[3];
	checksum = decodificar(c1,c2);     
	return digASCIIADecimal(c1)*16+digASCIIADecimal(c2);
}
unsigned char Firmware::decodificar(char byte1, char byte2){
	byte1 = digASCIIADecimal(byte1);
	byte2 = digASCIIADecimal(byte2);
	return (byte1)*16+byte2;
}
unsigned char Firmware::digASCIIADecimal(char byte){
	if (isalpha(byte)) byte = toupper(byte)-55;
	else byte = byte-48;
	return byte;
}
unsigned int Firmware::obtenerDir(const string& reg, unsigned char& checksum){
	char c1,c2,c3,c4; /* Bytes de la dirección*/
		
	c1 = reg[4];     
	c2 = reg[5];
	checksum += decodificar(c1,c2);
	c3 = reg[6];     
	c4 = reg[7];
	checksum += decodificar(c3,c4);     
	return digASCIIADecimal(c1)*4096+digASCIIADecimal(c2)*256+
	       digASCIIADecimal(c3)*16+digASCIIADecimal(c4);
}
int Firmware::procesarDatos(const string& reg, int largo, unsigned int direccion,
		   unsigned char& checksum){
	int i, j;
	char c, sigC;
	unsigned char byte, checksumOriginal;
	/* largo indica la cantidad de bytes CODIFICADOS en los campos
	 * largo, datos y checksum. De ésos, 2 son de largo y 1 de checksum. 
	 * Ergo, hay que leer (largo-3)*2 chars del archivo, tomándolos de a
	 * dos para decodificarlos y escribirlos en el archivo, empezando en
	 * la posición dada por la dirección. */     
	/* Me posiciono para escribir. */
	streamBin.seekp(direccion);
	j = 8;
	for (i=1; i<=(largo-3); i++){
		c = reg[j];
		sigC = reg[j+1];
		j++; j++;
		byte = decodificar(c, sigC);
		checksum += byte;
		/* Sólo escribo los datos decodificados si el registro es S1 */
		if (reg[1] == '1') streamBin << byte;
	}
	checksum = 0xFF-checksum;
	c = reg[j];
	sigC = reg[j+1];
	checksumOriginal = decodificar(c,sigC);
	if (checksum != checksumOriginal) return 2; // Error de checksum
	else return 0;
}
