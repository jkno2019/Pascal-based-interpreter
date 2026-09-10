//*****************************************************************************
// purpose: Recursive descent parsing of arithmetic expressions
//          Builds the parse tree while parsing the input
//*****************************************************************************

#include "parser.h"
#include "parse_tree_nodes.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

// Forward declarations of first_of functions
bool first_of_program();
bool first_of_block();
bool first_of_statement();
bool first_of_expr();
bool first_of_simpExp();
bool first_of_term();
bool first_of_factor();

int nextToken = 0;            // hold nextToken returned by lex
bool printParse = false;      // shall we print the parse tree?

// Which tree level are we currently in?  Setting this to -1
// means the top-level expression is at level 0.
static int level = -1; // change to 0 eventually

// Handle syntax errors
void error() {
  cout << endl << "===========================" << endl;
  cout << "ERROR near: " << yytext;
  cout << endl << "===========================" << endl;
  if (yyin)
    fclose(yyin);
  exit(EXIT_FAILURE);
}
//*****************************************************************************
// Symbol table
symbolTableT symbolTable;
// Determine if a symbol is in the symbol table
bool inSymbolTable(string idName) {
  symbolTableT::iterator it;
  it = symbolTable.find( idName );
  // If idName is missing, will be set to the end
  return !(it == symbolTable.end());
}

//*****************************************************************************
// Print each level with appropriate indentation
string indent(void) {
  // for (int i = 0; i<level; i++)
  //   cout << ("  ");
  string str("");
  for(int i = 0; i < level; i++)
    str += "|  ";
  return str;
}
//*****************************************************************************
// Announce what the lexical analyzer has found
void output(string what) {
  // indent();
  // cout << "---> FOUND " << yytext << endl;
  cout << indent() << "found |" << yytext << "| " << what << endl;
}
//*****************************************************************************
int lex() {
  nextToken = yylex();

  if (nextToken == TOK_EOF) {
    // save a "lexeme" into yytext
    yytext[0] = 'E';
    yytext[1] = 'O';
    yytext[2] = 'F';
    yytext[3] = 0;
  }
  // if(printParse) {
  //   // Tell us about the token and lexeme
  //   indent();
  //   cout << "Next token is: ";
  //   switch(nextToken) {
  //   case TOK_INT_LIT:      cout << "TOK_INT_LIT";     break;
  //   case TOK_IDENT:        cout << "TOK_IDENT";       break;
  //   case TOK_ADD_OP:       cout << "TOK_ADD_OP";      break;
  //   case TOK_SUB_OP:       cout << "TOK_SUB_OP";      break;
  //   case TOK_MULT_OP:      cout << "TOK_MULT_OP";     break;
  //   case TOK_DIV_OP:       cout << "TOK_DIV_OP";      break;
  //   case TOK_LEFT_PAREN:   cout << "TOK_LEFT_PAREN";  break;
  //   case TOK_RIGHT_PAREN:  cout << "TOK_RIGHT_PAREN"; break;
  //   case TOK_EOF:          cout << "TOK_EOF";         break;
  //   case TOK_UNKNOWN:      cout << "TOK_UNKNOWN";     break;
  //   default: error();
  //   }
  //   cout << ", Next lexeme is: " << yytext << endl;
  // }
  return nextToken;
}
//*****************************************************************************
ProgramNode* program() {
  lex();
  if(!first_of_program)
    throw "3: 'PROGRAM' expected";

  if(printParse) output("PROGRAM");
  if (printParse) { cout << indent() << "enter <program>" << endl; }
  ++level;
  
  nextToken = lex();
  if(nextToken != TOK_IDENT)
    throw "2: identifier expected";
  if (printParse) output("IDENTIFIER");
  
  nextToken = lex();
  if(nextToken != TOK_SEMICOLON)
    throw "14: ';' expected";
  if (printParse) output("SEMICOLON");

  nextToken = lex();
  ProgramNode* newProgramNode = new ProgramNode(level, block());


   --level;
  if(printParse) { cout << indent() << "exit <program>" << endl; }

  nextToken = lex(); // should find EOF token

  return newProgramNode;
}
bool first_of_program(void) {
    return nextToken == TOK_PROGRAM;
}
//*****************************************************************************
BlockNode* block() {
  if (!first_of_block)
    throw "18: error in declaration part OR 17: 'BEGIN' expected";

  if (printParse) output("BLOCK");
  if (printParse) { cout << indent() << "enter <block>" << endl; }
  ++level;

  if (nextToken == TOK_VAR) {
    nextToken = lex();

    if (nextToken != TOK_IDENT)
      throw "2: identifier expected";

    while (nextToken == TOK_IDENT) {
      if (printParse) output("IDENTIFIER");
      string idName = yytext;
      symbolTable.insert( pair<string, float>(string(yytext), 2.0) );

      nextToken = lex();
      if (nextToken != TOK_COLON)
        throw "5: ':' expected";
      if (printParse) output("COLON");

      nextToken = lex();
      if (nextToken != TOK_INTEGER && nextToken != TOK_REAL)
        throw "10: error in type";
      if (printParse) output("TYPE");
      string idType = yytext;

      nextToken = lex();
      if (nextToken != TOK_SEMICOLON)
        throw "14: ';' expected";
      if (printParse) output("SEMICOLON");

      if (printParse) { cout << indent() << "-- idName: |" << idName << "| idType: |" << idType << "| --\n"; }

      nextToken = lex();

      if (inSymbolTable(yytext)) {
        if (printParse) output("IDENTIFIER");
        string idName = yytext;
        symbolTable.insert( pair<string, float>(string(yytext), 2.0) );

        nextToken = lex();
        if (nextToken != TOK_COLON)
          throw "5: ':' expected";
        if (printParse) output("COLON");

        nextToken = lex();
        if (nextToken != TOK_INT_LIT && nextToken != TOK_REAL)
          throw "10: error in type";
        if (printParse) output("TYPE");
        string idType = yytext;

        nextToken = lex();
        if (nextToken != TOK_SEMICOLON)
          throw "14: ';' expected";
        if (printParse) output("SEMICOLON");

        if (printParse) { cout << indent() << "-- idName: |" << idName << "| idType: |" << idType << "| --\n"; }
      
        throw "101: identifier declared twice";
      }
    }
  }

  if (nextToken != TOK_BEGIN)
    throw "17: 'BEGIN' expected";
  
  // if (printParse) output("BEGIN");
  // if (printParse) { cout << indent() << "enter <compound_stmt>" << endl; }
  //++level;

  //nextToken = lex();
  BlockNode* newBlockNode = new BlockNode(level, compound());
  //newBlockNode->statement = statement();
  // statement();

  // while (nextToken == TOK_SEMICOLON) {
  //   if (printParse) output("SEMICOLON");
  //   nextToken = lex(); //statement();
  //   // newBlockNode = new BlockNode(level/*, statement()*/);
  //   //newBlockNode->statement = statement();
  // }

  // if (nextToken != TOK_END) { 
  //   // newBlockNode = new BlockNode(level/*, statement()*/);
  //   //newBlockNode->statement = statement();
  // }

  //--level;
  // if (nextToken != TOK_END)
  //   throw "13: 'END' expected";
  // if (printParse) output("END");
  //if (printParse) { cout << indent() << "exit <compound_stmt>" << endl; }

  --level;
  nextToken = lex();
  if (printParse) { cout << indent() << "exit <block>" << endl; }

  return newBlockNode;
}
bool first_of_block(void) {
    return nextToken == TOK_VAR || nextToken == TOK_BEGIN;
}
//*****************************************************************************
AssignmentNode* assignment() {
  if (printParse) { cout << indent() << "enter <assignment>" << endl; }
  ++level;
  output("IDENTIFIER");
  cout << indent() << yytext << endl;
  AssignmentNode* newAssignmentNode = new AssignmentNode(level, string(yytext));

  nextToken = lex();
  if (nextToken != TOK_ASSIGN)
    throw "51: ':=' expected";
  if (printParse) output("ASSIGN");


  nextToken = lex();
  newAssignmentNode->exprPtr = expr();

  --level;
  if (printParse) { cout << indent() << "exit <assignment>" << endl; }
  return newAssignmentNode;
}
//*****************************************************************************
CompoundNode* compound() {
  if (printParse) output("BEGIN");
  if (printParse) { cout << indent() << "enter <compound_stmt>" << endl; }
  ++level;
  CompoundNode* newCompoundNode = new CompoundNode(level);

  nextToken = lex();
  newCompoundNode->statements.push_back(statement());

  while (nextToken == TOK_SEMICOLON) {
    if (printParse) output("SEMICOLON");
    nextToken = lex();
    newCompoundNode->statements.push_back(statement());
  }
  
  --level;
  if (nextToken != TOK_END)
    throw "13: 'END' expected";
  if (printParse) output("END"); //cout << indent() << yytext << endl;
  if (printParse) { cout << indent() << "exit <compound_stmt>" << endl; }
  //cout << indent() << yytext << endl;
  nextToken = lex(); // ???????????
  return newCompoundNode;  
}
//*****************************************************************************
IfNode* ifstmt() {
    if (printParse) { cout << indent() << "enter <if>" << endl; }
    ++level;
    
    nextToken = lex();
    IfNode* newIfNode = new IfNode(level, expr());

    if (nextToken != TOK_THEN)
      throw "52: 'THEN' expected";
    output("THEN");

    nextToken = lex();
    newIfNode->statePtr1 = statement();

    --level;

    //lex();
    if (nextToken == TOK_ELSE) {
      output("ELSE");
      cout << indent() << "enter <else>" << endl;
      ++level;
      nextToken = lex();
      newIfNode->statePtr2 = statement();
      --level;
    }
    
    cout << indent() << "exit <if>" << endl;
    //cout << indent() << yytext << endl;
    return newIfNode;
}
//*****************************************************************************
WhileNode* whilestmt () {
  if (printParse) { cout << indent() << "enter <while>" << endl; }
  ++level;

  nextToken = lex();
  WhileNode* newWhileNode = new WhileNode(level, expr());

  newWhileNode->statePtr = statement();

  --level;
  if (printParse) { cout << indent() << "exit <while>" << endl; }
  return newWhileNode;
}
//*****************************************************************************
ReadNode* read() {
  if (printParse) { cout << indent() << "enter <read>" << endl; }
  ++level;

  nextToken = lex();
  if (nextToken != TOK_LEFT_PAREN)
    throw "9: '(' expected";
  if (printParse) output("OPENPAREN");

  nextToken = lex();
  if (nextToken != TOK_IDENT)
    throw "104: identifier not declared";
  if (printParse) output("IDENTIFIER");
  cout << indent() << yytext << endl;
  ReadNode* newReadNode = new ReadNode(level, string(yytext));

  nextToken = lex();
  if (nextToken != TOK_RIGHT_PAREN)
    throw "4: ')' expected";
  if (printParse) output("CLOSEPAREN");

  --level;
  cout << indent() << "exit <read>" << endl;
  nextToken = lex();

  return newReadNode;
}
//*****************************************************************************
WriteNode* write() {
  if (printParse) { cout << indent() << "enter <write>" << endl; }
  ++level;

  nextToken = lex();
  if (nextToken != TOK_LEFT_PAREN)
    throw "9: '(' expected";
  if (printParse) output("OPENPAREN");

  nextToken = lex();
  if (nextToken != TOK_STRINGLIT && nextToken != TOK_IDENT)
    throw "134: illegal type of operand(s)";
  if (printParse) output("WRITE");
  WriteNode* newWriteNode = new WriteNode(level, string(yytext));
  newWriteNode->type = nextToken;
  if (printParse) { cout << indent() << yytext << endl; }

  nextToken = lex();
  if (nextToken != TOK_RIGHT_PAREN)
    throw "4: ')' expected";
  if (printParse) output("CLOSEPAREN");

  --level;
  if (printParse) { cout << indent() << "exit <write>" << endl; }
  nextToken = lex();
  //cout << indent() << yytext << endl;

  return newWriteNode;
}
//*****************************************************************************
StatementNode* statement() {
  if (!first_of_statement)
    throw "900: illegal type of statement";

    if (printParse) output("STATEMENT");
    StatementNode* newStatementNode = nullptr;

    if (nextToken == TOK_IDENT) { newStatementNode = assignment(); }
      
    else if (nextToken == TOK_BEGIN) { newStatementNode = compound(); }

    else if (nextToken == TOK_IF) { newStatementNode = ifstmt(); }

    else if (nextToken == TOK_WHILE) { newStatementNode = whilestmt(); }

    else if (nextToken == TOK_READ) { newStatementNode = read(); }

    else if (nextToken == TOK_WRITE) { newStatementNode = write(); }

    else 
      throw "999: an error has occurred";

    return newStatementNode;
  }
    
bool first_of_statement(void) {
  return nextToken == TOK_IDENT || nextToken == TOK_BEGIN || nextToken == TOK_IF || nextToken == TOK_WHILE || nextToken == TOK_READ || nextToken == TOK_WRITE;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <expr> -> <term> {{ (( + || - )) <term> }}
ExprNode* expr() {
  // Check that the <expr> starts with a valid token 
  if(!first_of_expr())
    error();

  if(printParse) output("EXPRESSION");
  if(printParse) {
    cout << indent() << "enter <expression>" << endl;
  }
  ++level;
  //level = level + 1;
  ExprNode* newExprNode = new ExprNode(level, simpExp());

  /* Parse the first term */
  //newExprNode->firstSimpleExp = simpExp();

  // As long as the next token is + or -, get the next token and parse the next term
  while(nextToken == TOK_EQUALTO || nextToken == TOK_GREATERTHAN || nextToken == TOK_LESSTHAN || nextToken == TOK_NOTEQUALTO) {
    switch (nextToken)
    {
    case TOK_EQUALTO:
      if(printParse) output("EQUALTO");
      cout << indent() << yytext << endl;
      newExprNode->restSimpleExpOps.push_back(nextToken);
      lex();
      newExprNode->restSimpleExps.push_back(simpExp());
      break;

    case TOK_GREATERTHAN:
      if(printParse) output("GREATERTHAN");
      cout << indent() << yytext << endl;
      newExprNode->restSimpleExpOps.push_back(nextToken);
      lex();
      newExprNode->restSimpleExps.push_back(simpExp());
      break;

    case TOK_LESSTHAN:
      if(printParse) output("LESSTHAN");
      cout << indent() << yytext << endl;
      newExprNode->restSimpleExpOps.push_back(nextToken);
      lex();
      newExprNode->restSimpleExps.push_back(simpExp());
      break;

    case TOK_NOTEQUALTO:
      if(printParse) output("NOTEQUALTO");
      cout << indent() << yytext << endl;
      newExprNode->restSimpleExpOps.push_back(nextToken);
      lex();
      newExprNode->restSimpleExps.push_back(simpExp());
      break;
    
    default:
      // If we made it to here, syntax error
      throw "999: an error has occurred";
    }    
  }
  --level;
  //level = level - 1;
  if(printParse) {
    cout << indent() << "exit <expression>" << endl;;
  }
  return newExprNode;
}
bool first_of_expr(){
  return nextToken == TOK_INT_LIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_LEFT_PAREN || nextToken == TOK_NOT || nextToken == TOK_SUB_OP;
}
//*****************************************************************************
SimpleExpNode* simpExp() {
  if(!first_of_simpExp())
    throw "901: illegal type of simple expression";
  
  if (printParse) output("SIMPLE_EXP");
  if(printParse) { cout << indent() << "enter <simple_exp>" << endl; }
  ++level;
  SimpleExpNode* newSimpleExpNode = new SimpleExpNode(level, term());

  //newSimpleExpNode->firstTerm = term();

  while(nextToken == TOK_ADD_OP || nextToken == TOK_SUB_OP || nextToken == TOK_OR) {
    switch (nextToken)
    {
    case TOK_ADD_OP:
      if(printParse) output("PLUS");
      cout << indent() << yytext << endl;
      newSimpleExpNode->restTermOps.push_back(nextToken);
      lex();
      newSimpleExpNode->restTerms.push_back(term());
      break;

    case TOK_SUB_OP:
      if(printParse) output("MINUS");
      cout << indent() << yytext << endl;
      newSimpleExpNode->restTermOps.push_back(nextToken);
      lex();
      newSimpleExpNode->restTerms.push_back(term());
      break;

    case TOK_OR:
      if(printParse) output("OR");
      cout << indent() << yytext << endl;
      newSimpleExpNode->restTermOps.push_back(nextToken);
      lex();
      newSimpleExpNode->restTerms.push_back(term());
      break;
    
    default:
      // If we made it to here, syntax error
      throw "999: an error has occurred";
    }
  }

  --level;
  if(printParse) { cout << indent() << "exit <simple_exp>" << endl; }
  return newSimpleExpNode;
}
bool first_of_simpExp(){
  return nextToken == TOK_INT_LIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_LEFT_PAREN || nextToken == TOK_NOT || nextToken == TOK_SUB_OP;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <term> -> <factor> {{ (( * || / )) <factor> })
TermNode* term() {
  /* Check that the <term> starts with a valid token */
  if(!first_of_term())
    throw "902: illegal type of term";

  if(printParse) output("TERM");
  if(printParse) {
    cout << indent() << "enter <term>" << endl;
  }
  ++level;
  //level = level + 1;
  TermNode* newTermNode = new TermNode(level, factor());

  /* Parse the first factor */
  //newTermNode->firstFactor = factor();

  /* As long as the next token is * or / or AND, get the
     next token and parse the next factor */
  while(nextToken == TOK_MULT_OP || nextToken == TOK_DIV_OP || nextToken == TOK_AND) {
    switch (nextToken)
    {
    case TOK_MULT_OP:
      if (printParse) output("MULTIPLY");
      cout << indent() << yytext << endl;
      newTermNode->restFactorOps.push_back(nextToken);
      lex();
      newTermNode->restFactors.push_back(factor());
      break;

    case TOK_DIV_OP:
      if (printParse) output("DIVIDE");
      cout << indent() << yytext << endl;
      newTermNode->restFactorOps.push_back(nextToken);
      lex();
      newTermNode->restFactors.push_back(factor());
      break;

    case TOK_AND:
      if (printParse) output("AND");
      cout << indent() << yytext << endl;
      newTermNode->restFactorOps.push_back(nextToken);
      lex();
      newTermNode->restFactors.push_back(factor());
      break;
    
    default:
      // If we made it to here, syntax error
      throw "999: an error has occurred";
    }
    
    // if(printParse) //output();
    // newTermNode->restFactorOps.push_back(nextToken);
    // lex();
    // newTermNode->restFactors.push_back(factor());
  }

  --level;
  //level = level - 1;
  if(printParse) {
    cout << indent() << "exit <term>" << endl;
  }
  return newTermNode;
}
bool first_of_term() {
  return nextToken == TOK_INT_LIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_LEFT_PAREN || nextToken == TOK_NOT || nextToken == TOK_SUB_OP;
}
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <factor> -> id || int_constant || ( <expr> )
FactorNode* factor() {
  // Check that the <factor> starts with a valid token
  if(!first_of_factor())
    throw "903: illegal type of factor";

  if(printParse) output("FACTOR");
  if(printParse) {
    cout << indent() << "enter <factor>" << endl;
  }
  //level = level + 1;
  ++level;
  FactorNode* newFactorNode = nullptr;

  // Determine which RHS token we have
  switch(nextToken) {

    case TOK_INT_LIT:
      if(printParse) output("INTLIT");
      cout << indent() << yytext << endl;
      newFactorNode = new IntLitNode(level, atoi(yytext));
      nextToken = lex();
      break;

    case TOK_FLOATLIT:
      if(printParse) output("FLOATLIT");
      cout << indent() << yytext << endl;
      newFactorNode = new FloatLitNode(level, atof(yytext));
      nextToken = lex();
      break;


    case TOK_IDENT:
      if(printParse) output("IDENTIFIER");
      cout << indent() << yytext << endl;
      if (!inSymbolTable(yytext))
        throw "104: identifier not declared";
      // check if it's in the symbol table
      symbolTable.insert( pair<string, float>(string(yytext), 2.0) );
      newFactorNode = new IdNode(level, string(yytext));
      nextToken = lex(); // Read past what we have found
      break;

    case TOK_LEFT_PAREN:
      // We expect ( <expr> ) parse it
      if(printParse) output("OPENPAREN");
      cout << indent() << yytext << endl;
      nextToken = lex();
      if (!first_of_expr()) // Check for 'IDENT' | 'INT_LIT' | (
        error();

      newFactorNode = new NestedExprNode(level, expr());

      if (nextToken == TOK_RIGHT_PAREN) {
        if(printParse) output("CLOSEPAREN");
        nextToken = lex();
      }
      else
        throw "<expr> does not end with )";
      break;

    case TOK_NOT:
      if (printParse) output("NOT");
      cout << indent() << yytext << endl;
      nextToken = lex();
      newFactorNode = new NotNode(level, factor());
      break;
      
    case TOK_SUB_OP:
        if(printParse) output("MINUS");
        cout << indent() << yytext << endl;
        nextToken = lex();
        newFactorNode = new MinusNode(level, factor());
        break;

    default:
      // If we made it to here, syntax error
      throw "999: an error has occurred";
	}
  --level;
  //level = level - 1;
  if(printParse) {
    cout << indent() << "exit <factor>" << endl;
  }
  return newFactorNode;
}
bool first_of_factor(){
  return nextToken == TOK_INT_LIT || nextToken == TOK_IDENT || nextToken == TOK_FLOATLIT || nextToken == TOK_LEFT_PAREN || nextToken == TOK_NOT || nextToken == TOK_SUB_OP;
}

