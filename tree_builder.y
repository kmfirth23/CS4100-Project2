/*
 * @file tree_builder.y
 * @author Kennedy Firth and Megan Penn
 * @brief Bison (YACC) file to parse and complete syntactic analysis
 * @date 2026-04-15
*/

%start start_var
%left '+'
%error-verbose

%{
    #include <iostream>
    #include <cstdio>
    #include <cstdlib>
    #include <string>
    #include "parse_tree.h"

    using namespace std;

    extern int yylex();
    extern FILE* yyin;
    void yyerror(const char* s);
%}
/*define tokens*/
%token <s_val> TKSTRING TKVARIABLE TKINT
%type <int_ptr> expr integer_expression
%type <s_ptr> statement for_statement assignment_statement print_statement build_statement
%type <c_ptr> program start_var
%type <s_exp_ptr> string_expr
%type <s_ptr> block_statement
%token TKBUILDNODE TKNAME TKWEIGHT TKISCHILD TKPRINT TKIN TKFOR

/*define pointers and values*/
%union {
    char* s_val;
    integer_expression *int_ptr;
    string_expression *s_exp_ptr;
    statement *s_ptr;
    compound_statement *c_ptr;
}

%%

start_var
    : program {
        map<string, int> my_sym_tab;
        map<string, string> my_str_tab;
        map<string, Node*> my_nod_tab;

        $$ = $1;
        if ($1) {
            $1->evaluate_statement(my_sym_tab, my_str_tab, my_nod_tab);
        }
    }
    ;

program 
    : statement program {
        $$ = new compound_statement($1,$2);
    }
    | 
    {
        $$ = nullptr;
    }
    ;

statement
    : build_statement {$$ = $1;}
    | for_statement {$$ = $1;}
    | print_statement {$$ = $1;}
    | assignment_statement {$$ = $1;}
    | block_statement { $$ = $1; }
    ;

block_statement
    : '{' program '}'
        {
            $$ = $2;
        }
    | '{' program '}' ';'
        {
            $$ = $2;
        }
    ;

expr 
    : integer_expression
        {
            $$ = $1;
        }
    ;

integer_expression
    : TKINT
        {
            $$ = new int_constant(atoi($1));
        }
    | TKVARIABLE
        {
            $$ = new int_variable($1);
        }
    /*adding two ints*/
    | integer_expression '+' integer_expression
        {
            $$ = new plus_expr($1, $3);
        }
    | '(' integer_expression ')'
        {
            $$ = $2;
        }
    ;

string_expr
    /*string on its own*/
    : TKSTRING
        {
            $$ = new string_constant($1);
        }
    /*variable of type string*/
    | TKVARIABLE
        {
            $$ = new string_variable($1);
        }
    /*string concatenation of two strings or an int and a string*/
    | string_expr '+' string_expr
        {
            $$ = new string_concat($1, $3);
        }
    | string_expr '+' integer_expression
        {
            $$ = new string_concat($1, $3);
        }
    ;

build_statement
    /*build for root*/
    : TKBUILDNODE '{' 
        TKNAME '=' string_expr ';' 
        TKWEIGHT '=' expr ';' 
      '}' ';'
        {
            $$ = new build_statement($5, $9);
        }
    /*build for child*/
    | TKBUILDNODE '{'
        TKNAME '=' string_expr ';'
        TKWEIGHT '=' expr ';'
        TKISCHILD '=' string_expr ';'
      '}' ';'
        {
            $$ = new build_statement($5, $9, $13);
        }
    ;
/*for statement syntax, call for_statement*/
for_statement
    : TKFOR TKVARIABLE TKIN '[' expr ':' expr ']' block_statement
        {
            $$ = new for_statement($2, $5, $7, $9);
        }
    ;
/*print the expression or the string*/
print_statement
    : TKPRINT '(' expr ')' ';'
        {
            $$ = new print_statement($3);
        }
    | TKPRINT '(' string_expr ')' ';'
        {
            $$ = new print_statement($3);
        }
    ;
/*variable and value to assign to it*/
assignment_statement
    : TKVARIABLE '=' expr ';'
        {
            $$ = new assignment_statement($1, $3);
        }
    ;

%%

#include "lex.yy.c"

/*help to display where the error is occuring*/
void yyerror(const char *error_string)
{
    cout << "Error : " << error_string << " on line " << line_num() << endl;
    exit(-1);
}
/*parse the input */
int main() {
    yyparse();
}






