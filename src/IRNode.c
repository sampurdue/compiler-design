#include "IRNode.h"
#include "parser.tab.h"

list<IRNode*> irNodeList;
extern int tempNum;

IRNode::IRNode(const char* op, const char* firstOp, const char* secondOp, const char* res)
{
	opcode = NULL;
	op1 = NULL;
	op2 = NULL;
	result = NULL;
	
	if(op != NULL)
		opcode = strdup(op);
	if(firstOp != NULL)	
		op1 = strdup(firstOp);
	if(secondOp != NULL)	
		op2 = strdup(secondOp);
	if(res != NULL)	
		result = strdup(res);

}
IRNode::~IRNode()
{
	//printf("Destroying node\n");
	printIRNode();
	if(opcode != NULL)
		free(opcode);
	if(op1 != NULL)	
		free(op1);
	if(op2 != NULL)	
		free(op2);
	if(result != NULL)	
		free(result);
}

void IRNode::printIRNode(void)
{
	if(opcode != NULL)
		printf("%s ",opcode);
	if(op1 != NULL)
		printf("%s ", op1);
	if(op2 != NULL)
		printf("%s ", op2);	
	if(result != NULL)
		printf("%s ",result);
	printf("\n");	
	return;
}

void generateIRCode(ASTNode* root)
{
	if(root == NULL)
		return;
	

	switch(root->nodeType)
	{
		case ERROR_NODE 	:
		case ASSIGN_NODE	:
		{
			char* res;
			Symbol* symVal = (Symbol*)(((SymNode*)root->left)->sym);

			if((root->right->nodeType != SYMBOL_NODE) && (root->right->nodeType != INT_RES) && (root->right->nodeType != FLOAT_RES))
			{
				//printf("going to right for assing node\n");
				generateIRCode(root->right);
			}
			res = ((Literal*)(root->right))->value;
			if(symVal->type == INT )
			{
				irNodeList.push_back(new IRNode("STOREI",res,symVal->name, NULL));
			}
			else if(symVal->type == FLOAT)
			{
				irNodeList.push_back(new IRNode("STOREF",res,symVal->name,NULL) );
			}
			else
			{
				//printf("probably a string symbol\n");
			}
			break;
		}	
		case ADD_NODE 		:
		{
		
			int type = -1;
			char* op1;
			char* op2;
			char* result ;
			
			ASTNode* left = root->left;
			ASTNode* right = root->right;
			//printf("collapsing add node\n");
			
			if((left->nodeType != SYMBOL_NODE) && (left->nodeType != INT_RES) && (left->nodeType != FLOAT_RES))
			{
				//printf("going to left for add node\n");
				generateIRCode(root->left);
			}
			
			if((right->nodeType != SYMBOL_NODE) && (right->nodeType != INT_RES) && (right->nodeType != FLOAT_RES))
			{
				//printf("going to right for add node\n");
				generateIRCode(root->right);
			}
			
			result = generateTemp();
			if(left->nodeType == SYMBOL_NODE)
			{
				Symbol* symVal = (Symbol*)(((SymNode*)(root->left))->sym);
				//printf("left is symbol %s for add node\n", symVal->name);
				type = symVal->type;
				op1 = symVal->name;	
			}
			else if(left->nodeType == INT_RES)
			{
				//printf("left is int res for add node\n");
				type = INT;
				op1 = ((Literal*)left)->value;
			}
			else
			{
				//printf("left is float res for add node\n");
				type = FLOAT;
				op1 = ((Literal*)left)->value;
			}
			
			if(right->nodeType == SYMBOL_NODE)
			{
				Symbol* symVal = (Symbol*)(((SymNode*)(root->right))->sym);
				//printf("right is symbol %s for add node\n", symVal->name);
				op2 = symVal->name;
			}
			else
			{	
				op2 = ((Literal*)(root->right))->value;
				//printf("right is %s for add node\n",op2);
			}
			const char* opcode = ((type == INT)?"ADDI":"ADDF");
			//printf("opcode %s op1 %s op2 %s result %s\n", opcode, op1, op2, result);
			irNodeList.push_back(new IRNode(opcode, op1, op2, result));
			root->nodeType = (type == INT) ? INT_RES : FLOAT_RES;
			((Literal*)root)->value = result;
			
			break;
			
		}
		case MINUS_NODE		:
		{
		
			int type = -1;
			char* op1;
			char* op2;
			char* result ;
			
			ASTNode* left = root->left;
			ASTNode* right = root->right;
			//printf("collapsing minus node\n");
			
			if((left->nodeType != SYMBOL_NODE) && (left->nodeType != INT_RES) && (left->nodeType != FLOAT_RES))
			{
				//printf("going to left for minus node\n");
				generateIRCode(root->left);
			}
			
			if((right->nodeType != SYMBOL_NODE) && (right->nodeType != INT_RES) && (right->nodeType != FLOAT_RES))
			{
				//printf("going to right for minus node\n");
				generateIRCode(root->right);
			}
			
			result = generateTemp();
			if(left->nodeType == SYMBOL_NODE)
			{
				Symbol* symVal = (Symbol*)(((SymNode*)(root->left))->sym);
				//printf("left is symbol %s for minus node\n", symVal->name);
				type = symVal->type;
				op1 = symVal->name;	
			}
			else if(left->nodeType == INT_RES)
			{
				//printf("left is int res for minus node\n");
				type = INT;
				op1 = ((Literal*)left)->value;
			}
			else
			{
				//printf("left is float res for minus node\n");
				type = FLOAT;
				op1 = ((Literal*)left)->value;
			}
			
			if(right->nodeType == SYMBOL_NODE)
			{
				Symbol* symVal = (Symbol*)(((SymNode*)(root->right))->sym);
				//printf("right is symbol %s for minus node\n", symVal->name);
				op2 = symVal->name;
			}
			else
			{	
				op2 = ((Literal*)(root->right))->value;
				//printf("right is %s for minus node\n",op2);
			}
			const char* opcode = ((type == INT)?"SUBI":"SUBF");
			//printf("opcode %s op1 %s op2 %s result %s\n", opcode, op1, op2, result);
			irNodeList.push_back(new IRNode(opcode, op1, op2, result));
			root->nodeType = (type == INT) ? INT_RES : FLOAT_RES;
			((Literal*)root)->value = result;
			
			break;
			
		}
		case MUL_NODE		: 
		{
		
			int type = -1;
			char* op1;
			char* op2;
			char* result ;
			
			ASTNode* left = root->left;
			ASTNode* right = root->right;
			//printf("collapsing mul node\n");
			
			if((left->nodeType != SYMBOL_NODE) && (left->nodeType != INT_RES) && (left->nodeType != FLOAT_RES))
			{
				//printf("going to left for mul node\n");
				generateIRCode(root->left);
			}
			
			if((right->nodeType != SYMBOL_NODE) && (right->nodeType != INT_RES) && (right->nodeType != FLOAT_RES))
			{
				//printf("going to right for mul node\n");
				generateIRCode(root->right);
			}
			
			result = generateTemp();
			if(root->left->nodeType == SYMBOL_NODE)
			{
				Symbol* symVal = (Symbol*)(((SymNode*)(root->left))->sym);
				//printf("left is symbol %s for mul node\n", symVal->name);
				type = symVal->type;
				op1 = symVal->name;	
			}
			else if(root->left->nodeType == INT_RES)
			{
				//printf("left is int res for mul node\n");
				type = INT;
				op1 = ((Literal*)left)->value;
			}
			else
			{
				//printf("left is float res for mul node\n");
				type = FLOAT;
				op1 = ((Literal*)left)->value;
			}
			
			if(root->right->nodeType == SYMBOL_NODE)
			{
				Symbol* symVal = (Symbol*)(((SymNode*)(root->right))->sym);
				//printf("right is symbol %s for mul node\n", symVal->name);
				op2 = symVal->name;
			}
			else
			{
				op2 = ((Literal*)(root->right))->value;
				//printf("right is %s for mul node\n",op2);
			}
			const char* opcode = ((type == INT)?"MULI":"MULF");
			//printf("opcode %s op1 %s op2 %s result %s\n", opcode, op1, op2, result);
			irNodeList.push_back(new IRNode(opcode, op1, op2, result));
			root->nodeType = (type == INT) ? INT_RES : FLOAT_RES;
			((Literal*)root)->value = result;
			
			break;
			
		}
		case DIV_NODE		:
		{
		
			int type = -1;
			char* op1;
			char* op2;
			char* result ;
			
			ASTNode* left = root->left;
			ASTNode* right = root->right;
			//printf("collapsing div node\n");
			
			if((left->nodeType != SYMBOL_NODE) && (left->nodeType != INT_RES) && (left->nodeType != FLOAT_RES))
			{
				//printf("going to left for div node\n");
				generateIRCode(left);
			}
			
			if((right->nodeType != SYMBOL_NODE) && (right->nodeType != INT_RES) && (right->nodeType != FLOAT_RES))
			{
				//printf("going to right for div node\n");
				generateIRCode(right);
			}
			
			result = generateTemp();
			if(left->nodeType == SYMBOL_NODE)
			{
				Symbol* symVal = (Symbol*)(((SymNode*)(left))->sym);
				//printf("left is symbol %s for div node\n", symVal->name);
				type = symVal->type;
				op1 = symVal->name;	
			}
			else if(left->nodeType == INT_RES)
			{
				//printf("left is int res for div node\n");
				type = INT;
				op1 = ((Literal*)left)->value;
			}
			else
			{
				//printf("left is float res for div node\n");
				type = FLOAT;
				op1 = ((Literal*)left)->value;
			}
			
			if(right->nodeType == SYMBOL_NODE)
			{
				Symbol* symVal = (Symbol*)(((SymNode*)(right))->sym);
				//printf("right is symbol %s for div node\n", symVal->name);
				op2 = symVal->name;
			}
			else
			{
				op2 = ((Literal*)right)->value;
				//printf("right is %s for div node\n",op2);
			}
			const char* opcode = ((type == INT)?"DIVI":"DIVF");
			//printf("opcode %s op1 %s op2 %s result %s\n", opcode, op1, op2, result);
			irNodeList.push_back(new IRNode(opcode, op1, op2, result));
			root->nodeType = (type == INT) ? INT_RES : FLOAT_RES;
			((Literal*)root)->value = result;
			
			break;
			
		}				
		case EQ_NODE		: 
		case NEQ_NODE 		:
		case LE_NODE		:
		case GT_NODE 		:
		case LBT_NODE		:
		case RBR_NODE 		:
		case SC_NODE 		:
		case LTE_NODE 		:
		case GTE_NODE		:
		case STMT_LIST		:
		case IF_STMT		:
		case FOR_STMT		:
		case PROG_ROOT		:
		case STMT		:
		case FUNC_BODY		:
		case FN_CALL		:
			generateIRCode(root->left);
			generateIRCode(root->right);
			//printf(" generic expr %s \n", nodeTypeArr[root->nodeType + OFFSET]);
			break;
		case SYMBOL_NODE	:
		{
			break;
		}	
		case INTLIT_NODE	:
		{
			char* temp	= generateTemp();
			//printf("int lit %s ",((Literal*)root)->value);
			irNodeList.push_back(new IRNode("STOREI", ((Literal*)root)->value, temp, NULL));
			((Literal*)root)->nodeType = INT_RES;
			((Literal*)root)->value = temp;
			break;
		}	
		case FLOATLIT_NODE	:
		{
			char* temp	= generateTemp();
			//printf("float lit %s ",((Literal*)root)->value);
			irNodeList.push_back(new IRNode("STOREF", ((Literal*)root)->value, temp, NULL));
			root->nodeType = FLOAT_RES;
			((Literal*)root)->value = temp;
			break;
		}
		case STRINGLIT_NODE	:
			//printf("string lit %s ",((Literal*)root)->value);
			//printLiteral((Literal*) root);
			break;
		case READ_NODE:
		{
			IONode *temp = (IONode*)root;
			Symbol * tempSym = NULL;
			list<Symbol*>::iterator it = temp->symList->begin();
			for(; it != temp->symList->end(); it++)
			{
				tempSym = *it;
				if(tempSym->type == INT )
					irNodeList.push_back(new IRNode("READI", tempSym->name, NULL, NULL));
				else if((tempSym->type == FLOAT ))	
					irNodeList.push_back(new IRNode("READF", tempSym->name, NULL, NULL));
				else
					;//printf("Got data type %d\n", tempSym->type);				
			}
			break;
		}
		case WRITE_NODE:
		{
			IONode *temp = (IONode*)root;
			Symbol * tempSym = NULL;
			list<Symbol*>::iterator it = temp->symList->begin();
			for(; it != temp->symList->end(); it++)
			{
				tempSym = *it;
				if(tempSym->type == INT )
					irNodeList.push_back(new IRNode("WRITEI", tempSym->name, NULL, NULL));
				else if((tempSym->type == FLOAT ))	
					irNodeList.push_back(new IRNode("WRITEF", tempSym->name, NULL, NULL));
				else
					;//printf("Got data type %d\n", tempSym->type);				
			}
			break;
		}	
		default:
			//printf("reached default node\n");
			break;
	};	
}



void printIRList(list<IRNode*> *irNodeList)
{
	list<IRNode*>::iterator it = irNodeList->begin();
	printf("; IR code\n");
    for(; it != irNodeList->end(); it++)
    {
		printf("; ");
		(*it)->printIRNode();
	}
}


char* generateTemp()
{
	char* tempVar = (char*)malloc(7*sizeof(char));
	memset(tempVar, 0 , 7);
	sprintf(tempVar, "$T%d",++tempNum);
	tempVar[7]=0;
	return tempVar;
}