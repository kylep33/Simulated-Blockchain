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

void *consumer(void *arg, ConsumerType current_consumer)
{
    SHARED *shared = (SHARED *)arg;
    // ConsumerType current_consumer = shared->current_consumer_type;
    // sem_post(&shared->mutex);
    unsigned int consumed[2];
    consumed[Bitcoin] = 0;
    consumed[Ethereum] = 0;

    while (true)
    {
        // wait for queue to not be empty
        //  checks if the queue is empty and there will be no more trade requests added to the queue.
        if (shared->brokerQueue.empty())
        {
            cout << shared->current_trade_requests << " :test to end consumer "<< current_consumer << ": " << shared->total_trade_requests << endl;
            if (shared->current_trade_requests >= shared->total_trade_requests)
            {
                cout << current_consumer << " ending" << endl;
                return arg;
            }
        }
        sem_wait(&shared->full_sem);
        RequestType request = shared->brokerQueue.front();

        shared->brokerQueue.pop();         // probably needs to switch to pop back
        shared->inRequestQueue[request]--; // removes from inRequestQueue
        consumed[request]++;

        shared->total_consumed[current_consumer][request]++;
        log_request_removed(current_consumer, request, consumed, shared->inRequestQueue);

        // semaphor for if bitcoins requests are below 5 in quueu. if is true, unlocks mutex.
        if (request == Bitcoin && shared->inRequestQueue[Bitcoin] < 5)
        {

            sem_post(&shared->mutex);
        }
        // unlock semaphor
        sem_post(&shared->empty_sem);
        // cout << "   queue is empty: " << shared->brokerQueue.empty() << " currnt: " << shared->current_trade_requests << " total: " << shared->total_trade_requests << endl;
        //  checks if the queue is empty and there will be no more trade requests added to the queue.
        if (shared->brokerQueue.empty())
        {
            cout << shared->current_trade_requests << " :test to end consumer "<< current_consumer << ": " << shared->total_trade_requests << endl;
            if (shared->current_trade_requests >= shared->total_trade_requests)
            {
                cout << current_consumer << " : now ending" << endl;

                return arg;
            }
        }

        // Simulated wait times for x and y blockchains
        if (current_consumer == BlockchainX)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(shared->x_consume_time));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(shared->y_consume_time));
        }
    }
}