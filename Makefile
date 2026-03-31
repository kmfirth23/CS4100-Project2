#Authors: Kennedy Firth and Megan Penn

tree: tree.l
	flex tree.l
	gcc lex.yy.c -o tree -lfl

clean:
	rm -f tree lex.yy.c