#include "ParserNew.h"
#include <ctype.h>


Token::Token() {
	type = Unknown;
	value = nullptr;
}

void Token::Print() {
	if(type > DataMin && type < DataMax)
		printf("Data: %s\n", value);
	else if(type > OperationMin && type < OperationMax)
		printf("Operation: %s\n", value);
	else
	{
		printf("[err]: Non printable: %s\n", value);
		assert(false);
	}
}

bool Token::IsOperation(int count, ...) {
	if (count == 0)
		return (type > OperationMin && type < OperationMax);
	printf("NOT IMPLEMENTED!!!");
	assert(false);
}

bool Token::IsConstant() {
	return (type == Constant);
}

Scanner::Scanner(const char* expr) {
	pCurrent = expr;
}

Scanner::~Scanner() {
	while(!tokens.IsEmpty())
		tokens.Pop();
}


Token Scanner::CreateToken(const char* ptr, int count, Token::EType type) {
	Token token;
	token.value = new char[count+1];
	strncpy(token.value, ptr, count); // копируем count символов из строки ptr
	token.type = type;
	tokens.Push(token.value);
	return token;
}

Token Scanner::GetToken() {
	while(true)
	{
		switch(*pCurrent)
		{
			// Прокуск пробелов
			case ' ':
			case '\t':
				pCurrent++;
				continue;
			case '+':
				return CreateToken(pCurrent++, 1, Token::Addition);
			case '-':
				return CreateToken(pCurrent++, 1, Token::Subtraction);
			case '*':
				return CreateToken(pCurrent++, 1, Token::Multiplication);
			case '/':
				return CreateToken(pCurrent++, 1, Token::Division);
			case '^':
				return CreateToken(pCurrent++, 1, Token::Power);
			case '(':
				return CreateToken(pCurrent++, 1, Token::Left);
			case ')':
				return CreateToken(pCurrent++, 1, Token::Right);
			case 'x':
				return CreateToken(pCurrent++, 1, Token::Variable);
			case 'c': // may be 'cos' or 'cot'
				++pCurrent;
				if(*(pCurrent++) == 'o')
				{
					if(*(pCurrent++) == 's')
						return CreateToken("cos", 3, Token::Function);
					if(*(pCurrent++) == 't')
						return CreateToken("cot", 3, Token::Function);
				}
				return CreateToken("", 0, Token::Empty);
			case 'e': // may be 'exp'
				++pCurrent;
				if((*pCurrent++) == 'e')
					if(*(pCurrent++) == 'x')
						if(*(pCurrent++) == 'p')
							return CreateToken("exp", 3, Token::Function);
				return CreateToken("", 0, Token::Empty);
			case 'p': // may be 'pi'
				++pCurrent;
				if(*(pCurrent++) == 'i')
					return CreateToken("pi", 2, Token::Constant);
				return CreateToken("", 0, Token::Empty);
			case 's': // may be 'sin'
				++pCurrent;
				if(*(pCurrent++) == 'i')
					if(*(pCurrent++) == 'n')
						return CreateToken("sin", 3, Token::Function);
				return CreateToken("", 0, Token::Empty);
			case 'l': // may be 'log' or 'log10'
				++pCurrent;
				if(*(pCurrent++) == 'l')
					if(*(pCurrent++) == 'o')
						if(*(pCurrent++) == 'g')
						{
							if(*(pCurrent++) == '1')
							{
								if(*(pCurrent++) == '0')
									return CreateToken("log10", 5, Token::Function);
								return CreateToken("", 0, Token::Empty);
							}
							else
								return CreateToken("log", 3, Token::Function);
						}
				return CreateToken("", 0, Token::Empty);
			case 't': // may be 'tan' or 'tg'
				++pCurrent;
				if(*(pCurrent++) == 'a')
				{
					if(*(pCurrent++) == 'n')
						return CreateToken("tan", 3, Token::Function);
				}
				else if(*(pCurrent-1) == 'g')
					return CreateToken("tg", 3, Token::Function);
				return CreateToken("", 0, Token::Empty);
		}
		if(isdigit(*pCurrent)) {
			char buffNum[MAX_NUM_LENGTH];
			unsigned i;
			for(i = 0; i < MAX_NUM_LENGTH; ++i, ++pCurrent)
			{
				if(!isdigit(*pCurrent))
					break;
				buffNum[i] = *pCurrent;
			}
			if(*pCurrent == '.')
			{
				buffNum[i] = *pCurrent;
				++i, ++pCurrent;
			}
			for(; i < MAX_NUM_LENGTH; ++i, ++pCurrent)
			{
				if(!isdigit(*pCurrent))
					break;
				buffNum[i] = *pCurrent;
			}
			if(*pCurrent == 'e' || *pCurrent == 'E') // для чисел вида 1.341E+12
			{
				buffNum[i] = *pCurrent;
				++i, ++pCurrent;
				if(*pCurrent == '+' || *pCurrent == '-')
				{
					buffNum[i] = *pCurrent;
					++i, ++pCurrent;
				}
				goto parseAfterDot;
			}
			return CreateToken(buffNum, i, Token::Number);
parseAfterDot:
			for(; i < MAX_NUM_LENGTH; ++i, ++pCurrent)
			{
				if(!isdigit(*pCurrent))
					return CreateToken(buffNum, i, Token::Number);
				buffNum[i] = *pCurrent;
			}
		}
		return CreateToken("", 0, Token::Empty);
	}
}

Node::Node(Node::EType type): type(type) {}

Parser::Parser(const char* expr): Scanner(expr) {}

Node* Parser::ParseAdditive() {
	Node* pLeft = ParseMultiplicative();
	while(token.type == Token::Addition || token.type == Token::Subtraction)
	{
		Node* pAdditive = new Node(Node::Additive);
		pAdditive->token.type = token.type;
		pAdditive->token.value = token.value;
		token = GetToken();
		pAdditive->pLeft = pLeft;
		Node* pRight = ParseMultiplicative();
		pAdditive->pRight = pRight;
		nodes.Push(pAdditive);
		pLeft = pAdditive;
	}
	return pLeft;
}

Node* Parser::ParseMultiplicative() {
	Node* pLeft = ParsePower();
	while(token.type == Token::Multiplication || token.type == Token::Division)
	{
		Node* pMultiplicative = new Node(Node::Multiplicative);
		pMultiplicative->token.type = token.type;
		pMultiplicative->token.value = token.value;
		token = GetToken();
		pMultiplicative->pLeft = pLeft;
		Node* pRight = ParsePower();
		pMultiplicative->pRight = pRight;
		nodes.Push(pMultiplicative);
		pLeft = pMultiplicative;
	}
	return pLeft;
}

Node* Parser::ParsePower() {
	Node* pLeft = ParsePrimary();
	while(token.type == Token::Power)
	{
		Node* pPower = new Node(Node::Power);
		pPower->token.type = token.type;
		pPower->token.value = token.value;
		token = GetToken();
		pPower->pLeft = pLeft;
		Node* pRight = ParsePrimary();
		pPower->pRight = pRight;
		nodes.Push(pPower);
		pLeft = pPower;
	}
	return pLeft;
}

Node* Parser::ParsePrimary() {
	if(token.type == Token::Left)
	{
		token = GetToken();
		Node* pPrim = ParseAdditive();
		if(token.type != Token::Right)
		{
			printf("[err]: Parse error: ')' expected\n");
			assert(false);
		}
		token = GetToken();
		return pPrim;
	}
	if(token.type == Token::Constant || token.type == Token::Number || token.type == Token::Variable)
	{
		Node* pPrim = new Node(Node::Primary);
		pPrim->token.value = new char[strlen(token.value)+1];
		pPrim->token.value = token.value;
		pPrim->token.type = token.type;
		pPrim->pLeft = nullptr;
		pPrim->pRight = nullptr;
		nodes.Push(pPrim);
		token = GetToken();
		return pPrim;
	}
	if(token.type == Token::Function)
	{
		Node* pPrim = new Node(Node::Unary);
		pPrim->token.value = new char[strlen(token.value)+1];
		pPrim->token.value = token.value;
		pPrim->token.type = token.type;
		token = GetToken();
		Node* pLeft = ParsePrimary();
		pPrim->pLeft = pLeft;
		pPrim->pRight = nullptr;
		nodes.Push(pPrim);
		return pPrim;
	}
	return ParseAdditive();
}

Node* Parser::Parse() {
	token = GetToken();
	Node* root = ParseAdditive();
	return root;
}

void Node::Print(int depth) {
	if(type == Primary)
	{
		printf("\n%d: %s -> ", depth, token.value);
	}
	else if(type == Unary)
	{
		printf("\n%d: %s", depth, token.value);
		printf("\n%d ->", depth);
		pLeft->Print(depth+1);
	}
	else if(type == Additive || type == Multiplicative || type == Power)
	{
		printf("\n%d: %s", depth, token.value);
		printf("\n%d left ->", depth);
		pLeft->Print(depth+1);
		printf("\n%d right ->", depth);
		pRight->Print(depth+1);
	}
	else
	{
		printf("[err]: Unknown node: %s\n", token.value);
		assert(false);
	}
}

Node* Differentiation(Node* node) {
	if(node->type == Node::Primary)
	{
		Node* result = new Node(Node::Primary);
		result->right = null;
		result->token.value = new char[2];
		if(node->token.type == Token::Number)
			strcpy(result->token.value, "0");
		else if(node->token.type == Token::Variable)
			strcpy(result->token.value, "1"); // КОСТЫЛЬ!!!
		else if(node->token.type == Token::Constant)
			strcpy(result->token.value, "0");
		nodes.Push(result);
		result->left = null;
		return result;
	}
	else if(node->type == Node::Unary) // IMPLEMENT!!!
	{/*
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
			return myAcot(Calculate(node->pLeft, firstVar));*/
		printf("[err]: Function not implemented: %s\n", node->token.value);
		assert(false); // Не реализовано
	}
	else if(node->type == Node::Additive || node->type == Node::Multiplicative || node->type == Node::Power)
	{
		if(node->token.type == Token::Addition) // (u+v)' = u' + v'
		{
			Node* result = new Node(Node::Additive);
			nodes.Push(result);
			result->token.value = new char[2];
			result->token.type = Token::Addition;
			strcpy(result->token.value, "+");
			result->left = Differentiation(node->left);
			result->right = Differentiation(node->right);
			return result;
		}
		if(node->token.type == Token::Subtraction) // (u-v)' = u' - v' 
		{
			Node* result = new Node(Node::Additive);
			nodes.Push(result);
			result->token.value = new char[2];
			result->token.type = Token::Subtraction;
			strcpy(result->token.value, "-");
			result->left = Differentiation(node->left);
			result->right = Differentiation(node->right);
			return result;
		}
		if(node->token.type == Token::Multiplication) // (u*v)' = u'*v + v'*u
		{
			Node* result = new Node(Node::Additive);
			nodes.Push(result);
			result->token.value = new char[2];
			result->token.type = Token::Addition;
			strcpy(result->token.value, "+"); // u'*v + v'*u
			result->left = new Node(Node::Multiplicative);
			nodes.Push(result->left);
			result->left->token.value = new char[2];
			result->left->token.type = Token::Multiplication;
			strcpy(result->token.value, "*"); // u'*v
			result->left->left = Differentiation(node->left); // u'
			result->left->right = new Node(Node::Expression); // v
			nodes.Push(result->left->right);
			result->left->right->token.type = node->right.token.type;
			result->left->right->token.value = node->right.token.value;
			result->left->right->left = node->right->left;
			result->left->right->right = node->right->right;
			result->right = new Node(Node::Multiplicative);
			nodes.Push(result->right);
			result->right->token.value = new char[2];
			result->right->token.type = Token::Multiplication;
			strcpy(result->right->token.value, "*"); // v'*u
			result->right->left = Differentiation(node->right); // v'
			result->right->right = new Node(Node::Expression); // u
			nodes.Push(result->right->right);
			result->right->right->token.type = node->left.token.type;
			result->right->right->token.value = node->left.token.value;
			result->right->right->left = node->left->left;
			result->right->right->right = node->left->right;
			return result;
		}
		if(node->token.type == Token::Division) // (u/v)' = (u'*v - v'*u)/(v^2)
		{
			Node* result = new Node(Node::Multiplicative);
			nodes.Push(result);
			result->token.value = new char[2];
			result->token.type = Token::Addition;
			strcpy(result->token.value, "/"); // (u'*v - v'*u)/(v^2)
			result->left = new Node(Node::Additive);
			result->left->token.value = new char[2];
			result->left->token.type = Token::Subtraction;
			strcpy(result->left->token.value, "-"); // u'*v - v'*u
			result->left->left = new Node(Node::Multiplicative);
			nodes.Push(result->left->left);
			result->left->left->token.value = new char[2];
			result->left->left->token.type = Token::Multiplication;
			strcpy(result->left->token.value, "*"); // u'*v
			result->left->left->left = Differentiation(node->left); // u'
			result->left->left->right = new Node(Node::Expression); // v
			nodes.Push(result->left->left->right);
			result->left->left->right->token.type = node->right.token.type;
			result->left->left->right->token.value = node->right.token.value;
			result->left->left->right->left = node->right->left;
			result->left->left->right->right = node->right->right;
			result->left->right = new Node(Node::Multiplicative);
			nodes.Push(result->left->right);
			result->left->right->token.value = new char[2];
			result->left->right->token.type = Token::Multiplication;
			strcpy(result->left->right->token.value, "*"); // v'*u
			result->left->right->left = Differentiation(node->right); // v'
			result->left->right->right = new Node(Node::Expression); // u
			nodes.Push(result->left->right->right);
			result->left->right->right->token.type = node->left.token.type;
			result->left->right->right->token.value = node->left.token.value;
			result->left->right->right->left = node->left->left;
			result->left->right->right->right = node->left->right;
			result->right = new Node(Node::Power); // v^2
			nodes.Push(result->right);
			result->right->token->value = new char[2];
			result->right->token->type = Token::Power;
			strcpy(result->right->token->value, "^");
			result->right->left = new Node(Node::Expression); // v
			nodes.Push(result->right->left);
			result->right->left->token.type = node->right.token.type;
			result->right->left->token.value = node->right.token.value;
			result->right->left->left = node->right->left;
			result->right->left->right = node->right->right;
			result->right->right = new Node(Node::Primary); // 2
			nodes.Push(result-right->right);
			result->right->right->token.value = new char[2];
			result->right->right->token.type = Token::Number;
			strcpy(result->right->right->token.value, "2");
			return result;
		}
		else 
		{
			printf("[err]: Operation not implemented: %s\n", node->token.value);
			assert(false); // Не реализовано
		}
		return result;
	}
	printf("[err]: Unknown node: %s\n", node->token.value);
	assert(false); // Не реализовано
}

T Calculate(Node* node, T firstVar) {
	if(node->type == Node::Primary)
	{
		if(node->token.type == Token::Number)
			return atoi(node->token.value);
		if(node->token.type == Token::Variable)
			return firstVar;
		if(node->token.type == Token::Constant)
		{
			printf("[err]: Constant not implemented: %s\n", node->token.value);
			assert(false); // Не реализовано
		}
	}
	else if(node->type == Node::Unary)
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
	else if(node->type == Node::Additive || node->type == Node::Multiplicative || node->type == Node::Power)
	{
		if(node->token.type == Token::Addition)
			return Calculate(node->pLeft, firstVar) + Calculate(node->pRight, firstVar);
		if(node->token.type == Token::Subtraction)
			return Calculate(node->pLeft, firstVar) - Calculate(node->pRight, firstVar);
		if(node->token.type == Token::Multiplication)
			return Calculate(node->pLeft, firstVar) * Calculate(node->pRight, firstVar);
		if(node->token.type == Token::Division)
			return Calculate(node->pLeft, firstVar) / Calculate(node->pRight, firstVar);
		if(node->token.type == Token::Power)
			return myPow(Calculate(node->pLeft, firstVar),  Calculate(node->pRight, firstVar));
		printf("[err]: Operation not implemented: %s\n", node->token.value);
		assert(false); // Не реализовано
	}
	printf("[err]: Unknown node: %s\n", node->token.value);
	assert(false);
}

void CalculatePrint(Node* node, T firstVar) {
	if(node->type == Node::Primary)
	{
		printf(" %s ", node->token.value);
		return;
	}
	else if(node->type == Node::Unary)
	{
		printf(" %s ", node->token.value);
		printf("[");
		CalculatePrint(node->pLeft, firstVar);
		printf("]");
		return;
	}
	else if(node->type == Node::Additive || node->type == Node::Multiplicative || node->type == Node::Power)
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
