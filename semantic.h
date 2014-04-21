//#include "type.h"
#include "symbol_table.h"
#include "node.h"

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

void exp_cal(node* exp){//exp is the node exp
	if(exp->type == 7){//Exp ASSIGNOP Exp

	}
	else if(exp->type == 8){//Exp AND Exp

	}
	else if(exp->type == 9){//Exp OR Exp

	}
	else if(exp->type == 10){//Exp RELOP Exp

	}
	else if(exp->type == 11){//Exp PLUS Exp

	}
	else if(exp->type == 12){//Exp MINUS Exp

	}
	else if(exp->type == 13){//Exp STAR Exp

	}
	else if(exp->type == 14){//Exp DIV Exp	

	}
	else if(exp->type == 15){//LP Exp RP

	}
	else if(exp->type == 16){//MINUS Exp

	}
	else if(exp->type == 17){//NOT Exp

	}
	else if(exp->type == 18){//ID LP Args RP

	}
	else if(exp->type == 19){//ID LP RP

	}
	else if(exp->type == 20){//Exp LB Exp RB

	}
	else if(exp->type == 21){//Exp DOT ID	

	}
	else if(exp->type == 22){//ID

	}
	else if(exp->type == 23){//INT
		exp->type = 1;
		exp->node_int = exp->child->node_int;
	}
	else(exp->type == 24){//FLOAT
		exp->type = 2;
		exp->node_float = exp->child->node_float;
	}

}

void minus(node* exp,node* left){
	exp->type = left->type;
	if(left->type == 1){
		exp->node_int = left->node_int;
	}
	else
		exp->node_float = left->node_float;
}

void Dec_anly(char* Spcid,node* Vardec){//vardec is the first child of the Dec
	//if(strcmp(Vardec->node_value,"a")==0){
	if(Find(Vardec->node_value)==1){//var is already in the table
		printf("Error type 3 at line %d: Variable is already defined \n",Vardec->line);
	}
	else{//not defined
		if(strcmp(Spcid,"int")==0){	
			printf("INT_Insert\n");
			INT_Insert(Vardec->node_value,0);		
		}
		else if(strcmp(Spcid,"float")==0){	
			FLOAT_Insert(Vardec->node_value,0);
			printf("FLOAT_Insert\n");
		}
	}
}

void DecList_anly(char* Spcid,node* Dec){//Dec is the first child of the DecList
	Dec_anly(Spcid,Dec);
	node* p = Dec;
	while(p->brother != NULL){
		p = p->brother->brother->child;//next DecList's first child
		Dec_anly(Spcid,p);
	}
}

void Def_anly(node *p){//p is the first child of the Def
	node* DecList = p->brother;
	DecList_anly(p->node_value,DecList->child);
}

void FunDec_def(node *p){
	node* FunDec = p->child->brother;
	if(Find(FunDec->child->node_value)==1){//var is already in the table
		printf("Error type 4 at line %d: FunDec is already defined \n",FunDec->line);
	}
	else{
		FUNC_Insert(p);
	}
}


void sem_analysis(node *p) {		
	char name[20];
	strcpy(name,p->name);
	if(strcmp(name,"Def")==0)
		Def_anly(p->child);
	else if(strcmp(name,"ExtDef")==0){
		if(strcmp(p->child->brother->name,"FunDec")==0)
			FunDec_def(p);
	}
	if(p->child != NULL)
		sem_analysis(p->child);
	if(p->brother != NULL)
		sem_analysis(p->brother);
}