
%{
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "node.h"
#include "lex.yy.c"
#include "semantic.h"

void show_tree(node *p, int depth);
node *reduction(char *name,int line,int num,...);
void free_tree(node *p);
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
	:	ExtDefList			{$$ = reduction("Program", @1.first_line,1, $1);printf("\n");if(!is_error){	Stackhead_init();sem_analysis($$);/*show_tree($$, 0);*/} free_tree($$); }
	;
ExtDefList
	:	ExtDef ExtDefList		{ $$ = reduction("ExtDefList", @1.first_line,2,$1, $2); }
	|					{ $$ = reduction("ExtDefList", @$.first_line,0); }
	;
ExtDef
	:	Specifier ExtDecList SEMI	{ $$ = reduction("ExtDef",@1.first_line,3, $1, $2, $3); }
	|	Specifier SEMI			{ $$ = reduction("ExtDef",@1.first_line,2, $1, $2); }
	|	Specifier FunDec CompSt		{ $$ = reduction("ExtDef",@1.first_line,3, $1, $2, $3);}
	;
ExtDecList	
	:	VarDec				{ $$ = reduction("ExtDecList",@1.first_line,1, $1); }
	|	VarDec COMMA ExtDecList		{ $$ = reduction("ExtDecList",@1.first_line,3, $1, $2, $3); }
	;
Specifier
	:	TYPE				{ $$ = reduction("Specifier", @1.first_line,1, $1); $$->type=0; strcpy($$->node_value,$1->node_value);}
	|	StructSpecifier			{ $$ = reduction("Specifier", @1.first_line,1, $1); $$->type = 4; strcpy($$->node_value,$1->node_value);}
	;
StructSpecifier
	:	STRUCT OptTag LC DefList RC	{ $$ = reduction("StructSpecifier",@1.first_line, 5, $1, $2, $3, $4,$5); $$->type = 4;strcpy($$->node_value,$2->node_value);}
	|	STRUCT Tag			{ $$ = reduction("StructSpecifier",@1.first_line, 2, $1, $2); $$->type = 4;strcpy($$->node_value,$2->node_value);}
	;
OptTag
	:	ID				{ $$ = reduction("OptTag",@1.first_line, 1, $1); strcpy($$->node_value,$1->node_value);}
	|					{ $$ = reduction("OptTag",@$.first_line, 0); }
	;
Tag
	:	ID				{ $$ = reduction("Tag",@1.first_line, 1, $1); strcpy($$->node_value,$1->node_value);}
	;
VarDec	
	:	ID				{ $$ = reduction("VarDec",@1.first_line, 1, $1); strcpy($$->node_value,$1->node_value);}
	|	VarDec LB INT RB		{ $$ = reduction("VarDec",@1.first_line, 4, $1, $2, $3, $4); $$->type = 3;$1->type = 3;strcpy($$->node_value,$1->node_value);}
	|	error RB
	;
FunDec
	:	ID LP VarList RP		{ $$ = reduction("FunDec", @1.first_line,4, $1, $2, $3, $4); strcpy($$->node_value,$1->node_value);}
	|	ID LP RP			{ $$ = reduction("FunDec",@1.first_line, 3, $1, $2, $3); strcpy($$->node_value,$1->node_value);}
	|	error RP
	;
VarList	
	:	ParamDec COMMA VarList		{ $$ = reduction("VarList",@1.first_line,3, $1, $2, $3); }
	|	ParamDec			{ $$ = reduction("VarList",@1.first_line,1, $1); }
	;
ParamDec
	:	Specifier VarDec		{ $$ = reduction("ParamDec",@1.first_line, 2, $1, $2); strcpy($$->node_value,$1->node_value);}
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
	:	Specifier DecList SEMI		{ $$ = reduction("Def",@1.first_line, 3, $1, $2, $3);}
	|	error SEMI
	;
DecList	
	:	Dec				{ $$ = reduction("DecList",@1.first_line, 1, $1); }
	|	Dec COMMA DecList		{ $$ = reduction("DecList",@1.first_line, 3, $1, $2, $3); }
	;
Dec	:	VarDec				{ $$ = reduction("Dec",@1.first_line, 1, $1); strcpy($$->node_value,$1->node_value);}
	|	VarDec ASSIGNOP Exp		{ $$ = reduction("Dec",@1.first_line, 3, $1, $2, $3); strcpy($$->node_value,$1->node_value);}
	;
Exp
	:	Exp ASSIGNOP Exp		{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 7;}
	|	Exp AND Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 8;}
	|	Exp OR Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 9;}
	|	Exp RELOP Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 10;}
	|	Exp PLUS Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 11;}
	|	Exp MINUS Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 12;}
	|	Exp STAR Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 13;}
	|	Exp DIV Exp			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 14;}
	|	LP Exp RP			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 15;}
	|	MINUS Exp			{ $$ = reduction("Exp",@1.first_line, 2, $1, $2); $$->type = 16;}
	|	NOT Exp				{ $$ = reduction("Exp",@1.first_line, 2, $1, $2); $$->type = 17;}
	|	ID LP Args RP			{ $$ = reduction("Exp",@1.first_line, 4, $1, $2, $3, $4); $$->type = 18;}
	|	ID LP RP			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 19;}
	|	Exp LB Exp RB			{ $$ = reduction("Exp",@1.first_line, 4, $1, $2, $3, $4);$$->type = 20;}
	|	Exp DOT ID			{ $$ = reduction("Exp",@1.first_line, 3, $1, $2, $3); $$->type = 21;}
	|	ID				{ $$ = reduction("Exp",@1.first_line, 1, $1); $$->type = 22;}
	|	INT				{ $$ = reduction("Exp",@1.first_line, 1, $1); $$->type = 23;}
	|	FLOAT				{ $$ = reduction("Exp",@1.first_line, 1, $1); $$->type = 24;}
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

void show_tree(node *p, int depth) {	
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
	else if(strcmp(name,"VarDec")==0)
		printf(": %s type:%d",p->node_value,p->type);
	else if(strcmp(name,"StructSpecifier")==0){
		printf(": %s",p->node_value);
	}
	else if(strcmp(name,"Specifier")==0){
		printf(": %s",p->node_value);
	}
	else if(strcmp(name,"ID")==0||strcmp(name,"TYPE")==0)
		printf(": %s",p->node_value);
	else if(strcmp(name,"Exp")==0){
		printf(": type: %d",p->type);
		if(p->type == 1 || p->type == 5)
			printf(" value %d",p->node_int);
		else if(p->type == 2 || p->type == 6)
			printf(" valua %f",p->node_float);
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

