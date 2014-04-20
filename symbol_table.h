#include "type.h"
#include <stdlib.h>
#include <stdio.h>
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
		if(strcmp(p->name,name) == 0)
			return 1;
	}
	return 0;
}
void INT_Insert(char *name,int value){
	int i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		SymbolTable[i] = (FieldList*)malloc(sizeof(FieldList));
		memset(SymbolTable[i],0,sizeof(FieldList));
		strcpy(SymbolTable[i]->name,name);
		SymbolTable[i]->type->kind = INT;
		SymbolTable[i]->type->INT = value;
		SymbolTable[i]->child = NULL;
	}
	FieldList *p = SymbolTable[i];
	p = p->child;
	while(p) p=p->child;
	p = (FieldList*)malloc(sizeof(FieldList));
	memset(p,0,sizeof(FieldList));
	strcpy(p->name,name);
	p->type->kind = INT;
	p->type->INT = value;
	p->child = NULL;
}
void FLOAT_Insert(char *name,float value){
	float i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		SymbolTable[i] = (FieldList*)malloc(sizeof(FieldList));
		memset(SymbolTable[i],0,sizeof(FieldList));
		strcpy(SymbolTable[i]->name,name);
		SymbolTable[i]->type->kind = FLOAT;
		SymbolTable[i]->type->FLOAT = value;
		SymbolTable[i]->child = NULL;
	}
	else{
		FieldList *p = SymbolTable[i];
		p = p->child;
		while(p->child!=NULL) p=p->child;
		p->child = (FieldList*)malloc(sizeof(FieldList));
		p=p->child;
		memset(p,0,sizeof(FieldList));
		strcpy(p->name,name);
		p->type->kind = FLOAT;
		p->type->FLOAT = value;
		p->child = NULL;
	}
}
void FUNC_Insert(node *ExtDef){
	node *Specifier = ExtDef->child;
	node *FunDec = Specifier->brother;
	node *VarList = FunDec->child->brother->brother;
	if(strcpy(VarList->name,"VarList") == 0){
		//node *ParamDec = p->child;
		int i = hash_pjw(FunDec->child->name);
		if(SymbolTable[i] == NULL){
			SymbolTable[i] = (FieldList*)malloc(sizeof(FieldList));
			memset(SymbolTable[i],0,sizeof(FieldList));
			strcpy(SymbolTable[i]->name,FunDec->child->name);
			SymbolTable[i]->type->kind = FUNC;
			//node *DefList = q1->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			//FieldList *temp = SymbolTable[i];
			if(strcmp(Specifier->child->name,"INT")==0)
				strcpy(SymbolTable[i]->type->Func->RETURN,"INT");
			if(strcmp(Specifier->child->name,"FLOAT")==0)
				strcpy(SymbolTable[i]->type->Func->RETURN,"FLOAT");
			else		//StructSpecifier STRUCT OptTag ID
				strcpy(SymbolTable[i]->child->Func->RETURN,Specifier->child->child->brother->child->name);
			SymbolTable[i]->child = NULL;
			SymbolTable[i]->brother = NULL;
			//此处可能报错

			node *ParamDec = VarList->child;
			FuncVar *temp = (FuncVar*)malloc(sizeof(FuncVar));
			temp->brother =NULL:
			SymbolTable[i]->type->func->brother = temp;
			node *Spec = VarList->child->child;
			if(strcmp(Spec->child->name,"INT")==0)
				strcpy(temp->kind,"INT");
			if(strcmp(Spec->child->name,"FLOAT")==0)
				strcpy(temp->kind,"FLOAT");
			else			
				strcpy(temp->kind,Spec->child->child->brother->child->name);
			VarList = VarList->child->child->brother;
			//temp = temp->brother;
			while(VarList!=NULL){
				//temp->brother = (FieldList*)malloc(sizeof(FieldList));
				//temp = temp->brother;				
				ParamDec = VarList->child;
				FuncVar *tem = (FuncVar*)malloc(sizeof(FuncVar));
				temp->brother = tem;
				temp = temp->brother;
				Spec = VarList->child->child;
				if(strcmp(Spec->child->name,"INT")==0)
					strcpy(temp->kind,"INT");
				if(strcmp(Spec->child->name,"FLOAT")==0)
					strcpy(temp->kind,"FLOAT");
				else			
					strcpy(temp->kind,Spec->child->child->brother->child->name);
				VarList = VarList->child->child->brother;
				//temp->child = NULL;
				//temp->brother = NULL:				
			}
		}
		else{
			FieldList *p = SymbolTable[i];
			p = p->child;
			while(p->child!=NULL) p=p->child;
			p->child = (FieldList*)malloc(sizeof(FieldList));
			strcpy(p->name,FunDec->child->name);
			p->type->kind = FUNC;
			//node *DefList = q1->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			//FieldList *temp = SymbolTable[i];
			if(strcmp(Specifier->child->name,"INT")==0)
				strcpy(p->type->Func->RETURN,"INT");
			if(strcmp(Specifier->child->name,"FLOAT")==0)
				strcpy(p->type->Func->RETURN,"FLOAT");
			else		//StructSpecifier STRUCT OptTag ID
				strcpy(p->child->Func->RETURN,Specifier->child->child->brother->child->name);
			p->child = NULL;
			p->brother = NULL;
			//此处可能报错

			node *ParamDec = VarList->child;
			FuncVar *temp = (FuncVar*)malloc(sizeof(FuncVar));
			temp->brother =NULL:
			p->type->func->brother = temp;
			node *Spec = VarList->child->child;
			if(strcmp(Spec->child->name,"INT")==0)
				strcpy(temp->kind,"INT");
			if(strcmp(Spec->child->name,"FLOAT")==0)
				strcpy(temp->kind,"FLOAT");
			else			
				strcpy(temp->kind,Spec->child->child->brother->child->name);
			VarList = VarList->child->child->brother;
			//temp = temp->brother;
			while(VarList!=NULL){
				//temp->brother = (FieldList*)malloc(sizeof(FieldList));
				//temp = temp->brother;				
				ParamDec = VarList->child;
				FuncVar *tem = (FuncVar*)malloc(sizeof(FuncVar));
				temp->brother = tem;
				temp = temp->brother;
				Spec = VarList->child->child;
				if(strcmp(Spec->child->name,"INT")==0)
					strcpy(temp->kind,"INT");
				if(strcmp(Spec->child->name,"FLOAT")==0)
					strcpy(temp->kind,"FLOAT");
				else			
					strcpy(temp->kind,Spec->child->child->brother->child->name);
				VarList = VarList->child->child->brother;			
			}
		}
	}
	else{
		int i = hash_pjw(name);
		if(SymbolTable[i] == NULL){ 
			SymbolTable[i] = (FieldList*)malloc(sizeof(FieldList));
			memset(SymbolTable[i],0,sizeof(FieldList));			
			strcpy(SymbolTable[i]->name,Specifier->child->name);
			SymbolTable[i]->type->kind = FUNC;
			SymbolTable[i]->type->Func->brother = NULL;
			if(strcmp(Specifier->child->name,"INT")==0)
				strcpy(SymbolTable[i]->type->Func->RETURN,"INT");
			if(strcmp(Specifier->child->name,"FLOAT")==0)
				strcpy(SymbolTable[i]->type->Func->RETURN,"FLOAT");
			else		//StructSpecifier STRUCT OptTag ID
				strcpy(SymbolTable[i]->child->Func->RETURN,Specifier->child->child->brother->child->name);
			SymbolTable[i]->child = NULL;
			SymbolTable[i]->brother = NULL;
		}
		else{
			FieldList *p = SymbolTable[i];
			p = p->child;
			while(p) p=p->child;
			p = (FieldList*)malloc(sizeof(FieldList));
			memset(p,0,sizeof(FieldList));
			strcpy(p->name,Specifier->child->name);
			p->type->kind = FUNC;
			p->type->Func->brother = NULL;
			if(strcmp(Specifier->child->name,"INT")==0)
				strcpy(p->type->Func->RETURN,"INT");
			if(strcmp(Specifier->child->name,"FLOAT")==0)
				strcpy(p->type->Func->RETURN,"FLOAT");
			else		//StructSpecifier STRUCT OptTag ID
				strcpy(p->child->Func->RETURN,Specifier->child->child->brother->child->name);
			p->child = NULL;
			p->brother = NULL;
		}
	}
	return;
}
void STRUCT_Insert(node *p){
	if(strcpy(p->child->brother->name,"OptTag") == 0){
		node *q1 = p->child->brother;
		int i = hash_pjw(q1->child->name);
		if(SymbolTable[i] == NULL){
			SymbolTable[i] = (FieldList*)malloc(sizeof(FieldList));
			memset(SymbolTable[i],0,sizeof(FieldList));
			strcpy(SymbolTable[i]->name,q1->child->name);
			SymbolTable[i]->type->kind = STRUCTURE;
			node *DefList = q1->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			FieldList *temp = SymbolTable[i];
			while(DefList!=NULL){
				temp->brother = (FieldList*)malloc(sizeof(FieldList));
				temp = temp->brother;				
				node *TYPEorSTRUCT = DefList->child->child->child;
				if(strcmp(TYPEorSTRUCT->name,"INT"))
					temp->type->kind = INT;
				else if(strcmp(TYPEorSTRUCT->name,"FLOAT"))
					temp->type->kind = FLOAT;
				else temp->type->kind = STRUCT;
				node *DefList = DefList->child->brother;
				temp->child = NULL;
				temp->brother = NULL:				
			}
		}
		else{
			FieldList *p = SymbolTable[i];
			p = p->child;
			while(p->child!=NULL) p=p->child;
			p->child = (FieldList*)malloc(sizeof(FieldList));
			strcpy(p->name,q1->child->name);
			SymbolTable[i]->type->kind = STRUCTURE;
			node *DefList = q1->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			FieldList *temp = p;
			while(DefList!=NULL){
				temp->brother = (FieldList*)malloc(sizeof(FieldList));
				temp = temp->brother;				
				node *TYPEorSTRUCT = DefList->child->child->child;
				if(strcmp(TYPEorSTRUCT->name,"INT"))
					temp->type->kind = INT;
				else if(strcmp(TYPEorSTRUCT->name,"FLOAT"))
					temp->type->kind = FLOAT;
				else temp->type->kind = STRUCT;
				temp->child = NULL;
				temp->brother = NULL:
				node *DefList = DefList->child->brother;				
			}
		}
	}
	return;
}
void ARRAY_Insert(node *VarDec){
	int j = 0;
	node *p = VarDec;
	for(;strcmp(p->child->name,"VarDec")==0;j++)
		p = p->child;
	int i = hash_pjw(p->child->name);
	//for() 
}
