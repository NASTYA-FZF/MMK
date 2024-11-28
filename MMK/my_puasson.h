#pragma once
#include <gdiplus.h>
#include <vector>

// my_puasson

class my_puasson : public CStatic
{
	DECLARE_DYNAMIC(my_puasson)

public:
	my_puasson();
	virtual ~my_puasson();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	UINT_PTR token;
	std::vector<std::vector<double>> setka;
	double xScale, yScale;
	bool draw;
	std::vector<std::vector<std::pair<double, double>>> my_izoline;
	std::vector<double> my_const;
	int size_izoline;
	void CreateIzoline(Gdiplus::Graphics* my_dr);
};

