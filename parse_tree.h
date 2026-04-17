//****************************************************************
// File: parse_tree.h
// Author: David W. Juedes
//
// Copyright 2007, All rights reserved.
// Purpose: This .h file includes a hierarchy 
// of classes that define possible 
// instructions in the pfig language
//  
// In general, there are statements, 
// boolean expressions, and integer_expressions
//
//******************************************************************

#include <map>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>
#include "tree_node.h"


using namespace std;

class integer_expression {
 public:
  virtual int evaluate_expression(map<string, int> &sym_tab) =0;
};

class string_expression {
  public:
    virtual string evaluate_expression(map<string, string> &str_tab) =0;
};


class boolean_expression {
 public:
     virtual bool evaluate_expression(map<string, int> &sym_tab)=0;
};


class int_constant:public integer_expression {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return saved_val;
  }
 private:
  int saved_val;
};

class string_variable: public string_expression {
  public: 
    string_variable(char *str_val) {
      saved_val = str_val;
    }

    virtual string evaluate_expression(map<string, string> &str_tab) {
      map<string,string>::iterator p;
      p = str_tab.find(saved_val);

      if (p!=str_tab.end()) {
        return p->second;
      } else {
        return 0;
      }
    }

    
  private:
    string saved_val;
};
class int_variable: public integer_expression {
 public:
  int_variable(char *in_val) {//cout << "Found variable = " << in_val << endl; 
                          saved_val =in_val;}

  virtual int evaluate_expression(map<string, int> &sym_tab) {

    map<string,int>::iterator p;
    p =sym_tab.find(saved_val);
    //cout << "Looking up variable " << saved_val << endl;
    if (p!=sym_tab.end()) {
      //cout << "Returning value of variable " << saved_val << endl;
      //cout << "= " << p->second << endl;

      return p->second;
    } else {
      // ERROR .... for now return -1;
      // Should throw error!
      return -1;
    }

  }
 private:
  string saved_val;
  
};

class plus_expr: public integer_expression {
 public:
  plus_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
  }

  private:
    integer_expression *l;
    integer_expression *r;
};

class less_expr: public boolean_expression {
 public:
  less_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) < r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};
class greater_expr: public boolean_expression {
 public:
  greater_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) > r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};
class ge_expr: public boolean_expression {
 public:
  ge_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) >= r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};
class le_expr: public boolean_expression {
 public:
  le_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) <= r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};
class ee_expr: public boolean_expression {
 public:
  ee_expr(integer_expression *left, integer_expression *right) {
    l=left; r=right;
  }
  virtual bool evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) == r->evaluate_expression(sym_tab);
  }
 private:
  integer_expression *l;
  integer_expression *r;
};



class statement {
 public:
  virtual void evaluate_statement(map<string, int> &sym_tab,
                                  map<string, string> &str_tab,
                                  map<string, Node*> &nod_tab) =0;
};

class compound_statement: public statement {
 public:
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }
  
  virtual void evaluate_statement(map<string, int> &sym_tab,
                                  map<string, string> &str_tab,
                                  map<string, Node*> &nod_tab) {
    if (f!=NULL) {
      f->evaluate_statement(sym_tab, str_tab, nod_tab);
    }
    if (r!=NULL) {
      r->evaluate_statement(sym_tab, str_tab, nod_tab);
    }
  }
 private:
  compound_statement *r;
  statement *f;
};
  

class for_statement: public statement {
 public:
    // current logic is for while loop, change for the for loop
  for_statement(string loopVar, 
                integer_expression *l, 
                integer_expression *u,
                statement* temp) 
  {
    loop_variable = loopVar;
    lower_bound = l;
    upper_bound = u;
    body = temp;
  }

  virtual void evaluate_statement(map<string, int> &sym_tab,
                                  map<string, string> &str_tab,
                                  map<string, Node*> &nod_tab) {
    int low = lower_bound->evaluate_expression(sym_tab);
    int high = upper_bound->evaluate_expression(sym_tab);

    for (int i = low; i <= high; i++){
      sym_tab[loop_variable] = i;
      body->evaluate_statement(sym_tab, str_tab, nod_tab);
    }
  }
    

    
  private:
    string loop_variable;
    integer_expression* lower_bound;
    integer_expression* upper_bound;
    statement* body;
  };

class assignment_statement: public statement {

 public:
  assignment_statement(char *id, integer_expression *rhs) {
    ident = id;
    r_side = rhs;
  }
  virtual void evaluate_statement(map<string, int> &sym_tab,
                                  map<string, string> &str_tab,
                                  map<string, Node*> &nod_tab) {
    
    int temp = r_side->evaluate_expression(sym_tab);

    //cout << "Assigning" << ident << " to " << temp << endl;

    sym_tab[ident]=temp;
  }


  private: 
    string ident;
    integer_expression *r_side;
  };

class print_statement: public statement {
 public:
  print_statement(integer_expression *expr) {
    e=expr;
  }
  virtual void evaluate_statement(map<string, int> &sym_tab,
                                  map<string, string> &str_tab,
                                  map<string, Node*> &nod_tab) {
    cout << e->evaluate_expression(sym_tab) << endl;
  }
    

  private:
    integer_expression *e;

};

class build_statement: public statement {
 public: 
    build_statement(string_expression* n, integer_expression* w, string_expression* p){
        name = n;
        weight = w;
        parent = p;

        // new Node(n, w, p);
    }

    build_statement(string_expression* n, integer_expression* w) {
      name = n;
      weight = w;
      parent = NULL;
    }
    virtual void evaluate_statement(map<string , string> &str_tab, 
                                    map<string, int> &sym_tab, 
                                    map<string, Node*> &nod_tab){
      string tempName = name->evaluate_expression(str_tab);
      int tempWeight = weight ->evaluate_expression(sym_tab);
      
      Node *node_temp;
      
      if (parent != NULL) {
        string tempParent = parent->evaluate_expression(str_tab);
        node_temp = new Node(tempName, tempWeight, tempParent);
      } else {
        node_temp = new Node(tempName, tempWeight);
      }
        //cout << "Assigning" << ident << " to " << temp << endl;

        nod_tab[tempName] = node_temp;

    }

 private:
    string_expression* name;
    integer_expression* weight;
    string_expression* parent;
};
