#include "producer.h"
#include "fifo.h"
#include <unistd.h>

void *produce_letter(void *size)
{
    printf("produce_letter started\n");

   char start, end;

    if (*(bool*)size)
    {
        start = 'A';
        end = 'Z';
    }
    else
    {
        start = 'a';
        end = 'z';
    }

    while(true)
    {
        for (char c = start; c <= end; c++)
        {
            push(c);
            
            printf("pushed letter %c\n", c);

            sleep(2);
        }
    }
}