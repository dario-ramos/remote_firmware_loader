#ifndef LOCKING_H
#define LOCKING_H
#include "mutex.h"

class Locking{
	Mutex mut;
public:
	Locking(Mutex& m);
	~Locking();
private:
	Locking(const Locking&);
	Locking operator=(const Locking&);
};

#endif
