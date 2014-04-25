//#include "type.h"
#include "symbol_table.h"
#include "node.h"

int instruct = 0;//表示当前遍历是否在结构体定义中
char func_return[20];//记录当前遍历函数的返回类型

void sem_analysis(node *p);

/*void ParamDec_anly(node *p){//p is the first child of the ParamDec
	char* spcid;
	spcid = getSpecifier_id(p);
	node* Vardec = p->brother;
	char* varid = getVarDec_id(Vardec);
	
	if(strcmp(spcid,"int")==0){	
		if(Find(varid)==0){//var isn't in the table
			INT_Insert()
		}
		else{//already defined
			printf("Error type 3 at line %d: Variable is already defined \n",Vardec->line)
		}
	}
	else if(strcmp(spcid,"float")==0){	
		if(Find(varid)==0){//var isn't in the table
			FLOAT_Insert()
		}
		else{//already defined
			printf("Error type 3 at line %d: Variable is already defined \n",Vardec->line)
		}
	}
}*/

int exp_cmp(node* left,node*right){
	int ltype = left->type;
	int rtype = right->type;
	if(left->type == 4 && right->type == 4){
		if(strcmp(get_structname(left->node_value),get_structname(right->node_value))==0){
			return 1;
		}
		else
			return 0;
	}
	else if(left->type == 3 || right->type == 3){
		if(left->type == 3){
			if(strcmp(get_Array(left->node_value),"int") == 0){
				ltype = 5;
			}
			else if (strcmp(get_Array(left->node_value),"float") == 0){
				ltype = 6;
			}
			else//struct
				ltype = 4;
		}
		if(right->type == 3){
			if(strcmp(get_Array(right->node_value),"int") == 0){
				rtype = 5;
			}
			else if (strcmp(get_Array(right->node_value),"float") == 0){
				rtype = 6;
			}
			else//struct
				rtype = 4;
		}
	}
	if(ltype == 0 || rtype == 0)
		return 1;
	if(((ltype == 1 || ltype == 5) && (rtype == 1 || rtype == 5)) || ((ltype == 2 || ltype == 6)&&(rtype == 2 || rtype == 6)))
		return 1;
	else
		return 0;
}

int get_exp_judg(node* p){//p is the node of the exp
	if(p->type == 1||p->type == 5){
		if (p->node_int > 0)
			return 1;
		else
			return 0;
	}
	else{
		if (p->node_float > 0)
			return 1;
		else
			return 0;
	}
}

int get_relop(node* p){//p is the node RELOP
	/* 	1 >
		2 <
		3 >=
		4 <=
		5 ==
		6 !=
	*/
	if(strcmp(p->node_value,">")){
		return 1;
	}
	else if(strcmp(p->node_value,"<")){
		return 2;
	}
	else if(strcmp(p->node_value,">=")){
		return 3;
	}
	else if(strcmp(p->node_value,"<=")){
		return 4;
	}
	else if(strcmp(p->node_value,"==")){
		return 5;
	}
	else if(strcmp(p->node_value,"!=")){
		return 6;
	}
}

void exp_cal(node* exp){//exp is the node exp
	//-------------------------------------------------Exp ASSIGNOP Exp
	if(exp->type == 7){
		node* left = exp->child;
		node* right = left->brother->brother;
		if(exp_cmp(left,right)==1){
			exp->type = exp->child->type;
			if(left->type == 1 || left->type == 2)
				printf("Error type 6 at line %d:  The left-hand side of anassignment must be a variable\n",exp->line);
			else{
				exp->type = left->type;
				if(left->type == 5){
					left->node_int = right->node_int;
					exp->node_int = right->node_int;
				}
				else{
					left->node_float = right->node_float;
					exp->node_float = right->node_float;
				}
			}
		return;
		}
		exp->type = left->type;
		printf("Error type 5 at line %d:  Type mismatched\n",exp->line);
	}
	//-------------------------------------------------Exp AND Exp
	else if(exp->type == 8){
		node* left = exp->child;
		node* right = left->brother->brother;
		exp->type = 1;
		if(get_exp_judg(left) && get_exp_judg(right)){
			exp->node_int = 1;
		}
		else
			exp->node_int = 0;
	}

	//-------------------------------------------------Exp OR Exp
	else if(exp->type == 9){
		node* left = exp->child;
		node* right = left->brother->brother;
		exp->type = 1;
		if(get_exp_judg(left) || get_exp_judg(right)){
			exp->node_int = 1;
		}
		else
			exp->node_int = 0;
	}

	//-------------------------------------------------Exp RELOP Exp
	else if(exp->type == 10){
		node* left = exp->child;
		node* right = left->brother->brother;
		if(exp_cmp(left,right)==1){
			exp->type = 1;
			if(left->type == 1 || left->type == 5){//int
				if(get_relop(left->brother)==1){// >
					if(left->node_int > left->node_int)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==2){// <
					if(left->node_int < left->node_int)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==3){// >=
					if(left->node_int >= left->node_int)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==4){// <=
					if(left->node_int <= left->node_int)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==5){// ==
					if(left->node_int == left->node_int)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==6){// !=
					if(left->node_int != left->node_int)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
			}
			else{//float
				if(get_relop(left->brother)==1){// >
					if(left->node_float > left->node_float)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==2){// <
					if(left->node_float < left->node_float)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==3){// >=
					if(left->node_float >= left->node_float)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==4){// <=
					if(left->node_float <= left->node_float)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==5){// ==
					if(left->node_float == left->node_float)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
				else if(get_relop(left->brother)==6){// !=
					if(left->node_float != left->node_float)
						exp->node_int = 1;
					else
						exp->node_int = 0;
				}
			}
		}
		else
			printf("Error type 5 at line %d: Type mismatched\n",exp->line);
	}

	//-------------------------------------------------Exp PLUS Exp
	else if(exp->type == 11){
		node* left = exp->child;
		node* right = left->brother->brother;
		if(exp_cmp(left,right)==1){
			if(left->type == 1 || left->type == 5){
				exp->type = 1;
				exp->node_int = left->node_int + right->node_int;
			}
			else{
				exp->type = 2;
				exp->node_float = left->node_float + right->node_float;
			}
		return;
		}
		exp->type = 0;
		printf("Error type 7 at line %d:  Operands type mismatched \n",exp->line);
	}

	//-------------------------------------------------Exp MINUS Exp
	else if(exp->type == 12){
		node* left = exp->child;
		node* right = left->brother->brother;
		if(exp_cmp(left,right)==1){
			if(left->type == 1 || left->type == 5){
				exp->type = 1;
				exp->node_int = left->node_int - right->node_int;
			}
			else{
				exp->type = 2;
				exp->node_float = left->node_float - right->node_float;
			}
		return;
		}
		exp->type = 0;
		printf("Error type 7 at line %d:  Operands type mismatched \n",exp->line);
	}

	//--------------------------------------------------Exp STAR Exp
	else if(exp->type == 13){
		node* left = exp->child;
		node* right = left->brother->brother;
		if(exp_cmp(left,right)==1){
			if(left->type == 1 || left->type == 5){
				exp->type = 1;
				exp->node_int = left->node_int * right->node_int;
			}
			else{
				exp->type = 2;
				exp->node_float = left->node_float * right->node_float;
			}
		return;
		}
		exp->type = 0;
		printf("Error type 7 at line %d:  Operands type mismatched \n",exp->line);
	}

	//--------------------------------------------------Exp DIV Exp
	else if(exp->type == 14){	
		node* left = exp->child;
		node* right = left->brother->brother;
		if(exp_cmp(left,right)==1){
			if(left->type == 1 || left->type == 5){
				exp->type = 1;
				exp->node_int = left->node_int / right->node_int;
			}
			else{
				exp->type = 2;
				exp->node_float = left->node_float / right->node_float;
			}
		return;
		}
		exp->type = 0;
		printf("Error type 7 at line %d:  Operands type mismatched \n",exp->line);
	}

	//--------------------------------------------------LP Exp RP
	else if(exp->type == 15){
		exp->type = exp->child->type;
		strcpy(exp->node_value,exp->child->node_value); 
	}

	//--------------------------------------------------MINUS Exp
	else if(exp->type == 16){
		node* left = exp->child->brother;
		if(left->type == 1 || left->type == 5){
			exp->type = 1;
			exp->node_int = -left->node_int;
		}
		else{
			exp->type = 2;
			exp->node_float = -left->node_float;
		}
	}

	//---------------------------------------------------NOT Exp
	else if(exp->type == 17){
		exp->type = 1;
		if(get_exp_judg(exp->child->brother))
			exp->node_int = 0;
		else
			exp->node_int = 1;
	}

	//---------------------------------------------------ID LP Args RP
	else if(exp->type == 18){
		exp->type = 0;
		if(Find(exp->child->node_value)==0){
			printf("Error type 2 at line %d: Function %s is not defined \n",exp->line,exp->child->node_value);
		}
		else if(get_kind(exp->child->node_value) != 4){
			printf("Error type 11 at line %d: %s must be a function\n",exp->line,exp->child->node_value);
		}
		else if(Args_match(exp->child)==0){//varlist not match
			printf("Error type 9 at line %d: The method “%s” is notapplicable”\n",exp->line,exp->child->node_value);
		}
		else{//exp's type is the return of func
			char* RUTURN = get_return(exp->child->node_value);
			if(strcmp(RUTURN,"INT")==0)
				exp->type = 1;
			else if(strcmp(RUTURN,"FLOAT")==0)
				exp->type = 2;
		}
	}

	//---------------------------------------------------ID LP RP
	else if(exp->type == 19){
		exp->type = 0;
		if(Find(exp->child->node_value)==0){
			printf("Error type 2 at line %d: Function %s is not defined \n",exp->line,exp->child->node_value);
		}
		else if(get_kind(exp->child->node_value) != 4){
			printf("Error type 11 at line %d: %s must be a function\n",exp->line,exp->child->node_value);
		}
		else{//exp's type is the return of func
			char* RUTURN = get_return(exp->child->node_value);
			if(strcmp(RUTURN,"INT")==0)
				exp->type = 1;
			else if(strcmp(RUTURN,"FLOAT")==0)
				exp->type = 2;
		}
	}

	//--------------------------------------------------Exp LB Exp RB
	else if(exp->type == 20){
		//printf("name %s\n",exp->child->node_value);
		//printf("kind %d\n", get_kind(exp->child->node_value));
		if(get_kind(exp->child->node_value) != 2){// not array
			printf("Error type 10 at line %d: %s must be an array\n",exp->line,exp->child->node_value);
			exp->type = 0;
		}
		else if(exp->child->brother->brother->type != 1 && exp->child->brother->brother->type != 5 ){
			printf("Error type 12 at line %d: Operands type mistaken\n",exp->line);
			exp->type = 0;
		}
		else{//right
			exp->child->type = 3;
			exp->type = 3;
			strcpy(exp->node_value,exp->child->node_value);
		}
	}

	//--------------------------------------------------Exp DOT ID
	else if(exp->type == 21){	
		exp->type = 0;
		strcpy(exp->node_value,exp->child->node_value);
		if(Find(exp->child->node_value) == 0){
			printf("Error type 1 at line %d: Variable is not defined \n",exp->line);
			printf("Error type 13 at line %d: Illegal use of “.”\n",exp->line);
		}
		else if(get_kind(exp->child->node_value) != 5){
			printf("Error type 13 at line %d: Illegal use of “.”\n",exp->line);
		}
		else if(strcmp(FindStruct(exp->child->node_value,exp->child->brother->brother->node_value),"NULL") == 0){
			printf("Error type 14 at line %d: Un-existed field %s\n", exp->line,exp->child->brother->brother->node_value);
		}
		else{
			strcat(exp->node_value,".");
			strcat(exp->node_value,exp->child->brother->brother->node_value);
			char* type = FindStruct(exp->child->node_value,exp->child->brother->brother->node_value);
			printf("type:%s\n",type);
			if(strcmp(type,"int")==0){
				exp->type = 5;
			}
			else if(strcmp(type,"float")==0){
				exp->type = 6;
			}
			/*else if(strcmp(type,"array")==0){
				exp->type = 3;
			}*/
		}
	}

	//---------------------------------------------------ID
	else if(exp->type == 22){
		if(Find(exp->child->node_value)==0){//var is not in the table
			printf("Error type 1 at line %d: Variable is not defined \n",exp->line);
			exp->type = 0;
			strcpy(exp->node_value ,exp->child->node_value);
		}
		else{
			int kind = get_kind(exp->child->node_value);
			if(kind == 0){//int
				exp->type = 5;
				strcpy(exp->node_value,exp->child->node_value);
			}
			else if(kind == 1){//float
				exp->type = 6;
				strcpy(exp->node_value ,exp->child->node_value);
			}
			else if(kind == 2){//array
				exp->type = 3;
				strcpy(exp->node_value,exp->child->node_value);
			}
			else if(kind == 5){//struct var
				exp->type = 4;
				strcpy(exp->node_value,exp->child->node_value);
			}
			else
				printf("exp->id error\n");
		}
	}

	//--------------------------------------------------INT
	else if(exp->type == 23){
		exp->type = 1;
		exp->node_int = exp->child->node_int;
	}

	//--------------------------------------------------FLOAT
	else if(exp->type == 24){
		exp->type = 2;
		exp->node_float = exp->child->node_float;
	}

}

void Dec_anly(char* Spcid,node* Vardec){//vardec is the first child of the Dec
	if(Find(Vardec->node_value)==1){//var is already in the table
		if(instruct == 0)
			printf("Error type 3 at line %d: Redefined variable '%s' \n",Vardec->line,Vardec->node_value);
		else
			printf("Error type 15 at line %d: Redefined field ‘%s’\n",Vardec->line, Vardec->node_value);
	}
	else{//not defined
		if(Vardec->brother != NULL){
			sem_analysis(Vardec->brother->brother);
		}
		if(strcmp(Spcid,"int")==0){	
			//printf("INT_Insert\n");
			if(Vardec->brother != NULL){
				if(Vardec->brother->brother->type != 1 && Vardec->brother->brother->type != 5){
					printf("Error type 5 at line %d:  Type mismatched\n",Vardec->line);
				}
			}
			if(Vardec->type != 3){
				INT_Insert(Vardec->node_value,0);		
			}
			else{//array
				ARRAY_Insert(Vardec,Vardec->node_value,Spcid);
			}
		}
		else if(strcmp(Spcid,"float")==0){	
			//printf("FLOAT_Insert\n");
			if(Vardec->brother != NULL){
				if(Vardec->brother->brother->type != 2 && Vardec->brother->brother->type != 6){
					printf("Error type 5 at line %d:  Type mismatched\n",Vardec->line);
				}
			}
			if(Vardec->type != 3){
				FLOAT_Insert(Vardec->node_value,0);
			}
			else{
				ARRAY_Insert(Vardec,Vardec->node_value,Spcid);
			}
		}
		else{//var of struct 
			if(Vardec->brother != NULL){
				printf("Error type 5 at line %d:  Type mismatched\n",Vardec->line);
			}
			if(Vardec->type != 3){
				varStruct_Insert(Vardec->node_value,Spcid);
			}
			else{
				ARRAY_Insert(Vardec,Vardec->node_value,Spcid);
			}
		}
	}
}

void DecList_anly(char* Spcid,node* Dec){//Dec is the first child of the DecList
	Dec_anly(Spcid,Dec->child);
	node* p = Dec;
	while(p->brother != NULL){
		p = p->brother->brother->child;//next DecList's first child
		Dec_anly(Spcid,p->child);
	}
}

void Def_anly(node *p){//p is the first child of the Def
	if(strcmp(p->child->name,"StructSpecifier")==0){
		if(Find(p->child->node_value)==0){
			printf("Error type 17 at line %d: Undefined struct “%s” \n",p->line,p->child->node_value);
			return;
		}
	}
	node* DecList = p->brother;
	DecList_anly(p->node_value,DecList->child);
}

void FunDec_def(node *p){
	node* FunDec = p->child->brother;
	if(Find(FunDec->child->node_value)==1){//var is already in the table
		printf("Error type 4 at line %d: Redefined function “%s” \n",FunDec->line,FunDec->child->node_value);
	}
	else{
		FUNC_Insert(p);
	}
}

void ExtDef_anly(node* p){//p is the node of Extdef
	if(strcmp(p->child->brother->name,"FunDec")==0){//func defiend
		memset(func_return,0,sizeof(func_return));
		strcpy(func_return,p->child->node_value);
		FunDec_def(p);
	}
	else if(strcmp(p->child->child->name,"StructSpecifier")==0){
		instruct = 1;
		node* structspecifer = p->child->child;//structspecifer
		if(structspecifer->child->brother->brother != NULL){//define
			if(Find(structspecifer->node_value)==1){
				printf("Error type 16 at line %d: Duplicated name ‘%s’\n",p->child->child->line,p->child->child->node_value);
			}
			else
				STRUCT_Insert(p->child->child);
		}
		else{
			if(Find(structspecifer->node_value)==0){
				printf("Error type 17 at line %d:Undefined struct ‘%s’\n",structspecifer->line,structspecifer->node_value);
			}
		}
	}
	else{//???
		node* structspecifer = p->child->child;//structspecifer
		if(structspecifer->child->brother->brother != NULL){//define
			if(Find(structspecifer->node_value)==1){
				printf("Error type 16 at line %d: Duplicated name ‘%s’\n",p->child->child->line,p->child->child->node_value);
			}
			else
				STRUCT_Insert(p->child->child);
		}
		else{
			if(Find(structspecifer->node_value)==0){
				printf("Error type 17 at line %d:Undefined struct ‘%s’\n",structspecifer->line,structspecifer->node_value);
			}
		}
	}
}

void Stmt_anly(node *p){//p is the first child of the Stmt
	if(p->brother->type == 1 || p->brother->type == 5){//int
		if(strcmp(func_return,"int") == 0){
			return;
		}
	}
	else if(p->brother->type == 2 || p->brother->type == 6){//float
		if(strcmp(func_return,"float") == 0){
			return;
		}
	}
	else if(strcmp(func_return,p->brother->node_value)==0){
		return;
	}
	printf("Error type 8 at line %d: The return type mismatched\n",p->brother->line);
}

void sem_analysis(node *p){		
	char name[20];
	strcpy(name,p->name);
	if(strcmp(name,"Def")==0)
		Def_anly(p->child);
	else if(strcmp(name,"ExtDef")==0){
		ExtDef_anly(p);
		if(p->child != NULL)
			sem_analysis(p->child);
		instruct = 0;
		if(p->brother != NULL)
			sem_analysis(p->brother);
		return;
	}
	else if(strcmp(name,"Exp")==0){
		if(p->child != NULL)
			sem_analysis(p->child);
		exp_cal(p);
		if(p->brother != NULL)
			sem_analysis(p->brother);
		return;
	}
	else if(strcmp(name,"CompSt")==0){
		stack_push();
		if(p->child != NULL)
			sem_analysis(p->child);
		stack_pop(head);
		if(p->brother != NULL)
			sem_analysis(p->brother);
		return;
	}
	else if(strcmp(name,"Args")==0){
		sem_analysis(p->child);
		node* temp = p->child;
		while(temp->brother != NULL){
			temp = temp->brother->brother;
			sem_analysis(temp);
		}
		return;
	}
	else if(strcmp(name,"Stmt")==0){
		if(p->child != NULL)
			sem_analysis(p->child);
		if(strcmp(p->child->name,"RETURN")==0){
			Stmt_anly(p->child);
		if(p->brother != NULL)
			sem_analysis(p->brother);
		return;
		}
	}
	/*else if(strcmp(name,"StructSpecifier")==0){
		if(strcmp(p->child->brother->name,"OptTag") == 0){
			STRUCT_Insert(p);
		}
	}*/
	if(p->child != NULL)
		sem_analysis(p->child);
	if(p->brother != NULL)
		sem_analysis(p->brother);
}
