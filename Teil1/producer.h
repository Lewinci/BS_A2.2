
#ifndef PRODUCER_H
#define PRODUCER_H

#include <stdbool.h>
#include <stdio.h>

#define LETTER_SIZE bool
#define LOWER_CASE 0
#define UPPER_CASE 1

void *produce_letter(void *size);

#endif // PRODUCER_H