%{
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "syntax.tab.h"
#include "node.h"

int yycolumn = 1;
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; \
yycolumn += yyleng;

node* init_node(char *name,char* yytext);

%}

%option yylineno

letter		[a-zA-Z]
digit		[0-9]
id		_(_|{letter}|{digit})+|{letter}+(_|{letter}|{digit})*
float		{digit}+(\.{digit}+)
int		0|[1-9]{digit}*
oct		0[1-7][0-7]*
hex		0[xX][0-9a-fA-F]+
relop		>|<|>=|<=|==|!=


%%
\t		{}
\n		{yycolumn = 1;}
"+"		{yylval.node_type = init_node("PLUS",yytext);return PLUS;}
-		{yylval.node_type = init_node("MINUS",yytext);return MINUS;}
\*		{yylval.node_type = init_node("STAR",yytext);return STAR;}
\/		{yylval.node_type = init_node("DIV",yytext);return DIV;}
&&		{yylval.node_type = init_node("AND",yytext);return AND;}
\|\|		{yylval.node_type = init_node("OR",yytext);return OR;}
\.		{yylval.node_type = init_node("DOT",yytext);return DOT;}
!		{yylval.node_type = init_node("NOT",yytext);return NOT;}
\(		{yylval.node_type = init_node("LP",yytext);return LP;}
\)		{yylval.node_type = init_node("RP",yytext);return RP;}
\{ 		{yylval.node_type = init_node("LC",yytext);return LC;}
\} 		{yylval.node_type = init_node("RC",yytext);return RC;}
\[ 		{yylval.node_type = init_node("LB",yytext);return LB;}
\] 		{yylval.node_type = init_node("RB",yytext);return RB;}
; 		{yylval.node_type = init_node("SEMI",yytext);return SEMI;}
"," 		{yylval.node_type = init_node("COMMA",yytext);return COMMA;}
[~!@#$%^&:]	{is_error = 1; printf("Error type A at line %d: Mysterious character\'%s\'\n",yylineno,yytext);}
"="		{yylval.node_type = init_node("ASSIGNOP", yytext); return ASSIGNOP; }
struct 	        {yylval.node_type = init_node("STRUCT",yytext);return STRUCT;}
return 	        {yylval.node_type = init_node("RETURN",yytext);return RETURN;}
if 	        {yylval.node_type = init_node("IF",yytext);return IF;}
else		{yylval.node_type = init_node("ELSE",yytext);return ELSE;}
while		{yylval.node_type = init_node("WHILE",yytext);return WHILE;}
int|float	{yylval.node_type = init_node("TYPE",yytext);return TYPE;}
{oct}		{yylval.node_type = init_node("OCT",yytext);return INT;}
{hex}		{yylval.node_type = init_node("HEX",yytext);return INT;}
{int}		{yylval.node_type = init_node("INT",yytext);return INT;}
{float}		{yylval.node_type = init_node("FLOAT",yytext);return FLOAT;}	
{relop}		{yylval.node_type = init_node("RELOP",yytext);return RELOP;}
{id} 		{yylval.node_type = init_node("ID",yytext);return ID;}

%%

int yywarp(){
	return 1;
}

node *init_node(char *name,char* yytext){
	node *p;
	if ((p = malloc(sizeof(node))) == NULL)
        	yyerror("out of memory\n");
	p->child = NULL;
	p->brother = NULL;
	p->type = 0;

	if(strcmp(name,"INT")==0){
		strcpy(p->name,name);
		p->node_int = atoi(yytext);
		return p;
	}
	else if(strcmp(name,"FLOAT")==0){
		strcpy(p->name,name);
		p->node_float = atof(yytext);
		return p;
	}
	else if(strcmp(name,"OCT")==0){
		strcpy(p->name,"INT");
		p->node_int = (int)strtol(yytext,NULL,8);
		return p;
	}
	else if(strcmp(name,"HEX")==0){
		strcpy(p->name,"INT");
		p->node_int = (int)strtol(yytext,NULL,16);
		return p;
	}
	else if(strcmp(name,"ID")==0){
		strcpy(p->name,name);
		strcpy(p->node_value,yytext);
		return p;
	}
	else if(strcmp(name,"TYPE")==0){
		strcpy(p->name,name);
		strcpy(p->node_value,yytext);
		return p;
	}
	else{
		strcpy(p->name,name);
		strcpy(p->node_value,yytext);
		return p;
	}
} 



