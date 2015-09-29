#include "locking.h"

Locking::Locking(Mutex& m){
	pMut = &m;
	m.lock();
}
Locking::~Locking(){
	pMut->unlock();
}
