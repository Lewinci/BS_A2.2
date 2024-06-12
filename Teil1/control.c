#include "control.h"
#include "producer.h"
#include "consumer.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fifo.h"
#include "myError.h"

#define PRODUCER_1 1
#define PRODUCER_2 2
#define CONSUMER 0
#define START_STOP_PRODUCER_1 '1'
#define START_STOP_PRODUCER_2 '2'
#define START_STOP_CONSUMER_U 'C'
#define START_STOP_CONSUMER_L 'c'
#define TERMINATE_U 'T'
#define TERMINATE_L 't'
#define HELP 'h'
#define MAX_THREADS 3

static pthread_t threads[MAX_THREADS];
static bool is_running[MAX_THREADS] = {false};
static bool arg[3] = {NULL, LOWER_CASE, UPPER_CASE};

void control_loop() {
    while (true) {
        int input = getchar();

        switch (input) {
            case START_STOP_PRODUCER_1:
                if (init(PRODUCER_1)) {
                    kill(PRODUCER_1);
                }
                break;

            case START_STOP_PRODUCER_2:
                if (init(PRODUCER_2)) {
                    kill(PRODUCER_2);
                }
                break;

            case START_STOP_CONSUMER_U:
            case START_STOP_CONSUMER_L:
                if (init(CONSUMER)) {
                    kill(CONSUMER);
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

int init(int thread) {
    if (is_running[thread]) {
        return IS_RUNNING;
    }
    if (thread) {
        HANDLE_ERR(pthread_create(&threads[thread], NULL, &produce_letter, &arg[thread]))
    } else {
        HANDLE_ERR(pthread_create(&threads[thread], NULL, &print_letters, NULL))
    }

    is_running[thread] = true;
    return 0;
}

void print_usage() {
    printf(
        "\t1: Start/ Stop Producer 1\n\
        2: Start/ Stop Producer 2\n\
        C: Start/ Stop Consumer\n\
        T: Terminate System\n\
        h: Help\n"
    );
}

void kill(int i) {
    HANDLE_ERR(pthread_cancel(threads[i]));
    HANDLE_ERR(pthread_join(threads[i], NULL));
    is_running[i] = false;
}

void kill_all() {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (is_running[i]) {
            kill(i);
        }
    }
    destroy_fifo();
}