// std includes
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <getopt.h>

#include "log.h"
#include "cryptoexchange.h"
#include "producer.h"
#include "consumer.h"
#include "shared.h"
// thread includes
#include <thread>
#include <semaphore.h>

using namespace std;

int main(int argc, char **argv)
{
    SHARED shared;
    void *arg = &shared;
    int opt;
    // default parameters;
    shared.total_trade_requests = 100; // for param -r N
    shared.x_consume_time = 0;         // param for -x N
    shared.y_consume_time = 0;         // param for -y N
    shared.bitcoin_publish_time = 0;   // param for -b N
    shared.ethereum_publish_time = 0;  // param for -e N

    // parsing, setting and handling optional parameters:
    // -r N Total number of trade requests (production limit). Default is 100 if
    // not specified.

    // -x N Specifies the number of milliseconds N that the consumer using
    // Blockchain X requires for processing a trade request and
    // completing its transaction. You would simulate this time to consume
    // a request by putting the consumer thread to sleep for N
    // milliseconds. Other consumer and producer threads (consuming
    // over Blockchain Y, producing Bitcoin request, and producing
    // Ethereum request) are handled similarly.

    // -y N Similar argument for consuming over Blockchain Y.

    // -b N Specifies the number of milliseconds required to produce and
    // publish a Bitcoin request.

    // -e N Specifies the number of milliseconds required to produce and
    // publish an Ethereum request
    while ((opt = getopt(argc, argv, "r:x:y:b:e:")) != -1)
    {
        switch (opt)
        {
        case 'r':
            shared.total_trade_requests = atoi(optarg);
            break;
        case 'x':
            shared.x_consume_time = atoi(optarg);
            break;
        case 'y':
            shared.y_consume_time = atoi(optarg);
            break;
        case 'b':
            shared.bitcoin_publish_time = atoi(optarg);
            break;
        case 'e':
            shared.ethereum_publish_time = atoi(optarg);
            break;
        default: /* '?' */
            std::cerr << "incorrect parameters etc: " << argv[0] << " [-r N] [-x N] [-y N] [-b N] [-e N]\n";
            return EXIT_FAILURE;
        }
    }
    // Print out the parsed parameters
    // std::cout << "Parsed parameters:\n";
    // std::cout << "-r: " << shared.total_trade_requests << "\n";
    // std::cout << "-x: " << shared.x_consume_time << "\n";
    // std::cout << "-y: " << shared.y_consume_time << "\n";
    // std::cout << "-b: " << shared.bitcoin_publish_time << "\n";
    // std::cout << "-e: " << shared.ethereum_publish_time << "\n";

    // declarations:
    shared.current_trade_requests = 0;
    shared.produced[Bitcoin] = 0;
    shared.produced[Ethereum] = 0;

    shared.total_consumed = new unsigned int *[2];
    for (int i = 0; i < 2; i++)
    {
        shared.total_consumed[i] = new unsigned int[2];
    }
    shared.total_consumed[BlockchainX][Bitcoin] = 0;
    shared.total_consumed[BlockchainX][Ethereum] = 0;
    shared.total_consumed[BlockchainY][Bitcoin] = 0;
    shared.total_consumed[BlockchainY][Ethereum] = 0;

    shared.inRequestQueue[Bitcoin] = 0;
    shared.inRequestQueue[Ethereum] = 0;

    // sem_t empty, full, mutex;

    sem_init(&shared.mutex, 0, 5);
    sem_init(&shared.empty_sem, 0, BUFFER_SIZE);
    sem_init(&shared.full_sem, 0, 0);

    // producer threads
    // shared.current_producer_type = Bitcoin;
    std::thread producer_bitcoin_thread(&producer, arg, Bitcoin);

    // sem_wait(&shared.mutex);
    shared.current_producer_type = Ethereum;
    std::thread producer_ethereum_thread(&producer, arg, Ethereum);

    // consumer threads
    // sem_wait(&shared.mutex);
    shared.current_consumer_type = BlockchainX;
    std::thread consumer_BlockchainX_thread(&consumer, arg, BlockchainX);
    std::thread consumer_BlockchainY_thread(&consumer, arg, BlockchainY);

    // sem_wait(&shared.mutex);
    // shared.current_consumer_type = BlockchainY;
    // std::thread consumer_BlockchainY_thread(&consumer, arg);
    cout << "test 1" << endl;
    producer_bitcoin_thread.join();
    cout << "test 2" << endl;

    producer_ethereum_thread.join();
    cout << "test 3" << endl;

    consumer_BlockchainX_thread.join();
    cout << "test 4" << endl;

    consumer_BlockchainY_thread.join();

    log_production_history(shared.produced, shared.total_consumed);
// 9 20
}