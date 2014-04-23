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
unsigned int hash_pjw(char* name);

FieldList  SymbolTable[SIZE];

typedef struct stack{
	struct stack *brother;
	FieldList child;
}Stackhead;

Stackhead *head;
void Stackhead_init(){
	head = (Stackhead *)malloc(sizeof(Stackhead));
	head->child = NULL;
	head->brother = NULL;
}


FieldList FieldList_init(){
	FieldList temp =  (FieldList)malloc(sizeof(struct FieldList_));
	memset(temp,0,sizeof(struct FieldList_));
	temp->type = (Type)malloc(sizeof(struct Type_));
	return temp;
}
void stack_push(){
	Stackhead *temp = (Stackhead *)malloc(sizeof(Stackhead));
	if(head == NULL) return;
	temp->brother = head;
	head = temp;
}
void stack_pop(Stackhead *p){//delete
	FieldList s = p->child;
	while(s){
		FieldList temp = s;
		int i = hash_pjw(s->name);
		FieldList q = SymbolTable[i];
		FieldList q1 = NULL;
		while(q!=NULL){
			if(strcmp(q->name,s->name) == 0){
				break;
			}
			q1 = q;
			q = q->child;
		}
		if(!q1){
			if(SymbolTable[i]->child){
				SymbolTable[i] = SymbolTable[i]->child;
			}
			else SymbolTable[i] = NULL;
		}
		else{
			if(q->child){
				q1->child = q->child;
			}
			else q1->child = NULL;
		}
		s = s->scope;
	}
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
int FindStruct(char *name,char *insname){
	int i = hash_pjw(name);
	FieldList p = SymbolTable[i];
	while(p!=NULL){
		if(strcmp(p->name,name) == 0)
			break;
		p = p->child;
	}
	char sname[20];
	strcpy(sname,p->type->name);
	int j = hash_pjw(sname);
	FieldList q = SymbolTable[i];
	while(q!=NULL){
		if(strcmp(q->name,sname) == 0)
			break;
		q = q->child;
	}
	FieldList temp ;
	while(!q->brother){
		q=q->brother;
		if(strcmp(q->name,insname)==0)
			return 1;
	}
	return 0;
}

char *get_Array(char *name){
	int i = hash_pjw(name);
	FieldList p = SymbolTable[i];
	while(p!=NULL){
		if(strcmp(p->name,name) == 0)
			break;
		p = p->child;
	}
	Type temp;
	temp = p->type;
	while(!temp->array.elem)
		temp = temp->array.elem;
	if(temp->array.elem->kind == Int){
		return "int";
	}
	else if(temp->array.elem->kind == Float){
		return "float";
	}
	else{
		printf("struct :%s\n",temp->array.elem->name); 
		return temp->name;
	}
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
		if(head->child == NULL){
			head->child = SymbolTable[i];
			head->brother = NULL;
		}
		else{
			SymbolTable[i]->scope = head->child;;
			head->child = SymbolTable[i];
		}
	}
	else{
		FieldList p = FieldList_init();
		p->child = SymbolTable[i]->child;
		SymbolTable[i] = p;
		strcpy(p->name,name);
		p->type->kind = Int;
		p->type->INT = value;
		p->child = NULL;
		if(head->child == NULL){
			head->child = p;
			head->brother = NULL;
		}
		else{
			SymbolTable[i]->scope = head->child;
			head->child = SymbolTable[i];
		}
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
		if(head->child == NULL){
			head->child = SymbolTable[i];
			head->brother = NULL;
		}
		else{
			SymbolTable[i]->scope = head->child;
			head->child = SymbolTable[i];
		}
	}
	else{
		FieldList p = FieldList_init();
		p->child = SymbolTable[i]->child;
		SymbolTable[i] = p;
		strcpy(p->name,name);
		p->type->kind = Float;
		p->type->FLOAT = value;
		p->child = NULL;
		if(head->child == NULL){
			head->child = p;
			head->brother = NULL;
		}
		else{
			SymbolTable[i]->scope = head->child;
			head->child = SymbolTable[i];
		}
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
			if(head->child == NULL){
				head->child = SymbolTable[i];
				head->brother = NULL;
			}
			else{
				SymbolTable[i]->scope = head->child;
				head->child = SymbolTable[i];
			}
		}
		else{
			FieldList p = FieldList_init();
			p->child = SymbolTable[i]->child;
			SymbolTable[i] = p;
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
			if(head->child == NULL){
				head->child = p;
				head->brother = NULL;
			}
			else{
				SymbolTable[i]->scope = head->child;
				head->child = SymbolTable[i];
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
			if(head->child == NULL){
				head->child = SymbolTable[i];
				head->brother = NULL;
			}
			else{
				SymbolTable[i]->scope = head->child;
				head->child = SymbolTable[i];
			}
		}
		else{
			FieldList p = FieldList_init();
			p->child = SymbolTable[i]->child;
			SymbolTable[i] = p;
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
			if(head->child == NULL){
				head->child = p;
				head->brother = NULL;
			}
			else{
				SymbolTable[i]->scope = head->child;
				head->child = SymbolTable[i];
			}
		}
	}
	return;
}
void varStruct_Insert(char *name,char *spec){
	int i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		SymbolTable[i] = FieldList_init();
		strcpy(SymbolTable[i]->name,name);
		SymbolTable[i]->type->kind = STRUCTVAR;
		strcpy(SymbolTable[i]->type->name,spec);
		SymbolTable[i]->child = NULL;
		if(head->child == NULL){
			head->child = SymbolTable[i];
			head->brother = NULL;
		}
		else{
			SymbolTable[i]->scope = head->child;;
			head->child = SymbolTable[i];
		}
	}
	else{
		FieldList p = FieldList_init();
		p->child = SymbolTable[i]->child;
		SymbolTable[i] = p;
		strcpy(p->name,name);
		p->type->kind = STRUCTVAR;
		strcpy(SymbolTable[i]->type->name,spec);
		p->child = NULL;
		if(head->child == NULL){
			head->child = p;
			head->brother = NULL;
		}
		else{
			SymbolTable[i]->scope = head->child;
			head->child = SymbolTable[i];
		}
	}	
}
void STRUCT_Insert(node *p){
	if(strcmp(p->child->brother->name,"OptTag") == 0){
		node *q1 = p->child->brother;
		int i = hash_pjw(q1->child->node_value);
		if(SymbolTable[i] == NULL){
			SymbolTable[i] = FieldList_init();
			strcpy(SymbolTable[i]->name,q1->child->node_value);
			SymbolTable[i]->type->kind = STRUCTURE;
			node *DefList = p->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			FieldList temp = SymbolTable[i];
			while(DefList!=NULL){
				temp->brother = FieldList_init();
				temp = temp->brother;				
				node *TYPEorSTRUCT = DefList->child->child->child;
				if(strcmp(TYPEorSTRUCT->name,"int"))
					temp->type->kind = Int;
				else if(strcmp(TYPEorSTRUCT->name,"float"))
					temp->type->kind = Float;
				else temp->type->kind = STRUCT;
				node *DefList = DefList->child->brother;
				temp->child = NULL;
				temp->brother = NULL;				
			}
			if(head->child == NULL){
				head->child = SymbolTable[i];
				head->brother = NULL;
			}
			else{
				SymbolTable[i]->scope = head->child;
				head->child = SymbolTable[i];
			}
		}
		else{
			FieldList s = FieldList_init();
			s->child = SymbolTable[i]->child;
			SymbolTable[i] = s;
			strcpy(s->name,q1->child->node_value);
			SymbolTable[i]->type->kind = STRUCTURE;
			node *DefList = p->child->brother->brother->brother;		//STRUCT(child) OptTag(b) LC(b) DefList(b) RC(b)
			/*deflist (c)def (b)deflist*/
			FieldList temp = s;
			while(DefList!=NULL){
				temp->brother = FieldList_init();
				temp = temp->brother;				
				node *TYPEorSTRUCT = DefList->child->child->child;
				if(strcmp(TYPEorSTRUCT->name,"int"))
					temp->type->kind = Int;
				else if(strcmp(TYPEorSTRUCT->name,"float"))
					temp->type->kind = Float;
				else temp->type->kind = STRUCT;
				temp->child = NULL;
				temp->brother = NULL;
				node *DefList = DefList->child->brother;				
			}
			if(head->child == NULL){
				head->child = s;
				head->brother = NULL;
			}
			else{
				SymbolTable[i]->scope = head->child;
				head->child = SymbolTable[i];
			}	
		}
	}
	return;
}
void ARRAY_Insert(node *VarDec,char *name,char *spec){
	int j = 0;
	int a[20];
	node *p = VarDec;
	for(;strcmp(p->child->name,"VarDec")==0;j++){
		
		a[j] = p->child->brother->brother->node_int;
		p = p->child;
	}
	int i = hash_pjw(name);
	if(SymbolTable[i] == NULL){
		SymbolTable[i] = FieldList_init();
		strcpy(SymbolTable[i]->name,name);
		Type q = SymbolTable[i]->type;
		int m = j-1;
		for(;m>0;m--){
			q->array.elem = (Type)malloc(sizeof(struct Type_));
			q->array.size = a[m];
			q->array.elem->kind = ARRAY;
			q = q->array.elem;
		}
		q->array.size = a[m];
		q->kind = ARRAY;
		q->array.elem = (Type)malloc(sizeof(struct Type_));
		if(strcmp(spec,"int")==0){
			q->array.elem->kind = Int;
			printf("%s\n",spec);
		}
		else if(strcmp(spec,"float")==0)
			q->array.elem->kind = Float;
		else {
			q->array.elem->kind = STRUCTURE;
			strcpy(q->array.elem->name,spec);
		}
		if(head->child == NULL){
			head->child = SymbolTable[i];
			head->brother = NULL;
		}
		else{
			SymbolTable[i]->scope = head->child;
			head->child = SymbolTable[i];
		}
		SymbolTable[i]->type->kind;
	}
	else{
		FieldList temp = FieldList_init();
		temp->child = SymbolTable[i]->child;
		SymbolTable[i] = temp;
		strcpy(temp->name,name);
		Type q = temp->type;
		int m = j-1;
		for(;m>0;m--){
			q->array.elem = (Type)malloc(sizeof(struct Type_));
			q->array.size = a[m];
			q->array.elem->kind = ARRAY;
			q = q->array.elem;
		}
		q->array.size = a[m];
		q->array.elem = (Type)malloc(sizeof(struct Type_));
		if(strcmp(name,"int")==0)
			q->array.elem->kind = Int;
		else if(strcmp(name,"float")==0)
			q->array.elem->kind = Float;
		else {
			q->array.elem->kind = STRUCTURE;
			strcpy(q->array.elem->name,name);
		}
		if(head->child == NULL){
			head->child = temp;
			head->brother = NULL;
		}
		else{
			SymbolTable[i]->scope = head->child;
			head->child = SymbolTable[i];
		}
	}
}
