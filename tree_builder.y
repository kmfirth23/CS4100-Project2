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

    ParseTree parseTree;
}

%token <s_val> TKSTRING TKVARIABLE
%token <i_val> TKINT
%token TKBUILDNODE TKNAME TKWEIGHT TKISCHILD TKPRINT TKSTRING TKINT TKVARIABLE TKSEMI TKOPEN TKIN TKFOR

%union {
    char* s_val;
    int i_val;
}

start_var
    : program
    ;

program 
    : statement program
    |
    ;

statement
    : build_statement
    | for_statement
    | print_statement
    ;

build_statement
    : TKBUILDNODE '{' node_field '}' ';'
        {
            parseTree.buildNode($2)
        }

node_field
    : TKNAME '=' expr ';'
    | TKWEIGHT '=' expr ';'
    | TKISCHILD '=' expr ';'
    ;

print_statement
    : TKPRINT '(' expr ')' ';'





