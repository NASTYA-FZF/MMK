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

void diffuz::Main(int tmax, int ymax, int xmax)
{
	atoms.clear();
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
	}
}

std::vector<std::vector<bool>> diffuz::GetPosition()
{
	return occupansy;
}
