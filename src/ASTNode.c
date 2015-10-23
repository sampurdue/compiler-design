#include "ASTNode.h"

const char* nodeTypeArr[NUM_NODES+OFFSET] = { "ERROR_NODE", "ASSIGN_NODE","ADD_NODE","MINUS_NODE","MUL_NODE","DIV_NODE","EQ_NODE","NEQ_NODE","LE_NODE","GT_NODE","LBT_NODE","RBR_NODE","SC_NODE","LTE_NODE","GTE_NODE","STMT_LIST","IF_STMT","FOR_STMT","FN_CALL","SYMBOL_NODE","INTLIT_NODE","FLOATLIT_NODE","	STRINGLIT_NODE","PROG_ROOT","STMT","FUNC_BODY"}; 

int tempNum;

ASTNode::ASTNode(int nType, void* lchild, void* rchild) 
{
	nodeType = nType; 
	left = (ASTNode*)lchild; 
	right = (ASTNode*)rchild; 
	
	/*
	fprintf(stderr,"creating ast node %s ",nodeTypeArr[nodeType + OFFSET]); 
	if(left != NULL)fprintf(stderr,"left child  %s ",nodeTypeArr[left->nodeType + OFFSET]);
	if(right != NULL)fprintf(stderr,"right child  %s ",nodeTypeArr[right->nodeType + OFFSET]);
	fprintf(stderr,"\n");
	*/
}

SymNode::SymNode(int nType, Symbol* symVal)
{
	nodeType = nType; 
	sym= symVal; 
	dummy = NULL;
	/*
	fprintf(stderr,"creating symbol node for %s\n",sym->name);
	*/
}

AssignNode::AssignNode(int nType, SymNode* symVal, ASTNode* _expr )
{
	nodeType = nType; 
	sym= symVal; 
	expr = _expr;
	/*
	fprintf(stderr,"creating assign node %s\n",nodeTypeArr[nodeType + OFFSET]); 
	if(sym != NULL)fprintf(stderr,"left child  %s ",sym->sym->name);
	if(expr != NULL)fprintf(stderr,"right child  %s ",nodeTypeArr[expr->nodeType + OFFSET]);
	fprintf(stderr,"\n");
	*/
}

Literal::Literal(int nType, char* Val)
{
	nodeType = nType; 
	value = Val; 
	dummy = NULL; 
	/*
	fprintf(stderr,"creating literal node %s %s \n", nodeTypeArr[nodeType+OFFSET], value );
	*/
}
	
Flow::Flow(int nType, ASTNode* _condn, ASTNode* _tr, ASTNode* _tl)
{
	nodeType = nType; 
	condn = _condn; 
	tr = _tr; 
	el = _tl; 
	/*
	printf("creating flow node %s\n",nodeTypeArr[nodeType + OFFSET]);
	*/
}
IONode::IONode(int ioType, list<Symbol*> *SymbolList) 
{ 
	nodeType = ioType; 
	symList = SymbolList;
	/*
	printf("creating node \n");
	*/
}	

void printEntireAST(ASTNode* root)
{
	if(root == NULL)
		return;
	//printf(" got %d \n", root->nodeType);
	//post order travsersal left, right root


	switch(root->nodeType)
	{
		case ERROR_NODE 	:
		case ASSIGN_NODE	:
			//generate code for assign node
			printEntireAST(root->left);
						printf(" := (");
			printEntireAST(root->right);
			printf(")\n");

			break;
		case ADD_NODE 		:
									printf("(");
			printEntireAST(root->left);
						printf(")");
						printf(" + ");
						printf("(");
			printEntireAST(root->right);
			printf(")");

			break;
		case MINUS_NODE		:
			printf("(");
			printEntireAST(root->left);
			printf(")");

						printf(" - ");
									printf("(");
			printEntireAST(root->right);
			printf(")");

			break;
		case MUL_NODE		: 
		printf("(");
			printEntireAST(root->left);
			printf(")");

						printf(" * ");
						printf("(");
			printEntireAST(root->right);
			printf(")");

			break;
		case DIV_NODE		:
			//generate code for assign node
			printf("(");
			printEntireAST(root->left);
			printf(")");

						printf(" / ");
						printf("(");
			printEntireAST(root->right);
			printf(")");

			break;				
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
			printEntireAST(root->left);
			printEntireAST(root->right);
			printf(" generic expr %s \n", nodeTypeArr[root->nodeType + OFFSET]);
			break;
		case SYMBOL_NODE	:
			printf("%s ", ((SymNode*)root)->sym->name);
			//printSymbol(((SymNode*)root)->sym);
			break;
		case INTLIT_NODE	:
		case FLOATLIT_NODE	:
		case STRINGLIT_NODE	:
			printf("%s ",((Literal*)root)->value);
			//printLiteral((Literal*) root);
			break;
		case READ_NODE:
		case WRITE_NODE:
		{
			printf(" %s ", (root->nodeType == READ_NODE) ? "READ": "WRITE");
			list<Symbol*>::iterator it = ((IONode*)root)->symList->begin();
			for(; it != ((IONode*)root)->symList->end(); it++)
				printf("%s ", (*it)->name);
			printf("\n");	
			break;
		}	
		default:
			printf("reached default node\n");
			break;
	};
	
}





void printLiteral(Literal* lit)
{
	if(lit == NULL)
	{
		printf("Trying to print empty literal");
		return ;
	}
	else
	{
		printf("literal %s %s \n", nodeTypeArr[lit->nodeType + OFFSET], lit->value);
	}
}
