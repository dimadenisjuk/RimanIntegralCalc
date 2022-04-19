#include "ParserNew.h"
#include <ctype.h>


void InitToken(Token* token) {
	token->type = TokenUnknown;
	token->value = NULL;
}

void PrintToken(Token* token) {
	if(token->type > TokenDataMin && token->type < TokenDataMax)
		printf("Data: %s\n", token->value);
	else if(token->type > TokenOperationMin && token->type < TokenOperationMax)
		printf("Operation: %s\n", token->value);
	else
	{
		printf("[err]: Non printable: %s, %d\n", token->value, token->type);
		assert(false);
	}
}

bool TokenIsOperation(Token* token, int count, ...) {
	if (count == 0)
		return (token->type > TokenOperationMin && token->type < TokenOperationMax);
	printf("NOT IMPLEMENTED!!!");
	assert(false);
	return false;
}

bool TokenIsConstant(Token* token) {
	return (token->type == TokenConstant);
}

void CreateScanner(Scanner* scn, const char* expr) {
	scn->pCurrent = expr;
	CreateStack(&scn->tokens);
}

void DeleteScanner(Scanner* scn) {
	while(scn->tokens.nCount!=0)
		PopStack(&scn->tokens);
}


Token CreateToken(Scanner* scn, const char* ptr, int count, enum ETokenType type) {
	Token token;
	InitToken(&token);
	token.value = malloc((count+1) * sizeof(char));
	strncpy(token.value, ptr, count); // копируем count символов из строки ptr
	token.value[count] = '\0';
	token.type = type;
	PushStack(&scn->tokens, token.value);
	return token;
}

Token GetToken(Scanner* scn) {
	while(true)
	{
		switch(*scn->pCurrent)
		{
			// Прокуск пробелов
			case ' ':
			case '\t':
				scn->pCurrent++;
				continue;
			case '+':
				return CreateToken(scn, scn->pCurrent++, 1, TokenAddition);
			case '-':
				return CreateToken(scn, scn->pCurrent++, 1, TokenSubtraction);
			case '*':
				return CreateToken(scn, scn->pCurrent++, 1, TokenMultiplication);
			case '/':
				return CreateToken(scn, scn->pCurrent++, 1, TokenDivision);
			case '^':
				return CreateToken(scn, scn->pCurrent++, 1, TokenPower);
			case '(':
				return CreateToken(scn, scn->pCurrent++, 1, TokenLeft);
			case ')':
				return CreateToken(scn, scn->pCurrent++, 1, TokenRight);
			case 'x':
				return CreateToken(scn, scn->pCurrent++, 1, TokenVariable);
			case 'c': // may be 'cos' or 'cot'
				++scn->pCurrent;
				if(*(scn->pCurrent++) == 'o')
				{
					if(*(scn->pCurrent++) == 's')
						return CreateToken(scn, "cos", 3, TokenFunction);
					if(*(scn->pCurrent++) == 't')
						return CreateToken(scn, "cot", 3, TokenFunction);
				}
				return CreateToken(scn, "", 0, TokenEmpty);
			case 'e': // may be 'exp'
				++scn->pCurrent;
				if((*scn->pCurrent++) == 'e')
					if(*(scn->pCurrent++) == 'x')
						if(*(scn->pCurrent++) == 'p')
							return CreateToken(scn, "exp", 3, TokenFunction);
				return CreateToken(scn, "", 0, TokenEmpty);
			case 'p': // may be 'pi'
				++scn->pCurrent;
				if(*(scn->pCurrent++) == 'i')
					return CreateToken(scn, "pi", 2, TokenConstant);
				return CreateToken(scn, "", 0, TokenEmpty);
			case 's': // may be 'sin'
				++scn->pCurrent;
				if(*(scn->pCurrent++) == 'i')
					if(*(scn->pCurrent++) == 'n')
						return CreateToken(scn, "sin", 3, TokenFunction);
				return CreateToken(scn, "", 0, TokenEmpty);
			case 'l': // may be 'log' or 'log10'
				++scn->pCurrent;
				if(*(scn->pCurrent++) == 'o')
					if(*(scn->pCurrent++) == 'g')
					{
						if(*(scn->pCurrent++) == '1')
						{
							if(*(scn->pCurrent++) == '0')
								return CreateToken(scn, "log10", 5, TokenFunction);
							return CreateToken(scn, "", 0, TokenEmpty);
						}
						else
							return CreateToken(scn, "log", 3, TokenFunction);
					}
				return CreateToken(scn, "", 0, TokenEmpty);
			case 't': // may be 'tan' or 'tg'
				++scn->pCurrent;
				if(*(scn->pCurrent++) == 'a')
				{
					if(*(scn->pCurrent++) == 'n')
						return CreateToken(scn, "tan", 3, TokenFunction);
				}
				else if(*(scn->pCurrent-1) == 'g')
					return CreateToken(scn, "tg", 3, TokenFunction);
				return CreateToken(scn, "", 0, TokenEmpty);
		}
		if(isdigit(*scn->pCurrent)) {
			char buffNum[MAX_NUM_LENGTH];
			unsigned i;
			for(i = 0; i < MAX_NUM_LENGTH; ++i, ++scn->pCurrent)
			{
				if(!isdigit(*scn->pCurrent))
					break;
				buffNum[i] = *scn->pCurrent;
			}
			if(*scn->pCurrent == '.')
			{
				buffNum[i] = *scn->pCurrent;
				++i, ++scn->pCurrent;
			}
			for(; i < MAX_NUM_LENGTH; ++i, ++scn->pCurrent)
			{
				if(!isdigit(*scn->pCurrent))
					break;
				buffNum[i] = *scn->pCurrent;
			}
			if(*scn->pCurrent == 'e' || *scn->pCurrent == 'E') // для чисел вида 1.341E+12
			{
				buffNum[i] = *scn->pCurrent;
				++i, ++scn->pCurrent;
				if(*scn->pCurrent == '+' || *scn->pCurrent == '-')
				{
					buffNum[i] = *scn->pCurrent;
					++i, ++scn->pCurrent;
				}
				goto parseAfterDot;
			}
			return CreateToken(scn, buffNum, i, TokenNumber);
parseAfterDot:
			for(; i < MAX_NUM_LENGTH; ++i, ++scn->pCurrent)
			{
				if(!isdigit(*scn->pCurrent))
					return CreateToken(scn, buffNum, i, TokenNumber);
				buffNum[i] = *scn->pCurrent;
			}
		}
		return CreateToken(scn, "", 0, TokenEmpty);
	}
}

Node* CreateNode(enum ENodeType type) {
	Node* node = malloc(sizeof(Node));
	node->type = type;
	return node;
}

void CreateParser(Parser* prs, const char* expr) {
	CreateScanner(prs, expr);
	prs->root = NULL;
	CreateStack(&prs->nodes);
}

void DeleteParser(Parser* prs) {
	DeleteChild(prs->root);
	DeleteScanner(prs);
}

Node* ParseAdditive(Parser* prs) {
	Node* pLeft = ParseMultiplicative(prs);
	while(prs->token.type == TokenAddition || prs->token.type == TokenSubtraction)
	{
		Node* pAdditive = CreateNode(NodeAdditive);
		pAdditive->token.type = prs->token.type;
		pAdditive->token.value = prs->token.value;
		prs->token = GetToken(prs);
		pAdditive->pLeft = pLeft;
		Node* pRight = ParseMultiplicative(prs);
		pAdditive->pRight = pRight;
		PushStack(&prs->nodes, pAdditive);
		pLeft = pAdditive;
	}
	return pLeft;
}

Node* ParseMultiplicative(Parser* prs) {
	Node* pLeft = ParsePower(prs);
	while(prs->token.type == TokenMultiplication || prs->token.type == TokenDivision)
	{
		Node* pMultiplicative = CreateNode(NodeMultiplicative);
		pMultiplicative->token.type = prs->token.type;
		pMultiplicative->token.value = prs->token.value;
		prs->token = GetToken(prs);
		pMultiplicative->pLeft = pLeft;
		Node* pRight = ParsePower(prs);
		pMultiplicative->pRight = pRight;
		PushStack(&prs->nodes, pMultiplicative);
		pLeft = pMultiplicative;
	}
	return pLeft;
}

Node* ParsePower(Parser* prs) {
	Node* pLeft = ParsePrimary(prs);
	while(prs->token.type == TokenPower)
	{
		Node* pPower = CreateNode(NodePower);
		pPower->token.type = prs->token.type;
		pPower->token.value = prs->token.value;
		prs->token = GetToken(prs);
		pPower->pLeft = pLeft;
		Node* pRight = ParsePrimary(prs);
		pPower->pRight = pRight;
		PushStack(&prs->nodes, pPower);
		pLeft = pPower;
	}
	return pLeft;
}

Node* ParsePrimary(Parser* prs) {
	if(prs->token.type == TokenLeft)
	{
		prs->token = GetToken(prs);
		Node* pPrim = ParseAdditive(prs);
		if(prs->token.type != TokenRight)
		{
			printf("[err]: Parse error: ')' expected\n");
			assert(false);
		}
		prs->token = GetToken(prs);
		return pPrim;
	}
	if(prs->token.type == TokenConstant || prs->token.type == TokenNumber || prs->token.type == TokenVariable)
	{
		Node* pPrim = CreateNode(NodePrimary);
		pPrim->token.value = malloc((strlen(prs->token.value)+1) * sizeof(char));
		pPrim->token.value = prs->token.value;
		pPrim->token.type = prs->token.type;
		pPrim->pLeft = NULL;
		pPrim->pRight = NULL;
		PushStack(&prs->nodes, pPrim);
		prs->token = GetToken(prs);
		return pPrim;
	}
	if(prs->token.type == TokenFunction)
	{
		Node* pPrim = CreateNode(NodeUnary);
		pPrim->token.value = malloc((strlen(prs->token.value)+1) * sizeof(char));
		pPrim->token.value = prs->token.value;
		pPrim->token.type = prs->token.type;
		prs->token = GetToken(prs);
		Node* pLeft = ParsePrimary(prs);
		pPrim->pLeft = pLeft;
		pPrim->pRight = NULL;
		PushStack(&prs->nodes, pPrim);
		return pPrim;
	}
	return ParseAdditive(prs);
}

Node* Parse(Parser* prs) {
	prs->token = GetToken(prs);
	prs->root = ParseAdditive(prs);
	return prs->root;
}

void PrintNode(Node* node, int depth) {
	if(node->type == NodePrimary)
	{
		printf("\n%d: %s -> ", depth, node->token.value);
	}
	else if(node->type == NodeUnary)
	{
		printf("\n%d: %s", depth, node->token.value);
		printf("\n%d ->", depth);
		PrintNode(node->pLeft, depth+1);
	}
	else if(node->type == NodeAdditive || node->type == NodeMultiplicative || node->type == NodePower)
	{
		printf("\n%d: %s", depth, node->token.value);
		printf("\n%d left ->", depth);
		PrintNode(node->pLeft, depth+1);
		printf("\n%d right ->", depth);
		PrintNode(node->pRight, depth+1);
	}
	else
	{
		printf("[err]: Unknown node: %s\n", node->token.value);
		assert(false);
	}
}

void DeleteChild(Node* node) {
	if (node->pLeft)
		DeleteChild(node->pLeft);
	if (node->pRight)
		DeleteChild(node->pRight);
	free(node);
}

T Calculate(Node* node, T firstVar) {
	if(node->type == NodePrimary)
	{
		if(node->token.type == TokenNumber)
			return atof(node->token.value);
		if(node->token.type == TokenVariable)
			return firstVar;
		if(node->token.type == TokenConstant)
		{
			printf("[err]: Constant not implemented: %s\n", node->token.value);
			assert(false); // Не реализовано
		}
	}
	else if(node->type == NodeUnary)
	{
		if(!strcmp(node->token.value, "sin"))
			return mySin(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "cos"))
			return myCos(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "tan"))
			return myTan(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "tg"))
			return myTan(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "acos"))
			return myAcos(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "asin"))
			return myAsin(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "atan"))
			return myAtan(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "exp"))
			return myAtan(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "tanh"))
			return myTanh(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "log"))
			return myLog(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "log10"))
			return myLog10(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "cot"))
			return myCot(Calculate(node->pLeft, firstVar));
		if(!strcmp(node->token.value, "acot"))
			return myAcot(Calculate(node->pLeft, firstVar));
		printf("[err]: Function not implemented: %s\n", node->token.value);
		assert(false); // Не реализовано
	}
	else if(node->type == NodeAdditive || node->type == NodeMultiplicative || node->type == NodePower)
	{
		if(node->token.type == TokenAddition)
			return Calculate(node->pLeft, firstVar) + Calculate(node->pRight, firstVar);
		if(node->token.type == TokenSubtraction)
			return Calculate(node->pLeft, firstVar) - Calculate(node->pRight, firstVar);
		if(node->token.type == TokenMultiplication)
			return Calculate(node->pLeft, firstVar) * Calculate(node->pRight, firstVar);
		if(node->token.type == TokenDivision)
			return Calculate(node->pLeft, firstVar) / Calculate(node->pRight, firstVar);
		if(node->token.type == TokenPower)
			return myPow(Calculate(node->pLeft, firstVar),  Calculate(node->pRight, firstVar));
		printf("[err]: Operation not implemented: %s\n", node->token.value);
		assert(false); // Не реализовано
	}
	printf("[err]: Unknown node: %s\n", node->token.value);
	assert(false);
	return false;
}

void PrintTree(Node* node, T firstVar, int depth) {
	if (node->pLeft)
		PrintTree(node->pLeft, firstVar, depth + 1);
	printf("\n");
	for (int i = 0; i < depth; i++)
		printf("\t");
	printf("%s\n", node->token.value);
	if (node->pRight)
		PrintTree(node->pRight, firstVar, depth + 1);
}

void CalculatePrint(Node* node, T firstVar) {
	if(node->type == NodePrimary)
	{
		printf("%s", node->token.value);
		return;
	}
	if(node->type == NodeUnary)
	{
		printf(" %s", node->token.value);
		printf("(");
		CalculatePrint(node->pLeft, firstVar);
		printf(")");
		return;
	}
	if(node->type == NodeAdditive || node->type == NodeMultiplicative || node->type == NodePower)
	{
		printf("[");
		CalculatePrint(node->pLeft, firstVar);
		printf("]");
		printf(" %s ", node->token.value);
		printf("[");
		CalculatePrint(node->pRight, firstVar);
		printf("]");
		return;
	}
	printf("[err]: Unknown node: %s\n", node->token.value);
	assert(false);
}

