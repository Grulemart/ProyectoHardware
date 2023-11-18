
#include "llamadas_swi.h"

uint32_t __SWI_0() { return CTIME0; }

uint32_t __SWI_1(void) { return T1TC; }