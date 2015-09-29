#include "mutex.h"

Mutex::Mutex() {}
int Mutex::lock(){
	pthread_mutex_lock(&mutex);
}
int Mutex::unlock(){
	pthread_mutex_unlock(&mutex);
}
Mutex::~Mutex(){}


