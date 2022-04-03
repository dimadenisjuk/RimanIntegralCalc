#ifndef PARSERNEW_H
#define PARSERNEW_H

#define MAX_NUM_LENGTH 40

#include "Stack.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

class Token {
public:
	enum EType {
		Unknown, // Для начальной инициализаци
		DataMin,
			Constant, // Константа (предустановленная)
			Number, // Константа (заданная пользователем)
			Variable, // Переменная
		DataMax,
		OperationMin,
			Addition,
			Subtraction,
			Multiplication,
			Division,
			Power,
			Left,
			Right,
			Function,
		OperationMax,
		Empty // Последняя лексема в лексической свёртке
	} type;
	char* value;
	Token();
	void Print();
	bool IsOperation(int count, ...);
	bool IsConstant();
};

class Scanner {
	const char* pCurrent;
	Stack<char*> tokens;
public:
	Scanner(const char* expr);

~Scanner();
	Token CreateToken(const char* ptr, int count, Token::EType type); // %.*s, count, ptr
	Token GetToken();
};

class Node {
public:
	enum EType {
		Expression,
		Additive,
		Multiplicative,
		Power,
		Primary
	} type;
	struct {
		struct {
			Node *pLeft, *pRight; // Дочерние узлы
		};
		Token token; // Операция над ними
	} data;
	Node(EType type);
};

class Parser : Scanner {
	Stack <Node*> nodes;
	Token token;
	
};

#endif //PARSERNEW_H
