#include <stdio.h>
#include "thread.h"

void fct(long n)
{
  while (1) {
    printf("%ld\n",n);
  }
}

int main()
{
  printf("début du main\n");
  resume(create_thread((long)fct, DEFAULT_PRIORITY, "thread1", 1, 1));
  resume(create_thread((long)fct, DEFAULT_PRIORITY, "thread2", 1, 2));
  resume(create_thread((long)fct, DEFAULT_PRIORITY, "thread3", 1, 3));
  printf("fin du main\n");
  return 0;
}
