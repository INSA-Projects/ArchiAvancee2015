#include "thread.h"
#include "utility.h"
#include "ctxsw.h"
#include "scheduler.h"
#include "semaphore.h"
#include "interrupt.h"
#include "time.h"
#include "sleep.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

void initialize_thread()
{
  int i;
  for (i = 0; i < MAX_NB_THREAD; i++) 
    {
      thread_table[i].state = FREE;
      thread_table[i].semaphore = -1;
      thread_table[i].join_list = EMPTY_LIST;
      thread_table[i].join_thread = -1;
      thread_table[i].received = false;
    }
}

int free_slot_thread_table()
{
  int i;
  for (i = 1; i < MAX_NB_THREAD; i++)
    if (thread_table[i].state == FREE) return i;
  return -1;
}

void enable_interrupt() 
{
  thread* threadptr = &thread_table[current_thread];
  enable();
  long rdi, rsi, rdx, rcx, r8, r9;
  rdi = threadptr->registers[REGISTER_TO_INDEX(RDI)];
  rsi = threadptr->registers[REGISTER_TO_INDEX(RSI)];
  rdx = threadptr->registers[REGISTER_TO_INDEX(RDX)];
  rcx = threadptr->registers[REGISTER_TO_INDEX(RCX)];
  r8 = threadptr->registers[REGISTER_TO_INDEX(R8)];
  r9 = threadptr->registers[REGISTER_TO_INDEX(R9)];
  asm("movq %0, %%rdi\n\t"
      "movq %1, %%rsi\n\t"
      "movq %2, %%rdx\n\t"
      "movq %3, %%rcx\n\t"
      "movq %4, %%r8\n\t"
      "movq %5, %%r9\n\t"
      ::"g"(rdi), "g"(rsi), "g"(rdx), "g"(rcx), "g"(r8), "g"(r9));
}

/*
Lorsque la proc�dure repr�sentant le code d'un thread se termine,
la proc�dure userret est appell�e pour, entre autre, passer la main � un autre thread.
*/
void userret()
{
  destroy_thread(get_thread_id());
}

void display_stack(int thread_id)
{
  status old = disable();
  if (is_bad_thread_id(thread_id) || thread_table[thread_id].state == FREE)
    {
      restore(old);
      return;
    }

  int i;
  for (i = 0; i < STACK_SIZE; i++)
    {
      printf("index %d : %ld\n", i, thread_table[thread_id].stack[i]);
    }
  restore(old);
}

int paramater_index_to_register_index(int paramater_index)
{
  switch (paramater_index) 
    {
    case 0: return REGISTER_TO_INDEX(RDI);
    case 1: return REGISTER_TO_INDEX(RSI);
    case 2: return REGISTER_TO_INDEX(RDX);
    case 3: return REGISTER_TO_INDEX(RCX);
    case 4: return REGISTER_TO_INDEX(R8);
    case 5: return REGISTER_TO_INDEX(R9);
    default: return -1;
    }
}

/*
la pile initiale du thread doit avoir la forme suivante :

low address

   +-----------------+
   | enable_interrupt|  <-- %rsp
   +-----------------+
   | initial_address |
   +-----------------+
   | userret         |
   +-----------------+ 

high address
*/
int create_thread(long initial_address, int priority, char* name, int nb_args, ...)
{
  // � compl�ter
  return -1;
}

bool destroy_thread(int thread_id)
{
  thread* threadptr;
  semaphore* semptr;

  status old = disable();

  if (is_bad_thread_id(thread_id) || (threadptr = &thread_table[thread_id])->state == FREE)
    {
      restore(old);
      return false;
    }
  
  int state = threadptr->state;
  threadptr->state = FREE;
  free(threadptr->name);  
  
  alive_list = remove_list(alive_list, thread_id);

  while(!is_empty_list(threadptr->join_list))
    {
      int id = get_first_element(threadptr->join_list);      
      ready(id,false);
      threadptr->join_list = remove_list(threadptr->join_list, id);
    }

  nb_thread--;

  switch (state)
    {
    case CURRENT:
      reschedule();
      break;

    case READY:
      ready_list = remove_list(ready_list, thread_id);
      break;

    case WAITING:
      semptr = &semaphore_table[threadptr->semaphore];
      semptr->count++;
      semptr->waiting_list = remove_list(semptr->waiting_list, thread_id);
      threadptr->semaphore = -1;
      break;

    case ASLEEP:
      sleep_list = remove_list(sleep_list, thread_id);
      break;

    case JOIN:
      thread_table[threadptr->join_thread].join_list = 
	remove_list(thread_table[threadptr->join_thread].join_list, thread_id);
      threadptr->join_thread = -1;
      break;
    }
  restore(old);
  return true;
}

bool resume(int thread_id)
{
  status old = disable();
  if (is_bad_thread_id(thread_id) || thread_table[thread_id].state != SUSPENDED)
    {
      restore(old);
      return false;
    }

  ready(thread_id, true);
  restore(old);
  return true;
}

bool suspend(int thread_id)
{
  thread* threadptr;
  status old = disable();

  if (is_bad_thread_id(thread_id) || (threadptr = &thread_table[thread_id])->state != CURRENT
      || threadptr->state != READY)
    {
      restore(old);
      return false;
    }

  if (threadptr->state == READY)
    {
      ready_list = remove_list(ready_list, thread_id);
      threadptr->state = SUSPENDED;
    }
  else
    {
      threadptr->state = SUSPENDED;
      reschedule();
    }

  restore(old);
  return true;
}


void yield()
{
  status old = disable();
  reschedule();
  restore(old);
}

/*
Le thread courant est mis en attente jusqu'� la mort du
thread thread_id.
*/
bool join(int thread_id)
{  
  status old = disable();
  if (is_bad_thread_id(thread_id) || thread_table[thread_id].state == FREE)
    {
      restore(old);
      return false;
    }

  thread_table[thread_id].join_list = 
    insert_list(thread_table[thread_id].join_list, thread_table[current_thread].priority, current_thread);

  thread_table[current_thread].state = JOIN;
  thread_table[current_thread].join_thread = thread_id;

  reschedule();
  restore(old);
  return true;
}

int get_priority(int thread_id)
{
  status old = disable();
  int priority;

  if (is_bad_thread_id(thread_id) || thread_table[thread_id].state == FREE)
    {
      restore(old);
      return -1;
    }
  priority = thread_table[thread_id].priority;
  restore(old);
  return priority;
}
