#include <stdio.h>
#include "thread.h"
#include "semaphore.h"

int thread_a_ok = 0;
int thread_b_ok = 0;
int thread_c_ok = 0;
0
void fct1()
{	
	
  printf("avant synchronisation\n");
	v(thread_a_ok);
	p(thread_b_ok);
	p(thread_c_ok);
  printf("après synchronisation\n");
	v(thread_b_ok);
	v(thread_c_ok);
}

void fct2()
{
  printf("avant synchronisation\n");
	v(thread_b_ok);
	p(thread_a_ok);
	p(thread_c_ok);
  printf("après synchronisation\n");
	v(thread_a_ok);
	v(thread_c_ok);
	
}

void fct3()
{
  printf("avant synchronisation\n");
	v(thread_c_ok);
	p(thread_b_ok);
	p(thread_a_ok);
  printf("après synchronisation\n");
	v(thread_b_ok);
	v(thread_a_ok);
}

int main()
{
	thread_a_ok = create_semaphore(0);
	thread_b_ok = create_semaphore(0);
	thread_c_ok = create_semaphore(0);
  resume(create_thread((long)fct1, DEFAULT_PRIORITY, "thread1", 0));
  resume(create_thread((long)fct2, DEFAULT_PRIORITY, "thread2", 0));
  resume(create_thread((long)fct3, DEFAULT_PRIORITY, "thread3", 0));
  return 0;
}
