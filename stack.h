#pragma once

typedef int T;


struct Stack {
	struct Entry {//��������� ���������
		T value; //��������,���������� � ����
		Entry* pPrev;//��������� �� ��������� �������
	};
	Entry* pTop;//��������� �� ���� �����
	unsigned nCount;//C������ ����� ��������� �����
	
	public:

	Stack();

	~Stack();

	bool Push(T value);

	T Pop();

	inline bool IsEmpty() {
		return !nCount;
	}
};

