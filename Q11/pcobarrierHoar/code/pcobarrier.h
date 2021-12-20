#ifndef PCOBARRIER_H
#define PCOBARRIER_H

#include <pcosynchro/pcohoaremonitor.h>
class PcoBarrier : PcoHoareMonitor
{

    unsigned int nbToWait;
    unsigned int nbTot;
    Condition cond;
public:
    PcoBarrier(unsigned int nbToWait) :nbToWait(nbToWait), nbTot(nbToWait)
    {
    }

    ~PcoBarrier()
    {
    }

    void wait()
    {
        monitorIn();
        nbToWait--;
        if(nbToWait > 0){
            PcoHoareMonitor::wait(cond);
        }
        /* OU à la place du else
         * Reveille un par un les threads
         * signal(cond);
         */
        else{
            // On est le dernier on reveil tout le monde
            for(unsigned i = 0; i < nbTot - 1; ++i){
             signal(cond);
            }
        }
        monitorOut();
    }
};

#endif // PCOBARRIER_H
