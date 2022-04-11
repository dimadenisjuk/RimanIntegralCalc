#ifndef STRINGS_H
#define STRINGS_H

enum EStrings {
	IStrMainTip,
	IStrIntegValue,
	IStrQuitGNUPlot,
	IStrUnderstoodLike,
	IStrCalculating,
	IStrIllegalFunc,
};

typedef struct {enum EStrings strName; const char* en; const char* ru;} stLocales;

const static stLocales Strings[] = {
	{IStrMainTip,
	"# Enter интегрируемую function, begin and end of отрезка\nand necessary accuracy of calculations (LL < L < M < H < HH < FINE) ",
	"# Введите интегрируемую функцию, начало и конец отрезка\nи необходимую точность вычислений (LL < L < M < H < HH < FINE) "},
	{IStrIntegValue,
	"# Integral value (square under the figure): ",
	"# Значение интеграла (площадь под фигурой): "},
	{IStrQuitGNUPlot,
	"# Type \"quit\" to exit from GNUPlot",
	"# Введите \"quit\" для выхода из GNUPlot"},
	{IStrUnderstoodLike,
	"Understood expression like:",
	"Выражение понимается как:"},
	{IStrCalculating,
	"Calculating:",
	"Вычисление:"},
	{IStrIllegalFunc,
	"Illegal function",
	"Некорректная функция"},

};

#endif //STRINGS_H
