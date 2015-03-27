#include "time.h"
#include "scheduler.h"
#include "interrupt.h"
#include "sleep.h"
#include <stdio.h>

// appel� toutes les 10ms
void clock_handler(int sig)
{
	preempt -= 1; // preempt est initialis� � 10 pour le thread courant
	if (preempt <= 0)
	{
		reschedule();
	}
}
