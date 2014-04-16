#include "type.h"

#define SIZE 16;
/*typedef struct  
{  
    FieldList_ * value[SIZE];  
}SymbolTable;  
SymbolTable* create_SymbolTable()  
{  
    SymbolTable* pHashTbl = (SymbolTable*)malloc(sizeof(SymbolTable));  
    memset(pHashTbl, 0, sizeof(SymbolTable));  
    return pHashTbl;  
}  */

FieldList * SymbolTable[SIZE];
unsigned int hash_pjw(char* name)
{
	unsigned int val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val;
}

int Find(char *name){
	int i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		return 0;
	}
	FieldList *p = SymbolTable[i];
	while(p!=NULL){
		if(strcmp(p.name,name) == 0)
			return 1;
	}
	return 0;
}
void INT_Insert(char *name,int value){
	int i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		SymbolTable[i] = (FieldList*)malloc(sizeof(FieldList));
		memset(SymbolTable[i],0,sizeof(FieldList));
		strcpy(SymbolTable[i].name,name);
		SymbolTable[i].type.kind = INT;
		SymbolTable[i].type.INT = value;
		SymbolTable[i].child = NULL;
	}
	FieldList *p = SymbolTable[i];
	p = p->child;
	while(p) p=p->child;
	p = (FieldList*)malloc(sizeof(FieldList));
	memset(p,0,sizeof(FieldList));
	strcpy(p.name,name);
	p.type.kind = INT;
	p.type.INT = value;
	p.child = NULL;
}
void FLOAT_Insert(char *name,float value){
	float i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		SymbolTable[i] = (FieldList*)malloc(sizeof(FieldList));
		memset(SymbolTable[i],0,sizeof(FieldList));
		strcpy(SymbolTable[i].name,name);
		SymbolTable[i].type.kind = FLOAT;
		SymbolTable[i].type.FLOAT = value;
		SymbolTable[i].child = NULL;
	}
	FieldList *p = SymbolTable[i];
	p = p->child;
	while(p) p=p->child;
	p = (FieldList*)malloc(sizeof(FieldList));
	memset(p,0,sizeof(FieldList));
	strcpy(p.name,name);
	p.type.kind = FLOAT;
	p.type.FLOAT = value;
	p.child = NULL;
}
