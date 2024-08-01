#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>


//*** Mutexes are implemented corresponding to the forks ***
//*** The idea of the owner array is implemented by github.com/khashayaramini ***

const int philNum = 5;
std::mutex mutexs[philNum];
std::mutex coutMutex;

int owner[philNum];

// owner array used to make sure that
// after picking up a fork ,
// owner doesn't try to pick the fork again


void philFunction (int k, int num) {

    while (1) {

        if (owner[k] == k || mutexs[k].try_lock()){ 

            owner[k] = k; 
            // philosopher picked the first fork
            // not going to try_lock() first fork again
             

            if ( owner[(k + 1) %philNum] || mutexs[((k + 1) %philNum)].try_lock() ) {

                owner[(k + 1) % philNum] = k;
                //phil picked the second fork

                coutMutex.lock();
                
                std::cout << "Philosopher "<< k << " eats" << std::endl;
                
                coutMutex.unlock();

                std::this_thread::sleep_for(std::chrono::microseconds(500));

                mutexs[k].unlock();
                owner[k] = -1;
                
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                mutexs[(k + 1) % philNum].unlock();
                owner[(k + 1) % philNum] = -1;

            }    
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{


    std::thread phils[philNum];

    for (int i = 0; i < philNum; i++)
    {
        phils[i] = std::thread (&philFunction, i, philNum);
        //this_thread::sleep_for(chrono::milliseconds(2));
    }
    for (int i = 0; i < philNum; i++)
    {
        if (phils[i].joinable())
        {
            phils[i].join();
        }
    }

    return 0;
}

