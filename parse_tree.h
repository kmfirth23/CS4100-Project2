//****************************************************************
// File: parse_tree.h
// Author: David W. Juedes (Base Program)
//         Kennedy Firth and Megan Penn (Additions for Tree Builder Class)
// 
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

/**
 * @name integer_expression
 *       evaluates an integer 
 * 
 */
class integer_expression {
 public:
  virtual int evaluate_expression(map<string, int> &sym_tab) =0;
};

/**
 * @name string_expression
 *       evaluates a string
 * 
 */
class string_expression {
  public:
    virtual string evaluate_expression(map<string, int> &sym_tab, 
                                      map<string, string> &str_tab, 
                                      map<string, Node*> &nod_tab ) =0;
};

/**
 * @name boolean_expression
 *       evaluates a boolean expression
 * 
 */
class boolean_expression {
 public:
     virtual bool evaluate_expression(map<string, int> &sym_tab)=0;
};

/**
 * @name int_constant
 *       evaluates a constant integer, not attatched to a variable
 * 
 * saved_val: value stored in the symbol table
 */
class int_constant:public integer_expression {
 public:
  int_constant(int val) {saved_val = val;}

  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return saved_val;
  }
 private:
  int saved_val;
};

/**
 * @name string variable
 *       evaluates a string that is attached to a variable
 * 
 * saved_val: value stored in the symbol table
 */
class string_variable: public string_expression {
  public: 
    string_variable(char *str_val) {
      if(str_val != nullptr)
      {
        saved_val = str_val;
      }
      else
      {
        saved_val = "";
      }
    }

    virtual string evaluate_expression(map<string, int> &sym_tab, 
                                      map<string, string> &str_tab, 
                                      map<string, Node*> &nod_tab) {
      map<string,string>::iterator p;
      p = str_tab.find(saved_val);

      if (p!=str_tab.end()) {
        return p->second;
      } else {
        return "";
      }
    }

    
  private:
    string saved_val;
};

/**
 * @name integer_expression
 *       returns an integer value from the symbol table
 * 
 */
class int_variable: public integer_expression {
 public:
  int_variable(char *in_val) {
    if(in_val != nullptr)
    {
      saved_val = in_val;
    }
    else
    {
      saved_val = "";
    }
  }

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

/**
 * @name plus_expr
 *       evaluates an addition equation between two integers
 * 
 * l - pointer to the integer on the left side of the expression
 * r - pointer to the integer on the right side of the expression
 */
class plus_expr: public integer_expression {
 public:
  // constructor
  plus_expr(integer_expression *left, integer_expression *right) {
    l = left;
    r = right;
  }

  // evaluation
  virtual int evaluate_expression(map<string, int> &sym_tab) {
    return l->evaluate_expression(sym_tab) + r->evaluate_expression(sym_tab);
  }

  private:
    integer_expression *l;
    integer_expression *r;
};

/**
 * @name less_expr
 *       evaluates a less than boolean expression
 */
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

/**
 * @name greater_expr
 *       evaluates a greater than expression
 * 
 */
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

/**
 * @name ge_expr
 *       evaluates a greater than or equal to expression
 */
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

/**
 * @name le_expr
 *       evaluates a less than or equal to expression 
 */
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

/**
 * @name ee_expr
 *       evaluates an equal to expression
 * 
 */
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

/**
 * @name statement
 *       parent class for statements
 */
class statement {
 public:
  virtual void evaluate_statement(map<string, int> &sym_tab,
                                  map<string, string> &str_tab,
                                  map<string, Node*> &nod_tab) =0;
};

/**
 * @name compound_statement
 *       evaluates statements that combine different types
 * 
 * f - the current statement being evaluated
 * r - the rest of the program
 */
class compound_statement: public statement {
 public:
  // constructor
  compound_statement(statement *first, compound_statement *rest) {
    f = first;
    r = rest;
  }
  
  // evaluation
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
  

/**
 * @name for_statement
 *       evaluates a for loop
 * 
 * loop_variable - variable being incremented
 * lower_bound - starting value for loop
 * upper_bound - terminating value for loop
 * body - content inside the loop
 */
class for_statement: public statement {
 public:
    // constructor
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

  // evaluation
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

/**
 * @name assignment_statement
 *       evaluates the assignment of variables
 * 
 * id - the name of the variable
 * rhs - the value to be assigned
 */
class assignment_statement: public statement {

 public:
  // constructor
  assignment_statement(char *id, integer_expression *rhs) {
    if(id != nullptr)
    {
      ident = id;
    }
    else
    {
      ident = "";
    }
    r_side = rhs;
  }

  // evaluation
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

/**
 * @name print_statement
 *       calls the print function for the node class
 * 
 * e - integer expression to print
 * s - string expression to print
 */
class print_statement: public statement {
 public:
  print_statement(integer_expression *expr) {
    e=expr;
    s = nullptr;
  }
  print_statement(string_expression *str)
  {
    e=nullptr;
    s = str;
  }
  virtual void evaluate_statement(map<string, int> &sym_tab,
                                  map<string, string> &str_tab,
                                  map<string, Node*> &nod_tab) {
    if(e != nullptr)
      cout << e->evaluate_expression(sym_tab) << endl;
    else if (s != nullptr) {
      string val = s->evaluate_expression(sym_tab, str_tab, nod_tab);
      auto p = nod_tab.find(val);
      if (p != nod_tab.end()) {
        p->second->print();
        cout << endl;
      } else {
        cout << val << endl;
      }
    }

  }
    

  private:
    integer_expression *e;
    string_expression *s; 

};

/**
 * @name build_statement
 *       evaluates the build function in the tree builder language
 * 
 * name - name of the node
 * weight - weight of the node
 * parent - parent of the node, defaults to empty string
 */
class build_statement: public statement {
 public: 
    // constructor
    build_statement(string_expression* n, integer_expression* w, string_expression* p){
        name = n;
        weight = w;
        parent = p;

        // new Node(n, w, p);
    }

    // constructor for root node
    build_statement(string_expression* n, integer_expression* w) {
      name = n;
      weight = w;
      parent = NULL;
    }

    // evaluation
    virtual void evaluate_statement(map<string, int> &sym_tab,
                                    map<string , string> &str_tab,  
                                    map<string, Node*> &nod_tab){
      string tempName = name->evaluate_expression(sym_tab, str_tab, nod_tab);
      int tempWeight = weight ->evaluate_expression(sym_tab);
      
      Node *node_temp;
      
      if (parent != NULL) {
        string tempParent = parent->evaluate_expression(sym_tab, str_tab, nod_tab);
        node_temp = new Node(tempName, tempWeight, tempParent);
        nod_tab[tempName] = node_temp;
        auto p = nod_tab.find(tempParent);
        if (p != nod_tab.end()) {
          p->second->children.push_back(node_temp);
        }
      } else {
        node_temp = new Node(tempName, tempWeight);
        nod_tab[tempName] = node_temp;
      }
        //cout << "Assigning" << ident << " to " << temp << endl;

    }

 private:
    string_expression* name;
    integer_expression* weight;
    string_expression* parent;
};

/**
 * @name string constant
 *       evaluates a string that is literal, not attached to a variable
 * 
 * saved_val - the value being stored from the string
 */
class string_constant : public string_expression {
  public:
    // constructor
    string_constant(char *str_val) {
      if(str_val != nullptr)
      {
          saved_val = str_val;
      }
      else
      {
        saved_val = "";
      }
      if (saved_val.size() >= 2 &&
          saved_val.front() == '"' &&
          saved_val.back() == '"') {
        saved_val = saved_val.substr(1, saved_val.size() - 2);
      }
    }

    // evaluation
    virtual string evaluate_expression(map<string, int> &sym_tab,
                                      map<string, string> &str_tab,
                                      map<string, Node*> &nod_tab) {
      return saved_val;
    }
  
  private:
    string saved_val;
};

/**
 * @name int_to_string_expr
 *       converts an integer to a string for string concatenation
 * 
 * expr - the integer to be converted
 */
class int_to_string_expr : public string_expression {
  public:
    // constructor
    int_to_string_expr(integer_expression *e) { expr = e; }

    // calling to_string to convert
    virtual string evaluate_expression(map<string, int> &sym_tab,
                                       map<string, string> &str_tab,
                                       map<string, Node*> &nod_tab) {
      return to_string(expr->evaluate_expression(sym_tab));
    }

  private:
    integer_expression *expr;
};

/**
 * @name string_concat
 *       evaluates instances of string concatenation
 * 
 * first - the first half of the concatenation expression
 * second - the second half of the concatenation expression
 */
class string_concat: public string_expression {
    public: 
        // constructor for concatenating two strings
        string_concat(string_expression *str_val1, string_expression *str_val2){
            first = str_val1;
            second = str_val2;
        }
        
        // constructor for concatenating a string and an integer
        string_concat(string_expression *str_val, integer_expression *int_val){
          first = str_val;
          second = new int_to_string_expr(int_val);
        }

        // evaluating
        virtual string evaluate_expression(map<string, int> &sym_tab, 
                                      map<string, string> &str_tab, 
                                      map<string, Node*> &nod_tab) {
            string left;
            string right;
            if(first != nullptr)
            {
              left = first->evaluate_expression(sym_tab, str_tab, nod_tab);
            }
            else
            {
              left = "";
            }

            if(second != nullptr)
            {
              right = second->evaluate_expression(sym_tab, str_tab, nod_tab);
            }
            else
            {
              right = "";
            }
            return left + right;
        }
    private: 
        string_expression *first;
        string_expression *second;
};