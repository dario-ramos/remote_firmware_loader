#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>

class Mutex{
	pthread_mutex_t mutex;
public:
	Mutex();
	int lock();
	int unlock();
	~Mutex();
private:
	Mutex(const Mutex&);
	Mutex& operator=(const Mutex&);
};

#endif
