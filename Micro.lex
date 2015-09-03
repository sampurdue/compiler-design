DIGIT [0-9]
ALPHA [a-zA-Z]

FLOATLITERAL {DIGIT}+"."{DIGIT}*
INTLITERAL {DIGIT}+

KEYWORD PROGRAM|BEGIN|END|FUNCTION|READ|WRITE|IF|ELSE|FI|FOR|ROF|CONTINUE|BREAK|RETURN|INT|VOID|STRING|FLOAT
COMMENT --[^\n]*\n 
IDENTIFIER {ALPHA}+({ALPHA}{DIGIT})*

OPERATOR  ":="|"+"|"-"|"*"|"/"|"="|"!="|"<"|">"|"("|")"|";"|","|"<="|">="
STRINGLITERAL 	\"(\\.|[^"])*\"

EATUP [\n \t]


%%
{INTLITERAL}								{printf("Token Type: INTLITERAL\n");
										printf("Value: %d\n",atoi(yytext));	}	

{FLOATLITERAL}								{printf("Token Type: FLOATLITERAL\n");
										printf("Value: %f\n",atof(yytext));	}	

{KEYWORD}								{printf("Token Type: KEYWORD\n");
										printf("Value: %s\n",(yytext));	}

{STRINGLITERAL}							{printf("Token Type: STRINGLITERAL\n");
										printf("Value: %s\n",(yytext));	}								

{COMMENT}								{printf("");}									
										
{OPERATOR}								{printf("Token Type: OPERATOR\n");
										printf("Value: %s\n",(yytext));	}	
										
{IDENTIFIER}							{printf("Token Type: IDENTIFIER\n");
										printf("Value: %s\n",(yytext));	}

									
{EATUP}								{printf("");}


								
%%

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("This needs a file. yyin will be stidin for now \n");
	}	
	else
	{
		FILE *sourceFile = fopen(argv[1], "r");
		if(sourceFile == NULL)
			printf("File not found");
		else
			yyin = sourceFile;
	}
	yylex();
}
