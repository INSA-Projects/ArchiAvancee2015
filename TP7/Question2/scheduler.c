#include "scheduler.h"
#include "ctxsw.h"
#include "thread.h"
#include "time.h"
#include "interrupt.h"
#include <stdio.h>

bool reschedule()
{
  return false;
}

bool ready(int thread_id, bool resched)
{
  thread* threadptr;
  
  if (is_bad_thread_id(thread_id) || (threadptr = &thread_table[thread_id])->state == FREE)
    return false;

  threadptr->state = READY;
  ready_list = insert_list(ready_list, threadptr->priority, thread_id);
  if (resched) reschedule();
  return true;
}
