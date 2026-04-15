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
}

%type <s_val> TKSTRING TKVARIABLE TKINT
%type <int_ptr> integer_expression
%type <bool_ptr> boolean_expr
%type <s_ptr> statement for_statement assignment_statement print_statement
%type <c_ptr> prog start_var
%token TKBUILDNODE TKNAME TKWEIGHT TKISCHILD TKPRINT TKSTRING TKINT TKVARIABLE TKIN TKFOR

%union {
    char* s_val;
    integer_expression *int_ptr;
    boolean expression *bool_ptr;
    statement *s_ptr;
    compound_statement *c_ptr;
}

start_var
    : program {
    map<string, Node*> my_sym_tab;
    // map<string, int> my_loop_tab;
    $$ = $1;
    $1->evaluate_statement(my_sym_tab);
    }

program 
    : statement program {
        $$ = new compound_statement($1,$2);
    }
    // probably need stuff here but don't know what 
    |
    ;

statement
    : build_statement {$$ = $1;}
    | for_statement {$$ = $1;}
    | print_statement {$$ = $1;}
    | assignment_statement {$$ = $1;}
    ;

build_statement
    : TKBUILDNODE '{' TKNAME '=' expr ';' TKWEIGHT '=' expr ';' TKISCHILD '=' expr ';' '}' ';'
        {
            $$ = new buildNode($3, $7, $11)
        }


node_fields
    : TKNAME '=' expr ';'
        {
            $$ = new assignment_statement($1, $3);
        }
    | TKWEIGHT '=' expr ';'
        {
            $$ = new assignment_statement($1, $3);
        }
    | TKISCHILD '=' expr ';'
        {
            $$ = new assignment_statement($1, $3);
        }
    ;

for_statement
    : 

print_statement
    : TKPRINT '(' expr ')' ';'
        {
            $$ = new print_statement($3);
        }

assignment_statement
    : TKVARIABLE '=' expr ';'
        {
            $$ = new assignment_statement($1, $3);
        }








