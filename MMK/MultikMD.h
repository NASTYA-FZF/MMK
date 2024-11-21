﻿#pragma once
#include <gdiplus.h>
#include <vector>

// MultikMD

class MultikMD : public CStatic
{
	DECLARE_DYNAMIC(MultikMD)

public:
	MultikMD();
	virtual ~MultikMD();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//надо для запуска gdi+
	UINT_PTR token;
	//коориднаты атомов
	std::vector<std::pair<int, int>> atoms;
	//радиус атомов, максимумы и минимумы изображения
	double r_atom, maxX, maxY, minX, minY;
	//параметры масштабирования изображения
	double scaleX, scaleY;
	//отступ, чтобы граничные атомы были полностью на картинке
	double otstup;
	//флаг первого запуска, так как параметры трансформации достаточно 1 раз посчитать
	bool start;
	//вызывается для перерисовки
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	//вычисление параметров масштабирования по ширине, высоте области вывода
	void CalcMas(double width_wnd, double height_wnd);
	//устанавливает параметры
	void SetParam(double maxY_, double maxX_, double r, double ots);
};


