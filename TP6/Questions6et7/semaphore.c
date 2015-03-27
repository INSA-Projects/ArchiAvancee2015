#include "semaphore.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include <stdio.h>

void initialize_semaphore()
{
  int i;
  for (i = 0; i < MAX_NB_SEMAPHORE; i++)
    {
      semaphore_table[i].state = SFREE;
      semaphore_table[i].waiting_list = EMPTY_LIST;
    }
}

int free_slot_semaphore_table()
{
  int i;
  for (i = 0; i < MAX_NB_SEMAPHORE; i++)
    if (semaphore_table[i].state == SFREE) return i;
  return -1;
}

int create_semaphore(int count)
{
  semaphore* semptr;
  status old = disable();
  int sem_id = free_slot_semaphore_table();  
  if (sem_id == -1) 
    {
      restore(old);
      return -1;
    }

  (semptr = &semaphore_table[sem_id])->state = SUSED;
  semptr->count = count;
  restore(old);
  return sem_id;
}

bool destroy_semaphore(int sem)
{
  semaphore* semptr;
  status old = disable();

  if (is_bad_sem_id(sem) || (semptr = &semaphore_table[sem])->state == SFREE)
    {
      restore(old);
      return false;
    }
  
  semptr->state = SFREE;
  
  while (semptr->waiting_list != EMPTY_LIST)
    {
      int thread_id = get_first_element(semptr->waiting_list);
      thread_table[thread_id].semaphore = -1;
      ready(thread_id, false);
      semptr->waiting_list = remove_list(semptr->waiting_list, thread_id);
    }
  if (semptr->count < 0) reschedule();
  restore(old);
  return true;
}

bool p(int sem)
{
  // à compléter
  return false;
}

bool v(int sem)
{
  // à compléter
  return false;
}
