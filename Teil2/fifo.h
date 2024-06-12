#ifndef FIFO_H
#define FIFO_H

/**
 * @brief This function initializes the fifo 
 *        by setting up mutexes and conditional variables or semaphores
 */
void init_fifo();

void destroy_fifo();

int push(char c);

int pop(char *c);

void cleanup(void *sem);

int enter_critical_section_push();

int leave_critical_section_push();

int enter_critical_section_pop();

int leave_critical_section_pop();


#endif // FIFO_H