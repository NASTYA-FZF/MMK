#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
//один атом
struct atom
{
	int x;
	int y;
};
//вид диффузии
enum condition
{
	unlimited,
	unlimitedNotAll,
	limited,
	wind
};
//процесс диффузии
class diffuz
{
	//набор атомов
	std::vector<atom> atoms;
	//набор атомов для добавки гран атомов
	std::vector<atom> add_atom;
	//false - свободен, true - занято
	std::vector<std::vector<bool>> occupansy;
	//графики концентраций 3 шт.
	std::vector<std::vector<double>> Cxt;
	std::vector<std::vector<double>> TheorCxt;
	//для периодических гу (число обозначает число пустых узлов)
	int period;
	//для окошечного гу (в середине какая доля окна)
	double part_window;
	//максимальное кол-во атомов по вертикали
	int maxY;

public:
	//крит секция
	CRITICAL_SECTION cs;
	CRITICAL_SECTION cs_pause;
	CRITICAL_SECTION cs_stop;
	//флаг остановки
	bool stop;
	bool pause;
	diffuz() { 
		InitializeCriticalSection(&cs); 
		InitializeCriticalSection(&cs_pause);
		InitializeCriticalSection(&cs_stop);
		srand(time(NULL)); stop = false; pause = false;
	} //ПОДУМАТЬ КУДА ВОТКНУТЬ ЗАТРАВКУ ДЛЯ ГЕНЕРАЦИИ
	~diffuz() { DeleteCriticalSection(&cs); DeleteCriticalSection(&cs_pause); DeleteCriticalSection(&cs_stop); }
	//установить начальные атомы
	void SetSurfacePos(condition my_cond);
	//неогр источник
	void SetUnlimitedCond();
	//неогр. источник, но с дырами
	void SetUnlimitedNotAll();
	void SetWind();
	//1-право, 2-вверх, 3-лево, 4-низ
	int GetMove();
	//один МКШ
	void OneMKSunlimited(condition my_cond);
	void OneMKSunlimitedNotAll();
	void OneMKSwind();
	//Двинуть 1 атом
	void Move(atom& my_atom);
	//периодические гу по переходу внизу и сверху
	void PeriodCond(int& y);
	//главна функция
	void Main(int tmax, int ymax, int xmax, std::vector<int> part_time, int _period, double _part_wnd, condition cond);
	//получить инфу о занятых положениях
	std::vector<std::pair<int, int>> GetPosition();
	//рассчет концентрации
	void CalcCxt(int xmax);
	//печать значений концентрации при неогр источнике
	void printCxt(std::vector<int> part_time);
	//Контроль горизонтальной оси x
	bool ControlX(int x);
	//вычисление теор зависимости концентрации в случае неограниченного источника
	void CalcTheorCxt(int xmax, double D, int t, int num_it);

	std::vector<std::vector<double>> GetCxtPrac();
	std::vector<std::vector<double>> GetCxtTheor(int xmax, std::vector<double> D, std::vector<int> t);
};