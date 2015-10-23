#ifndef _AST_H
#define _AST_H

#include "symbolTable.h"
#include <stdio.h>

#define OFFSET 1
// node type enum defn
enum NodeType
{
	ERROR_NODE = -OFFSET,
	ASSIGN_NODE , //0
	ADD_NODE, //1
	MINUS_NODE ,//2
	MUL_NODE, //3
	DIV_NODE ,//4
	EQ_NODE, //5
	NEQ_NODE ,//6
	LE_NODE ,//7
	GT_NODE ,//8
	LBT_NODE ,//9
	RBR_NODE ,//10
	SC_NODE ,//11
	LTE_NODE ,//12
	GTE_NODE,//13
	STMT_LIST,//14
	IF_STMT,//15
	FOR_STMT,//16
	FN_CALL,//17
	SYMBOL_NODE,//18
	INTLIT_NODE,//19
	FLOATLIT_NODE,//20
	STRINGLIT_NODE,//21
	PROG_ROOT,//22
	STMT,//23
	FUNC_BODY,//24
	READ_NODE,
	WRITE_NODE,
	INT_RES,
	FLOAT_RES,
	NUM_NODES,	//25
};

extern const char* nodeTypeArr[NUM_NODES+OFFSET];

class ASTNode
{
	public:
	int nodeType;
	ASTNode* left;
	ASTNode* right;
	
	ASTNode(){nodeType = ERROR_NODE; left = right = NULL;}
	ASTNode(int nType, void* lchild, void* rchild);

};

class SymNode
{
	public:
	int nodeType;
	Symbol* sym;
	ASTNode* dummy;
	
	SymNode(){nodeType = SYMBOL_NODE; sym = NULL; dummy = NULL;}
	SymNode(int nType, Symbol* symVal);
};


class AssignNode
{
	public:
	int nodeType;
	SymNode *sym;
	ASTNode* expr;
	
	AssignNode(){nodeType = ERROR_NODE; sym = NULL; expr = NULL;}
	AssignNode(int nType, SymNode* symVal, ASTNode* _expr );

};


class Literal
{
	public:
	int nodeType;
	char* value;
	char* dummy;
	
	Literal(){nodeType = ERROR_NODE; value = NULL; dummy = NULL;}
	Literal(int nType, char* Val);
};


class Flow
{
	public:
	int nodeType;
	ASTNode* condn;
	ASTNode* tr;	//ast for true,
	ASTNode* el;	//else condn
	
	Flow(){nodeType = ERROR_NODE; condn = tr = el = NULL;}
	Flow(int nType, ASTNode* _condn, ASTNode* _tr, ASTNode* _tl);
};

class IONode
{
	public:
	int nodeType;
	list<Symbol*> *symList;
	char* dummy;
	
	IONode() {nodeType = ERROR_NODE; symList = NULL; dummy = NULL;}
	IONode(int ioType, list<Symbol*> *SymbolList);

};

void printEntireAST(ASTNode* root);
void printLiteral(Literal* lit);









#endif
