#pragma once
#include <gdiplus.h>
#include <vector>

// MyGraph

class MyGraph : public CStatic
{
	DECLARE_DYNAMIC(MyGraph)

public:
	MyGraph();
	virtual ~MyGraph();

protected:
	DECLARE_MESSAGE_MAP()

public:
	ULONG_PTR token;
	std::vector<std::vector<double>> my_func;
	std::vector<std::vector<double>> my_x;
	std::vector<BYTE> Red, Green, Blue;
	bool on_off, fon;
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	double maxX, maxY;
	double minX, minY;
	double stepX;
	double otstup;
	int cur;
	CString osY, osX, nameGraph;

	Gdiplus::PointF pt1, pt2;
	int num_network;
	double step_ver_network;
	double step_hor_network;

	void DrawNetwork(Gdiplus::Graphics& draw, Gdiplus::Matrix& matr);

	void DrawMyText(Gdiplus::Graphics& draw, Gdiplus::Matrix& matr);

	void DrawFunction(Gdiplus::Graphics& draw, Gdiplus::Matrix& matr);

	void SetColor();

	bool FindMinMax(); //true - succes, false - error
	void SetParam(std::vector<std::vector<double>> funcs, bool clearing = false, std::vector<Gdiplus::Color> my_color = std::vector<Gdiplus::Color>(), std::vector<std::vector<double>> xs = std::vector<std::vector<double>>());
};


