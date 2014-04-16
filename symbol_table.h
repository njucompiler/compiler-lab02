#include "type.h"

#define SIZE 16;
typedef struct  
{  
    type* value[SIZE];  
}SymbolTable;  

unsigned inthash_pjw(char* name)
{
	unsignedint val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val;
}
