#include <stdio.h>
#include "thread.h"
#include "semaphore.h"

void fct1()
{
  printf("avant synchronisation\n");
  printf("après synchronisation\n");
}

void fct2()
{
  printf("avant synchronisation\n");
  printf("après synchronisation\n");
}

void fct3()
{
  printf("avant synchronisation\n");
  printf("après synchronisation\n");
}

int main()
{
  resume(create_thread((long)fct1, DEFAULT_PRIORITY, "thread1", 0));
  resume(create_thread((long)fct2, DEFAULT_PRIORITY, "thread2", 0));
  resume(create_thread((long)fct3, DEFAULT_PRIORITY, "thread3", 0));
  return 0;
}
