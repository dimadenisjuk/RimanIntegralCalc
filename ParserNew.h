#ifndef PARSERNEW_H
#define PARSERNEW_H

#define MAX_NUM_LENGTH 40

#include "Mathfunctions.h"
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
		Additive, // Выражение вида "a (+|-) b"
		Multiplicative, // Выражение вида "a (*|/) b"
		Power, // Выражение вида "a ^ b"
		Unary, // Унарное выражение вида "sin a"
		Primary // Конечное выражение вида "a"
	} type;
	Node *pLeft, *pRight; // Дочерние узлы
	Token token; // Операция над ними
	Node(EType type);
	void Print(int depth = 0);
};

class Parser : Scanner {
	Stack <Node*> nodes;
	Token token;
	Node* ParseAdditive();
	Node* ParseMultiplicative();
	Node* ParsePower();
	Node* ParsePrimary();
	public:
	Parser(const char* source);
	Node* Parse();
};

T Calculate(Node* node, T firstVar); // Подсчёт значения выражения
void CalculatePrint(Node* node, T firstVar); // Подсчёт значения выражения

#endif //PARSERNEW_H
