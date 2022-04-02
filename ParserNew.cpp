#include "ParserNew.h"

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
		assert(false);
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
	return CreateToken("", 0, Token::Empty);
}
