#include "pch.h"
#define _USE_MATH_DEFINES
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
	case limited:
		SetLimited(); break;
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
		if (i >= maxY * (1 - part_window) / 2 && it < add_atom.size())
		{
			add_atom[it].x = 0;
			add_atom[it].y = i;
			occupansy[add_atom[it].y][0] = true;
			it++;
		}
		else
		{
			occupansy[i][0] = true;
		}
	}
	LeaveCriticalSection(&cs);

	atoms.insert(atoms.end(), add_atom.begin(), add_atom.end());
}

void diffuz::SetLimited()
{
	atoms.resize(3 * maxY);
	center = floor(maxX / 2);
	for (int y = 0; y < maxY; y++)
	{
		atoms[3 * y].x = center;
		atoms[3 * y + 1].x = center - 1;
		atoms[3 * y + 2].x = center + 1;
		atoms[3 * y].y = atoms[3 * y + 1].y = atoms[3 * y + 2].y = y;

		occupansy[y][center - 1] = occupansy[y][center] = occupansy[y][center + 1] = true;
	}
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
		if (atoms[a].x == 0 && !occupansy[atoms[a].y][atoms[a].x + 1])
		{
			occupansy[atoms[a].y][atoms[a].x] = false;
			atoms[a].x++;
			occupansy[atoms[a].y][atoms[a].x] = true;
			continue;
		}
		if (atoms[a].x != 0)
			Move(atoms[a]);
	}
	SetSurfacePos(my_cond);
}

void diffuz::OneMKSunlimitedNotAll()
{
	int size = atoms.size();
	for (int a = 0; a < size; a++)
	{
		if (atoms[a].x == 0 && !occupansy[atoms[a].y][atoms[a].x + 1])
		{
			occupansy[atoms[a].y][atoms[a].x] = false;
			atoms[a].x++;
			occupansy[atoms[a].y][atoms[a].x] = true;
			continue;
		}
		if (atoms[a].x != 0)
			Move(atoms[a]);
	}
	SetUnlimitedNotAll();
}

void diffuz::OneMKSwind()
{
	int size = atoms.size();
	for (int a = 0; a < size; a++)
	{
		if (atoms[a].x == 0 && !occupansy[atoms[a].y][atoms[a].x + 1])
		{
			occupansy[atoms[a].y][atoms[a].x] = false;
			atoms[a].x++;
			occupansy[atoms[a].y][atoms[a].x] = true;
			continue;
		}
		if (atoms[a].x != 0)
			Move(atoms[a]);
	}
	SetWind();
}

void diffuz::OneMKSlimited()
{
	int size = atoms.size();
	for (int a = 0; a < size; a++)
	{
		Move(atoms[a]);
	}
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
	CxtWind.clear();
	TheorCxt.clear();
	maxY = ymax; maxX = xmax;
	period = _period;
	part_window = _part_wnd;
	EnterCriticalSection(&cs);
	occupansy = vector<vector<bool>>(ymax, vector<bool>(xmax));
	LeaveCriticalSection(&cs);
	SetSurfacePos(cond);
	switch (cond)
	{
	case unlimitedNotAll:
		for (int t = 0; t < tmax; t++)
		{
			Sleep(20);
			if (pause)
			{
				t--;
				continue;
			}
			EnterCriticalSection(&cs);
			OneMKSunlimitedNotAll();
			LeaveCriticalSection(&cs);
			if (part_time.end() != find(part_time.begin(), part_time.end(), t))
				CalcCxtUnlimited(xmax);
			EnterCriticalSection(&cs_stop);
			if (stop) break;
			LeaveCriticalSection(&cs_stop);
		}
		break;
	case wind:
		for (int t = 0; t < tmax; t++)
		{
			Sleep(20);
			if (pause)
			{
				t--;
				continue;
			}
			EnterCriticalSection(&cs);
			OneMKSwind();
			LeaveCriticalSection(&cs);
			if (t == tmax - 1 || stop)
				CalcCxtWind(xmax);
			EnterCriticalSection(&cs_stop);
			if (stop) break;
			LeaveCriticalSection(&cs_stop);
		}
		break;
	case limited:
		for (int t = 0; t < tmax; t++)
		{
			Sleep(20);
			if (pause)
			{
				t--;
				continue;
			}
			EnterCriticalSection(&cs);
			OneMKSlimited();
			LeaveCriticalSection(&cs);
			if (part_time.end() != find(part_time.begin(), part_time.end(), t))
				CalcCxtUnlimited(xmax);
			EnterCriticalSection(&cs_stop);
			if (stop) break;
			LeaveCriticalSection(&cs_stop);
		}
		break;
	}
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

void diffuz::CalcCxtUnlimited(int xmax)
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

void diffuz::CalcCxtWind(int xmax) //как сделать подсчет концентрации
{
	vector<vector<double>> Cx(maxY, vector<double>(xmax, 0));
	int cur_y = 0;
	int widthWind = maxY * part_window;
	int first_bortik = maxY * (1 - part_window) / 2;
	for (int x = 0; x < xmax; x++)//берем €чейку 5х5
	{
		if (x > 2 && x < xmax - 2)
		{
			for (int y = 0; y < maxY; y++)
			{
				for (int kx = -floor(r_matr / 2); kx <= floor(r_matr / 2); kx++)
				{
					for (int ky = -floor(r_matr / 2); ky <= floor(r_matr / 2); ky++)
					{
						cur_y = y + ky;
						PeriodCond(cur_y);
						Cx[y][x] += occupansy[cur_y][x + kx] ? 1 : 0;
					}
				}
				Cx[y][x] /= 25; //так как €чейка 5х5
			}
		}
		else
			if (x == 0)
			{
				for (int y = first_bortik; y < first_bortik + widthWind; y++)
				{
					if (y > first_bortik && y < first_bortik + widthWind - 1)
					{
						Cx[y][x] += occupansy[y - 1][x] ? 1 : 0;
						Cx[y][x] += occupansy[y + 1][x] ? 1 : 0;
						Cx[y][x] += occupansy[y][x + 1] ? 1 : 0;
						Cx[y][x] /= 3;
					}
					else
					{
						Cx[y][x] += occupansy[y == first_bortik ? y + 1 : y - 1][x] ? 1 : 0;
						Cx[y][x] += occupansy[y][x + 1] ? 1 : 0;
						Cx[y][x] /= 2;
					}
				}
			}
			else
				if (x == 1 || x == xmax - 1)
				{
					for (int y = 0; y < maxY; y++)
					{
						//if (y > first_bortik || y < first_bortik + widthWind - 1)
						//{
						//	for (int kx = -1; kx <= 1; kx++)
						//	{
						//		for (int ky = -1; ky <= 1; ky++)
						//		{
						//			cur_y = y + ky;
						//			PeriodCond(cur_y);
						//			Cx[y][x] += occupansy[cur_y][x + kx] ? 1 : 0;
						//		}
						//	}
						//	Cx[y][x] /= 9; //так как €чейка 3х3
						//}
						//else
						//{
							cur_y = y - 1;
							PeriodCond(cur_y);
							Cx[y][x] += occupansy[cur_y][x] ? 1 : 0;

							cur_y = y + 1;
							PeriodCond(cur_y);
							Cx[y][x] += occupansy[cur_y][x] ? 1 : 0;
							Cx[y][x] += occupansy[y][x < xmax / 2 ? x + 1 : x - 1] ? 1 : 0;
							Cx[y][x] /= 3;
						//}
					}
				}
				else
					if (x == 2 || x == xmax - 2)
					{
						for (int y = 0; y < maxY; y++)
						{
							for (int kx = -1; kx <= 1; kx++)
							{
								for (int ky = -1; ky <= 1; ky++)
								{
									cur_y = y + ky;
									PeriodCond(cur_y);
									Cx[y][x] += occupansy[cur_y][x + kx] ? 1 : 0;
								}
							}
							Cx[y][x] /= 9; //так как €чейка 3х3
						}
					}
	}
	CxtWind.push_back(Cx);
}

void diffuz::CalcCxt(condition cond, int xmax)
{
	switch (cond)
	{
	case unlimitedNotAll:
		CalcCxtUnlimited(xmax); break;
	case wind:
		CalcCxtWind(xmax); break;
	}
}

void diffuz::printCxt(std::vector<int> part_time)
{
	ofstream output("CxtUnlimited.txt");
	string str_x, str_t1, str_Cxt, str_theor, str_t2, str_t3;
	str_t1 = to_string(part_time[0]);
	replace(str_t1.begin(), str_t1.end(), '.', ',');
	str_t2 = to_string(part_time[1]);
	replace(str_t2.begin(), str_t2.end(), '.', ',');
	str_t3 = to_string(part_time[2]);
	replace(str_t3.begin(), str_t3.end(), '.', ',');
	for (int j = num_sloy; j < Cxt[0].size(); j++)
	{
		str_x = to_string(j);
		str_Cxt = to_string(Cxt[0][j]);
		str_theor = to_string(TheorCxt[0][j - num_sloy]);
		replace(str_x.begin(), str_x.end(), '.', ',');
		replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');
		replace(str_theor.begin(), str_theor.end(), '.', ',');

		output << str_x << "\t" << str_t1 << "\t" << str_Cxt << "\t" << str_theor << "\t";

		str_Cxt = to_string(Cxt[1][j]);
		str_theor = to_string(TheorCxt[1][j - num_sloy]);
		replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');
		replace(str_theor.begin(), str_theor.end(), '.', ',');

		output << str_t2 << "\t" << str_Cxt << "\t" << str_theor << "\t";

		str_Cxt = to_string(Cxt[2][j]);
		str_theor = to_string(TheorCxt[2][j - num_sloy]);
		replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');
		replace(str_theor.begin(), str_theor.end(), '.', ',');

		output << str_t3 << "\t" << str_Cxt << "\t" << str_theor << endl;
	}
	output.close();
}

void diffuz::printCxtWind()
{
	ofstream output("CxtUnlimited.txt");
	string str_Cxt;
	for (int i = 0; i < CxtWind.size(); i++)
	{
		for (int j = 0; j < CxtWind[i].size(); j++)
		{
			for (int k = 0; k < CxtWind[i][j].size(); k++)
			{
				str_Cxt = to_string(CxtWind[i][j][k]);
				replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');
				replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');

				output << str_Cxt << "\t";
			}
			output << endl;
		}
		output << endl;
	}
	output.close();
}

void diffuz::printCxtLimited(std::vector<int> part_time)
{
	ofstream output("CxtUnlimited.txt");
	string str_x, str_t1, str_Cxt, str_theor, str_t2, str_t3;
	str_t1 = to_string(part_time[0]);
	replace(str_t1.begin(), str_t1.end(), '.', ',');
	str_t2 = to_string(part_time[1]);
	replace(str_t2.begin(), str_t2.end(), '.', ',');
	str_t3 = to_string(part_time[2]);
	replace(str_t3.begin(), str_t3.end(), '.', ',');
	for (int j = 0; j < Cxt[0].size(); j++)
	{
		str_x = to_string(j);
		str_Cxt = to_string(Cxt[0][j]);
		str_theor = to_string(TheorCxt[0][j]);
		replace(str_x.begin(), str_x.end(), '.', ',');
		replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');
		replace(str_theor.begin(), str_theor.end(), '.', ',');

		output << str_x << "\t" << str_t1 << "\t" << str_Cxt << "\t" << str_theor << "\t";

		str_Cxt = to_string(Cxt[1][j]);
		str_theor = to_string(TheorCxt[1][j]);
		replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');
		replace(str_theor.begin(), str_theor.end(), '.', ',');

		output << str_t2 << "\t" << str_Cxt << "\t" << str_theor << "\t";

		str_Cxt = to_string(Cxt[2][j]);
		str_theor = to_string(TheorCxt[2][j]);
		replace(str_Cxt.begin(), str_Cxt.end(), '.', ',');
		replace(str_theor.begin(), str_theor.end(), '.', ',');

		output << str_t3 << "\t" << str_Cxt << "\t" << str_theor << endl;
	}
	output.close();
}

bool diffuz::ControlX(int x)
{
	if (x >= occupansy[0].size() || x < 0)
		stop = true;
	return stop;
}

void diffuz::CalcTheorCxt(int xmax, double D, int t, int num_it)
{
	vector<double> Cx(xmax - num_sloy, 0);
	double sum = 0;
	double energy = 0;
	double C0 = Cxt[num_it][num_sloy];
	for (int i = 0; i < Cx.size(); i++)
	{
		Cx[i] = C0 * erfc(i / (2. * sqrt(D * t)));
		sum += (Cx[i] - Cxt[num_it][i + num_sloy]) * (Cx[i] - Cxt[num_it][i + num_sloy]);
		energy += Cx[i] * Cx[i];
	}
	switch (num_it)
	{
	case 0:
		mark1 = 100 * sum / energy; break;
	case 1:
		mark2 = 100 * sum / energy; break;
	case 2:
		mark3 = 100 * sum / energy; break;
	}
	TheorCxt.push_back(Cx);
}

void diffuz::CalcTheorCxtLimited(int xmax, double D, int t, int num_it)
{
	vector<double> Cx(xmax, 0);
	double C0 = Cxt[num_it][center];
	double sum = 0;
	double energy = 0;
	//for (int i = -1; i <= 1; i++)
	//	C0 += Cxt[num_it][center + i];
	//C0 /= 3;
	for (int i = 0; i < xmax; i++)
	{
		Cx[i] = C0 * exp(-(i - center) * (i - center) / (4 * D * t))/* / sqrt(M_PI * D * t)*/;
		sum += (Cx[i] - Cxt[num_it][i + num_sloy]) * (Cx[i] - Cxt[num_it][i + num_sloy]);
		energy += Cx[i] * Cx[i];
	}
	switch (num_it)
	{
	case 0:
		mark1 = 100 * sum / energy; break;
	case 1:
		mark2 = 100 * sum / energy; break;
	case 2:
		mark3 = 100 * sum / energy; break;
	}
	TheorCxt.push_back(Cx);
}

std::vector<std::vector<double>> diffuz::GetCxtPrac()
{
	auto res = Cxt;
	res[0].erase(res[0].begin(), res[0].begin() + num_sloy);
	res[1].erase(res[1].begin(), res[1].begin() + num_sloy);
	res[2].erase(res[2].begin(), res[2].begin() + num_sloy);
	return res;
}

std::vector<std::vector<double>> diffuz::GetCxtTheor(int xmax, std::vector<double> D, std::vector<int> t, int nums_sloy)
{
	num_sloy = nums_sloy;
	TheorCxt.clear();
	for (int i = 0; i < D.size(); i++)
		CalcTheorCxt(xmax, D[i], t[i], i);
	return TheorCxt;
}

std::vector<std::vector<double>> diffuz::GetCxtTheorLimited(int xmax, std::vector<double> D, std::vector<int> t)
{
	TheorCxt.clear();
	for (int i = 0; i < D.size(); i++)
		CalcTheorCxtLimited(xmax, D[i], t[i], i);
	return TheorCxt;
}
