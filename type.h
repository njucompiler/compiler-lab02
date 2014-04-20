typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
struct Type_
{
	enum { INT, FLOAT, ARRAY, STRUCTURE, FUNC } kind;
	union
	{
		// 基本类型
		int INT;
		float FLOAT;
		// 数组类型信息包括元素类型与数组大小构成
		struct { Type elem; int size; } array;
		// 结构体类型信息是一个链表
		FieldList structure;
		//函数
		FUNC func;
	} u;
	//Type *next;
};
struct FieldList_
{
	char* name;			// 域的名字
	Type type;			// 域的类型
	FieldList *child;		// 下一个域
	FieldList *brother;		
};
struct FUNC{
	int declare;	//0声明，1实现
<<<<<<< HEAD
	enum{ VOID, INT, FLOAT, ARRAY, STRUCTURE }RETURN;	//返回值类型
	FuncReturn *brother;
};
struct FuncReturn{
	enum{INT, FLOAT, ARRAY, STRUCTURE}kind;
=======
	char RETURN[20];	//返回值类型
	FuncVar *brother;
}
struct FuncVar{
	char kind[20];
>>>>>>> 3e39625ae97f9ae85e846ac5da2c8694810d87fb
	RETURN *next;
};
