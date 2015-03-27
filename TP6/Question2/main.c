#include <stdio.h>
#include "thread.h"

void fct1(long t)
{
	join(t); // attend la fin de t2
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
	// ce main est aussi un thread
	
	resume(t1); // lance t1, qui attend la fin de t2
  resume(t2);	// lance t2
				// t2 s'affiche, meurt, donc t1 s'affiche
	join(t1);	// attend la fin de t1
  printf("fin du main\n");
  return 0;
}
