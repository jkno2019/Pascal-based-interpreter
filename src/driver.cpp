//*****************************************************************************
// purpose: driver file for interpreter example
// Joseph Knowles
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <iomanip>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"

#include <cstring>

using namespace std;

extern "C"
{
  // Instantiate global variables
  extern FILE *yyin;   // input stream
  extern FILE *yyout;  // output stream
  extern int   yyleng;   // length of current lexeme  
  extern char *yytext; // text of current lexeme
  extern int   yylineno;    // line number for current lexeme  [\n { yylineno++; }] in case you get the same error
  extern int   yylex();  // the generated lexical analyzer
  extern int   yylex_destroy(); // deletes memory allocated by yylex
}

int main( int argc, char* argv[] )
{
  // Whether to print these items
  bool printTree = false; 
  bool printSymbolTable = false;

  // Process any command-line switches
  for(int i = 1; i < argc; i++) {
    // -p flag: if requested, print while parsing
    if(strcmp(argv[i], "-p") == 0) {
      printParse = false;
    }
    // -t flag: if requested, print parse tree
    if(strcmp(argv[i], "-t") == 0) {
      printTree = false;
    }
    // -s flag: if requested, print symbol table
    if(strcmp(argv[i], "-s") == 0) {
      printSymbolTable = false;
    }
    // -d flag: if requested, print while deleting parse tree
    if(strcmp(argv[i], "-d") == 0) {
      printDelete = false;
    }
  }

  // Open the input data file and process its contents
  if (argc > 1) {
    printf("INFO: Using the %s file for input\n", argv[argc-1]);
    yyin = fopen(argv[argc-1], "r"); 
  } 
  // if ((yyin = fopen("front.in", "r")) == NULL) {
  //   cout << "ERROR - cannot open front.in" << endl;
  //   return(EXIT_FAILURE);
  // }

  // If uncommented, this line allows typing in an expression interactively
  // If so, to end must type "Ctrl-D" (generate EOF character)
  // yyin = stdin;

  // Create the root of the parse tree
  ProgramNode* root = nullptr;

  try {
        // Start symbol is <expr>
        // root = program(); // change later

        root = program();
        
        // if (nextToken != TOK_EOF)
        //     throw "end of file expected, but there is more here!";
            
    } catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near |" << yytext << "|, error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }

  // lex();  // prime the pump (get first token)
  // do {
  //   root = expr(); // start symbol is <expr>
  // } while(nextToken != TOK_EOF);

  //cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;
  
  // if (yyin)
  //   fclose(yyin);




  yylex_destroy();

  // Printing, Interpreting, and Deleting the tree all result in 
  // the same in-order traversal of the tree as parsing.  All
  // use the call stack.

  // if(printTree) {
  //   cout << endl << "*** Print the Tree ***" << endl;
  //   cout << *root << endl << endl;
  // }

  cout << "*** Interpret the Tree ***" << endl;
  root->interpret();

  if(printSymbolTable)
  {
    cout << "*** Print the Symbol Table ***" << endl;
    symbolTableT::iterator it;
    for(it = symbolTable.begin(); it != symbolTable.end(); ++it )
      cout << setw(8) << it->first << ": " << it->second << endl;
  }
  
  if(printDelete)
    cout << "*** Delete the Tree ***" << endl;
  delete root;
  root = nullptr;
    
  return(EXIT_SUCCESS);
}
