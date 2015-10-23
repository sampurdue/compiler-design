#include "TinyNode.h"

TinyNode::TinyNode(const char* op,const char* firstOp,const char* secondOp)
{
	opcode = NULL;
	op1 = NULL;
	op2 = NULL;
	if(op!= NULL)
		opcode = strdup(op);
	if(firstOp != NULL)
		op1 = strdup(firstOp);
	if(secondOp != NULL)
		op2 = strdup(secondOp);	
}
TinyNode::~TinyNode()
{
	printf("Destroying node\n");
	printTinyNode();
	if(opcode != NULL)
		free(opcode);
	if(op1 != NULL)	
		free(op1);
	if(op2 != NULL)	
		free(op2);
}

void TinyNode::printTinyNode(void)
{
	if(opcode != NULL)
		printf("%s ",opcode);
	if(op1 != NULL)
		printf("%s ", op1);
	if(op2 != NULL)
		printf("%s", op2);	
	printf("\n");	
	return;
}
