#include <stdio.h>
#include "thread.h"

void fct1(long t)
{
  printf("fct1\n");
}

void fct2()
{
  printf("fct2\n");
}

int main()
{
  int t2 = create_thread((long)fct2, DEFAULT_PRIORITY, "thread1", 0);
  int t1 = create_thread((long)fct1, DEFAULT_PRIORITY, "thread1", 1, t2);
  resume(t1);
  resume(t2);
  printf("fin du main\n");
  return 0;
}
