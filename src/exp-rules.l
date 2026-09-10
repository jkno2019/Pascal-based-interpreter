/******************************************************************* 
purpose: example rules file for flex,
recognizes lexemes for mathematical expressions 
 *******************************************************************/
%option noyywrap
%{
#include "lexer.h"
%}

%%

 /* Found BEGIN Keyword */
"BEGIN"     { return TOK_BEGIN; }

 /* Found BREAK Keyword */
"BREAK"     { return TOK_BREAK; }

 /* Found CONTINUE Keyword */
"CONTINUE"  { return TOK_CONTINUE; }

 /* Found DOWNTO Keyword */
"DOWNTO"    { return TOK_DOWNTO; }

 /* Found ELSE Keyword */
"ELSE"      { return TOK_ELSE; }

 /* Found END Keyword */
"END"       { return TOK_END; }

 /* Found FOR Keyword */
"FOR"       { return TOK_FOR; }

 /* Found IF Keyword */
"IF"        { return TOK_IF; }

 /* Found LET Keyword */
"LET"       { return TOK_LET; }

 /* Found PROGRAM Keyword */
"PROGRAM"   { return TOK_PROGRAM; }

 /* Found READ Keyword */
"READ"      { return TOK_READ; }

 /* Found THEN Keyword */
"THEN"      { return TOK_THEN; }

 /* Found TO Keyword */
"TO"        { return TOK_TO; }

 /* Found VAR Keyword */
"VAR"       { return TOK_VAR; }

 /* Found WHILE Keyword */
"WHILE"     { return TOK_WHILE; }

 /* Found WRITE Keyword */
"WRITE"     { return TOK_WRITE; }

 /* Found INTEGER Datatype */
"INTEGER"   { return TOK_INTEGER; }

 /* Found REAL Datatype */
"REAL"   { return TOK_REAL; }

 /* Found ';' */
";"           { return TOK_SEMICOLON; }
 
 /* Found ':' */
":"           { return TOK_COLON; }
 
 /* Found '(' */
"("         { return TOK_LEFT_PAREN; }
 
 /* Found ')' */
")"         { return TOK_RIGHT_PAREN; }
 
 /* Found '+' */
"+"         { return TOK_ADD_OP; }
 
 /* Found '-' */
"-"         { return TOK_SUB_OP; }
 
 /* Found '*' */
"*"         { return TOK_MULT_OP; }
 
 /* Found '/' */
"/"         { return TOK_DIV_OP; }
 
 /* Found ':=' */
":="        { return TOK_ASSIGN; }
 
 /* Found '=' */
"="         { return TOK_EQUALTO; }
 
 /* Found '<' */
"<"         { return TOK_LESSTHAN; }
 
 /* Found '>' */
">"         { return TOK_GREATERTHAN; }
 
 /* Found <> */
"<>"        { return TOK_NOTEQUALTO; }
 
 /* Found MOD */
"MOD"       { return TOK_MOD; }

 /* Found NOT */
"NOT"       { return TOK_NOT; }
 
 /* Found OR */
"OR"        { return TOK_OR; }
 
 /* Found AND */
"AND"       { return TOK_AND; }
 
 /* identifier */
[A-Z]{1}[A-Z0-9]{0,7}   { return TOK_IDENT; }

[A-Z]{1}[A-Z0-9]{8,}   { return TOK_UNKNOWN; }
 
 /* integer literal */
[0-9]+          { return TOK_INT_LIT; }
 
 /* floating-point literal */
[0-9]+"."[0-9]+    { return TOK_FLOATLIT; }
 
 /* string literal FIX */
\'[^'\n]{0,80}\'    { return TOK_STRINGLIT; }

\'[^'\n]{81,}\'    { return TOK_UNKNOWN; }

 /* Begins the comment state FIX */
\{[^\n]*\}

 /* Eat any whitespace */

[ \t\r\n]+             /* nop */

 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }
