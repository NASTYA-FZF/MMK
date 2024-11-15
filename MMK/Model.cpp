#include "pch.h"
#include "Model.h"
using namespace std;

void diffuz::SetSurfacePos(condition my_cond)
{
	switch (my_cond)
	{
	case unlimited:
		SetUnlimitedCond(); break;
	}
}

void diffuz::SetUnlimitedCond()
{
	add_atom.resize(occupansy.size());
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

int diffuz::GetMove()
{
	return floor((int)((double)rand() / RAND_MAX * 4 + 1));
}

void diffuz::OneMKSunlimited()
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
	SetUnlimitedCond();
}

void diffuz::Move(atom& my_atom)
{
	int x = my_atom.x;
	int y = my_atom.y;
	switch (GetMove())
	{
	case 1:
		x++;
		if (ControlX(x))
			return;
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
		if (ControlX(x))
			return;
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

void diffuz::Main(int tmax, int ymax, int xmax, std::vector<int> part_time)
{
	atoms.clear();
	Cxt.clear();
	maxY = ymax;
	EnterCriticalSection(&cs);
	occupansy = vector<vector<bool>>(ymax, vector<bool>(xmax));
	LeaveCriticalSection(&cs);
	SetSurfacePos(unlimited);
	for (int t = 0; t < tmax; t++)
	{
		Sleep(20);
		EnterCriticalSection(&cs);
		OneMKSunlimited();
		LeaveCriticalSection(&cs);
		if (part_time.end() != find(part_time.begin(), part_time.end(), t))
			CalcCxt(xmax);
		if (stop) break;
	}
	printCxt(part_time);
}

std::vector<std::vector<bool>> diffuz::GetPosition()
{
	return occupansy;
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
