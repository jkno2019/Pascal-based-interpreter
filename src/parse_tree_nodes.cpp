//*****************************************************************************
// purpose: Node classes used while building a parse tree for the arithmetic expression.
//          Added interpret methods for each node type.
//*****************************************************************************

#include "parse_tree_nodes.h"
#include "parser.h"

bool printDelete = false;   // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
  for (int i = 0; i < level; i++) // EDIT: I changed the 0 -> 1 future me
    cout << ("|  ");
}
// ---------------------------------------------------------------------
ProgramNode::ProgramNode(int level, BlockNode* bn){
  _level = level;
  block = bn;
}
ostream& operator<<(ostream& os, ProgramNode& pn){
  os << endl; indent(pn._level); os << "(program ";
	os << *(pn.block);
  os << endl; indent(pn._level); os << "program) ";
  return os;
}
ProgramNode::~ProgramNode(){
  if(printDelete)
    cout << "Deleting ProgramNode" << endl;
  delete block;
  block = nullptr;
}
void ProgramNode::interpret() {
  block->interpret();
}
// ---------------------------------------------------------------------
BlockNode::BlockNode(int level, CompoundNode* compN){
  _level = level;
  compound = compN;
}
ostream& operator<<(ostream& os, BlockNode& bn){
  os << endl; indent(bn._level); os << "(block ";
  os << *(bn.compound);
  os << endl; indent(bn._level); os << "block) ";
  return os;
}
BlockNode::~BlockNode(){
  if(printDelete)
    cout << "Deleting BlockNode" << endl;
  delete compound;
  compound = nullptr;
}
void BlockNode::interpret() {
  compound->interpret();
}
// ---------------------------------------------------------------------
StatementNode::~StatementNode() {}
ostream& operator<<(ostream& os, StatementNode& stateN) {
   stateN.printTo(os);
	 return os;
}
// Use double dispatch to route to proper interpret method
void StatementNode::interpret() {
  this->interpret();
}
// ---------------------------------------------------------------------
AssignmentNode::AssignmentNode(int level, string name) {
  _level = level;
  id = name;
}
void AssignmentNode::printTo(ostream& os) {
  os << endl; indent(_level); os << "(assignment_stmt ";
  os << "( " << id << " := )";
  os << *exprPtr;
  os << endl; indent(_level); os << "assignment_stmt)";
}
AssignmentNode::~AssignmentNode() {
  if (printDelete) {
    cout << "Deleting StatementNode:AssignmentStmtNode" << endl; 
  }
  
  delete exprPtr;
  exprPtr = nullptr;
}
void AssignmentNode::interpret() {
// Look up the variable that will store expression result
 symbolTableT::iterator variable = symbolTable.find(id);
 // Put the expression in the variable
 variable->second = exprPtr->interpret();
}
// ---------------------------------------------------------------------
CompoundNode::CompoundNode(int level) {
  _level = level;
}
void CompoundNode::printTo(ostream& os) {
  os << endl; indent(_level); os << "(compound_stmt";
  int length = statements.size();
  for (int i = 0; i < length; i++){
    statements[i]->printTo(os);
  };
  os << endl; indent(_level); os << "compound_stmt)";
}
CompoundNode::~CompoundNode() {
  if (printDelete) {
    cout << "Deleting StatementNode:CompoundStmtNode" << endl;
  }
  int length = statements.size();
  for (int i = 0; i < length; i++){
    delete statements[i];
    statements[i] = nullptr;
  }
}
void CompoundNode::interpret() {
  int length = statements.size();
  for (int i = 0; i < length; i++) {
    statements[i]->interpret();
  }
}
// ---------------------------------------------------------------------
IfNode::IfNode(int level, ExprNode* en) {
  _level = level;
  exprPtr = en;
}
void IfNode::printTo(ostream& os) {
  os << endl; indent(_level); os << "(if_stmt ";
  os << *exprPtr;
  os << endl; indent(_level); os << "(then ";
  os << *statePtr1;
  os << endl; indent(_level); os << "then) ";
  if (statePtr2 != nullptr) {
    // if statePrt2 has content then we print
    // if not, then we can skip
    os << endl; indent(_level); os << "(else ";
    os << *statePtr2;
    os << endl; indent(_level); os << "else) ";
  }
  os << endl; indent(_level); os << "if_stmt)";
}
IfNode::~IfNode() {
  if (printDelete) {
    cout << "Deleting StatementNode:IfStmtNode" << endl; 
  }
  delete exprPtr;
  exprPtr = nullptr;
  delete statePtr1;
  statePtr1 = nullptr;
  delete statePtr2;
  statePtr2 = nullptr;
}
void IfNode::interpret() {
  if(truth(exprPtr->interpret())) {
    statePtr1->interpret();
  }
  else {
    if (statePtr2 != nullptr)
      statePtr1->interpret();
  }
}
// ---------------------------------------------------------------------
WhileNode::WhileNode(int level, ExprNode* en) {
  _level = level;
  exprPtr = en;
}
void WhileNode::printTo(ostream& os) {
  os << endl; indent(_level); os << "(while_stmt ";
  os << *exprPtr;
  os << *statePtr;
  os << endl; indent(_level); os << "while_stmt)";
}
WhileNode::~WhileNode() {
  if (printDelete) {
    cout << "Deleting StatementNode:WhileStmtNode" << endl;
  }
  delete exprPtr;
  exprPtr = nullptr;
  delete statePtr;
  statePtr = nullptr;
}
void WhileNode::interpret() {
  while(truth(exprPtr->interpret())) {
  statePtr->interpret();
  }
}
// ---------------------------------------------------------------------
ReadNode::ReadNode(int level, string name) {
  _level = level;
  text = name;
}
void ReadNode::printTo(ostream& os) {
  os << endl; indent(_level); os << "(read_stmt ";
  os << "( " << text << " )";
  os << endl; indent(_level); os << "read_stmt)";
}
ReadNode::~ReadNode() {
  if (printDelete) {
    cout << "Deleting StatementNode:ReadStmtNode" << endl;
  }

}
void ReadNode::interpret() {
  // If we are writing a variable, look up and print value
  symbolTableT::iterator variable = symbolTable.find(text);
  cin >> variable->second;
}
// ---------------------------------------------------------------------
WriteNode::WriteNode(int level, string name) {
  _level = level;
  text = name;
}
void WriteNode::printTo(ostream& os) {
  os << endl; indent(_level); os << "(write_stmt "; 
  os << "( " << text << " )";
  os << endl; indent(_level); os << "write_stmt)";
}
WriteNode::~WriteNode() {
  if (printDelete) {
    cout << "Deleting StatementNode:WriteStmtNode" << endl;
  }
  
}
void WriteNode::interpret() {
  switch(type) {
    case TOK_STRINGLIT:
    // If we are writing a string, write it. First, strip
    // leading and trailing quote characters.
    cout << (text).substr(1, (text).length()-2) << endl;
    break;
    case TOK_IDENT:
    // If we are writing a variable, look up and print value
    symbolTableT::iterator variable = symbolTable.find(text);
    cout << variable->second << endl;
    break;
  }
}
// ---------------------------------------------------------------------
ExprNode::ExprNode(int level, SimpleExpNode* sn) {
  _level = level;
  firstSimpleExp = sn;
}
ostream& operator<<(ostream& os, ExprNode& en) {
  os << endl; indent(en._level); os << "(expression ";
	os << *(en.firstSimpleExp);

	int length = en.restSimpleExpOps.size();
	for (int i = 0; i < length; ++i) {
		int op = en.restSimpleExpOps[i];
    if (op == TOK_EQUALTO) {
      os << endl; indent(en._level); os << "= ";
    } else if (op == TOK_GREATERTHAN) {
      os << endl; indent(en._level); os << "> ";
    } else if (op == TOK_LESSTHAN) {
      os << endl; indent(en._level); os << "< ";
    } else {
      os << endl; indent(en._level); os << "<> ";
    }
		os << *(en.restSimpleExps[i]);
	}
  os << endl; indent(en._level); os << "expression) ";
	return os;
}
ExprNode::~ExprNode() {
  if(printDelete)
    cout << "Deleting ExpressionNode" << endl;
	delete firstSimpleExp;
	firstSimpleExp = nullptr;

	int length = restSimpleExpOps.size();
	for (int i = 0; i < length; ++i) {
		delete restSimpleExps[i];
		restSimpleExps[i] = nullptr;
	}
}
float ExprNode::interpret()
{
  // an Expr is a SimpleExp followed by 0 or more SimpleExps separated by =, <, >, or <> operations

  // get the value of the first Simple
  float returnValue = firstSimpleExp->interpret();

  int length = restSimpleExpOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Term
    float nextValue = restSimpleExps[i]->interpret();

    // perform the operation (+ or -) that separates the Terms
    switch(restSimpleExpOps[i]) {
      break;
      case TOK_EQUALTO:
      // Equality means the values are within EPSILON of each other.
      if(abs(returnValue - nextValue) <= EPSILON)
        return 1.0; // true
      else
        return 0.0; // false
      break;
      case TOK_GREATERTHAN:
      // Greater than means returnValue is greater than nextValue by at least EPSILON 
      if(abs(returnValue - nextValue) > EPSILON)
        return 1.0; // true
      else
        return 0.0; // false
      break;
      case TOK_LESSTHAN:
      // Less than means returnValue is less than nextValue by at least EPSILON
      if(abs(returnValue - nextValue) < EPSILON)
        return 1.0; // true
      else
        return 0.0; // false
      break;
      case TOK_NOTEQUALTO:
      // Non-equality means the values differ by at least EPSILON.
      if(abs(returnValue - nextValue) > EPSILON)
        return 1.0; // true
      else
        return 0.0; // false
      break;
    }
  }
  return returnValue;
}
// ---------------------------------------------------------------------
SimpleExpNode::SimpleExpNode(int level, TermNode* tn) {
  _level = level;
  firstTerm = tn;
}
ostream& operator<<(ostream& os, SimpleExpNode& sn) {
  os << endl; indent(sn._level); os << "(simple_exp ";
	os << *(sn.firstTerm);

	int length = sn.restTermOps.size();
	for (int i = 0; i < length; ++i) {
		int op = sn.restTermOps[i];
    if (op == TOK_ADD_OP) {
      os << endl; indent(sn._level); os << "+ ";
    } else if (op == TOK_SUB_OP) {
      os << endl; indent(sn._level); os << "- ";
    } else {
      os << endl; indent(sn._level); os << "OR ";
    }
		os << *(sn.restTerms[i]);
	}
  os << endl; indent(sn._level); os << "simple_exp) ";
	return os;
}
SimpleExpNode::~SimpleExpNode() {
  if(printDelete)
    cout << "Deleting SimpleExpNode" << endl;
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
}
float SimpleExpNode::interpret()
{
  // an SimpleExp is a Term followed by 0 or more Terms separated by + or - operations

  // get the value of the first Term
  float returnValue = firstTerm->interpret();

  int length = restTermOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Term
    float nextValue = restTerms[i]->interpret();

    // perform the operation (+ or -) that separates the Terms
    switch(restTermOps[i]) {
      case TOK_ADD_OP:
        returnValue = returnValue + nextValue; 
      break;
      case TOK_SUB_OP:
        returnValue = returnValue - nextValue; 
      break;
    }
  }
  return returnValue;
}
// ---------------------------------------------------------------------
TermNode::TermNode(int level, FactorNode* fn) {
  _level = level;
  firstFactor = fn;
}
ostream& operator<<(ostream& os, TermNode& tn) {
  os << endl; indent(tn._level); os << "(term ";
	os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = tn.restFactorOps[i];
    if (op == TOK_MULT_OP) {
      os << endl; indent(tn._level); os << "* ";
    } else if (op == TOK_DIV_OP) {
      os << endl; indent(tn._level); os << "/ ";
    } else {
      os << endl; indent(tn._level); os << "AND ";
    }
		os << *(tn.restFactors[i]);
	}
  os << endl; indent(tn._level); os << "term) ";
	return os;
}
TermNode::~TermNode() {
  if(printDelete)
    cout << "Deleting TermNode" << endl;
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}
float TermNode::interpret()
{
  // a Term is a Factor followed by 0 or more Factors separated by * or / operations
  // get the value of the first Factor
  float returnValue = firstFactor->interpret();

  int length = restFactorOps.size();
  for (int i = 0; i < length; ++i) {
    // get the value of the next Factor
    float nextValue = restFactors[i]->interpret();
    
    // perform the operation (* or /) that separates the Factors
    switch(restFactorOps[i]) {
      case TOK_MULT_OP:
        returnValue = returnValue * nextValue;
      break;
      case TOK_DIV_OP:
        returnValue = returnValue / nextValue;
      break;
    }
  }
  return returnValue;
}
// ---------------------------------------------------------------------
FactorNode::~FactorNode() {}
// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn) {
  os << endl; indent(fn._level); os << "(factor (";
  fn.printTo(os);
  os << endl; indent(fn._level); os << "factor) ";
	return os;
}
// Use double dispatch to route to proper interpret method
float FactorNode::interpret() 
{
  return this->interpret();
}
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int level, float value) {
  _level = level;
  // Although value is a float, and int_literal is also a float, 
  // the lexer will ensure that only integers are stored here.
  int_literal = value;
}
void IntLitNode::printTo(ostream& os) {
	os << " INTLIT: " << int_literal << " ) ";
}
IntLitNode::~IntLitNode() {
  if(printDelete)
    cout << "Deleting FactorNode:IntLitNode" << endl;
	  // Nothing to do since the only members are not pointers
}
float IntLitNode::interpret()
{
  return float(int_literal);   
}
// ---------------------------------------------------------------------
FloatLitNode::FloatLitNode(int level, float value) {
  _level = level;
  float_literal = value;
}
void FloatLitNode::printTo(ostream& os) {
	os << " FLOATLIT: " << float_literal << " ) ";
}
FloatLitNode::~FloatLitNode() {
  if(printDelete)
    cout << "Deleting FactorNode:FloatLitNode" << endl;
}
float FloatLitNode::interpret() {
  return float_literal;
}
// ---------------------------------------------------------------------
IdNode::IdNode(int level, string name) {
  _level = level;
  id = name;
}
void IdNode::printTo(ostream& os) {
	os << " IDENT: " << id << " ) ";
}
IdNode::~IdNode() {
  if(printDelete) 
    cout << "Deleting FactorNode:IdNode" << endl;

}
float IdNode::interpret(){
  //return 2;    // Use 1 for any identifier (in this example only)
               // In your interpreter, look up the current value in 
               //   the symbol table
  symbolTableT::iterator variable = symbolTable.find(id);
  return variable->second;
} 
// ---------------------------------------------------------------------
NestedExprNode::NestedExprNode(int level, ExprNode* en) {
  _level = level;
	exprPtr = en;
}
void NestedExprNode::printTo(ostream& os) {
	os << " " << *exprPtr << ")" ;
}
NestedExprNode::~NestedExprNode() {
  if(printDelete)
    cout << "Deleting FactorNode:NestedExprNode" << endl;
  delete exprPtr;
  exprPtr = nullptr;
}
float NestedExprNode::interpret()
{
  return exprPtr->interpret();
}
// ---------------------------------------------------------------------
NotNode::NotNode(int level, FactorNode* fn) {
  _level = level;
  factorPtr = fn;
}
void NotNode::printTo(ostream& os) {
  os << "NOT " << *factorPtr << ") ";
}
NotNode::~NotNode() {
  if (printDelete)
    cout << "Deleting FactorNode:NotNode" << endl;
  delete factorPtr;
  factorPtr = nullptr;
}
float NotNode::interpret() {
  return factorPtr->interpret();
}
// ---------------------------------------------------------------------
MinusNode::MinusNode(int level, FactorNode* fn) {
  _level = level;
  factorPtr = fn;
}
void MinusNode::printTo(ostream& os) {
  os << "- " << *factorPtr << ") ";
}
MinusNode::~MinusNode() {
  if (printDelete)
    cout << "Deleting FactorNode:MinusNode" << endl;
  delete factorPtr;
  factorPtr = nullptr;
}
float MinusNode::interpret() {
  return factorPtr->interpret();
}