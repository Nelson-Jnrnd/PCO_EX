#include <pcosynchro/pcosemaphore.h>
#include <iostream>


class MutexFromSem
{
protected:
    int lockDepth = 1;
    PcoSemaphore* sephlock;
public:
    MutexFromSem()
    {
        sephlock = new PcoSemaphore(lockDepth);
    }

    ~MutexFromSem()
    {
        delete sephlock;
    }

    void lock()
    {
        if(trylock()){
            lockDepth--;
            sephlock->acquire();
        }
    }

    void unlock()
    {
        if(lockDepth < 1){
            sephlock->release();
            lockDepth++;
        }
    }

    bool trylock()
    {
        return (lockDepth >= 1);
    }  //! Returns true if the mutex can be acquired, false if it is already locked
};
