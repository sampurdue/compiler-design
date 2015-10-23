#ifndef _IR_NODE_H
#define _IR_NODE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ASTNode.h"

class IRNode
{
	public:
	char* opcode;
	char* op1;
	char* op2;
	char* result;
	
	public:
	IRNode(){opcode = op1 = op2= result = NULL;}
	IRNode(const char* opcode, const char* firstOp, const char* secondOp, const char* res);
	void printIRNode(void);
	~IRNode();
};

void generateIRCode(ASTNode* root);
void printIRList(list<IRNode*> *irNodeList);
char* generateTemp(void);
#endif
