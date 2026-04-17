%start start_var
%left '+'

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

%token <s_val> TKSTRING TKVARIABLE TKINT
%type <int_ptr> expr integer_expression
%type <s_ptr> statement for_statement assignment_statement print_statement build_statement
%type <c_ptr> program start_var
%type <s_exp_ptr> string_expr
%token TKBUILDNODE TKNAME TKWEIGHT TKISCHILD TKPRINT TKIN TKFOR

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
    : TKSTRING
        {
            $$ = new string_variable($1);
        }
    | TKVARIABLE
        {
            $$ = new string_variable($1);
        }
    ;

build_statement
    : TKBUILDNODE '{' 
        TKNAME '=' string_expr ';' 
        TKWEIGHT '=' expr ';' 
      '}' ';'
        {
            $$ = new build_statement($5, $9);
        }
    | TKBUILDNODE '{'
        TKNAME '=' string_expr ';'
        TKWEIGHT '=' expr ';'
        TKISCHILD '=' string_expr ';'
      '}' ';'
        {
            $$ = new build_statement($5, $9, $13);
        }
    ;

for_statement
    : TKFOR TKVARIABLE TKIN '[' expr ':' expr ']' '{' build_statement '}' ';'
        {
            $$ = new for_statement($2, $5, $7, $10);
        }
    ;

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

assignment_statement
    : TKVARIABLE '=' expr ';'
        {
            $$ = new assignment_statement($1, $3);
        }
    ;

%%

#include "lex.yy.c"

void yyerror(const char *error_string)
{
    cout << "Error : " << error_string << " on line " << line_num() << endl;
    exit(-1);
}

int main() {
    yyparse();
}






