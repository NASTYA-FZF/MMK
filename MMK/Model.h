#pragma once
#include <vector>
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
	//��� ������������� ��
	int period;
	//��� ���������� ��
	double part_window;
	//������������ ���-�� ������ �� ���������
	int maxY;

public:
	//���� ������
	CRITICAL_SECTION cs;
	diffuz() { InitializeCriticalSection(&cs); }
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
	void Main(int tmax, int ymax, int xmax);
	//�������� ���� � ������� ����������
	std::vector<std::vector<bool>> GetPosition();
};