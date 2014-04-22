#include "type.h"
#include <stdlib.h>
#include <stdio.h>
#define SIZE 32
/*typedef struct  
{  
    ieldList_ * value[SIZE];  
}SymbolTable;  
SymbolTable* create_SymbolTable()  
{  
    SymbolTable* pHashTbl = (SymbolTable*)malloc(sizeof(SymbolTable));  
    memset(pHashTbl, 0, sizeof(SymbolTable));  
    return pHashTbl;  
}  */

void STRUCT_Insert(node *p);

FieldList  SymbolTable[SIZE];

FieldList FieldList_init(){
	FieldList temp =  (FieldList)malloc(sizeof(struct FieldList_));
	memset(temp,0,sizeof(struct FieldList_));
	temp->type = (Type)malloc(sizeof(struct Type_));
	return temp;
}

unsigned int hash_pjw(char* name)
{
	unsigned int val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val%32;
}

int Find(char *name){
	int i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		return 0;
	}
	FieldList p = SymbolTable[i];
	while(p!=NULL){
		if(strcmp(p->name,name) == 0)
			return 1;
		p = p->child;
	}
	return 0;
}

int get_kind(char *name){
	int i = hash_pjw(name);
	while(SymbolTable[i] != NULL){
		if(strcmp(SymbolTable[i]->name,name) == 0) 
			return SymbolTable[i]->type->kind;
	}
	return -1;
}

char* get_return(char *name){
	int i = hash_pjw(name);
	while(SymbolTable[i] != NULL){
		if(strcmp(SymbolTable[i]->name,name) == 0) 
			return SymbolTable[i]->type->func.RETURN;
	}
	return "-1";
}

void INT_Insert(char *name,int value){
	int i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		SymbolTable[i] = FieldList_init();
		strcpy(SymbolTable[i]->name,name);
		SymbolTable[i]->type->kind = Int;
		SymbolTable[i]->type->INT = value;
		SymbolTable[i]->child = NULL;
	}
	else{
		FieldList p = SymbolTable[i];
		//p = p->child;
		while(p->child!=NULL) p=p->child;
		p->child = FieldList_init();
		p = p->child;
		strcpy(p->name,name);
		p->type->kind = Int;
		p->type->INT = value;
		p->child = NULL;
	}
}
void FLOAT_Insert(char *name,float value){
	int i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		SymbolTable[i] = FieldList_init();
		strcpy(SymbolTable[i]->name,name);
		SymbolTable[i]->type->kind = Float;
		SymbolTable[i]->type->FLOAT = value;
		SymbolTable[i]->child = NULL;
	}
	else{
		FieldList p = SymbolTable[i];
		//p = p->child;
		while(p->child!=NULL) p=p->child;
		p->child = FieldList_init();
		p=p->child;
		strcpy(p->name,name);
		p->type->kind = Float;
		p->type->FLOAT = value;
		p->child = NULL;
	}
}
void FUNC_Insert(node *ExtDef){
	node *Specifier = ExtDef->child;
	node *FunDec = Specifier->brother;
	node *VarList = FunDec->child->brother->brother;
	if(strcmp(VarList->name,"VarList") == 0){
		//node *ParamDec = p->child;
		int i = hash_pjw(FunDec->child->node_value);
		if(SymbolTable[i] == NULL){
			SymbolTable[i] = FieldList_init();
			strcpy(SymbolTable[i]->name,FunDec->child->node_value);
			SymbolTable[i]->type->kind = FUNC;
			//node *DefList = q1->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			//FieldList *temp = SymbolTable[i];
			if(strcmp(Specifier->child->node_value,"int")==0){
				strcpy(SymbolTable[i]->type->func.RETURN,"INT");
			}
			else if(strcmp(Specifier->child->node_value,"Float")==0)
				strcpy(SymbolTable[i]->type->func.RETURN,"FLOAT");
			else		//StructSpecifier STRUCT OptTag ID
				strcpy(SymbolTable[i]->type->func.RETURN,Specifier->child->child->brother->child->name);
			SymbolTable[i]->child = NULL;
			SymbolTable[i]->brother = NULL;
			//此处可能报错

			node *ParamDec = VarList->child;
			FuncVar *temp = (FuncVar*)malloc(sizeof(FuncVar));
			temp->next =NULL;
			SymbolTable[i]->type->func.brother = temp;
			node *Spec = VarList->child->child;
			if(strcmp(Spec->child->node_value,"int")==0){
				strcpy(temp->kind,"INT");
				INT_Insert(Spec->brother->child->node_value,0);
			}
			else if(strcmp(Spec->child->node_value,"float")==0){
				strcpy(temp->kind,"FLOAT");
				FLOAT_Insert(Spec->brother->child->node_value,0);
			}
			else{			
				strcpy(temp->kind,Spec->child->child->brother->child->name);		
				STRUCT_Insert(Spec->child);
			}
			//VarList = VarList->child->brother->brother;
			//temp = temp->brother;
			while(VarList->child->brother!=NULL){
				//temp->brother = (FieldList*)malloc(sizeof(struct FieldList_));
				//temp = temp->brother;	
				VarList = VarList->child->brother->brother;			
				ParamDec = VarList->child;
				FuncVar *tem = (FuncVar*)malloc(sizeof(FuncVar));
				temp->next = tem;
				temp = temp->next;
				Spec = VarList->child->child;
				if(strcmp(Spec->child->node_value,"int")==0)
					strcpy(temp->kind,"INT");
				else if(strcmp(Spec->child->node_value,"float")==0){
					strcpy(temp->kind,"FLOAT");
					INT_Insert(Spec->brother->child->node_value,0);
				}
				else{			
					strcpy(temp->kind,Spec->child->child->brother->child->name);
					STRUCT_Insert(Spec->child);
				}
				//VarList = VarList->child->child->brother;
				//temp->child = NULL;
				//temp->brother = NULL:				
			}
		}
		else{
			FieldList p = SymbolTable[i];
			//p = p->child;
			while(p->child!=NULL) p=p->child;
			p->child = FieldList_init();
			p = p->child;
			strcpy(p->name,FunDec->child->node_value);
			p->type->kind = FUNC;
			//node *DefList = q1->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			//FieldList *temp = SymbolTable[i];
			if(strcmp(Specifier->child->node_value,"int")==0)
				strcpy(p->type->func.RETURN,"INT");
			else if(strcmp(Specifier->child->node_value,"float")==0)
				strcpy(p->type->func.RETURN,"FLOAT");
			else		//StructSpecifier STRUCT OptTag ID
				strcpy(p->type->func.RETURN,Specifier->child->child->brother->child->name);
			p->child = NULL;
			p->brother = NULL;
			//此处可能报错

			node *ParamDec = VarList->child;
			FuncVar *temp = (FuncVar*)malloc(sizeof(FuncVar));
			temp->next =NULL;
			p->type->func.brother = temp;
			node *Spec = VarList->child->child;
			if(strcmp(Spec->child->node_value,"int")==0){
				strcpy(temp->kind,"INT");
				INT_Insert(Spec->brother->child->node_value,0);
			}
			else if(strcmp(Spec->child->node_value,"float")==0){
				strcpy(temp->kind,"FLOAT");
				FLOAT_Insert(Spec->brother->child->node_value,0);
			}
			else{			
				strcpy(temp->kind,Spec->child->child->brother->child->name);
				STRUCT_Insert(Spec->child);
			}
			
			//temp = temp->brother;
			while(VarList->child->brother!=NULL){
				//temp->brother = (FieldList*)malloc(sizeof(struct FieldList_));
				//temp = temp->brother;		
				VarList = VarList->child->brother->brother;		
				ParamDec = VarList->child;
				FuncVar *tem = (FuncVar*)malloc(sizeof(FuncVar));
				temp->next = tem;
				temp = temp->next;
				Spec = VarList->child->child;
				if(strcmp(Spec->child->node_value,"int")==0){
					strcpy(temp->kind,"INT");
					INT_Insert(Spec->brother->child->node_value,0);
				}
				else if(strcmp(Spec->child->node_value,"float")==0){
					strcpy(temp->kind,"FLOAT");
					INT_Insert(Spec->brother->child->node_value,0);
				}
				else{			
					strcpy(temp->kind,Spec->child->child->brother->child->name);
					STRUCT_Insert(Spec->child);
				}		
			}
		}
	}
	else{
		int i = hash_pjw(FunDec->child->node_value);
		if(SymbolTable[i] == NULL){ 
			SymbolTable[i] = FieldList_init();		
			strcpy(SymbolTable[i]->name,FunDec->child->node_value);
			SymbolTable[i]->type->kind = FUNC;
			SymbolTable[i]->type->func.brother = NULL;
			if(strcmp(Specifier->child->node_value,"int")==0)
				strcpy(SymbolTable[i]->type->func.RETURN,"INT");
			else if(strcmp(Specifier->child->node_value,"float")==0)
				strcpy(SymbolTable[i]->type->func.RETURN,"FLOAT");
			else		//StructSpecifier STRUCT OptTag ID
				strcpy(SymbolTable[i]->type->func.RETURN,Specifier->child->child->brother->child->name);
			SymbolTable[i]->child = NULL;
			SymbolTable[i]->brother = NULL;
		}
		else{
			FieldList p = SymbolTable[i];
			//p = p->child;
			while(p->child!=NULL) p=p->child;
			p->child = FieldList_init();
			p = p->child;
			strcpy(p->name,Specifier->child->node_value);
			p->type->kind = FUNC;
			p->type->func.brother = NULL;
			if(strcmp(Specifier->child->node_value,"int")==0)
				strcpy(p->type->func.RETURN,"INT");
			else if(strcmp(Specifier->child->node_value,"float")==0)
				strcpy(p->type->func.RETURN,"FLOAT");
			else		//StructSpecifier STRUCT OptTag ID
				strcpy(p->type->func.RETURN,Specifier->child->child->brother->child->name);
			p->child = NULL;
			p->brother = NULL;
		}
	}
	return;
}
void STRUCT_Insert(node *p){
	if(strcmp(p->child->brother->name,"OptTag") == 0){
		node *q1 = p->child->brother;
		int i = hash_pjw(q1->child->name);
		if(SymbolTable[i] == NULL){
			SymbolTable[i] = FieldList_init();
			strcpy(SymbolTable[i]->name,q1->child->name);
			SymbolTable[i]->type->kind = STRUCTURE;
			node *DefList = q1->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			FieldList temp = SymbolTable[i];
			while(DefList!=NULL){
				temp->brother = FieldList_init();
				temp = temp->brother;				
				node *TYPEorSTRUCT = DefList->child->child->child;
				if(strcmp(TYPEorSTRUCT->name,"INT"))
					temp->type->kind = Int;
				else if(strcmp(TYPEorSTRUCT->name,"FLOAT"))
					temp->type->kind = Float;
				else temp->type->kind = STRUCT;
				node *DefList = DefList->child->brother;
				temp->child = NULL;
				temp->brother = NULL;				
			}
		}
		else{
			FieldList p = SymbolTable[i];
			//p = p->child;
			while(p->child!=NULL) p=p->child;
			p->child = FieldList_init();
			p = p->child;
			strcpy(p->name,q1->child->name);
			SymbolTable[i]->type->kind = STRUCTURE;
			node *DefList = q1->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			FieldList temp = p;
			while(DefList!=NULL){
				temp->brother = FieldList_init();
				temp = temp->brother;				
				node *TYPEorSTRUCT = DefList->child->child->child;
				if(strcmp(TYPEorSTRUCT->name,"INT"))
					temp->type->kind = Int;
				else if(strcmp(TYPEorSTRUCT->name,"FLOAT"))
					temp->type->kind = Float;
				else temp->type->kind = STRUCT;
				temp->child = NULL;
				temp->brother = NULL;
				node *DefList = DefList->child->brother;				
			}
		}
	}
	return;
}
void ARRAY_Insert(node *VarDec,char *name){
	int j = 0;
	int a[20];
	node *p = VarDec;
	for(;strcmp(p->child->name,"VarDec")==0;j++){
		p = p->child;
		a[j] = p->child->brother->brother->node_int;
	}
	int i = hash_pjw(p->child->name);

	if(SymbolTable[i] == NULL){
		SymbolTable[i] = FieldList_init();
		strcpy(SymbolTable[i]->name,p->child->name);
		Type q = SymbolTable[i]->type;
		int m = j-1;
		for(;m>0;m--){
			q->array.size = a[m];
			q->array.elem->kind = ARRAY;
			q = q->array.elem;
		}
		q->array.size = a[m];
		if(strcmp(name,"INT")==0)
			q->array.elem->kind = Int;
		else if(strcmp(name,"FLOAT")==0)
			q->array.elem->kind = Float;
		else {
			q->array.elem->kind = STRUCTURE;
			strcpy(q->array.elem->name,name);
		}
	}
	else{
		FieldList temp = SymbolTable[i];
		temp = temp->child;
		while(temp->child!=NULL) temp=temp->child;
		temp->child = FieldList_init();
		temp = temp->child;
		strcpy(temp->name,p->child->name);
		Type q = temp->type;
		int m = j-1;
		for(;m>0;m--){
			q->array.size = a[m];
			q->array.elem->kind = ARRAY;
			q = q->array.elem;
		}
		q->array.size = a[m];
		if(strcmp(name,"INT")==0)
			q->array.elem->kind = Int;
		else if(strcmp(name,"FLOAT")==0)
			q->array.elem->kind = Float;
		else {
			q->array.elem->kind = STRUCTURE;
			strcpy(q->array.elem->name,name);
		}
	}
}
