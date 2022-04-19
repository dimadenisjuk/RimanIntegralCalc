#ifndef PARSERNEW_H
#define PARSERNEW_H

#define MAX_NUM_LENGTH 40

#include "Mathfunctions.h"
#include "Stack.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

enum ETokenType {
	TokenUnknown, // Для начальной инициализаци
	TokenDataMin,
		TokenConstant, // Константа (предустановленная)
		TokenNumber, // Константа (заданная пользователем)
		TokenVariable, // Переменная
	TokenDataMax,
	TokenOperationMin,
		TokenAddition,
		TokenSubtraction,
		TokenMultiplication,
		TokenDivision,
		TokenPower,
		TokenLeft,
		TokenRight,
		TokenFunction,
	TokenOperationMax,
	TokenEmpty // Последняя лексема в лексической свёртке
};

enum ENodeType {
	NodeExpression,
	NodeAdditive, // Выражение вида "a (+|-) b"
	NodeMultiplicative, // Выражение вида "a (*|/) b"
	NodePower, // Выражение вида "a ^ b"
	NodeUnary, // Унарное выражение вида "sin a"
	NodePrimary // Конечное выражение вида "a"
};

typedef struct {
	char* value;
	enum ETokenType type;
} Token;
void InitToken(Token* token);
void PrintToken(Token* token);
bool TokenIsOperation(Token* token, int count, ...);
bool TokenIsConstant(Token* token);

typedef struct {
	const char* pCurrent;
	Stack tokens;
} Scanner;

void CreateScanner(Scanner* scn, const char* expr);
void DeleteScanner(Scanner* scn);
Token CreateToken(Scanner* scn, const char* ptr, int count, enum ETokenType type); // %.*s, count, ptr
Token GetToken(Scanner* scn);

typedef struct {
	void *pLeft, *pRight; // Дочерние узлы
	Token token; // Операция над ними
	enum ENodeType type;
} Node;

Node* CreateNode(enum ENodeType type);
void PrintNode(Node* node, int depth);

typedef struct { // "Наследуется" от Scanner
	const char* pCurrent;
	Stack tokens;
	Stack nodes;
	Token token;
	Node* root;
} Parser;

Node* ParseAdditive(Parser* prs);
Node* ParseMultiplicative(Parser* prs);
Node* ParsePower(Parser* prs);
Node* ParsePrimary(Parser* prs);
Node* Parse(Parser* prs);
void CreateParser(Parser* prs, const char* source);
void DeleteParser(Parser* prs);

void DeleteChild(Node* node);
T Calculate(Node* node, T firstVar); // Подсчёт значения выражения
void CalculatePrint(Node* node, T firstVar); // Печать вычисляемого выражения
void PrintTree(Node* node, T firstVar, int depth); // Печать вычисляемого выражения в виде дерева

#endif //PARSERNEW_H
