#include <list>
#include "symbolTable.h"
#include "IRNode.h"
#include <string.h>
#include "TinyNode.h"
#include <stdio.h>
using namespace std;

int startsWith (char* base, char* str);
TinyNode* convertToTiny(IRNode *irNode);
list<TinyNode*> *tinyList;

list<TinyNode*>* IRtoTinyCode(list <IRNode*> *IRlist, symTab* symTable)
{	
	tinyList = new list<TinyNode*>();
	Symbol *temp;
	IRNode *tempNode;

	if(symTable != NULL){
	for(list<Symbol*>::iterator it = symTable->symList->begin(); it !=symTable->symList->end();it++)
	{
		temp = *it;
		tinyList->push_back(new TinyNode("var",temp->name,NULL));
	}}
	

	for(list<IRNode*>::iterator it = IRlist->begin();it != IRlist->end();it++)
	{
		tempNode = *it;
		TinyNode *tNode = convertToTiny(tempNode);
		if(tNode != NULL) 
		{
			tinyList->push_back(tNode);
		}

  	}
	tinyList->push_back(new TinyNode("sys halt",NULL,NULL));
	return tinyList;

}



//Check if a string (base) begins with another string (str)
int startsWith (char* base, char* str)
{
	return (strstr(base, str) - base) == 0;
}


//Remove "$T" and append "r" in the beginning of the string
char* regFormat(char* reg)
{
	char* retStr = reg;
	int i =2;
	if(reg == NULL)
	{
		printf("Got null for reg format.\n");
	}
	else
	{
		if (strstr(reg,"$T"))
		{
			char* temp = strdup(reg+i);
			retStr = (char*) malloc((strlen(temp)+i)*sizeof(char));
			memset(retStr, 0, strlen(temp)+i);
			retStr[0]='r';
			strcat(retStr, temp);

		}
	}
	return retStr;

}

TinyNode* convertToTiny(IRNode *irNode)
{
	char* opcode = irNode->opcode;
	char* op1 = NULL;
	char* op2 = NULL;

	//printf("converting ir node \n");
	//irNode->printIRNode();
   if(strstr(irNode->opcode, "WRITEI")) 
	{
		opcode = strdup("sys");
		op1 = strdup("writei");
		op2 = irNode->op1;
	}
	else if(strstr(irNode->opcode, "WRITEF"))
	{
		opcode = strdup("sys");
		op1 = strdup("writer");
		op2 = irNode->op1;    
	}
	else if(strstr(irNode->opcode,"READI")) 
	{
		opcode = strdup("sys");
		op1 = strdup("readi");
		op2 = irNode->op1;
	}
	else if(strstr(irNode->opcode,"READF")) 
	{
		opcode = strdup("sys");
		op1 = strdup("readf");
		op2 = irNode->op1;
	}
	else if(strstr(irNode->opcode, "STORE")) 
	{
		opcode = strdup("move");
		op1 = irNode->op1;
		if(strstr (op1,"$T"))
		{
			op1 = regFormat(op1);
		}	
		op2 = irNode->op2;
		if((irNode->op2 != NULL) && (strstr (irNode->op2,"$T")))
		{
			op2 = regFormat(irNode->op2);
		}		
	}
	else if(strstr(irNode->opcode, "ADD") != NULL) 
	{
		opcode= strdup("move");
		tinyList->push_back(new TinyNode(opcode,regFormat(irNode->op1),regFormat(irNode->result)));
		if(strstr(irNode->opcode,"ADDI"))
		{
				opcode = strdup("addi");
		}
		else 
		{
				opcode = strdup("addr");
		}
		tinyList->push_back(new TinyNode(opcode,regFormat(irNode->op2),regFormat(irNode->result)));
		return NULL;
	}
	else if(strstr(irNode->opcode, "SUB") != NULL) 
	{
		opcode= strdup("move");
		tinyList->push_back(new TinyNode(opcode,regFormat(irNode->op1),regFormat(irNode->result)));
		if(strstr(irNode->opcode,"SUBI"))
		{
				opcode = strdup("subi");
		}
		else 
		{
				opcode = strdup("subr");
		}
		tinyList->push_back(new TinyNode(opcode,regFormat(irNode->op2),regFormat(irNode->result)));
		return NULL;
	}
	else if(strstr(irNode->opcode, "MUL") != NULL) 
	{
		opcode= strdup("move");
		tinyList->push_back(new TinyNode(opcode,regFormat(irNode->op1),regFormat(irNode->result)));
		if(strstr(irNode->opcode,"MULI"))
		{
				opcode = strdup("muli");
		}
		else 
		{
				opcode = strdup("mulr");
		}
		tinyList->push_back(new TinyNode(opcode,regFormat(irNode->op2),regFormat(irNode->result)));
		return NULL;
	}
	else if(strstr(irNode->opcode, "DIV") != NULL) 
	{
		opcode= strdup("move");
		tinyList->push_back(new TinyNode(opcode,regFormat(irNode->op1),regFormat(irNode->result)));
		if(strstr(irNode->opcode,"DIVI"))
		{
				opcode = strdup("divi");
		}
		else 
		{
				opcode = strdup("divr");
		}
		tinyList->push_back(new TinyNode(opcode,regFormat(irNode->op2),regFormat(irNode->result)));
		return NULL;
	}
 

	TinyNode* retNode = new TinyNode(opcode, op1, op2);
	//printf("Converted tiny node \n");
	//retNode->printTinyNode();
	return retNode;
}

void printTinyList(std::list<TinyNode*> *tinyList)
{
	printf("; Tiny code\n");
	std::list<TinyNode*>::iterator itIRNode = tinyList->begin();
	for(; itIRNode != tinyList->end(); itIRNode++)
		(*itIRNode)->printTinyNode();

}









