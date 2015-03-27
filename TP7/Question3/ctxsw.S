#include "ctxsw.h"	
	
/* 
void ctxsw(long* old_registers, long* new_registers)

Au début de cette procédure, la pile ressemble à :

+-----------------------------------------------------------------+
| @ retour (dans reschedule ou début du code de enable_interrupt) | <-- %rsp
+-----------------------------------------------------------------+
| ...                                                             |
+-----------------------------------------------------------------+
*/

        .globl  ctxsw
ctxsw:
	# à compléter
        ret
