#ifndef BUFFER2CONSO_H
#define BUFFER2CONSO_H

#include <pcosynchro/pcosemaphore.h>

#include "abstractbuffer.h"

template<typename T> class Buffer2ConsoSemaphore : public AbstractBuffer<T> {
protected:
public:
    Buffer2ConsoSemaphore(){}

    virtual ~Buffer2ConsoSemaphore() {}

    virtual void put(T item) {
    }
    virtual T get(void) {
    }
};


#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>

template<typename T> class Buffer2ConsoMesa : public AbstractBuffer<T> {
protected:
    const unsigned NB_TIMES_ITEMS_CONSUMED = 2;

    T buffer;
    unsigned nbTimeConsumed;

    PcoMutex mutex;
    PcoConditionVariable isFree;
    PcoConditionVariable isFull;
    bool bufferFull;
public:
    Buffer2ConsoMesa() : bufferFull(false), nbTimeConsumed(0), mutex() {}

    virtual ~Buffer2ConsoMesa() {}

    virtual void put(T item) {
        mutex.lock();

        // Evaluate if we can drop our ressource
        while(bufferFull)
            isFree.wait(&mutex);

        // Put the ressource
        buffer = item;
        bufferFull = true;

        // Wake up two consumers
        isFull.notifyOne();
        isFull.notifyOne();

        mutex.unlock();
    }
    virtual T get(void) {
        T item;
        mutex.lock();

        // Evlutate if we can pick up the ressource
        while(!bufferFull)
            isFull.wait(&mutex);

        // Pick up the ressource
        item = buffer;

        // Last consumer...
        if(++nbTimeConsumed == NB_TIMES_ITEMS_CONSUMED){
            bufferFull = false;
            nbTimeConsumed = 0;
            isFree.notifyOne();
        }
        mutex.unlock();
        return item;
    }
};


#include <pcosynchro/pcohoaremonitor.h>

template<typename T> class Buffer2ConsoHoare : public AbstractBuffer<T>, public PcoHoareMonitor {
protected:
    Condition cond;
    Condition cond2;
    const unsigned NB_TIMES_ITEMS_CONSUMED = 2;

    T buffer;
    unsigned nbTimeConsumed;
    bool bufferFull;
public:
    Buffer2ConsoHoare() : bufferFull(false), nbTimeConsumed(0){}

    virtual ~Buffer2ConsoHoare() {}

    virtual void put(T item) {
        monitorIn();

        // Evaluate if we can drop our ressource
        while(bufferFull)
            wait(cond2);

        // Put the ressource
        buffer = item;
        bufferFull = true;

        // Wake up two consumers
        signal(cond);
        signal(cond);
        monitorOut();
    }
    virtual T get(void) {
        T item;
        monitorIn();

        // Evlutate if we can pick up the ressource
        while(!bufferFull)
            wait(cond);

        // Pick up the ressource
        item = buffer;

        // Last consumer...
        if(++nbTimeConsumed == NB_TIMES_ITEMS_CONSUMED){
            bufferFull = false;
            nbTimeConsumed = 0;
            signal(cond2);
        }
        monitorOut();
        return item;
    }
};


#endif // BUFFER2CONSO_H
