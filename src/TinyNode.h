#ifndef _TINY_NODE_H
#define _TINY_NODE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <list>
#include "IRNode.h"

class TinyNode
{
	char* opcode;
	char* op1;
	char* op2;
	
	public:
	TinyNode(){opcode = op1 = op2= NULL;}
	TinyNode(const char* opcode, const char* firstOp, const char* secondOp);
	void printTinyNode(void);
	~TinyNode();
};

list<TinyNode*>* IRtoTinyCode(list <IRNode*> *IRlist, symTab* symTable);
void printTinyList(std::list<TinyNode*> *tinyList);
TinyNode* convertToTiny(IRNode *irNode);
#endif
