// std includes
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include "log.h"

#include "cryptoexchange.h"

// thread includes
#include <thread>
#include <semaphore.h>
using namespace std;


// producer method to be ran in thread
void* producer(void *arg,  RequestType current_type);