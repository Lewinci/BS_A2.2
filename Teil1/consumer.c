#include "consumer.h"
#include "fifo.h"
#include <unistd.h>

void *print_letters(void)
{
    printf("print_letter started\n");

    while (true)
    {
        char c = 0;
        pop(&c);

        printf("poped letter %c\n", c);

        sleep(2);
    }
}