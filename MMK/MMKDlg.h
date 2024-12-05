
// MMKDlg.h: файл заголовка
//

#pragma once
#include "MultikMD.h"
#include "Model.h"
#include "MyGraph.h"
#include "my_puasson.h"
#include <cstdlib>
#include <ctime>

// Диалоговое окно CMMKDlg
class CMMKDlg : public CDialogEx
{
// Создание
public:
	CMMKDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MMK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	static DWORD __stdcall MyThreadFunction(LPVOID lpParam);
	MultikMD evolution;
	int maxY;
	int maxT;
	afx_msg void OnBnClickedButton1();
	UINT_PTR my_timer;
	diffuz material;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int xmax;
	int t1;
	int t2;
	int t3;
	double part_wnd;
	int period;
	CButton r_unlimited;
	CButton r_wind;
	CButton r_limited;
	CEdit ed_part;
	CEdit ed_period;
	void SetUnlimited();
	void SetWind();
	void SetLimited();
	condition cond;
	afx_msg void OnBnClickedRunlimited();
	afx_msg void OnBnClickedRwind();
	afx_msg void OnBnClickedRlimited();
	CStatic text_period;
	CStatic text_part;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	MyGraph gr1;
	MyGraph gr2;
	MyGraph gr3;
	afx_msg void OnBnClickedButton4();
	double D1;
	double D2;
	double D3;
	my_puasson pic_izoline;
	std::vector<double> x1;
	int random_my;
	bool animation;
	CButton r_anim_yes;
	CButton r_anim_no;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	int num_sloy1;
	int num_sloy2;
	int num_sloy3;
	CEdit ed_mark1;
	CEdit ed_mark2;
	CEdit ed_mark3;
};
