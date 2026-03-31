/*
 * @file tree.l
 * @author Kennedy Firth and Megan Penn
 * @brief - Lex program to tokenize C++ files. 
 * @date 2026-03-31
*/

%{ //declarations

#include <iostream>
#include <stdlib.h> 
#include <string>

using namespace std;

#define TKBUILDNODE 001
#define TKNAME 002
#define TKWEIGHT 003
#define TKISCHILD 004
#define TKPRINT 005
#define TKSTRING 006
#define TKINT 007
#define TKVARIABLE 010
#define TKSEMI 011
#define TKOPEN 012


extern "C" int yylex();
extern char *yytext;

int linecount =1; 
int indent;
int n;
char *return_string;
int value;
string var;
string str;

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


/*variable names alphabet*/
/*V [a-zA-Z1-9_]
S [a-zA-Z1-9_ ]*/
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

buildnode			{return TKBUILDNODE;}
"print"				{return TKPRINT;}
name 				{return TKNAME;}
isachildof			{return TKISCHILD;}
weight				{return TKWEIGHT;}
[0-9]+				{value = atoi(yytext); return TKINT;}
";"					{return TKSEMI;}
"{"					{return TKOPEN;}
["“][a-zA-Z0-9]+["”]	{str = yytext; return TKSTRING;}
[a-zA-Z_][a-zA-Z0-9_]*		{var = yytext; return TKVARIABLE;}
[0-9][a-zA-Z_][a-zA-Z0-9_]*		{var = yytext; return TKVARIABLE;}


%%
int yywrap(){
    return true; 
}

int main() {
	int token = 0;
	token = yylex();
	while(token){

		switch (token) {
			case TKBUILDNODE : cout << "BUILDNODE";
				break; 
			case TKNAME : cout << "NAME";
				break;
			case TKWEIGHT : cout << "WEIGHT";
				break;
			case TKISCHILD : cout << "ISACHILDOF";
				break;
			case TKPRINT : cout <<"PRINT";
				break;
			case TKINT : cout << "INT " << value;
				break;
			case TKSEMI : cout << "; " << endl;
				break;
			case TKOPEN : cout << "{ " << endl;
				break; 
			case TKVARIABLE : cout << "VARIABLE " << var;
				break;
			case TKSTRING : cout << "STRING " << str;
		}

		token = yylex();
        
	}
}