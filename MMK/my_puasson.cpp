// my_puasson.cpp: файл реализации
//

#include "pch.h"
#include "MMK.h"
#include "my_puasson.h"
using namespace std;
using namespace Gdiplus;

// my_puasson

IMPLEMENT_DYNAMIC(my_puasson, CStatic)

my_puasson::my_puasson()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	if (s != Ok)
	{
		MessageBox(L"Не удалось нарисовать", L"Ошибка", NULL);
	}
	draw = false;
}

my_puasson::~my_puasson()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(my_puasson, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений my_puasson



double MYsign(double x)
{
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

void my_puasson::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Добавьте исходный код для отображения указанного элемента
	if (!draw) return;

	if (setka.empty()) return;

	//if (my_izoline.empty()) izoline = false;

	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	Graphics draw_in_buffer(&buffer);
	draw_in_buffer.SetSmoothingMode(SmoothingModeAntiAlias);

	draw_in_buffer.Clear(Color::LightGreen);

	SolidBrush b_metall(Color::Gray), b_sreda(Color::LightGreen), b_granica_vne(Color::RosyBrown), b_granica_vnut(Color::LightGoldenrodYellow);
	Pen white_pen(Color::White, 3), black_pen(Color::Black, 2);

	double cur_width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	double cur_height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	int fi_width = setka[0].size();
	int fi_height = setka.size();

	xScale = cur_width / fi_width;
	yScale = cur_height / fi_height;

	CreateIzoline(&draw_in_buffer);

	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}

void my_puasson::CreateIzoline(Gdiplus::Graphics* my_dr)
{
	my_const.resize(size_izoline);
	for (int i = 0; i < size_izoline; i++)
	{
		my_const[i] = (double)(i + 1) / size_izoline;
	}
	my_izoline = vector<vector<pair<double, double>>>(size_izoline);

	//bool first = true;
	
	Pen white_pen(Color::White, 3), black_pen(Color::Black, 2);

	Matrix matr;
	matr.Scale(xScale, yScale);

	PointF pt[4];

	for (int i = 0; i < setka.size() - 1; i++)
	{
		for (int j = 1; j < setka[i].size() - 1; j++)
		{
			for (int k = 0; k < size_izoline; k++)
			{
				if ((my_const[k] <= setka[i][j] && my_const[k] >= setka[i][j + 1])
					|| (my_const[k] >= setka[i][j] && my_const[k] <= setka[i][j + 1]))
					my_izoline[k].push_back(pair<double, double>((double)(j + 1) - fabs(setka[i][j + 1] - my_const[k]) / fabs(setka[i][j] - setka[i][j + 1]), (double)i));

				if ((my_const[k] <= setka[i][j + 1] && my_const[k] >= setka[i + 1][j + 1])
					|| (my_const[k] >= setka[i][j + 1] && my_const[k] <= setka[i + 1][j + 1]))
					my_izoline[k].push_back(pair<double, double>((double)(j + 1), (double)(i + 1) - fabs(setka[i + 1][j + 1] - my_const[k]) / fabs(setka[i][j + 1] - setka[i + 1][j + 1])));

				if ((my_const[k] <= setka[i + 1][j] && my_const[k] >= setka[i + 1][j + 1])
					|| (my_const[k] >= setka[i + 1][j] && my_const[k] <= setka[i + 1][j + 1]))
					my_izoline[k].push_back(pair<double, double>((double)(j + 1) - fabs(setka[i + 1][j + 1] - my_const[k]) / fabs(setka[i + 1][j] - setka[i + 1][j + 1]), (double)(i + 1)));

				if ((my_const[k] <= setka[i][j] && my_const[k] >= setka[i + 1][j])
					|| (my_const[k] >= setka[i][j] && my_const[k] <= setka[i + 1][j]))
					my_izoline[k].push_back(pair<double, double>((double)(j), (double)(i + 1) - fabs(setka[i + 1][j] - my_const[k]) / fabs(setka[i][j] - setka[i + 1][j])));
				
				/*if (!my_izoline[k].empty())
				{
					pt[0] = PointF(my_izoline[k][my_izoline[k].size() - 2].first, my_izoline[k][my_izoline[k].size() - 2].second);
					pt[1] = PointF(my_izoline[k][my_izoline[k].size() - 1].first, my_izoline[k][my_izoline[k].size() - 1].second);
					matr.TransformPoints(pt, 2);
				}*/

				if (my_izoline[k].size() == 2)
				{
					pt[0] = PointF(my_izoline[k][my_izoline[k].size() - 2].first, my_izoline[k][my_izoline[k].size() - 2].second);
					pt[1] = PointF(my_izoline[k][my_izoline[k].size() - 1].first, my_izoline[k][my_izoline[k].size() - 1].second);
					matr.TransformPoints(pt, 2);
					my_dr->DrawLine(&black_pen, pt[0], pt[1]);
				}

				//my_dr->DrawLine(&black_pen, pt[0], pt[1]);
				my_izoline[k].clear();
			}
		}
	}
}