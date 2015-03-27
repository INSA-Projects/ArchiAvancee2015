#include <stdio.h>
#include "thread.h"
#include "semaphore.h"

int n = 0;
int sem = 0; // semaphore

int dummy() {
  unsigned int i;
  for (i = 0; i < 1000000000; i++);
  return 1;
}

void fct1()
{
    p(sem); // prise du sem
  n += dummy();
    v(sem); // liberation du sem
}

void fct2()
{
    p(sem);
  n -= dummy();
    v(sem);
}

int main()
{
  sem = create_semaphore(1); // initialisation du semaphore
  int t1 = create_thread((long)fct1, DEFAULT_PRIORITY, "thread1", 0);
  int t2 = create_thread((long)fct2, DEFAULT_PRIORITY, "thread2", 0);
  resume(t1);
  resume(t2);
  join(t1);
  join(t2);
  printf("%d\n",n);
  return 0;
}
