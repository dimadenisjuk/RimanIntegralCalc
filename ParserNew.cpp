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

Node::Node(Node::EType type, Node* pLeft, Node* pRight, char* tokenValue): type(type), pLeft(pLeft), pRight(pRight) {
	token.value = tokenValue;
}

Parser::Parser(const char* expr): Scanner(expr) {}

Node* Parser::CreateNode(Node::EType type, Node* pLeft, Node* pRight, char* tokenValue) // Создаёт узел и помещает его в стек
{
	Node* result = new Node(type, pLeft, pRight, tokenValue);
	nodes.Push(result);
	return result;
}

Node* Parser::ParseAdditive() {
	Node* pLeft = ParseMultiplicative();
	while(token.type == Token::Addition || token.type == Token::Subtraction)
	{
		Node* pAdditive = CreateNode(Node::Additive, nullptr, nullptr, nullptr);
		pAdditive->token.type = token.type;
		pAdditive->token.value = token.value;
		token = GetToken();
		pAdditive->pLeft = pLeft;
		Node* pRight = ParseMultiplicative();
		pAdditive->pRight = pRight;
		pLeft = pAdditive;
	}
	return pLeft;
}

Node* Parser::ParseMultiplicative() {
	Node* pLeft = ParsePower();
	while(token.type == Token::Multiplication || token.type == Token::Division)
	{
		Node* pMultiplicative = CreateNode(Node::Multiplicative, nullptr, nullptr, nullptr);
		pMultiplicative->token.type = token.type;
		pMultiplicative->token.value = token.value;
		token = GetToken();
		pMultiplicative->pLeft = pLeft;
		Node* pRight = ParsePower();
		pMultiplicative->pRight = pRight;
		pLeft = pMultiplicative;
	}
	return pLeft;
}

Node* Parser::ParsePower() {
	Node* pLeft = ParsePrimary();
	while(token.type == Token::Power)
	{
		Node* pPower = CreateNode(Node::Power, nullptr, nullptr, nullptr);
		pPower->token.type = token.type;
		pPower->token.value = token.value;
		token = GetToken();
		pPower->pLeft = pLeft;
		Node* pRight = ParsePrimary();
		pPower->pRight = pRight;
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
		Node* pPrim = CreateNode(Node::Primary, nullptr, nullptr, new char[strlen(token.value)+1]);
		pPrim->token.value = token.value;
		pPrim->token.type = token.type;
		pPrim->pLeft = nullptr;
		pPrim->pRight = nullptr;
		token = GetToken();
		return pPrim;
	}
	if(token.type == Token::Function)
	{
		Node* pPrim = CreateNode(Node::Unary, nullptr, nullptr, new char[strlen(token.value)+1]);
		pPrim->token.value = token.value;
		pPrim->token.type = token.type;
		token = GetToken();
		Node* pLeft = ParsePrimary();
		pPrim->pLeft = pLeft;
		pPrim->pRight = nullptr;
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

Node* Parser::Differentiation(Node* node) {
	if(node->type == Node::Primary)
	{
		Node* result = CreateNode(Node::Primary, nullptr, nullptr, new char[2]);
		if(node->token.type == Token::Number)
			strcpy(result->token.value, "0");
		else if(node->token.type == Token::Variable)
			strcpy(result->token.value, "1"); // КОСТЫЛЬ!!!
		else if(node->token.type == Token::Constant)
			strcpy(result->token.value, "0");
		return result;
	}
	else if(node->type == Node::Unary) // IMPLEMENT!!!
	{
		if(!strcmp(node->token.value, "sin")) // (sin(u))' = cos(u)*u'
		{
			Node* result = CreateNode(Node::Multiplicative, nullptr, nullptr, new char[2]); // cos(u)*u'
			result->token.type = Token::Multiplication;
			strcpy(result->token.value, "*");
			result->pLeft = CreateNode(Node::Unary, nullptr, nullptr, new char[4]); // cos(u)
			result->pLeft->token.type = Token::Function;
			strcpy(result->pLeft->token.value, "cos");
			result->pLeft->pLeft = CreateNode(Node::Expression, nullptr, nullptr, nullptr); // u
			result->pLeft->pLeft->type = node->pLeft->type;
			result->pLeft->pLeft->token.type = node->pLeft->token.type;
			result->pLeft->pLeft->token.value = node->pLeft->token.value;
			result->pLeft->pLeft->pLeft = node->pLeft->pLeft;
			result->pLeft->pLeft->pRight = node->pLeft->pRight;
			result->pLeft->pRight = nullptr;
			result->pRight = Differentiation(node->pLeft); // u'
			return result;
		}
		/*if(!strcmp(node->token.value, "cos"))
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
			Node* result = CreateNode(Node::Additive, nullptr, nullptr, new char[2]); // u' + v'
			result->token.type = Token::Addition;
			strcpy(result->token.value, "+");
			result->pLeft = Differentiation(node->pLeft);
			result->pRight = Differentiation(node->pRight);
			return result;
		}
		if(node->token.type == Token::Subtraction) // (u-v)' = u' - v' 
		{
			Node* result = CreateNode(Node::Additive, nullptr, nullptr, new char[2]); // u' - v'
			result->token.type = Token::Subtraction;
			strcpy(result->token.value, "-");
			result->pLeft = Differentiation(node->pLeft);
			result->pRight = Differentiation(node->pRight);
			return result;
		}
		if(node->token.type == Token::Multiplication) // (u*v)' = u'*v + v'*u
		{
			Node* result = CreateNode(Node::Additive, nullptr, nullptr, new char[2]);
			result->token.type = Token::Addition;
			strcpy(result->token.value, "+"); // u'*v + v'*u
			result->pLeft = new Node(Node::Multiplicative);
			nodes.Push(result->pLeft);
			result->pLeft->token.value = new char[2];
			result->pLeft->token.type = Token::Multiplication;
			strcpy(result->pLeft->token.value, "*"); // u'*v
			result->pLeft->pLeft = Differentiation(node->pLeft); // u'
			result->pLeft->pRight = new Node(Node::Expression); // v
			nodes.Push(result->pLeft->pRight);
			result->pLeft->pRight->type = node->pRight->type;
			result->pLeft->pRight->token.type = node->pRight->token.type;
			result->pLeft->pRight->token.value = node->pRight->token.value;
			result->pLeft->pRight->pLeft = node->pRight->pLeft;
			result->pLeft->pRight->pRight = node->pRight->pRight;
			result->pRight = new Node(Node::Multiplicative);
			nodes.Push(result->pRight);
			result->pRight->token.value = new char[2];
			result->pRight->token.type = Token::Multiplication;
			strcpy(result->pRight->token.value, "*"); // v'*u
			result->pRight->pLeft = Differentiation(node->pRight); // v'
			result->pRight->pRight = new Node(Node::Expression); // u
			nodes.Push(result->pRight->pRight);
			result->pRight->pRight->type = node->pLeft->type;
			result->pRight->pRight->token.type = node->pLeft->token.type;
			result->pRight->pRight->token.value = node->pLeft->token.value;
			result->pRight->pRight->pLeft = node->pLeft->pLeft;
			result->pRight->pRight->pRight = node->pLeft->pRight;
			return result;
		}
		if(node->token.type == Token::Division) // (u/v)' = (u'*v - v'*u)/(v^2)
		{
			Node* result = CreateNode(Node::Multiplicative, nullptr, nullptr, new char[2]);
			result->token.type = Token::Addition;
			strcpy(result->token.value, "/"); // (u'*v - v'*u)/(v^2)
			result->pLeft = CreateNode(Node::Additive, nullptr, nullptr, new char[2]);
			result->pLeft->token.type = Token::Subtraction;
			strcpy(result->pLeft->token.value, "-"); // u'*v - v'*u
			result->pLeft->pLeft = CreateNode(Node::Multiplicative, nullptr, nullptr, new char[2]);
			result->pLeft->pLeft->token.type = Token::Multiplication;
			strcpy(result->pLeft->pLeft->token.value, "*"); // u'*v
			result->pLeft->pLeft->pLeft = Differentiation(node->pLeft); // u'
			result->pLeft->pLeft->pRight = CreateNode(Node::Expression, nullptr, nullptr, nullptr); // v
			result->pLeft->pLeft->pRight->type = node->pRight->type;
			result->pLeft->pLeft->pRight->token.type = node->pRight->token.type;
			result->pLeft->pLeft->pRight->token.value = node->pRight->token.value;
			result->pLeft->pLeft->pRight->pLeft = node->pRight->pLeft;
			result->pLeft->pLeft->pRight->pRight = node->pRight->pRight;
			result->pLeft->pRight = CreateNode(Node::Multiplicative, nullptr, nullptr, new char[2]);
			result->pLeft->pRight->token.type = Token::Multiplication;
			strcpy(result->pLeft->pRight->token.value, "*"); // v'*u
			result->pLeft->pRight->pLeft = Differentiation(node->pRight); // v'
			result->pLeft->pRight->pRight = CreateNode(Node::Expression, nullptr, nullptr, nullptr); // u
			result->pLeft->pRight->pRight->type = node->pLeft->type;
			result->pLeft->pRight->pRight->token.type = node->pLeft->token.type;
			result->pLeft->pRight->pRight->token.value = node->pLeft->token.value;
			result->pLeft->pRight->pRight->pLeft = node->pLeft->pLeft;
			result->pLeft->pRight->pRight->pRight = node->pLeft->pRight;
			result->pRight = CreateNode(Node::Power, nullptr, nullptr, new char[2]); // v^2
			result->pRight->token.type = Token::Power;
			strcpy(result->pRight->token.value, "^");
			result->pRight->pLeft = CreateNode(Node::Expression, nullptr, nullptr, nullptr); // v
			nodes.Push(result->pRight->pLeft);
			result->pRight->pLeft->type = node->pRight->type;
			result->pRight->pLeft->token.type = node->pRight->token.type;
			result->pRight->pLeft->token.value = node->pRight->token.value;
			result->pRight->pLeft->pLeft = node->pRight->pLeft;
			result->pRight->pLeft->pRight = node->pRight->pRight;
			result->pRight->pRight = CreateNode(Node::Primary, nullptr, nullptr, new char[2]); // 2
			result->pRight->pRight->token.type = Token::Number;
			strcpy(result->pRight->pRight->token.value, "2");
			return result;
		}
		else 
		{
			printf("[err]: Operation not implemented: %s\n", node->token.value);
			assert(false); // Не реализовано
		}
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
