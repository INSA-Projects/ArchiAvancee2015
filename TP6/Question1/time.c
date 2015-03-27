#include "time.h"
#include "scheduler.h"
#include "interrupt.h"
#include "sleep.h"
#include <stdio.h>

// appelé toutes les 10ms
void clock_handler(int sig)
{
	preempt -= 1; // preempt est initialisé à 10 pour le thread courant
	if (preempt <= 0)
	{
		reschedule();
	}
}
