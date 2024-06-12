#include <stdio.h>
#include "consumer.h"
#include "taskQueue.h"
#include "myError.h"
#include "fifo.h"

#define ARG_BUF_SIZE sizeof(char)
#define QUEUE_SIZE 50
#define THREAD_POOL_SIZE 5

#define QUEUE_NAME "/consumer_queue"

static mqd_t consumerQueue;
static pthread_t threadPool[THREAD_POOL_SIZE];

int init_consumer()
{
    consumerQueue = createTaskQueue(QUEUE_NAME, QUEUE_SIZE, ARG_BUF_SIZE);

    for(int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        HANDLE_ERR(pthread_create(&threadPool[i], NULL, &consumer, NULL));
    }

    return 0;
}

void destroy_consumer()
{
    for(int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_cancel(threadPool[i]);
        pthread_join(threadPool[i], NULL);
    }
 
    closeTaskQueue(consumerQueue);
    destroyTaskQueue(QUEUE_NAME);
}

void* consumer_generator()
{
    while (true)
    {
        struct TaskHeader header = { print_letters, ARG_BUF_SIZE};

        printf("Creating consumer Task\n");

        char c; // not used

        sendToTaskQueue(consumerQueue, header, &c, true);

        sleep(1);
    }   
}

void *print_letters()
{
    char c = 0;
    pop(&c);

    printf("Poped %c\n", c);
}

void* consumer()
{
    while (true) {
        struct TaskHeader header = {};

        char c; // unused

        receiveFromTaskQueue(consumerQueue, &header, &c, ARG_BUF_SIZE);

        header.routineForTask(NULL);
    }
}