#ifndef CONTROL_H
#define CONTROL_H

int init(int thread);

void control_loop();

void print_usage();

void kill(int i);

void kill_all();

#endif // CONTROL_H