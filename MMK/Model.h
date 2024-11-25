#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
//���� ����
struct atom
{
	int x;
	int y;
};
//��� ��������
enum condition
{
	unlimited,
	unlimitedNotAll,
	limited,
	wind
};
//������� ��������
class diffuz
{
	//����� ������
	std::vector<atom> atoms;
	//����� ������ ��� ������� ���� ������
	std::vector<atom> add_atom;
	//false - ��������, true - ������
	std::vector<std::vector<bool>> occupansy;
	//������� ������������ 3 ��.
	std::vector<std::vector<double>> Cxt;
	std::vector<std::vector<double>> TheorCxt;
	//��� ������������� �� (����� ���������� ����� ������ �����)
	int period;
	//��� ���������� �� (� �������� ����� ���� ����)
	double part_window;
	//������������ ���-�� ������ �� ���������
	int maxY;

public:
	//���� ������
	CRITICAL_SECTION cs;
	CRITICAL_SECTION cs_pause;
	CRITICAL_SECTION cs_stop;
	//���� ���������
	bool stop;
	bool pause;
	diffuz() { 
		InitializeCriticalSection(&cs); 
		InitializeCriticalSection(&cs_pause);
		InitializeCriticalSection(&cs_stop);
		srand(time(NULL)); stop = false; pause = false;
	} //�������� ���� �������� �������� ��� ���������
	~diffuz() { DeleteCriticalSection(&cs); DeleteCriticalSection(&cs_pause); DeleteCriticalSection(&cs_stop); }
	//���������� ��������� �����
	void SetSurfacePos(condition my_cond);
	//����� ��������
	void SetUnlimitedCond();
	//�����. ��������, �� � ������
	void SetUnlimitedNotAll();
	void SetWind();
	//1-�����, 2-�����, 3-����, 4-���
	int GetMove();
	//���� ���
	void OneMKSunlimited(condition my_cond);
	void OneMKSunlimitedNotAll();
	void OneMKSwind();
	//������� 1 ����
	void Move(atom& my_atom);
	//������������� �� �� �������� ����� � ������
	void PeriodCond(int& y);
	//������ �������
	void Main(int tmax, int ymax, int xmax, std::vector<int> part_time, int _period, double _part_wnd, condition cond);
	//�������� ���� � ������� ����������
	std::vector<std::pair<int, int>> GetPosition();
	//������� ������������
	void CalcCxt(int xmax);
	//������ �������� ������������ ��� ����� ���������
	void printCxt(std::vector<int> part_time);
	//�������� �������������� ��� x
	bool ControlX(int x);
	//���������� ���� ����������� ������������ � ������ ��������������� ���������
	void CalcTheorCxt(int xmax, double D, int t, int num_it);

	std::vector<std::vector<double>> GetCxtPrac();
	std::vector<std::vector<double>> GetCxtTheor(int xmax, std::vector<double> D, std::vector<int> t);
};