parser: syntax.y test.l main.c node.h semantic.h type.h symbol_table.h
	bison -d syntax.y
	flex test.l
	gcc main.c syntax.tab.c -lfl -ly -o parser
clean:
	rm lex.yy.c syntax.tab.* parser
