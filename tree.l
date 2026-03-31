/*
 * @file cmos.l
 * @author Kennedy Firth and Megan Penn
 * @brief - Lex program to tokenize C++ files. 
 * @date 2026-03-31
*/

%{ //declarations

#include <iostream> 
using namespace std;

//add here

extern "C" int yylex();
extern char *yytext;

int linecount =1; 
int indent;
int n;
char *return_string;

/*Count the number of lines. */
void Addline(char c)
{
	if(c=='\n')
	{
		linecount++;
	}
}
/*An accessor function that returns the line number*/
int line_num()
{
    return linecount;
}

%}

%%
[ \t\n\f]       {Addline(yytext[0]);
                /*Remove tabs/spaces/newlines */}

\/\*            {
                char c;
                while (1) 
                {
                  if ((c = yyinput()) == EOF ){
						//unmatched comment
						cout << "Unterminated comment starting on line" << line_num() << endl;
						return (0);
					}
					Addline(c);
					if (c == '*') {
						char x = yyinput();
						if( x == '/') {
							break;
						}
						else
						{
							unput(x);
						}
					}
				}
				}

////add here

%%
int yywrap(){
    return true; 
}

int main() {
	int token = 0;
	token = yylex();
	while(token){

		switch (token) {
			///add here 
		}

		token = yylex();
        
	}
}