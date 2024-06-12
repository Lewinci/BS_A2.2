#include <stdio.h>
#include <stdbool.h>
#include "producer.h"
#include "taskQueue.h"
#include "fifo.h"
#include "myError.h"
static mqd_t producerQueue;
static bool running = true;

#define ARG_BUF_SIZE sizeof(char)
#define QUEUE_SIZE 50
#define THREAD_POOL_SIZE 5

#define QUEUE_NAME "/producer_queue"

static pthread_t threadPool[THREAD_POOL_SIZE];

int init_producer()
{
    producerQueue = createTaskQueue(QUEUE_NAME, QUEUE_SIZE, ARG_BUF_SIZE);

    for(int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        HANDLE_ERR(pthread_create(&threadPool[i], NULL, &producer, NULL));
    }

    return 0;
}

void destroy_producer()
{
    for(int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_cancel(threadPool[i]);
        pthread_join(threadPool[i], NULL);
    }
 
    closeTaskQueue(producerQueue);
    destroyTaskQueue(QUEUE_NAME);
}

void* producer_generator()
{
    char letters[26] = {};

    for (int i = 0; i < 26; i++)
    {
        letters[i] = 'a' + i;
    }

    while (true)
    {
        for (int i = 0; i < 26; i++)
        {
            printf("Producing Task %c\n", letters[i]);

            struct TaskHeader header = { produce_letter, ARG_BUF_SIZE};

            sendToTaskQueue(producerQueue, header, &letters[i], true);     

            sleep(1);
        }
    }   
}

void *produce_letter(char c)
{
    push(c);

    printf("Pushed %c\n", c);
}

void* producer()
{
    while (true) {
        struct TaskHeader header = {};

        char argBuf[ARG_BUF_SIZE] = {};

        receiveFromTaskQueue(producerQueue, &header, argBuf, ARG_BUF_SIZE);

        header.routineForTask(argBuf[0]);
    }
}