%{
#include "ASTNode.h"
#include "IRNode.h"
#include "TinyNode.h"

#define YYDEBUG 1

extern int linenum;
int currVarType;
int ongoingDecl;
extern int yylex();
extern char* yytext;


/* local variable defn*/
int accepted;
std::stack<char*> nameStack ;
ASTNode* progRoot;
int blockNum =0;
extern int tempNum;
extern list<IRNode*> irNodeList;
list<TinyNode*>* tinyNodeList;
extern symTab* currSymTab;;

/* local function defn*/


void yyerror(const char *s) { accepted = 10;printf("ERROR: %s at linenum %d\n", s,linenum+1); }
%}

%union
{
	float fval;
	int iVal;
	char* str;
	void* astNode;
}

/* token definition */

%token <iVal>INT VOID
%token <iVal>FLOAT
%token CASE  IF ELSE FOR ROF CONTINUE BREAK RETURN FI
%token PROGRAM WRITE STRING READ _BEGIN END FUNCTION
%token <str> IDENTIFIER STRINGLITERAL INTLITERAL FLOATLITERAL
%token <iVal> ASSIGN_OP ADD_OP MINUS_OP MUL_OP DIV_OP EQ_OP NEQ_OP LE_OP GT_OP LBT_OP RBR_OP SC_OP LTE_OP GTE_OP

%type <iVal> var_type
%type <str> id str
%type <astNode> program pgm_body base_stmt assign_stmt read_stmt write_stmt return_stmt stmt stmt_list if_stmt for_stmt assign_expr expr primary func_body func_decl func_declarations postfix_expr factor factor_prefix expr_prefix call_expr expr_list
%type <iVal> addop mulop


%left  ADD_OP MINUS_OP
%left  MUL_OP DIV_OP 
%left  EQ_OP NEQ_OP LE_OP GT_OP LTE_OP GTE_OP






%%
/* Program */
program           : PROGRAM {ongoingDecl = 0; } id _BEGIN {createGlobalTable();} pgm_body END 
					{ 
						progRoot = (ASTNode*)$6;  
						$$ = (void*)progRoot; 
						/*printEntireAST(progRoot); */
						tempNum = 0; 
						generateIRCode(progRoot);
						printIRList(&irNodeList);
						tinyNodeList = IRtoTinyCode(&irNodeList, currSymTab); 
						printTinyList(tinyNodeList);
					}
id                : IDENTIFIER										{$$ = $1;}
pgm_body          : decl func_declarations {$$ = $2;}
decl		        : string_decl decl | var_decl decl | /*empty*/

/* Global String Declaration */
string_decl       : STRING id ASSIGN_OP str {addStringElementToTable($2, $4);} ';' 
str               : STRINGLITERAL

/* Variable Declaration */
var_decl          : var_type id_list ';' {addElementsToTable();currVarType = -1;}
var_type	      : FLOAT{$$ = FLOAT; currVarType = FLOAT;ongoingDecl = 10;} | INT {$$ = INT;currVarType = INT;ongoingDecl = 10;}
any_type          : var_type | VOID 
id_list           : id id_tail {if(currVarType != -1){nameStack.push($1);}}
id_tail           : ',' id id_tail {if(currVarType != -1){nameStack.push($2);}} | /*empty*/ 

/* Function Paramater List */
param_decl_list   : param_decl param_decl_tail | /*empty*/
param_decl        : var_type id {nameStack.push($2); addElementsToTable();}
param_decl_tail   : ',' param_decl param_decl_tail | /*empty*/

/* Function Declarations */
func_declarations : func_decl func_declarations {$$ = $1}| /*empty*/ {$$ = (void*)NULL;}
func_decl         : FUNCTION any_type {currVarType=-1;} id '('{createBlockTable($4);}param_decl_list')' _BEGIN  func_body END 	{finishScope(); $$ = $10;}
func_body         : decl stmt_list {$$ = $2;}

/* Statement List */
stmt_list         : stmt stmt_list {$$ = (void*)new ASTNode(STMT_LIST,$1,$2);}| /*empty*/ {$$ = NULL;}
stmt              : base_stmt {$$ = $1;} | if_stmt {$$ = $1;} | for_stmt {$$ = $1;}
base_stmt         : assign_stmt {$$ = $1;} | read_stmt {$$ = $1;} | write_stmt {$$ = $1;} | return_stmt  {$$ = $1;}

/* Basic Statements */
assign_stmt       : assign_expr ';' {$$ = $1;}
assign_expr       : id ASSIGN_OP expr 
					{	Symbol* temp = lookUpSymbolInScope($1);
						SymNode* newSymNode = new SymNode(SYMBOL_NODE, temp);
						$$ = (void*) new AssignNode(ASSIGN_NODE, newSymNode, (ASTNode*)$3);
					}
read_stmt         : READ {currVarType = READ;}'(' id_list ')'';' 
					{
						list<Symbol*> *symList = new list<Symbol*>();
						while(!nameStack.empty())
						{
							Symbol *temp = lookUpSymbolInScope(nameStack.top());
							symList->push_front(temp);
							nameStack.pop();
						}
						$$ = (void*)new IONode(READ_NODE, symList);
					}
write_stmt        : WRITE {currVarType = WRITE;}'(' id_list ')'';'
					{
						list<Symbol*> *symList = new list<Symbol*>();
						while(!nameStack.empty())
						{
							Symbol *temp = lookUpSymbolInScope(nameStack.top());
							symList->push_front(temp);
							nameStack.pop();
						}
						$$ = (void*)new IONode(WRITE_NODE, symList);
					}
return_stmt       : RETURN expr ';' {$$ = $2; /*need to implement for function calls*/}

/* Expressions */
expr              : expr_prefix factor 
					{
						ASTNode* temp = (ASTNode*)$1;
						if(temp != NULL)
						{
							temp->right = (ASTNode*)$2;
							$$ = (void*)temp;
						}
						else
						{
							$$ = $2;
						}
					}
expr_prefix       : expr_prefix factor addop 
					{
						ASTNode* temp = (ASTNode*) $1;
						if(temp != NULL)
						{
							//printf("cracked expression prefix !\n");
							temp->right = (ASTNode*)$2;
							$$ = (void*)new ASTNode($3,temp,NULL);
						}
						else
						{
							$$ = (void*)new ASTNode($3,$2,NULL);
						}	
					} | /*empty*/{$$ = NULL;}
factor            : factor_prefix postfix_expr 
					{
						ASTNode* temp = (ASTNode*)$1; 
						if ((temp != NULL))
						{
							temp->right = (ASTNode*)$2; 
							$$ = (void*)temp;
						}
						else
						{
							$$ = $2;
						} 
					}
factor_prefix     : factor_prefix postfix_expr mulop 
					{
						ASTNode* temp = (ASTNode*) $1;
						if(temp != NULL)
						{
							//printf("cracked factor prefix !");
							temp->right = (ASTNode*)$2;
							$$ = (void*)new ASTNode($3,temp,NULL);
						}
						else
						{
							$$ = (void*)new ASTNode($3,$2,NULL);
						}	
					}| /*empty*/ {$$ = NULL;}
postfix_expr      : primary {$$ = $1;} | call_expr {$$ = $1;}
call_expr         : id '(' expr_list ')' {$$ = $3;}
expr_list         : expr expr_list_tail | /*empty*/ {$$= NULL}
expr_list_tail    : ',' expr expr_list_tail | /*empty*/
primary           : '(' expr ')' {$$ = $2}| 
					id 
					{
						Symbol* temp = lookUpSymbolInScope($1);
						$$ = (void*) new SymNode(SYMBOL_NODE,temp);
					}| 
					INTLITERAL 
					{	
						$$ = new Literal(INTLIT_NODE, $1);
					}|
					FLOATLITERAL 
					{
						$$ = new Literal(FLOATLIT_NODE, $1);
					}
addop             : ADD_OP {$$ = ADD_NODE;} | MINUS_OP {$$ = MINUS_NODE;}
mulop             : MUL_OP {$$ = MUL_NODE;} | DIV_OP {$$ = DIV_NODE;}

/* Complex Statements and Condition */ 
if_stmt           : IF '(' cond ')' {createBlockTable(++blockNum);} decl stmt_list {finishScope();}else_part FI
else_part         : ELSE {createBlockTable(++blockNum);} decl stmt_list {finishScope();}| /*empty*/
cond              : expr compop expr
compop            : '<' | '>' | EQ_OP | NEQ_OP | LTE_OP | GTE_OP

init_stmt         : assign_expr | /*empty*/
incr_stmt         : assign_expr | /*empty*/

/* ECE 573 students use this version of for_stmt */
for_stmt       : FOR '(' init_stmt ';' cond ';' incr_stmt ')' {createBlockTable(++blockNum);} decl aug_stmt_list ROF {finishScope();} 

/* CONTINUE and BREAK statements. ECE 573 students only */
aug_stmt_list     : aug_stmt aug_stmt_list | /*empty*/
aug_stmt          : base_stmt | aug_if_stmt | for_stmt | CONTINUE';' | BREAK';'

/* Augmented IF statements for ECE 573 students */ 
aug_if_stmt       : IF '(' cond ')' {createBlockTable(++blockNum);} decl aug_stmt_list {finishScope();} aug_else_part FI
aug_else_part     : ELSE {createBlockTable(++blockNum);} decl aug_stmt_list {finishScope();}| /*empty*/


%%
