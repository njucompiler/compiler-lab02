
%{
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "node.h"
#include "lex.yy.c"
//#include "type.h"
//#include "symbol_table.h"
//#include "semantic.h"

void show_tree(node *p, int depth);
node *reduction(char *name,int line,int num,...);
void free_tree(node *p);
void exp_cal(node* exp,node* left,node* op,node* rigth);
void minus(node* exp,node* left);
%}

%union { 
	node* node_type;	
};
%locations
%token <node_type> INT ID FLOAT RETURN IF ELSE WHILE TYPE STRUCT
%token <node_type> COMMA SEMI LC RC error

%right <node_type> ASSIGNOP
%left <node_type> OR
%left <node_type> AND
%left <node_type> RELOP
%left <node_type> PLUS MINUS
%left <node_type> STAR DIV
%right <node_type> NOT
%left <node_type> LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <node_type> Program ExtDefList ExtDef ExtDecList
%type <node_type> Specifier StructSpecifier OptTag Tag
%type <node_type> VarDec FunDec VarList ParamDec
%type <node_type> CompSt StmtList Stmt
%type <node_type> DefList Def DecList Dec
%type <node_type> Exp Args

%%
Program		
	:	ExtDefList			{$$ = reduction("Program", @1.first_line
,1, $1);printf("\n");if(!is_error)show_tree($$, 0); free_tree($$); }
	;
ExtDefList
	:	ExtDef ExtDefList		{ $$ = reduction("ExtDefList", @1.first_line,2,$1, $2); }
	|					{ $$ = reduction("ExtDefList", @$.first_line,0); }
	;
ExtDef
	:	Specifier ExtDecList SEMI	{ $$ = reduction("ExtDef",@1.first_line,3, $1, $2, $3); }
	|	Specifier SEMI			{ $$ = reduction("ExtDef",@1.first_line,2, $1, $2); }
	|	Specifier FunDec CompSt		{ $$ = reduction("ExtDef",@1.first_line,3, $1, $2, $3); }
	;
ExtDecList	
	:	VarDec				{ $$ = reduction("ExtDecList",@1.first_line,1, $1); }
	|	VarDec COMMA ExtDecList		{ $$ = reduction("ExtDecList",@1.first_line,3, $1, $2, $3); }
	;
Specifier
	:	TYPE				{ $$ = reduction("Specifier", @1.first_line,1, $1); strcpy($$->node_value,$1->node_value);}
	|	StructSpecifier			{ $$ = reduction("Specifier", @1.first_line,1, $1); strcpy($$->node_value,$1->node_value);}
	;
StructSpecifier
	:	STRUCT OptTag LC DefList RC	{ $$ = reduction("StructSpecifier",@1.first_line, 5, $1, $2, $3, $4,$5); strcpy($$->node_value,$1->node_value);}
	|	STRUCT Tag			{ $$ = reduction("StructSpecifier",@1.first_line, 2, $1, $2); strcpy($$->node_value,$1->node_value);}
	;
OptTag
	:	ID				{ $$ = reduction("OptTag",@1.first_line, 1, $1); }
	|					{ $$ = reduction("OptTag",@$.first_line, 0); }
	;
Tag
	:	ID				{ $$ = reduction("Tag",@1.first_line, 1, $1); }
	;
VarDec	
	:	ID				{ $$ = reduction("VarDec",@1.first_line, 1, $1); strcpy($$->node_value,$1->node_value);}
	|	VarDec LB INT RB		{ $$ = reduction("VarDec",@1.first_line, 4, $1, $2, $3, $4); }
	|	error RB
	;
FunDec
	:	ID LP VarList RP		{ $$ = reduction("FunDec", @1.first_line,4, $1, $2, $3, $4); }
	|	ID LP RP			{ $$ = reduction("FunDec",@1.first_line, 3, $1, $2, $3); }
	|	error RP
	;
VarList	
	:	ParamDec COMMA VarList		{ $$ = reduction("VarList",@1.first_line,3, $1, $2, $3); }
	|	ParamDec			{ $$ = reduction("VarList",@1.first_line,1, $1); }
	;
ParamDec
	:	Specifier VarDec		{ $$ = reduction("ParamDec",@1.first_line, 2, $1, $2); }
	;
CompSt
	:	LC DefList StmtList RC		{ $$ = reduction("CompSt",@1.first_line, 4, $1, $2, $3, $4); }
	|	error RC	
	;
StmtList
	:	Stmt StmtList			{ $$ = reduction("StmtList",@1.first_line,2, $1, $2); }
	|					{ $$ = reduction("StmtList",@$.first_line, 0); }
	;
Stmt	
	:	Exp SEMI			{ $$ = reduction("Stmt",@1.first_line, 2, $1, $2); }
	|	CompSt				{ $$ = reduction("Stmt",@1.first_line, 1, $1); }
	|	RETURN Exp SEMI			{ $$ = reduction("Stmt",@1.first_line, 3, $1, $2, $3); }
	|	IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE{ $$ = reduction("Stmt",@1.first_line, 5, $1, $2, $3, $4, $5); }
	|	IF LP Exp RP Stmt ELSE Stmt	{ $$ = reduction("Stmt",@1.first_line, 7, $1, $2, $3, $4, $5, $6, $7); }
	|	WHILE LP Exp RP Stmt		{ $$ = reduction("Stmt",@1.first_line, 5, $1, $2, $3, $4, $5); }
	|	error SEMI	
	;
DefList
	:	Def DefList			{ $$ = reduction("DefList",@1.first_line, 2, $1, $2); }
	|					{ $$ = reduction("DefList",@$.first_line, 0); }
	;
Def
	:	Specifier DecList SEMI		{ $$ = reduction("Def",@1.first_line, 3, $1, $2, $3); }
	|	error SEMI
	;
DecList	
	:	Dec				{ $$ = reduction("DecList",@1.first_line, 1, $1); }
	|	Dec COMMA DecList		{ $$ = reduction("DecList",@1.first_line, 3, $1, $2, $3); }
	;
Dec	:	VarDec				{ $$ = reduction("Dec",@1.first_line, 1, $1); }
	|	VarDec ASSIGNOP Exp		{ $$ = reduction("Dec",@1.first_line, 3, $1, $2, $3); }
	;
Exp
	:	Exp ASSIGNOP Exp		{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); exp_cal($$, $1, $2, $3);}
	|	Exp AND Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); exp_cal($$, $1, $2, $3);}
	|	Exp OR Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); exp_cal($$, $1, $2, $3);}
	|	Exp RELOP Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); exp_cal($$, $1, $2, $3);}
	|	Exp PLUS Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); exp_cal($$, $1, $2, $3);}
	|	Exp MINUS Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); exp_cal($$, $1, $2, $3);}
	|	Exp STAR Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); exp_cal($$, $1, $2, $3);}
	|	Exp DIV Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); exp_cal($$, $1, $2, $3);}
	|	LP Exp RP			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); }
	|	MINUS Exp			{ $$ = reduction("Exp",@1.first_line, 2, $1, $2); minus($$,$2);}
	|	NOT Exp				{ $$ = reduction("Exp",@1.first_line, 2, $1, $2); }
	|	ID LP Args RP			{ $$ = reduction("Exp",@1.first_line, 4, $1, $2, $3, $4); }
	|	ID LP RP			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); }
	|	Exp LB Exp RB			{ $$ = reduction("Exp",@1.first_line, 4, $1, $2, $3, $4); }
	|	Exp DOT ID			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); }
	|	ID				{ $$ = reduction("Exp",@1.first_line, 1, $1); }
	|	INT				{ $$ = reduction("Exp",@1.first_line, 1, $1); $$->type = 1;$$->node_int = $1->node_int;}
	|	FLOAT				{ $$ = reduction("Exp",@1.first_line, 1, $1); $$->type = 2;$$->node_float = $1->node_float;}
	|	error RP
	|	error RB
	;
Args
	:	Exp COMMA Args			{ $$ = reduction("Args",@1.first_line, 3, $1, $2, $3); }
	|	Exp				{ $$ = reduction("Args",@1.first_line, 1, $1); }
	;
%%

yyerror(char* msg) {
	is_error++;
	printf("\nError type B at line %d: %s",yylineno,msg);
}

node *reduction(char *name,int line,int num,...){
	va_list ap;	
	int i;
	node *p=node_init();
	p->type = 1;
	p->line = line;
	strcpy(p->name,name);
	//strcpy(p->node_value,name);
	va_start(ap,num);
	if(num == 0){
		va_end(ap);
		return p;
	}
	else{
	p->child=node_init();
	p->child=(node*)va_arg(ap,node*);
	node *q = node_init();
	q = p->child;
	for(i = 0;i<num-1;i++){
		q->brother = node_init();
		q->brother = (node*)va_arg(ap,node*);
		q = (node*)q->brother;
	}
	va_end(ap);
	return p;
	}
} 

void exp_cal(node* exp,node* left,node* op,node* rigth){
	if(left->type != rigth->type){
			printf("Error type 16 at line %d:Type mismatched",exp->line);
	}
	else{
		exp->type = left->type;
		if(strcmp(op->name,"ASSIGNOP")==0){//ASSIGNOP
			if(left->type == 1)
				exp->node_int = rigth->node_int;
			else
				exp->node_float = rigth->node_float;
		}
		else if(strcmp(op->name,"AND")==0){//AND

		}
		else if(strcmp(op->name,"OR")==0){//OR

		}
		else if(strcmp(op->name,"PLUS")==0){//PLUS
			if(left->type == 1)
				exp->node_int = left->node_int + rigth->node_int;
			else
				exp->node_float = left->node_float + rigth->node_float;
		}
		else if(strcmp(op->name,"MINUS")==0){//MINUS
			if(left->type == 1)
				exp->node_int = left->node_int - rigth->node_int;
			else
				exp->node_float = left->node_float - rigth->node_float;
		}
		else if(strcmp(op->name,"STAR")==0){//STAR
			if(left->type == 1)
				exp->node_int = left->node_int * rigth->node_int;
			else
				exp->node_float = left->node_float * rigth->node_float;
		}
		else if(strcmp(op->name,"DIV")==0){//DIV
			if(left->type == 1)
				exp->node_int = left->node_int / rigth->node_int;
			else
				exp->node_float = left->node_float / rigth->node_float;
		}
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

void show_tree(node *p, int depth) {	
	//printf("\n");
	//printf("depth:%d\n",depth);
	int i;	
	for(i=0;i<depth;i++)
		printf(" ");	
	//printf("%d\n",p->type);printf("%d\n",p->line);
	printf("%s",p->name);
	//printf("%s\n",p->child->name);printf("%s\n",p->child->child->name);printf("%s\n",p->child->child->brother->name);printf("%s\n",p->child->child->child->name);printf("%s\n",p->child->child->child->brother->name);printf("%s\n",p->child->child->child->brother->brother->name);
	char name[20];
	strcpy(name,p->name);
	if(strcmp(name,"INT")==0)
		printf(": %d",p->node_int);
	else if(strcmp(name,"FLOAT")==0)
		printf(": %f",p->node_float);
	else if(strcmp(name,"StructSpecifier")==0){
		printf(": %s",p->node_value);
	}
	else if(strcmp(name,"Specifier")==0){
		printf(": %s",p->node_value);
	}
	else if(strcmp(name,"ID")==0||strcmp(name,"TYPE")==0)
		printf(": %s",p->node_value);
	else if(strcmp(name,"Exp")==0){
		if(p->type == 1)
			printf(" type1:%d",p->node_int);
		else
			printf(" type2:%f",p->node_float);
	}
	else 
		printf(" (%d)",p->line);
	printf("\n");
	if(p->child != NULL)
		show_tree(p->child,depth+1);
	if(p->brother != NULL)
		show_tree(p->brother,depth);
}

void free_tree(node *p){
	if(p->brother != NULL)
		free_tree(p->brother);
	if(p->child != NULL)
		free_tree(p->child);
	free(p);
}

