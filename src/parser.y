%{
#include <stdio.h>
#include "symbolTable.h"

#define YYDEBUG 1

extern int linenum;
extern int currVarType;
extern int ongoingDecl;
extern int yylex();
extern char* yytext;

/* local variable defn*/
int accepted;
std::stack<char*> nameStack ;

/* local function defn*/
void createGlobalTable();
void createBlockTable(char* blockName);
void finishScope();

void yyerror(const char *s) { accepted = 10;/*printf("ERROR: %s at linenum %d\n", s,linenum+1); */}
%}

%union
{
	float fval;
	int iVal;
	char * str;
}

/* token definition */

%token <iVal>INT VOID
%token <iVal>FLOAT
%token CASE  IF ELSE FOR ROF CONTINUE BREAK RETURN FI
%token PROGRAM WRITE STRING READ _BEGIN END FUNCTION
%token <str> IDENTIFIER STRINGLITERAL
%token <iVal> INTLITERAL 
%token <fval> FLOATLITERAL
%token <iVal> ASSIGN_OP ADD_OP MINUS_OP MUL_OP DIV_OP EQ_OP NEQ_OP LE_OP GT_OP LBT_OP RBR_OP SC_OP LTE_OP GTE_OP

%type <iVal> var_type
%type <str> id 







%%
/* Program */
program           : PROGRAM {ongoingDecl = 0; printf("Symbol table GLOBAL\n");} id _BEGIN {createGlobalTable();} pgm_body END 
id                : IDENTIFIER										{$$ = $1; }
pgm_body          : decl func_declarations
decl		        : string_decl decl | var_decl decl | /*empty*/

/* Global String Declaration */
string_decl       : STRING id ASSIGN_OP str ';'
str               : STRINGLITERAL

/* Variable Declaration */
var_decl          : var_type id_list ';' {/*printf("Yohoo got a variable decl at linenum %d\n",linenum+1);*/while(!nameStack.empty()){printf("name  %s type %s\n",nameStack.top(), ((currVarType == INT)?"INT":"FLOAT"));nameStack.pop();}currVarType=-1;}
var_type	      : FLOAT{$$ = FLOAT; currVarType = FLOAT;ongoingDecl = 10;} | INT {$$ = INT;currVarType = INT;ongoingDecl = 10;}
any_type          : var_type | VOID 
id_list           : id id_tail {if(currVarType != -1){/*printf("Pushing single %s\n",$1);*/nameStack.push($1);}}
id_tail           : ',' id id_tail {if(currVarType != -1){/*printf("Pushing list %s\n",$2);*/nameStack.push($2);}} | /*empty*/ 

/* Function Paramater List */
param_decl_list   : param_decl param_decl_tail | /*empty*/
param_decl        : var_type id
param_decl_tail   : ',' param_decl param_decl_tail | /*empty*/

/* Function Declarations */
func_declarations : func_decl func_declarations | /*empty*/
func_decl         : FUNCTION any_type {currVarType=-1;} id {printf("Symbol table %s\n",$4);} '('param_decl_list')' _BEGIN {createBlockTable($4);} func_body END {finishScope();}
func_body         : decl stmt_list 

/* Statement List */
stmt_list         : stmt stmt_list | /*empty*/
stmt              : base_stmt | if_stmt | for_stmt
base_stmt         : assign_stmt | read_stmt | write_stmt | return_stmt

/* Basic Statements */
assign_stmt       : assign_expr ';'
assign_expr       : id ASSIGN_OP expr
read_stmt         : READ '(' id_list ')'';'
write_stmt        : WRITE '(' id_list ')'';'
return_stmt       : RETURN expr ';'

/* Expressions */
expr              : expr_prefix factor
expr_prefix       : expr_prefix factor addop | /*empty*/
factor            : factor_prefix postfix_expr
factor_prefix     : factor_prefix postfix_expr mulop | /*empty*/
postfix_expr      : primary | call_expr
call_expr         : id '(' expr_list ')'
expr_list         : expr expr_list_tail | /*empty*/
expr_list_tail    : ',' expr expr_list_tail | /*empty*/
primary           : '(' expr ')' | id | INTLITERAL | FLOATLITERAL
addop             : '+' | '-'
mulop             : '*' | '/'

/* Complex Statements and Condition */ 
if_stmt           : IF '(' cond ')' decl stmt_list else_part FI
else_part         : ELSE decl stmt_list | /*empty*/
cond              : expr compop expr
compop            : '<' | '>' | EQ_OP | NEQ_OP | LTE_OP | GTE_OP

init_stmt         : assign_expr | /*empty*/
incr_stmt         : assign_expr | /*empty*/

/* ECE 573 students use this version of for_stmt */
for_stmt       : FOR '(' init_stmt ';' cond ';' incr_stmt ')' decl aug_stmt_list ROF 

/* CONTINUE and BREAK statements. ECE 573 students only */
aug_stmt_list     : aug_stmt aug_stmt_list | /*empty*/
aug_stmt          : base_stmt | aug_if_stmt | for_stmt | CONTINUE';' | BREAK';'

/* Augmented IF statements for ECE 573 students */ 
aug_if_stmt       : IF '(' cond ')' decl aug_stmt_list aug_else_part FI
aug_else_part     : ELSE decl aug_stmt_list | /*empty*/


%%

void createGlobalTable()
{
	symTab* temp = createSymbolTable("GLOBAL");
	symTabList.push_back(temp);
	currSymTab = temp;
}

void createBlockTable(char* blockName)
{
	symTab* temp = createSymbolTable(blockName);
	symTabList.push_back(temp);
	symTabStack.push(currSymTab);
	currSymTab = temp;
}

void finishScope()
{
	currSymTab = symTabStack.top();
	symTabStack.pop(); 
}

void addElementsToTable()
{
	
}
