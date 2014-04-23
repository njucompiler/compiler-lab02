#ifndef _NODE_H_
#define _NODE_H_

int is_error = 0;

typedef struct Tnode{
	int type; /*0 undefiend 1 int，2 float 3 array 4 struct 5 var-int 6 var-float 7-24 exp*/
	struct Tnode *child;
	struct Tnode *brother;
	char name[50];
	int line; 
	int node_int;
	float node_float;
	char node_value[20];	
}node;

node* node_init(){
	node *p;
	if ((p = malloc(sizeof(node))) == NULL)
		yyerror("out of memory\n");
	p->type = 2;
	p->child = NULL;
	p->brother = NULL;
	strcpy(p->name,"");
	p->line = 0;
	int node_int=0;
	float node_float=0;
	return p;	
}
#endif
