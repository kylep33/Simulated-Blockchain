// std includes
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include "log.h"

#include "producer.h"
#include "shared.h"

// thread includes
#include <thread>
#include <semaphore.h>
using namespace std;

void *producer(void *arg, RequestType current_type)
{
    SHARED *shared = (SHARED *)arg;
    // RequestType current_type = shared->current_producer_type; // Variable to determine if producer thread is Bitcoin or Etherium

    // sem_post(&shared->mutex);

    while (shared->current_trade_requests < shared->total_trade_requests)
    {
        // wait for queue to not be full
        // semaphor here probably

        // if (!(current_type == Bitcoin && shared->inRequestQueue[Bitcoin] >= 5))
        if ((current_type == Bitcoin))
        {
            sem_wait(&shared->mutex);
        }
        sem_wait(&shared->empty_sem);
        cout << "Producer: " << current_type << " " << shared->current_trade_requests << " : " << shared->total_trade_requests + 1 << endl;
        if (!(shared->current_trade_requests < shared->total_trade_requests)){
            //sem_post(&shared->full_sem);

            return arg;
        }
        // produces!
        shared->brokerQueue.push(current_type); // pushes to queue 
        shared->produced[current_type]++; // incriments the current types
        shared->inRequestQueue[current_type]++;
        log_request_added(current_type, shared->produced, shared->inRequestQueue);
        shared->current_trade_requests++;

        // unlock semaphor
        sem_post(&shared->full_sem);

        // sleep for the time it takes to publish a trade request
        if (current_type == Bitcoin)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(shared->bitcoin_publish_time));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(shared->ethereum_publish_time));
        }
    }

    return arg;
}