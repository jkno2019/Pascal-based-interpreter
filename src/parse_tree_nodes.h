//*****************************************************************************
// purpose: Node classes used while building a parse tree for the arithmetic expression.
//          Added interpret methods for each node type.
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

extern bool printDelete;      // shall we print deleting the tree?

// Define truth for a floating-point number:
// falsehood == F is within EPSILON of 0.0
// truth == not falsehood
#define EPSILON 0.001
static bool truth(float F) {
 return !((EPSILON > F) && (F > -EPSILON));
}

// ---------------------------------------------------------------------
// Forward declaration of node types
class ProgramNode;
class BlockNode;
class StatementNode;
class CompoundNode;
class ExprNode;
class SimpleExpNode; 
class TermNode;
class FactorNode;


// ---------------------------------------------------------------------
class ProgramNode {
  public:
    int _level = 0;
    BlockNode* block = nullptr;

    ProgramNode(int level, BlockNode* bn);
    ~ProgramNode();
    void interpret();
};
ostream& operator<<(ostream&, ProgramNode&); // Node print operator
// ---------------------------------------------------------------------
class BlockNode {
  public:
    int _level = 0;
    CompoundNode* compound = nullptr;

    BlockNode(int level, CompoundNode* compN);
    ~BlockNode();
    void interpret();
};
ostream& operator<<(ostream&, BlockNode&); // Node print operator
// ---------------------------------------------------------------------
class StatementNode {
  public:
    int _level = 0;

    virtual void printTo(ostream &os) = 0;
    virtual void interpret() = 0;
    virtual ~StatementNode();
};
ostream& operator<<(ostream&, StatementNode&); // Node print operator
// ---------------------------------------------------------------------
class AssignmentNode : public StatementNode {
  public:
    string id;
    ExprNode* exprPtr = nullptr;

    AssignmentNode(int level, string name);
    ~AssignmentNode();
    void printTo(ostream & os);
    void interpret();
};
// ---------------------------------------------------------------------
class CompoundNode : public StatementNode {
  public:
    vector<StatementNode*> statements;
    StatementNode* statePtr = nullptr;

    CompoundNode(int level);
    ~CompoundNode();
    void printTo(ostream &os);
    void interpret();
};
// ---------------------------------------------------------------------
class IfNode : public StatementNode {
  public:
    ExprNode* exprPtr = nullptr;
    StatementNode* statePtr1 = nullptr;
    StatementNode* statePtr2 = nullptr;

    IfNode(int level, ExprNode* en);
    ~IfNode();
    void printTo(ostream & os);
    void interpret();
};
// ---------------------------------------------------------------------
class WhileNode : public StatementNode {
  public:
    ExprNode* exprPtr = nullptr;
    StatementNode* statePtr = nullptr;

    WhileNode(int level, ExprNode* en);
    ~WhileNode();
    void printTo(ostream & os);
    void interpret();
};
// ---------------------------------------------------------------------
class ReadNode : public StatementNode {
  public:
    string text;

    ReadNode(int level, string name);
    ~ReadNode();
    void printTo(ostream & os);
    void interpret();
};
// ---------------------------------------------------------------------
class WriteNode : public StatementNode {
  public:
    string text;
    int type;

    WriteNode(int level, string name);
    ~WriteNode();
    void printTo(ostream & os);
    void interpret();
};
// ---------------------------------------------------------------------
// <expr> -> <simple_exp> {{ (( = || < || > || <> )) <simple_exp> }}
class ExprNode {
public:
  int _level = 0;          // recursion level of this node
  SimpleExpNode* firstSimpleExp = nullptr;
  vector<int> restSimpleExpOps; // TOK_EQUALTO or TOK_LESSTHAN or TOK_GREATERTHAN or TOK_NOTEQUALTO
  vector<SimpleExpNode*> restSimpleExps;
  float interpret();

  ExprNode(int level, SimpleExpNode* sn);
  ~ExprNode();
};
ostream& operator<<(ostream&, ExprNode&); // Node print operator
// ---------------------------------------------------------------------
class SimpleExpNode {
  public:
    int _level = 0;          // recursion level of this node
  TermNode* firstTerm = nullptr;
  vector<int> restTermOps; // TOK_ADD_OP or TOK_SUB_OP
  vector<TermNode*> restTerms;
  float interpret();

  SimpleExpNode(int level, TermNode* tn);
  ~SimpleExpNode();
};
ostream& operator<<(ostream&, SimpleExpNode&); // Node print operator
// ---------------------------------------------------------------------
// <term> -> <factor> {{ (( * || / || AND )) <factor> }}
class TermNode {
public:
  int _level = 0;              // recursion level of this node
  FactorNode* firstFactor = nullptr;
  vector<int> restFactorOps;   // TOK_MULT_OP or TOK_DIV_OP
  vector<FactorNode*> restFactors;
  float interpret();

  TermNode(int level, FactorNode* fn);
  ~TermNode();
};
ostream& operator<<(ostream&, TermNode&); // Node print operator
// ---------------------------------------------------------------------
// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode, FloatLitNode, NotNode, MinusNode
// <factor> -> ID || INTLIT || ( <expr> ) || FLOATLIT || NOT <factor> || - <factor>
class FactorNode {
public:
  int _level = 0;                        // recursion level of this node

  virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
  virtual float interpret() = 0;
  virtual ~FactorNode();                 // labeling the destructor as virtual allows 
	                                       // the subclass destructors to be called
};
ostream& operator<<(ostream&, FactorNode&); // Node print operator
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int level, float value);
    ~IntLitNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class FloatLitNode (Float Literal Node)
class FloatLitNode : public FactorNode {
public:
    float float_literal = 0.0;

    FloatLitNode(int level, float value);
    ~FloatLitNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
    string id;

    IdNode(int level, string name);
    ~IdNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(int level, ExprNode* en);
    void printTo(ostream & os);
    ~NestedExprNode();
    float interpret();
};
// ---------------------------------------------------------------------
// class NotNode (Not Node)
class NotNode : public FactorNode {
public:
    FactorNode* factorPtr = nullptr;

    NotNode(int level, FactorNode* factor);
    ~NotNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class MinusNode (Identifier Node)
class MinusNode : public FactorNode {
  public:
      FactorNode* factorPtr = nullptr;

      MinusNode(int level, FactorNode* factor);
      ~MinusNode();
      void printTo(ostream & os);
      float interpret();
};

#endif /* PARSE_TREE_NODES_H */
