
#include <pcosynchro/pcosemaphore.h>

#define CARWEIGHT 1
#define TRUCKWEIGHT 10


class BridgeManager
{
    const uint MAX_WEIGHT;
    /// Semaphore to make sure we never overload the bridge
    PcoSemaphore* sem_bridgeAccess;
    /// Semaphore to protect the access to the sem_bridgeAccess for trucks
    /// We don't want 2 trucks blocking each others
    PcoSemaphore* sem_loading;
public:
    BridgeManager(unsigned int maxWeight) : MAX_WEIGHT(maxWeight)
    {
        sem_bridgeAccess = new PcoSemaphore(MAX_WEIGHT);
        sem_loading = new PcoSemaphore(1);
    }

    ~BridgeManager()
    {
        delete sem_bridgeAccess;
        delete sem_loading;
    }

    void carAccess()
    {
        sem_bridgeAccess->acquire();
    }

    void truckAccess()
    {
        // We only load trucks at la queuleuleu
        sem_loading->acquire();
        for(uint i = 0; i < TRUCKWEIGHT; ++i){
            sem_bridgeAccess->acquire();
        }
        sem_loading->release();
    }

    void carLeave()
    {
        sem_bridgeAccess->release();
    }

    void truckLeave()
    {
        for(uint i = 0; i < TRUCKWEIGHT; ++i){
            sem_bridgeAccess->release();
        }
    }
};
