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
		bool dotUsed = false, eUsed = false;
		for(i = 0; i < MAX_NUM_LENGTH; ++i, ++pCurrent)
		{
			if(!isdigit(*pCurrent))
			{
				if((*pCurrent == 'e' || *pCurrent == 'E') && !eUsed) // для чисел вида 1.341E+12
				{
					eUsed = true;
					buffNum[i] = *pCurrent;
					++i, ++pCurrent;
					if(*pCurrent == '+' || *pCurrent == '-')
					{
						buffNum[i] = *pCurrent;
						continue;
					}
					--i, --pCurrent;
					continue;
				}
				else if(*pCurrent == '.' && !dotUsed && !eUsed)
				{
					dotUsed = true;
					buffNum[i] = *pCurrent;
					continue;
				}
			return CreateToken(buffNum, i, Token::Number);
			}
			buffNum[i] = *pCurrent;
		}
	}
	return CreateToken("", 0, Token::Empty);
}

int main() {
	const char* str = "(x+2)*sin(x^22.1E-1)";
	Scanner scn(str);
	while(true)
		scn.GetToken().Print();
	return 0;
}
