#Authors: Kennedy Firth and Megan Penn

all: tree

tree: prog.tab.c lex.yy.c parse_tree.h tree_node.h
	g++ -o tree prog.tab.c -ll
	
prog.tab.c: tree_builder.y parse_tree.h tree_node.h
	bison tree_builder.y 
	cat prog.tab.c  | sed 's/  __attribute__ ((__unused__))/\/\/ /g' >temp.cc	
	mv temp.cc prog.tab.c
lex.yy.c: prog.l
	flex prog.l
clean:
	rm -f tree lex.yy.c prog.tab.c 