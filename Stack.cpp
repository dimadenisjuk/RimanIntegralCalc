#include "stack.h"
#include<assert.h>
#include<stdlib.h>

Stack::Stack(){
	pTop = nullptr;//��������� �� ����� � ������ �����
	nCount = 0;
}

Stack::~Stack() {
	while (nCount)
		Pop();
}

// ����� ����������� �������� � ����
bool Stack::Push(T value) {
	//�������� ������ �������� � ������
	Entry* pe = (Entry*)calloc(sizeof(Entry), 1);
	if (!pe)
		return false;
//������������� ������ ��������
	pe-> value = value;
	pe-> pPrev = nullptr;
//���������� ������ �������� � ����
	pe-> pPrev = pTop;	
	pTop = pe;
	nCount++;
	return true;
}

//����� ������������ �������� �� ����
T Stack::Pop() {
	assert(nCount);

//�������� ��������� �������� �� ��������� ��������
	Entry* pe = pTop;
	T value = pe->value;

//��������� �������� ������� �� �����
	pTop = pe->pPrev;
	nCount--;
//�������� ��������� ��������
	free(pe);

	return(value);
}
