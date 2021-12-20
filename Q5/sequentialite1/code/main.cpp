#include <iostream>
#include <algorithm>

#include <gtest/gtest.h>

#include <pcosynchro/pcologger.h>
#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcosemaphore.h>

class Checker
{
public:

    std::vector<int> executions;

    void compute(int id)
    {
        static std::mutex mutex;
        mutex.lock();
        executions.push_back(id);
        mutex.unlock();
        logger() << "Task " << id << std::endl;
    }

    bool executedBefore(int id0, int id1)
    {
        auto it0 = find(executions.begin(), executions.end(), id0);
        auto it1 = find(executions.begin(), executions.end(), id1);
        return it0 < it1;
    }

    void check()
    {
        EXPECT_TRUE(executedBefore(0, 1));
        EXPECT_TRUE(executedBefore(0, 2));
        EXPECT_TRUE(executedBefore(1, 3));
        EXPECT_TRUE(executedBefore(1, 4));
        EXPECT_TRUE(executedBefore(1, 5));
        EXPECT_TRUE(executedBefore(3, 6));
        EXPECT_TRUE(executedBefore(4, 6));
        EXPECT_TRUE(executedBefore(5, 6));
        EXPECT_TRUE(executedBefore(2, 7));
        EXPECT_TRUE(executedBefore(6, 7));
    }
};

class Step{
    /// Threads to launch during this step
    std::vector<std::unique_ptr<PcoThread>> threads;
    /// functions called by the threads
    std::vector<void(*)()> functions;
public:
    Step(std::vector<void(*)()> functions){
        this->functions = functions;
        threads = std::vector<std::unique_ptr<PcoThread>>(functions.size());
    }
    /// Launches all the threads in the steps and waits for them to finish.
    /// This function is blocking
    void StartThreads(){
        for(size_t idThread = 0; idThread < threads.size(); ++idThread){
            threads[idThread] = std::make_unique<PcoThread>(functions[idThread]);
        }
        for(size_t idThread = 0; idThread < threads.size(); ++idThread){
            threads[idThread]->join();
        }
    }
};

static Checker checker;

void t0()
{
    checker.compute(0);
}

void t1()
{
    checker.compute(1);

}

void t2()
{
    checker.compute(2);
}

void t3()
{
    checker.compute(3);
}


void t4()
{
    checker.compute(4);
}

void t5()
{
    checker.compute(5);
}

void t6()
{
    checker.compute(6);
}

void t7()
{
    checker.compute(7);
}


TEST(Sequentialite, Standard)
{
    PcoManager::getInstance()->setMaxSleepDuration(1000, PcoManager::EventType::ThreadCreation);

    Step firstStep = Step(std::vector<void(*)()>{t0});
    Step secondStep = Step(std::vector<void(*)()>{t1});
    Step thirdtep = Step(std::vector<void(*)()>{t2,t3,t4,t5});
    Step fourthStep = Step(std::vector<void(*)()>{t6});
    Step fifthStep = Step(std::vector<void(*)()>{t7});

    std::vector<Step*> threadSteps {
        &firstStep, &secondStep, &thirdtep, &fourthStep, &fifthStep
    };
    for(size_t idThread = 0; idThread < threadSteps.size(); ++idThread){
        threadSteps[idThread]->StartThreads();
    }
    checker.check();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    logger().initialize(argc, argv);
    return RUN_ALL_TESTS();
}
