#include <stdio.h>
#include "thread.h"

void fct(long n)
{
  while(1) {
    printf("%ld\n", n);
  }
}

int main()
{
  int t1 = create_thread((long)fct, DEFAULT_PRIORITY, "thread1", 1, 1);
  int t2 = create_thread((long)fct, DEFAULT_PRIORITY, "thread2", 1, 2);
  int t3 = create_thread((long)fct, DEFAULT_PRIORITY, "thread3", 1, 3);
  resume(t1);
  resume(t2);
  resume(t3);
  return 0;
}
