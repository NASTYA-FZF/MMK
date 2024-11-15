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
	//для периодических гу
	int period;
	//для окошечного гу
	double part_window;
	//максимальное кол-во атомов по вертикали
	int maxY;

public:
	//крит секция
	CRITICAL_SECTION cs;
	//флаг остановки
	bool stop;
	diffuz() { InitializeCriticalSection(&cs); srand(time(NULL)); stop = false; } //ПОДУМАТЬ КУДА ВОТКНУТЬ ЗАТРАВКУ ДЛЯ ГЕНЕРАЦИИ
	~diffuz() { DeleteCriticalSection(&cs); }
	//установить начальные атомы
	void SetSurfacePos(condition my_cond);
	//неогр источник
	void SetUnlimitedCond();
	//1-право, 2-вверх, 3-лево, 4-низ
	int GetMove();
	//один МКШ
	void OneMKSunlimited();
	//Двинуть 1 атом
	void Move(atom& my_atom);
	//периодические гу по переходу внизу и сверху
	void PeriodCond(int& y);
	//главна функция
	void Main(int tmax, int ymax, int xmax, std::vector<int> part_time);
	//получить инфу о занятых положениях
	std::vector<std::vector<bool>> GetPosition();
	//рассчет концентрации
	void CalcCxt(int xmax);
	//печать значений концентрации при неогр источнике
	void printCxt(std::vector<int> part_time);
	//Контроль горизонтальной оси x
	bool ControlX(int x);
};