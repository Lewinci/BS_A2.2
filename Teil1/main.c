#include <stdbool.h>
#include <stdio.h>
#include "fifo.h"
#include "control.h"


int main() {
#ifdef M1
    printf("Programm mit M1 gestartet.\n");
#endif
#ifdef M2
    printf("Programm mit M2 gestartet.\n");
#endif

    init_fifo();
    control_loop();

    return 0;
}