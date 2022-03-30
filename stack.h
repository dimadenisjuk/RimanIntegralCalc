#pragma once

typedef int T;


struct Stack {
	struct Entry {//Структура элементов
		T value; //Значение,помещаемое в стек
		Entry* pPrev;//Указатель на следующий элемент
	};
	Entry* pTop;//Указатель на верх стека
	unsigned nCount;//Cчетчик числа элементов стека
	
	public:

	Stack();

	~Stack();

	bool Push(T value);

	T Pop();

	inline bool IsEmpty() {
		return !nCount;
	}
};

