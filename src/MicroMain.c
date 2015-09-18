#include <stdio.h>
extern int yyparse();
extern FILE *yyin;
extern int yydebug;
extern int accepted ;

int main(int argc, char *argv[])
{
	accepted = 0;
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
//			printf("Testing file %s\n",argv[1]);
        }
	yydebug = 0;
        do {
		yyparse();
	} while (!feof(yyin));
	
	if(accepted == 0)
	{	
		printf("Accepted\n");
	}else
	{
		printf("Not accepted\n");
	}

	return 0;

}

