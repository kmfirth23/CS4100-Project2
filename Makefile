#Authors: Kennedy Firth and Megan Penn

all: tree

tree: prog.tab.c lex.yy.c parse_tree.h tree_node.h
	g++ -o tree prog.tab.c 
	
prog.tab.c: tree_builder.y parse_tree.h tree_node.h
	bison -d -o prog.tab.c tree_builder.y 
	cat prog.tab.c  | sed 's/  __attribute__ ((__unused__))/\/\/ /g' > temp.cc	
	mv temp.cc prog.tab.c
lex.yy.c: tree_builder.l
	flex -o lex.yy.c tree_builder.l
clean:
	rm -f tree lex.yy.c prog.tab.c prog.tab.h temp.cc