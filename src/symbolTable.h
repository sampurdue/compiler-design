#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H


#include <string.h>
#include <stdlib.h>
#include <list>
#include <stack>
#include <stdio.h>
#include "parser.tab.h"
using namespace std;

typedef union 
{
	int iVal;
	float fVal;
	char* strVal;
}Value;

class Symbol
{
	public:
	char* name;
	int type;
	Value symVal;
	
	public:
	Symbol(){name = NULL; type = 0; symVal.iVal =0;}
	Symbol(int varType, char* str, Value val);
	~Symbol(){free(name);}
};

typedef struct node
{
	char* str;
	struct node* next;
}stringList;

class symTab
{
	public:
	char* blockName;
	list<Symbol*> *symList;
	stringList* hashTable[10];
	
	symTab() {blockName = NULL; symList = new list<Symbol*>();}
	symTab(char const* name){int i =0; while(i<10){hashTable[i++] = NULL;}blockName = strndup(name, strlen(name)); symList = new list<Symbol*>();}
	~symTab(){free(blockName); delete(symList);}
	
};




void printSymbol(Symbol* sym);
void freeSymbol(Symbol* sym);
void printSymbolTable(symTab* symTable);
symTab* createSymbolTable(char const* blockName);
void freeSymbolTable(symTab* symTable);
void addElementToTable(symTab* symTable, Symbol* sym);
void printTableList();
int addToHashTable(char* str, symTab* Table);
int hash(char* str);
void printhashTable(symTab* Table);
int checkShadowVariable(Symbol* sym);
int checkShadowVariableInTable(symTab* symTable, Symbol* sym);


#endif //end symbol table definitions
