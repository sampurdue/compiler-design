#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H
#include <string.h>
#include <stdlib.h>
#include <list>
#include <stack>
#include "parser.tab.h"
using namespace std;

int currVarType;
int ongoingDecl;


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




list<symTab*> symTabList;
list<symTab*> shadowSymTabList;
stack<symTab*> symTabStack;
symTab* currSymTab;
list<char*> declErrList ;
list<char*> shadowVarList;

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




Symbol::Symbol(int varType, char* str, Value val)
{
	if(str == NULL)
	{
		printf("scream bloody murder\n");
	}
	else
	{
		name = strndup(str, strlen(str));
		type = varType;
		symVal = val;
	}		
}

void printSymbol(Symbol* sym)
{
	printf("Symbol info for %s\n", sym->name);
	printf("Symbol type %s\n", (sym->type == INT)?"INT":((sym->type == FLOAT)?"FLOAT":"STRING"));
}


void freeSymbol(Symbol* sym)
{
	if(sym == NULL)
	{
		return;
	}
	else
	{
		free (sym->name);
		delete sym;
	}
}
void printSymbolTable(symTab* symTable)
{
	Symbol *temp;
	if(symTable == NULL)
	{
		return;
	}
	else
	{
		printf("Symbol table %s\n",symTable->blockName);
		for(list<Symbol*>::iterator it = symTable->symList->begin(); it !=symTable->symList->end();it++)
		{
			temp = *it;
			printf("name %s type %s ",temp->name, (temp->type == INT)?"INT":((temp->type == FLOAT)?"FLOAT":"STRING"));
			if(temp->type == STRING)
			{
				printf("value %s",temp->symVal.strVal);
			}
			printf("\n");
		}
		
	}
}
symTab* createSymbolTable(char const* blockName)
{
	symTab* temp = new symTab(blockName);
	return temp;
}

void freeSymbolTable(symTab* symTable)
{
	delete symTable;
}
void addElementToTable(symTab* symTable, Symbol* sym)
{
	//printSymbol(sym);
	int found = addToHashTable(sym->name, symTable);
	int shadowVar = checkShadowVariable(sym);
	//printf("Match %d for %s\n",found, sym->name);
	//int found = 10;
	if(found == 0)
	{
		declErrList.push_back(sym->name);
	}
	else if(shadowVar == 0)
	{
		shadowVarList.push_back(sym->name);
	}
	symTable->symList->push_back(sym);
	
}


void printTableList()
{

	if(declErrList.size() != 0)
	{
		list<char*>::iterator it = declErrList.begin();
		//for(; it != declErrList.end(); it++)
		//{
			printf("DECLARATION ERROR %s\n",*it);
		//}
	}
	else
	{
	
		list<char*>::iterator shadowIt = shadowVarList.begin();
		for(; shadowIt != shadowVarList.end(); shadowIt++)
		{
			printf("SHADOW WARNING %s\n",*shadowIt);
		}
	
		list<symTab*>::iterator it = symTabList.begin();
		list<symTab*>::iterator listEnd = symTabList.end();
		for( ; it != listEnd; it++)
		{
			printSymbolTable(*it);
		}
	}
}


int hash(char* str)
{
	char* temp = str;
	unsigned int key = 0;
	if(str == NULL)
	{
		return -1;
	}
	else
	{
		while(*temp != 0)
		{
				key += *(temp++);
		}
	}
	//printf("Key calculated for %s is %d\n",str, key%10);
	return key%10;
}

int addToHashTable(char* str, symTab* Table)
{
	int idx = hash(str);
	int found = 10;
	stringList **head ;	
	head = &Table->hashTable[idx];
	stringList* curr = *head;
	stringList* prev = NULL;
	stringList* tempStr = (stringList*)malloc(sizeof(stringList));
	tempStr->str = str;
	tempStr->next = NULL;

	if(*head == NULL)
	{
		*head = tempStr;
	}
	else
	{
		while(curr !=NULL)
		{
			if(found != 0)
			{
				found = strcmp(str, curr->str);
			}	
			prev = curr;
			curr = curr->next;
		}
		prev->next = tempStr;
	}
	//printhashTable(Table);
	//printf("Match %d before return\n",found);
	return found;
}

void printhashTable(symTab* Table)
{
	stringList* curr;
	for(int i = 0; i<10; i++)
	{
		printf("Index %d \n", i);
		//if(Table->hashTable[i] != NULL)
		//{
			curr = Table->hashTable[i];
			while(curr != NULL)
			{
				printf("%s ", curr->str);
				curr = curr->next;
			}
			printf("\n");
		//}
	}
}

int checkShadowVariableInTable(symTab* symTable, Symbol* sym)
{
	int found = 10;
	if((symTable == NULL) || (sym == NULL))
	{
		printf("Scream bloody murder\n");
	}
	else
	{
		//printhashTable(symTable);
		int idx = hash(sym->name);
		stringList* curr = symTable->hashTable[idx];
		while(curr !=NULL)
		{
			if(found != 0)
			{
				
				found = strcmp(sym->name, curr->str);
				//printf("comparing %s and %s found value %d\n",sym->name, curr->str, found);
			}	
			else
			{
				break;
			}
			curr = curr->next;
		}
	}
	//printf("match found for shadow %d \n", found);
	return found;
}

int checkShadowVariable(Symbol* sym)
{
	int found = 10;
	while(!symTabStack.empty())
	{
		symTab* temp = symTabStack.top();
		shadowSymTabList.push_front(temp);
		symTabStack.pop();
		if((found = checkShadowVariableInTable(temp, sym)) == 0)
		{
			break;
		}
	}
	list<symTab*>::iterator it = shadowSymTabList.begin();
	for(; it != shadowSymTabList.end() ; it++)
	{
		symTabStack.push(*it);
	}
	while(!shadowSymTabList.empty()){shadowSymTabList.pop_front();}

	return found;
}

#endif //end symbol table definitions
