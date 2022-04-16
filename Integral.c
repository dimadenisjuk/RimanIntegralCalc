#include"Integral.h"
#include "Mathfunctions.h"

IntegCreate(Integ* integ) {
	integ->_a = 0.0 + 0.0 * I;
	integ->_b = 0.0 + 0.0 * I;
	integ->_func = (char*)calloc(MAX_INPUT_LENGTH, sizeof(char)); // выделяет динамическую память и инициализирует нулями
}

IntegCreate(Integ* integ, T a, T b) {
	integ->_a = a;
	integ->_b = b;
	integ->_func = (char*)calloc(MAX_INPUT_LENGTH, sizeof(char)); // выделяет динамическую память и инициализирует нулями
}

IntegDelete(Integ* integ) {
	free(integ->_func);
}

void SetFunc(Integ* integ, const char* func) {
	strcpy(integ->_func, func);
}



//PrintIntegral and InputIntegral
/*
ostream& operator <<(ostream& stream, const Integ& ob1) {
	stream << "интегрируемая функция: " << ob1._func << " начало отрезка:  " << ob1._a << " конец отрезка: " << ob1._b << endl;
	return stream;
}
istream& operator >>(istream& stream, Integ& ob1) {
	stream >> ob1._func >> ob1._a >> ob1._b;
	return stream;
}
*/

/*#pragma intrinsic(log)
#pragma intrinsic(log10)
#pragma intrinsic(tan)
#pragma intrinsic(atan)
#pragma intrinsic(sin)
#pragma intrinsic(cos)
#pragma intrinsic(tan)
#pragma intrinsic(acos)
#pragma intrinsic(asin)
#pragma intrinsic(atan)
#pragma intrinsic(exp)
#pragma intrinsic(tanh)
#pragma intrinsic(pow)*/



inline void ECfree(char* pointer) {
	if (pointer != nullptr)
		free(pointer);
}

// разбираем строку формата:  функция(коэффицент*x+константа) -- нового образца

T IntegParseExprOld(Integ* integ, const char* expr) {
	const static char* sep1 = "+-";
	const static char* sep2 = "*/";
	const static char* sep3 = "^";
	const static char* sepEnd = "";
	char* left, * right;
	char* expr2 = strdup(expr);
	char* posAdd = strchr(expr2, '+');
	char* posSub = strchr(expr2, '-');
	char* posMul = strchr(expr2, '*');
	char* posDiv = strchr(expr2, '/');
	char* posPow = strchr(expr2, '^');
stage1:
	if (posAdd == NULL && posSub == NULL)
		goto stage2;
	left = strtok(expr2, sep1);
	right = strtok(NULL, sepEnd);
	T(*op)(const T num, const T x);
	if ((posAdd < posSub || posSub == NULL) && posAdd != NULL)
	{
		op = myAdd;
		goto doOperation;
	}
	else if (posSub != NULL)
	{
		op = mySub;
		goto doOperation;
	}
stage2:
	if (posMul == NULL && posDiv == NULL)
		goto stage3;
	expr2 = strdup(expr);
	left = strtok(expr2, sep2);
	right = strtok(NULL, sepEnd);
	if ((posMul < posDiv || posDiv == NULL) && posMul != NULL)
	{
		op = myMul;
		goto doOperation;
	}
	else if (posDiv != NULL)
	{
		op = myDiv;
		goto doOperation;
	}
stage3:
	if (posPow == NULL)
		goto stage4;
	expr2 = strdup(expr);
	left = strtok(expr2, sep3);
	right = strtok(NULL, sepEnd);
	// повторная проверка не нужна
	op = myPow;
	goto doOperation;
stage4:
	ECfree(expr2);

	if (expr[0] == 'x')
		return E * integ->i + integ->_a;
	return atof(expr);

doOperation:
	char leftS[100];
	char rightS[100];
	strcpy(leftS, left);
	strcpy(rightS, right);

	ECfree(expr2);
	return op(IntegParseExprOld(integ, leftS), IntegParseExprOld(integ, rightS));
}

T IntegResultNew(Integ* integ, int* progress, long long presition) {
	T absS;
	absS = abs(integ->_b - integ->_a);
	T Square = 0 + 0*I;
	i = 1;
	T n = presition + 3;
	E = absS / n;
	
	T value;
	
//	Parser prs(integ->_func);
//	Node* root = prs.Parse(); //Получаем корневой узел дерева вычислений
	
	printf("%s\n", Strings[IStrUnderstoodLike].M_LANG);
	CalculatePrint(root, 0);  //Вывод разобранного выражения
	printf("\n");

	bool complexResult = false;
	ofstream outFile;
	// заполняем файл с данными для GNUPlot
	outFile.open("graph", ios::trunc);
	double progressExact = 0;
	while (creal(i) < creal(n))
	{
//		value = Calculate(root, E*i + integ->_a);
		Square += E * value;
		if(cimag(value)!=0)
			complexResult = true;
		if((int)creal(i) % 500 == 0)
			outFile << creal(E*i + integ->_a) << " " << cimag(value) << " " << creal(value) << endl;
		i += 1 + 0*I;
		progressExact = 100 * creal(i) / (creal(n) - 3);
		if(((int)(progressExact) % 5 == 0) && ((int)(progressExact) == progressExact))
		{
			printf("\r%s %d%%", Strings[IStrCalculating].M_LANG, (int)progressExact);
			cout.flush(); // мгновенная печать потока cout
			*progress = (int)(progressExact);
		}
	}
	outFile.close();
	// заполняем файл с командами для GNUPlot
	outFile.open("graphP", ios::trunc);
	if(complexResult)
		outFile << "set grid" << endl << "set title \"" << integ->_func << "\"" << endl << "splot \"graph\" with impulses" << endl;
	else
		outFile << "set grid" << endl << "set title \"" << integ->_func << "\"" << endl << "plot \"graph\" using 1:3 with boxes" << endl;
	outFile.close();
	return Square;
}

// разбираем строку формата:  функция(коэффицент*x+константа)
T IntegResultOld(Integ* integ, int* progress, long long presition) {
	T absS;
	absS = abs(integ->_b - integ->_a);
	T Square = 0 + 0*I;
	integ->i = 1;
	T n = presition + 3;
	integ->E = absS / n;

	const static struct {
		const char* opcode;
		T(*pfUnaryOperation)(const T x);
	} functions[] = {
		{"log", myLog},
		{"log10", myLog10},
		{"cos", myCos},
		{"sin", mySin},
		{"tan", myTan},
		{"tg", myTan},
		{"ctg", myCot},
		{"acos", myAcos},
		{"asin", myAsin},
		{"atan", myAtan},
		{"acot", myAcot},
		{"tanh", myTanh},
		{"exp", myExp},
		{"const", myConst},
	};
	char* strFunc = strdup(integ->_func); // продублировали строку
	const char separators[] = "()"; // массив разделителей
	char* token = strtok(strFunc, separators); // откусили название операции
	bool complexResult = false;

	int j;
	for (j = 0; j < _countof(functions); j++)
	{
		if (!strcmp(functions[j].opcode, token)) {
			break;
		}
	}
	if (j == _countof(functions))
	{
		cout << Strings[IStrIllegalFunc].M_LANG << endl;
		return 0;
	}
	T value;
	token = strtok(NULL, separators); // откусили аргумент функции
	ofstream outFile;
	// заполняем файл с данными для GNUPlot
	outFile.open("graph", ios::trunc);
	double progressExact = 0;
	while (creal(i) < creal(n))
	{
		value = functions[j].pfUnaryOperation(IntegParseExprOld(integ, token));
		Square += E * value;
		if(cimag(value)!=0)
			complexResult = true;
		if((int)creal(integ->i) % 500 == 0)
			outFile << creal(integ->E*integ->i + integ->_a) << " " << cimag(value) << " " << creal(value) << endl;
		integ->i += 1 + 0 * I;
		progressExact = 100 * creal(i) / (creal(n) - 3);
		if(((int)(progressExact) % 5 == 0) && ((int)(progressExact) == progressExact))
		{
			cout << "\r" << Strings[IStrCalculating].M_LANG << (int)progressExact << "%";
			cout.flush(); // мгновенная печать потока cout
			*progress = (int)(progressExact);
		}
	}
	outFile.close();
	// заполняем файл с командами для GNUPlot
	outFile.open("graphP", ios::trunc);
	if(complexResult)
		outFile << "set grid" << endl << "set title \"" << integ->_func << "\"" << endl << "splot \"graph\" with impulses" << endl;
	else
		outFile << "set grid" << endl << "set title \"" << integ->_func << "\"" << endl << "plot \"graph\" using 1:3 with boxes" << endl;
	outFile.close();
	return Square;
};

T fun(Integ* ob1, int* progress, int parserVersion, long long presition)
{
	if(!parserVersion)
		return IntegResultOld(ob1, progress, presition);
	return IntegResultNew(ob1, progress, presition);
}
