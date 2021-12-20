
#include <pcosynchro/pcohoaremonitor.h>


class BridgeManagerFloat : PcoHoareMonitor
{ 
    Condition cond;

    float currentWeight;
    float maxWeight;

public:
    BridgeManagerFloat(float maxWeight) : currentWeight(0), maxWeight(maxWeight) {}

    void access(float weight)
    {
        monitorIn();
        // Evaluate if we can access
        while(currentWeight + weight > maxWeight){
            wait(cond);
        }
        // Access...
        currentWeight += weight;
        monitorOut();
    }

    void leave(float weight)
    {
        monitorIn();
        currentWeight -= weight;
        signal(cond);
        monitorOut();
    }
};
