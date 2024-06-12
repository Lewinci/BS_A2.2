#include "fifo.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "myError.h"

#define SHARED_IN_PROCESS 0
#define BUFFER_SIZE 10

static char buffer[BUFFER_SIZE] = {0};
static int counter = 0;

static pthread_mutex_t m;
static pthread_mutexattr_t atribut;
#define M1
#ifdef M1
static sem_t b; // initialisiert mit 0
static sem_t f; // initialisiert mit BUFFER_SIZE;

void destroy_fifo() {
    pthread_mutex_destroy(&m);
    pthread_mutexattr_destroy(&atribut);
    sem_destroy(&b);
    sem_destroy(&f);
}

void init_fifo()
{
    pthread_mutexattr_init(&atribut);
    pthread_mutexattr_settype(&atribut, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&m, &atribut);
    sem_init(&b, SHARED_IN_PROCESS, 0);
    sem_init(&f, SHARED_IN_PROCESS, BUFFER_SIZE);
}

int enter_critical_section_push()
{
    sem_wait(&f);

    int old;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);

    HANDLE_ERR(pthread_mutex_lock(&m));

    return 0;
}

int leave_critical_section_push()
{
    pthread_mutex_unlock(&m);
    sem_post(&b);

    int old = 0;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);

    return 0;
}

int enter_critical_section_pop()
{
    int old;
    sem_wait(&b); // down

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);

    HANDLE_ERR(pthread_mutex_lock(&m));

    return 0;
}

int leave_critical_section_pop()
{
    pthread_mutex_unlock(&m);
    sem_post(&f); // up

    int old = 0;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);

    return 0;
}
#elifdef M2
pthread_cond_t cv_p = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv_c = PTHREAD_COND_INITIALIZER;

void destroy_fifo() {
    pthread_mutex_destroy(&m);
    pthread_mutexattr_destroy(&atribut);
    pthread_cond_destroy(&cv_c);
    pthread_cond_destroy(&cv_p);
}

void init_fifo() {
    pthread_mutexattr_init(&atribut);
    pthread_mutexattr_settype(&atribut, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&m, &atribut);
}

int enter_critical_section_push() {
    HANDLE_ERR(pthread_mutex_lock(&m));

    while (counter >= BUFFER_SIZE) {
        pthread_cond_wait(&cv_p, &m);
    }

    return 0;
}

int leave_critical_section_push() {
    pthread_cond_signal(&cv_c);

    return 0;
}

int enter_critical_section_pop() {
    HANDLE_ERR(pthread_mutex_lock(&m));

    while (counter <= 0) { pthread_cond_wait(&cv_c, &m); }

    return 0;
}

int leave_critical_section_pop() {
    pthread_cond_signal(&cv_p);
    pthread_mutex_unlock(&m);
    
    return 0;
}

void cleanup(void *mutex) {
    pthread_mutex_unlock(mutex);
}

#endif

//wie controlieren ob schon gelockt ist?
int push(char c) {
#ifdef M2
    pthread_cleanup_push(cleanup, &m);
#endif
            enter_critical_section_push();

            buffer[counter] = c;
            counter++;

            leave_critical_section_push();
#ifdef M2
    pthread_cleanup_pop(true);
#endif
    return 0;
}

int pop(char *c) {
#ifdef M2
    pthread_cleanup_push(cleanup, &m);
#endif
            enter_critical_section_pop();
            
            *c = buffer[0];

            for (int i = 1; i <= counter; i++)
            {
                buffer[i-1] = buffer[i];
            }

            counter--;

            leave_critical_section_pop();
#ifdef M2
    pthread_cleanup_pop(true);
#endif
    return 0;
}
