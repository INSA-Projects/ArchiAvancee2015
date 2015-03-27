#include <stdio.h>
#include "thread.h"
#include "message.h"

int t1,t2;

void fct1()
{
}

void fct2()
{
}

int main()
{
  t1 = create_thread((long)fct1, DEFAULT_PRIORITY, "thread1", 0);
  t2 = create_thread((long)fct2, DEFAULT_PRIORITY, "thread2", 0);
  resume(t1);
  resume(t2);
  return 0;
}
