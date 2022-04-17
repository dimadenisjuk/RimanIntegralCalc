#include "Stack.h"

void CreateStack(Stack* pSt){
	pSt->pTop = NULL;//Указатель вершину стека
	pSt->nCount = 0;
}

void DeleteStack(Stack* pSt) {
	while (pSt->nCount)
		PopStack(pSt);
}

// Метод вталкивания значения в стек
bool PushStack(Stack* pSt, S_T value) {
	//Создание нового элемента в памяти
	Entry* pe = (Entry*)calloc(sizeof(Entry), 1);
	if (!pe)
		return false;
//Инициализация нового элемента
	pe-> value = value;
	pe-> pPrev = NULL;
//Добавление нового элемента в стек
	pe-> pPrev = pSt->pTop;	
	pSt-> pTop = pe;
	pSt-> nCount++;
	return true;
}

//Метод выталкивания значения из стека
S_T PopStack(Stack* pSt) {
	assert(pSt->nCount);

//Временно сохраняем значение из головного элемента
	Entry* pe = pSt->pTop;
	S_T value = pe->value;

//Исключаем головной элемент из стека
	pSt->pTop = pe->pPrev;
	pSt->nCount--;
//Удаление головного элемента
	free(pe);

	return(value);
}
