%{
#include <string.h>
#include "parser.tab.h"
//#define SAVE_TOKEN strcpy(yytext,yylval.string)
//#define TOKEN(t) (yylval.token = t)


int linenum =0;
//static void comment();
%}

DIGIT [0-9]
ALPHA [a-zA-Z]

FLOATLITERAL {DIGIT}*"."{DIGIT}+
INTLITERAL {DIGIT}+



KEYWORD PROGRAM|BEGIN|END|FUNCTION|READ|WRITE|IF|ELSE|FI|FOR|ROF|CONTINUE|BREAK|RETURN|INT|VOID|STRING|FLOAT
COMMENT --[^\n]*\n 
IDENTIFIER {ALPHA}+({ALPHA}|{DIGIT})*

OPERATOR  ":="|"+"|"-"|"*"|"/"|"="|"!="|"<"|">"|"("|")"|";"|","|"<="|">="
STRINGLITERAL 	\"(\\.|[^"])*\"

EATUP [\n \t\r]


%%
"PROGRAM"									{return (PROGRAM);}
"BEGIN"										{return (_BEGIN);}
"END"										{return (END);}
"FUNCTION"									{return (FUNCTION);}
"READ"										{return (READ);} 
"WRITE"										{return (WRITE);} 
"IF" 										{return (IF);}
"ELSE" 										{return (ELSE);}
"FI" 										{return (FI);}
"FOR" 										{return (FOR);}
"ROF" 										{return (ROF);}
"CONTINUE" 									{return (CONTINUE);}
"BREAK" 									{return (BREAK);}
"RETURN" 									{return (RETURN);}
"INT" 										{return (INT);}
"VOID" 										{return (VOID);}
"STRING" 									{return (STRING);}
"FLOAT"										{return (FLOAT);}

":=" 										{return ASSIGN_OP;}
"+" 										{return '+';}
"-" 										{return '-';}
"*" 										{return '*';}
"/" 										{return '/';}
"=" 										{return EQ_OP;}
"!=" 										{return NEQ_OP;}
"<" 										{return '<';}
">" 										{return '>';}
"(" 										{return '(';}
")" 										{return ')';}
";" 										{return ';';}
"," 										{return ',';}
"<=" 										{return LTE_OP;}
">="										{return GTE_OP;}

"\n"										{linenum++;}

{INTLITERAL}								{yylval.iVal = atoi(yytext);return INTLITERAL;}								
												
												

{FLOATLITERAL}								{yylval.fval = atof(yytext); return FLOATLITERAL;	}	


{STRINGLITERAL}							{return STRINGLITERAL;}								

{COMMENT}								{;}									
										
										
{IDENTIFIER}							{yylval.str = strdup(yytext);return IDENTIFIER;;	}

									
{EATUP}								{;}

.								{;}
								
%%


