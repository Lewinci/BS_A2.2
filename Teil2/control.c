#include "control.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#include "producer.h"
#include "consumer.h"
#include "fifo.h"
#include "myError.h"

#define START_STOP_PRODUCER_U 'P'
#define START_STOP_PRODUCER_L 'p'
#define START_STOP_CONSUMER_U 'C'
#define START_STOP_CONSUMER_L 'c'
#define TERMINATE_U 'q'
#define TERMINATE_L 'Q'
#define HELP 'h'

static pthread_t consumerGenerator = NULL;
static pthread_t producerGenerator = NULL;
static bool consumerRunning = false;
static bool producerRunning = false;

void control_loop() {

    init_consumer();
    init_producer();

    while (true) {
        int input = getchar();

        switch (input) {
            case START_STOP_PRODUCER_U:
            case START_STOP_PRODUCER_L:
                if (initProducerGenerator()) {
                    killProducerGenerator();
                }
                break;

            case START_STOP_CONSUMER_U:
            case START_STOP_CONSUMER_L:
                if (initConsumerGenerator()) {
                    killConsumerGenerator();
                }
                break;

            case TERMINATE_U:
            case TERMINATE_L:
                kill_all();
                return;

            case HELP:
                print_usage();
                break;

            case '\n':
                break;

            default:
                printf("Invalid Input. 'h' for Usage\n");
                break;
        }
    }

}

int initProducerGenerator() {
    if (producerRunning)
    {
        return -1;
    }

    printf("Creating Producer Generator\n");
    HANDLE_ERR(pthread_create(&producerGenerator, NULL, &producer_generator, NULL));
    producerRunning = true;
    return 0;
}

int initConsumerGenerator() {
    if (consumerRunning)
    {
        return -1;
    }

    printf("Creating Consumer Generator\n");
    HANDLE_ERR(pthread_create(&consumerGenerator, NULL, &consumer_generator, NULL));
    consumerRunning = true;
    return 0;
}

void print_usage() {
    printf(
            "\tP: Start/ Stop Producer\n\
        C: Start/ Stop Consumer\n\
        T: Terminate System\n\
        h: Help\n"
    );
}

void killProducerGenerator() {
    if (!producerRunning)
    {
        return;
    }

    printf("Killing Producer Generator\n");
    
    HANDLE_ERR(pthread_cancel(producerGenerator));
    producerRunning = false;
}

void killConsumerGenerator() {
    if (!consumerRunning)
    {
        return;
    }

    printf("Killing Consumer Generator\n");
    
    HANDLE_ERR(pthread_cancel(consumerGenerator))
    consumerRunning = false;
}

void kill_all() {
    printf("Killing threads\n");
    killProducerGenerator();
    killConsumerGenerator();
    destroy_producer();
    destroy_consumer();
    destroy_fifo();
    printf("Threads killed\n");
}
