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
			case 'c': // may be 'cos'
				++pCurrent;
				if(*(pCurrent++) == 'o')
					if(*(pCurrent++) == 's')
						return CreateToken(pCurrent-3, 3, Token::Function);
				return CreateToken("", 0, Token::Empty);
			case 'p': //may be 'pi'
				++pCurrent;
				if(*(pCurrent++) == 'i')
					return CreateToken(pCurrent-2, 2, Token::Constant);
				return CreateToken("", 0, Token::Empty);
			case 's': // may be 'sin'
				++pCurrent;
				if(*(pCurrent++) == 'i')
					if(*(pCurrent++) == 'n')
						return CreateToken(pCurrent-3, 3, Token::Function);
				return CreateToken("", 0, Token::Empty);
			case 't': // may be 'tan' or 'tg'
				++pCurrent;
				if(*(pCurrent++) == 'a')
				{
					if(*(pCurrent++) == 'n')
						return CreateToken(pCurrent-3, 3, Token::Function);
				}
				else if(*(pCurrent-1) == 'g')
					return CreateToken(pCurrent-3, 3, Token::Function);
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
		token = GetToken();
		return pPrim;
	}
	return ParseAdditive();
}

Node* Parser::Parse() {
	token = GetToken();
	Node* root = ParseAdditive();
	return root;
}

int main() {
	const char* str = "(x+2)*sin(x^22.1E-1)";
	Scanner scn(str);
	//while(true)
	//	scn.GetToken().Print();
	Parser prs(str);
	prs.Parse();
	return 0;
}
