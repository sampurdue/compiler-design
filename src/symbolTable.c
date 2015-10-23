#include "symbolTable.h"

extern std::stack<char*> nameStack ;
extern int currVarType;

list<symTab*> symTabList;
list<symTab*> shadowSymTabList;
stack<symTab*> symTabStack;
symTab* currSymTab;
list<char*> declErrList ;
list<char*> shadowVarList;

/* class member definitions */

Symbol::Symbol(int varType, char* str, Value val)
{
	if(str == NULL)
	{
		printf("scream bloody murder\n");
	}
	else
	{
		//name = strndup(str, strlen(str));
		name = str;
		type = varType;
		symVal = val;
	}		
}

symTab::symTab(char* name)
{	
	int i =0; 
	while(i<HASH_TABLE_SIZE)
	{
		hashTable[i++] = NULL;
	}
	//blockName = strndup(name, strlen(name)); 
	blockName = name;
	symList = new list<Symbol*>();
}

symTab::~symTab()
{
	free(blockName);
	list<Symbol*>::iterator it = symList->begin(); 
	for(; it != symList->end(); it++){delete(*it);} 
	freeHashTable(hashTable);
	delete(symList);
}

/* Other function definitions*/

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
		delete(sym);
	}
}

symTab* createSymbolTable(char* blockName)
{
	symTab* temp = new symTab(blockName);
	return temp;
}

void addElementToTable(symTab* symTable, Symbol* sym)
{
	//printSymbol(sym);
	int found = addToHashTable(sym->name, symTable);
	int shadowVar = checkShadowVariable(sym);
	int insert = 10;

	if(found == 0)
	{
		declErrList.push_back(sym->name);
	}
	else if(shadowVar == 0)
	{
		list<char*>::iterator it = shadowVarList.begin();
		for(; it != shadowVarList.end(); it++)
		{
			insert = strcmp(*it, sym->name);
			if(insert == 0) {break;};
		}
		if(insert != 0)
		{
			shadowVarList.push_back(sym->name);
		}
	}
	symTable->symList->push_back(sym);
	
}

void addStringElementToTable(char* varName, char* strValue)
{
	Value tempVal;
	//tempVal.strVal = strndup(strValue, strlen((const char*)strValue));
	tempVal.strVal = strValue;
	Symbol* tempSym = new Symbol(STRING, varName,tempVal);
	addElementToTable(currSymTab, tempSym);
}

void addElementsToTable()
{
		while(!nameStack.empty())
		{
			char* name = nameStack.top();
			//printf("name  %s type %s\n",name, ((currVarType == INT)?"INT":"FLOAT"));
			Value tempVal;
			tempVal.iVal = 0;
			Symbol* tempSym = new Symbol(currVarType, name,tempVal);
			addElementToTable(currSymTab, tempSym);
			nameStack.pop();
		}
		currVarType=-1;
}
void freeSymbolTable(symTab* symTable)
{
	delete(symTable);
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

Symbol* lookUpSymbolInScope(char* symName)
{
	Symbol* retSym = NULL;
	list<symTab*> tempSymTabList ;

	//check in current scope
	list<Symbol*>::iterator it = currSymTab->symList->begin();
	for(; it != currSymTab->symList->end(); it++)
	{
		if(!(strcmp((*it)->name, symName)))
		{
			retSym = *it;
			break;
		}
	}
	
	if(retSym == NULL)
	{ //check in other tables in scope
		while(!symTabStack.empty())
		{
			symTab* temp = symTabStack.top();
			tempSymTabList.push_front(temp);
			symTabStack.pop();
			//check for symbol in this table
			list<Symbol*>::iterator it = temp->symList->begin();
			for(; it != temp->symList->end(); it++)
			{
				if(!(strcmp((*it)->name, symName)))
				{
					retSym = *it;
					break;
				}
			}
			if(retSym != NULL)
				break;
		}
		list<symTab*>::iterator it = tempSymTabList.begin();
		for(; it != tempSymTabList.end() ; it++)
		{
			symTabStack.push(*it);
		}
		while(!tempSymTabList.empty()){tempSymTabList.pop_front();}
	}
	return retSym;
	
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
	return found;
}

void freeHashTable(stringList* hashTable[])
{
	for(int i =0; i < HASH_TABLE_SIZE; i++)
	{
		stringList* curr = hashTable[i];
		stringList* next ;
		while(curr != NULL)
		{
			next = curr->next;
			free(curr);
			curr = next;
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
	return key%HASH_TABLE_SIZE;
}

void printhashTable(symTab* Table)
{
	stringList* curr;
	for(int i = 0; i<HASH_TABLE_SIZE; i++)
	{
		printf("Index %d \n", i);
		curr = Table->hashTable[i];
		while(curr != NULL)
		{
			printf("%s ", curr->str);
			curr = curr->next;
		}
		printf("\n");
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
			}	
			else
			{
				break;
			}
			curr = curr->next;
		}
	}
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
		//shadowSymTabList.pop_front(); //remove this. it will cause crash
	}
	while(!shadowSymTabList.empty()){shadowSymTabList.pop_front();}
	return found;
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
	freeAllTables();
}

void freeAllTables()
{
	list<symTab*>::iterator it = symTabList.begin();
	for(; it != symTabList.end(); it++)
	{
		freeSymbolTable(*it);
	}
}



void createGlobalTable()
{
	//printf("creating global table\n");
	char *str = strdup("GLOBAL");
	symTab* temp = createSymbolTable(str);
	symTabList.push_back(temp);
	currSymTab = temp;
}

void createBlockTable(int blockNum)
{
	char str[20] = {0,};
	sprintf(str,"%s%d","BLOCK ",blockNum);
	char *buf = strdup(str);
	createBlockTable(buf);
}


void createBlockTable(char* blockName)
{
	//printf("creating table for %s\n", blockName);
	symTab* temp = createSymbolTable(blockName);
	symTabList.push_back(temp);
	symTabStack.push(currSymTab);
	currSymTab = temp;
}

void finishScope()
{
	//printf("Finished scope for %s\n",currSymTab->blockName);
	currSymTab = symTabStack.top();
	symTabStack.pop(); 
}

