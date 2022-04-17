#ifndef STACK_H
#define STACK_H

#include <assert.h>
#include <stdlib.h>
#include "Definitions.h"

typedef void* S_T;

typedef struct {//Структура элементов
	S_T value; //Значение,помещаемое в стек
	void* pPrev;//Указатель на следующий элемент
} Entry;

typedef struct {
	Entry* pTop;//Указатель на хвост и голову стека
	unsigned nCount;//Cчетчик числа элементов стека
} Stack;

void CreateStack(Stack* pSt);

void DeleteStack(Stack* pSt);

bool PushStack(Stack* pSt, S_T value);

S_T PopStack(Stack* pSt);

#endif //STACK_H
