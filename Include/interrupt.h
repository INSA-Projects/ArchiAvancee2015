#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <signal.h>
#include <sys/time.h>

typedef sigset_t status;

status disable();
void restore(sigset_t old);
void enable();

#endif
