// MultikMD.cpp: файл реализации
//

#include "pch.h"
#include "MMK.h"
#include "MultikMD.h"
using namespace std;
using namespace Gdiplus;

// MultikMD

IMPLEMENT_DYNAMIC(MultikMD, CStatic)

MultikMD::MultikMD()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	if (s != Ok) MessageBox(L"s != Ok", L"Error!");
	minX = minY = 0; //всегда будут 0, так как у нас область (расчетная ячейка) не двигается
}

MultikMD::~MultikMD()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(MultikMD, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений MultikMD


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void MultikMD::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Добавьте исходный код для отображения указанного элемента

	if (start)
	{
		start = false; //больше первого запуска не будет
		//вычисление коэффициентов масштабирования
		CalcMas((double)lpDrawItemStruct->rcItem.right - 2. * otstup, (double)lpDrawItemStruct->rcItem.bottom - 2. * otstup);
		return;
	}

	//если пустой вектор, то выходим от греха подальше
	if (atoms.empty())
	{
		//MessageBox(L"Нет атомов для отрисовки!", L"Ошибка!");
		return;
	}

	//получаем дескриптор окна (вроде так называется)
	Graphics wnd(lpDrawItemStruct->hDC);
	//заводим буфер для рисования, чтобы не сразу на экран рисовалось
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	//заводим эту штуку для буфера
	Graphics draw_in_buffer(&buffer);

	//заливка в белый (делаем до преобразований, а то заливка съедет тоже)
	draw_in_buffer.Clear(Color::White);

	//матрица трансформаций (преобразований) пока не работает, хрен пойми почему
	Matrix matr;
	//сдвиг небольшой, чтобы атом крайний был не в углу (не совсем работало)
	matr.Translate(otstup, otstup);
	//масштабироавние под область вывода
	matr.Scale(scaleX, scaleY);

	//цвет для атомов
	SolidBrush b_atom(Color::Orange);
	Pen p_setka(Color::Black);
	PointF points[2];
	//рисуем линии
	for (int i = 0; i < maxY; i++)
	{
		points[0] = PointF((REAL)0., (REAL)i);
		//запоминаем правую нижнюю точку прямоугольника
		points[1] = PointF((REAL)(maxX - 1),
			(REAL)i);
		//преобразуем координаты
		matr.TransformPoints(points, 2);
		draw_in_buffer.DrawLine(&p_setka, points[0], points[1]);
		for (int j = 0; j < maxX; j++)
		{
			points[0] = PointF((REAL)j, (REAL)0);
			//запоминаем правую нижнюю точку прямоугольника
			points[1] = PointF((REAL)j,
				(REAL)(maxY - 1));
			//преобразуем координаты
			matr.TransformPoints(points, 2);
			draw_in_buffer.DrawLine(&p_setka, points[0], points[1]);
		}
	}
	//рисуем атомы
	for (int i = 0; i < atoms.size(); i++)
	{
		//запоминаем левую верхную точку прямоугольника, ограничивающего эллипс (круг)
		points[0] = PointF((REAL)(atoms[i].first - r_atom),
			(REAL)(atoms[i].second - r_atom));
		//запоминаем правую нижнюю точку прямоугольника
		points[1] = PointF((REAL)(atoms[i].first + r_atom),
			(REAL)(atoms[i].second + r_atom));
		//преобразуем координаты
		matr.TransformPoints(points, 2);
		//рисуем точку
		draw_in_buffer.FillEllipse(&b_atom, points[0].X,
			points[0].Y, points[1].X - points[0].X, points[1].Y - points[0].Y);
	}
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	buffer.Save(L"configuration.png", &pngClsid);
	//выводи из буфера
	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}

void MultikMD::CalcMas(double width_wnd, double height_wnd)
{
	//вычисляем параметры масштабирования
	scaleX = width_wnd / (maxX - minX);
	scaleY = height_wnd / (maxY - minY);
}

void MultikMD::SetParam(double maxY_, double maxX_, double r, double ots)
{
	maxY = maxY_; maxX = maxX_; r_atom = r; otstup = ots;
	start = true; //флаг первого запуска
}
