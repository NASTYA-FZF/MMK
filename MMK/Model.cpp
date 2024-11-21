#include "pch.h"
#include "Model.h"
using namespace std;

void diffuz::SetSurfacePos(condition my_cond)
{
	switch (my_cond)
	{
	case unlimited:
		SetUnlimitedCond(); break;
	case unlimitedNotAll:
		SetUnlimitedNotAll(); break;
	case wind:
		SetWind(); break;
	}
}

void diffuz::SetUnlimitedCond()
{
	add_atom.resize(maxY);
	EnterCriticalSection(&cs);
	for (int i = 0; i < add_atom.size(); i++)
	{
		add_atom[i].x = 0;
		add_atom[i].y = i;
		occupansy[i][0] = true;
	}
	LeaveCriticalSection(&cs);

	atoms.insert(atoms.end(), add_atom.begin(), add_atom.end());
}

void diffuz::SetUnlimitedNotAll()
{
	add_atom.resize(ceil(maxY / (period + 1)));
	EnterCriticalSection(&cs);
	for (int i = 0; i < add_atom.size(); i++)
	{
		add_atom[i].x = 0;
		add_atom[i].y = i * (period + 1);
		occupansy[add_atom[i].y][0] = true;
	}
	LeaveCriticalSection(&cs);

	atoms.insert(atoms.end(), add_atom.begin(), add_atom.end());
}

void diffuz::SetWind()
{
	add_atom.resize(maxY * part_window);
	int it = 0;
	EnterCriticalSection(&cs);
	for (int i = 0; i < maxY; i++)
	{
		if (i < maxY * (1 - part_window) / 2 || i >= maxY * (1 - (1 - part_window) / 2))
			occupansy[i][0] = true;
		else
		{
			add_atom[it].x = 0;
			add_atom[it].y = i;
			occupansy[add_atom[it].y][0] = true;
			it++;
		}
	}
	LeaveCriticalSection(&cs);

	atoms.insert(atoms.end(), add_atom.begin(), add_atom.end());
}

int diffuz::GetMove()
{
	return floor((int)((double)rand() / RAND_MAX * 4 + 1));
}

void diffuz::OneMKSunlimited(condition my_cond)
{
	int size = atoms.size();
	for (int a = 0; a < size; a++)
	{
		if (atoms[a].x == 0)
		{
			atoms[a].x++;
			occupansy[atoms[a].y][atoms[a].x] = true;
			continue;
		}
		Move(atoms[a]);
	}
	SetSurfacePos(my_cond);
}

void diffuz::OneMKSunlimitedNotAll()
{
	int size = atoms.size();
	for (int a = 0; a < size; a++)
	{
		if (atoms[a].x == 0)
		{
			atoms[a].x++;
			occupansy[atoms[a].y][atoms[a].x] = true;
			continue;
		}
		Move(atoms[a]);
	}
	SetUnlimitedNotAll();
}

void diffuz::OneMKSwind()
{
	int size = atoms.size();
	for (int a = 0; a < size; a++)
	{
		if (atoms[a].x == 0)
		{
			atoms[a].x++;
			occupansy[atoms[a].y][atoms[a].x] = true;
			continue;
		}
		Move(atoms[a]);
	}
	SetWind();
}

void diffuz::Move(atom& my_atom)
{
	int x = my_atom.x;
	int y = my_atom.y;
	switch (GetMove())
	{
	case 1:
		x++;
		EnterCriticalSection(&cs_stop);
		if (ControlX(x))
			return;
		LeaveCriticalSection(&cs_stop);
		if (!occupansy[y][x])
		{
			occupansy[y][x] = true;
			occupansy[my_atom.y][my_atom.x] = false;
			my_atom.x = x;
		}
		break;
	case 2:
		y--;
		PeriodCond(y);
		if (!occupansy[y][x])
		{
			occupansy[y][x] = true;
			occupansy[my_atom.y][my_atom.x] = false;
			my_atom.y = y;
		}
		break;
	case 3:
		x--;
		EnterCriticalSection(&cs_stop);
		if (ControlX(x))
			return;
		LeaveCriticalSection(&cs_stop);
		if (!occupansy[y][x])
		{
			occupansy[y][x] = true;
			occupansy[my_atom.y][my_atom.x] = false;
			my_atom.x = x;
		}
		break;
	case 4:
		y++;
		PeriodCond(y);
		if (!occupansy[y][x])
		{
			occupansy[y][x] = true;
			occupansy[my_atom.y][my_atom.x] = false;
			my_atom.y = y;
		}
		break;
	}
}

void diffuz::PeriodCond(int& y)
{
	if (y < 0)
		y += maxY;
	else
		if (y >= maxY)
			y -= maxY;
}

void diffuz::Main(int tmax, int ymax, int xmax, std::vector<int> part_time, int _period, double _part_wnd, condition cond)
{
	atoms.clear();
	Cxt.clear();
	TheorCxt.clear();
	maxY = ymax;
	period = _period;
	part_window = _part_wnd;
	EnterCriticalSection(&cs);
	occupansy = vector<vector<bool>>(ymax, vector<bool>(xmax));
	LeaveCriticalSection(&cs);
	SetSurfacePos(cond);
	for (int t = 0; t < tmax; t++)
	{
		Sleep(20);
		if (pause)
		{
			t--;
			continue;
		}
		EnterCriticalSection(&cs);
		OneMKSunlimited(cond);
		LeaveCriticalSection(&cs);
		if (part_time.end() != find(part_time.begin(), part_time.end(), t))
			CalcCxt(xmax);
		EnterCriticalSection(&cs_stop);
		if (stop) break;
		LeaveCriticalSection(&cs_stop);
	}
	printCxt(part_time);
}

std::vector<std::pair<int, int>> diffuz::GetPosition()
{
	int size = atoms.size();
	vector<pair<int, int>> position(size);
	for (int i = 0; i < size; i++)
	{
		position[i].first = atoms[i].x;
		position[i].second = atoms[i].y;
	}
	return position;
}

void diffuz::CalcCxt(int xmax)
{
	vector<double> Cx(xmax, 0);
	for (int i = 0; i < xmax; i++)
	{
		EnterCriticalSection(&cs);
		for (int j = 0; j < maxY; j++)
		{
			Cx[i] += occupansy[j][i] ? 1 : 0;
		}
		LeaveCriticalSection(&cs);
		Cx[i] /= maxY;
	}
	Cxt.push_back(Cx);
}

void diffuz::printCxt(std::vector<int> part_time)
{
	ofstream output("CxtUnlimited.txt");
	string str_x, str_t, str_Cxt;
	for (int i = 0; i < Cxt.size(); i++)
	{
		str_t = to_string(part_time[i]);
		replace(str_t.begin(), str_t.end(), '.', ',');
		for (int j = 0; j < Cxt[i].size(); j++)
		{
			str_x = to_string(j);
			str_Cxt = to_string(Cxt[i][j]);
			replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');
			replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');

			output << str_t << "\t" << str_x << "\t" << str_Cxt << endl;
		}
	}
	output.close();
}

bool diffuz::ControlX(int x)
{
	if (x >= occupansy.size() || x < 0)
		stop = true;
	return stop;
}
