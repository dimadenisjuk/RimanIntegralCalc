#ifndef STACK_H
#define STACK_H
#pragma once
#include <assert.h>

template <typename T1_s>
class Stack {
	struct Entry {//Структура элементов
		T1_s value; //Значение,помещаемое в стек
		Entry* pPrev;//Указатель на следующий элемент
	};
	Entry* pTop;//Указатель на верх стека
	unsigned nCount;//Cчетчик числа элементов стека
public:
	Stack(){
		pTop = nullptr;//Указатель на хвост и голову стека
		nCount = 0;
	}

	~Stack() {
		while (nCount)
			Pop();
	}

	// Метод вталкивания значения в стек
	bool Push(T1_s value) {
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
	T1_s Pop() {
		assert(nCount);

		//Временно сохраняем значение из головного элемента
		Entry* pe = pTop;
		T1_s value = pe->value;

		//Исключаем головной элемент из стека
		pTop = pe->pPrev;
		nCount--;
		//Удаление головного элемента
		free(pe);

		return(value);
	}

	inline bool IsEmpty() {
		return !nCount;
	}
};

#endif //STACK_H

