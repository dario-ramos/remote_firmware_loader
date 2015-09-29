#include "locking.h"

Locking::Locking(Mutex& m){   // MAL : Esto no lockea nada
	mut.lock();
}
Locking::~Locking(){
	mut.unlock();
}
