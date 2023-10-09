
#ifndef SHARED_H
#define SHARED_H

#include <pthread.h>
#include <string>
#include <iostream>
#include <queue>
#include <semaphore.h>
#include <vector>
#include <queue>

#include "log.h"
#include "cryptoexchange.h"

using namespace std; // Make sure std components accessible

typedef struct
{

    int total_trade_requests;  // for param -r N
    int x_consume_time;        // param for -x N
    int y_consume_time;        // param for -y N
    int bitcoin_publish_time;  // param for -b N
    int ethereum_publish_time; // param for -e N

    int current_trade_requests; // current number of produced trade requests

    // Array of number of requests of each type that are
    // the request queue and have not yet been consumed.
    // (inRequestQueue[Bitcoin] and inRequestQueue[Ethereum])
    unsigned int produced[2];
    unsigned int **total_consumed;

    unsigned int inRequestQueue[2];

    queue<Requests> brokerQueue;

    RequestType current_producer_type;
    ConsumerType current_consumer_type;

    // semaphor vars
    sem_t empty_sem;
    sem_t full_sem;
    sem_t mutex;

} SHARED;

#endif // SHARED_H