
#include <pcosynchro/pcosemaphore.h>


class BridgeManagerFloat
{
    uint nbVehiclesWaiting;
    uint maxWeight;
    uint currentCharge;

    PcoSemaphore s_synchro;
    PcoSemaphore s_mutex;
public:
    BridgeManagerFloat(float maxWeight) : nbVehiclesWaiting(0), maxWeight(maxWeight),
        currentCharge(0), s_synchro(0), s_mutex(1)
    {
    }

    ~BridgeManagerFloat()
    {
    }

    void access(float weight)
    {
        s_mutex.acquire();
        nbVehiclesWaiting++;
        // Est-ce que le pont support le poid ?
        while(currentCharge + weight > maxWeight){
            s_mutex.release();
            // Non, on attend un départ
            s_synchro.acquire();
            s_mutex.acquire();
        }
        // Oui, on entre
        nbVehiclesWaiting--;
        currentCharge += weight;
        s_mutex.release();
    }

    void leave(float weight)
    {
        // On sort
        s_mutex.acquire();
        currentCharge -= weight;
        s_mutex.release();

        // Les vehicules en attente peuvent essayer d'entrer
        for(uint i = 0; i < nbVehiclesWaiting; ++i){
            s_synchro.release();
        }
    }
};
