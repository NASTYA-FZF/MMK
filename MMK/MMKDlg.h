
// MMKDlg.h: файл заголовка
//

#pragma once
#include "MultikMD.h"
#include "Model.h"


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
};
