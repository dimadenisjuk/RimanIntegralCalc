#include "Stack.h"
#include<assert.h>
#include<stdlib.h>

Stack::Stack(){
	pTop = nullptr;//Указатель на хвост и голову стека
	nCount = 0;
}

Stack::~Stack() {
	while (nCount)
		Pop();
}

// Метод вталкивания значения в стек
bool Stack::Push(T value) {
	//Создание нового элемента в памяти
	Entry* pe = (Entry*)calloc(sizeof(Entry), 1);
	if (!pe)
		return false;
//Инициализация нового элемента
	pe-> value = value;
	pe-> pPrev = nullptr;
//Добавление нового элемента в стек
	pe-> pPrev = pTop;	
	pTop = pe;
	nCount++;
	return true;
}

//Метод выталкивания значения из стек
T Stack::Pop() {
	assert(nCount);

//Временно сохраняем значение из головного элемента
	Entry* pe = pTop;
	T value = pe->value;

//Исключаем головной элемент из стека
	pTop = pe->pPrev;
	nCount--;
//Удаление головного элемента
	free(pe);

	return(value);
}
