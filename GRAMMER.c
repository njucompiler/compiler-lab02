Program		
	:	ExtDefList			
	;
ExtDefList
	:	ExtDef ExtDefList		
	|		
	;
ExtDef
	:	Specifier ExtDecList SEMI	//变量定义
	|	Specifier SEMI			
	|	Specifier FunDec CompSt	//函数声明与定义，插入表或检测重复	
	;
ExtDecList	//检测变量是否定义
	:	VarDec				
	|	VarDec COMMA ExtDecList		
	;
Specifier //所有的specifier，有int，float
	:	TYPE				
	|	StructSpecifier			
	;
StructSpecifier   // struct的sprcifier，struct的检测，
	:	STRUCT OptTag LC DefList RC	//定义，插入
	|	STRUCT Tag			//检测有无定义
	;
OptTag	//无操作
	:	ID				
	|					
	;
Tag	//无操作
	:	ID			
	;
VarDec	 //变量名字，包含数组，不可在此来检测是否存在（在更上层），无操作
	:	ID				
	|	VarDec LB INT RB	
	|	error RB
	;
FunDec    //函数，不可检测函数重复（在ExtDef中）
	:	ID LP VarList RP		
	|	ID LP RP			
	|	error RP
	;
VarList	//变量定义list，无操作
	:	ParamDec COMMA VarList		
	|	ParamDec		
	;
ParamDec //params
	:	Specifier VarDec		
	;
CompSt	//无操作
	:	LC DefList StmtList RC		
	|	error RC	
	;
StmtList //语句list，无操作
	:	Stmt StmtList			
	|					
	;
Stmt	//语句，无操作
	:	Exp SEMI			
	|	CompSt			
	|	RETURN Exp SEMI		
	|	IF LP Exp RP Stmt	
	|	IF LP Exp RP Stmt ELSE Stmt
	|	WHILE LP Exp RP Stmt		
	|	error SEMI	
	;
DefList //定义Def的list，无操作
	:	Def DefList			
	|			
	;
Def	//变量定义，插入变量表或者检测重复
	:	Specifier DecList SEMI	
	|	error SEMI
	;
DecList	//在dec中已有操作，变量串
	:	Dec			
	|	Dec COMMA DecList		
	;
Dec	//变量只有赋值，不可检测变量重复（在Def中）	
	:	VarDec				
	|	VarDec ASSIGNOP Exp	//可用来检测类型错误	
	;
Exp	//用来检测类型错误，也有是否定义检测
	:	Exp ASSIGNOP Exp	//	
	|	Exp AND Exp			
	|	Exp OR Exp		
	|	Exp RELOP Exp			
	|	Exp PLUS Exp			
	|	Exp MINUS Exp			
	|	Exp STAR Exp			
	|	Exp DIV Exp			
	|	LP Exp RP			
	|	MINUS Exp		
	|	NOT Exp				
	|	ID LP Args RP	//函数调用，检测是否定义	
	|	ID LP RP	//函数调用，检测是否定义		
	|	Exp LB Exp RB			
	|	Exp DOT ID	//结构是否由该成员	
	|	ID		//是否定义		
	|	INT				
	|	FLOAT				
	|	error RP
	|	error RB
	;
Args
	:	Exp COMMA Args			
	|	Exp				
	;
%%
