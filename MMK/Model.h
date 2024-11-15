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
	//��� ������������� ��
	int period;
	//��� ���������� ��
	double part_window;
	//������������ ���-�� ������ �� ���������
	int maxY;

public:
	//���� ������
	CRITICAL_SECTION cs;
	//���� ���������
	bool stop;
	diffuz() { InitializeCriticalSection(&cs); srand(time(NULL)); stop = false; } //�������� ���� �������� �������� ��� ���������
	~diffuz() { DeleteCriticalSection(&cs); }
	//���������� ��������� �����
	void SetSurfacePos(condition my_cond);
	//����� ��������
	void SetUnlimitedCond();
	//1-�����, 2-�����, 3-����, 4-���
	int GetMove();
	//���� ���
	void OneMKSunlimited();
	//������� 1 ����
	void Move(atom& my_atom);
	//������������� �� �� �������� ����� � ������
	void PeriodCond(int& y);
	//������ �������
	void Main(int tmax, int ymax, int xmax, std::vector<int> part_time);
	//�������� ���� � ������� ����������
	std::vector<std::vector<bool>> GetPosition();
	//������� ������������
	void CalcCxt(int xmax);
	//������ �������� ������������ ��� ����� ���������
	void printCxt(std::vector<int> part_time);
	//�������� �������������� ��� x
	bool ControlX(int x);
};