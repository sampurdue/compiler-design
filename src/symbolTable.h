#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H


#include <string.h>
#include <stdlib.h>
#include <list>
#include <stack>
#include <stdio.h>
#include "parser.tab.h"
using namespace std;


/*Data type definitions */

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
	symTab(char* name);
	~symTab();
	
};

/*Function declarations */

void printSymbol(Symbol* sym);
void freeSymbol(Symbol* sym);

symTab* createSymbolTable(char* blockName);
void addElementToTable(symTab* symTable, Symbol* sym);
void addStringElementToTable(char* varName, char* strVal);
void addElementsToTable();
void freeSymbolTable(symTab* symTable);
void printSymbolTable(symTab* symTable);

int addToHashTable(char* str, symTab* Table);
void freeHashTable(stringList* hashTable[]);
int hash(char* str);
void printhashTable(symTab* Table);

int checkShadowVariable(Symbol* sym);
int checkShadowVariableInTable(symTab* symTable, Symbol* sym);

void printTableList();
void freeAllTables();


void createGlobalTable();
void createBlockTable(char* blockName);
void createBlockTable(int blockNum);
void finishScope();



#endif //end symbol table definitions
