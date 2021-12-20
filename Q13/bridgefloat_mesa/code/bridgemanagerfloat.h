
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>

class BridgeManagerFloat
{
    PcoMutex mutex;
    PcoConditionVariable cond;

    float currentWeight;
    float maxWeight;

public:
    BridgeManagerFloat(float maxWeight) : currentWeight(0), maxWeight(maxWeight), mutex(), cond() {}

    void access(float weight)
    {
        mutex.lock();
        // Evaluate if we can access
        while (currentWeight + weight > maxWeight) {
            cond.wait(&mutex);
        }
        // Access...
        currentWeight += weight;
        mutex.unlock();
    }

    void leave(float weight)
    {
        mutex.lock();
        currentWeight -= weight;
        // The weight of the leaving vehicle
        // could allow multiple smaller vehicles to access.
        cond.notifyAll();
        mutex.unlock();
    }
};
