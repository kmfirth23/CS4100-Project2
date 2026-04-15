%start start_var
%left '*' '/' '%'
%left '+' '-'

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
    | name_statement
    | weight_statement
    | child_statement
    | print_statement
    | for_statement
    ;

build_statement
    : TKBUILDNODE TKVARIABLE TKSEMI
        {
            parseTree.buildNode($2)
        }
