#ifndef PCOBARRIER_H
#define PCOBARRIER_H

#include <pcosynchro/pcosemaphore.h>
class PcoBarrier
{
    PcoSemaphore sem_synchro;
    PcoSemaphore sem_mutex;
    unsigned int nbtoWait;
    unsigned int nbWaiting;
public:
    PcoBarrier(unsigned int nbToWait):sem_synchro(0), sem_mutex(1), nbtoWait(nbToWait), nbWaiting(0)
    {
    }

    ~PcoBarrier()
    {
    }

    void wait()
    {
        sem_mutex.acquire();
        if(nbWaiting < nbtoWait){
            nbWaiting++;
            sem_mutex.release();
            sem_synchro.acquire();
        }else{
            for(unsigned int i = 0; i < nbWaiting; ++i){
            sem_synchro.release();
            }
            //nbWaiting = 0;
            sem_mutex.release();
        }
    }
};

#endif // PCOBARRIER_H
