#Authors: Kennedy Firth and Megan Penn

tree: tree.l
	flex tree.l
	g++ -o tree lex.yy.c
	./tree < example.txt

clean:
	rm -f tree lex.yy.c