﻿
// MMKDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "MMK.h"
#include "MMKDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CMMKDlg



CMMKDlg::CMMKDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MMK_DIALOG, pParent)
	, maxY(10000)
	, maxT(600)
	, xmax(400)
	, t1(10)
	, t2(300)
	, t3(590)
	, part_wnd(0.3)
	, period(0)
	, D1(0.5)
	, D2(0.5)
	, D3(0.5)
	, num_sloy1(2)
	, num_sloy2(0)
	, num_sloy3(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMMKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MULT, evolution);
	DDX_Text(pDX, IDC_EDIT1, maxY);
	DDX_Text(pDX, IDC_EDIT2, maxT);
	DDX_Text(pDX, IDC_EDIT3, xmax);
	DDX_Text(pDX, IDC_EDIT4, t1);
	DDX_Text(pDX, IDC_EDIT5, t2);
	DDX_Text(pDX, IDC_EDIT6, t3);
	DDX_Text(pDX, IDC_EDIT8, part_wnd);
	DDX_Text(pDX, IDC_EDIT7, period);
	DDX_Control(pDX, IDC_RUNLIMITED, r_unlimited);
	DDX_Control(pDX, IDC_RWIND, r_wind);
	DDX_Control(pDX, IDC_RLIMITED, r_limited);
	DDX_Control(pDX, IDC_EDIT8, ed_part);
	DDX_Control(pDX, IDC_EDIT7, ed_period);
	DDX_Control(pDX, IDC_TEXTPERIOD, text_period);
	DDX_Control(pDX, IDC_TEXTPART, text_part);
	DDX_Control(pDX, IDC_GR1, gr1);
	DDX_Control(pDX, IDC_GR2, gr2);
	DDX_Control(pDX, IDC_GR3, gr3);
	DDX_Text(pDX, IDC_EDIT9, D1);
	DDX_Text(pDX, IDC_EDIT10, D2);
	DDX_Text(pDX, IDC_EDIT11, D3);
	DDX_Control(pDX, IDC_PICIZOLINE, pic_izoline);
	DDX_Control(pDX, IDC_RADIO1, r_anim_yes);
	DDX_Control(pDX, IDC_RADIO2, r_anim_no);
	DDX_Text(pDX, IDC_EDIT12, num_sloy1);
	DDX_Text(pDX, IDC_EDIT13, num_sloy2);
	DDX_Text(pDX, IDC_EDIT14, num_sloy3);
	DDX_Control(pDX, IDC_EDIT13, ed_mark1);
	DDX_Control(pDX, IDC_EDIT14, ed_mark2);
	DDX_Control(pDX, IDC_EDIT15, ed_mark3);
}

BEGIN_MESSAGE_MAP(CMMKDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMMKDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RUNLIMITED, &CMMKDlg::OnBnClickedRunlimited)
	ON_BN_CLICKED(IDC_RWIND, &CMMKDlg::OnBnClickedRwind)
	ON_BN_CLICKED(IDC_RLIMITED, &CMMKDlg::OnBnClickedRlimited)
	ON_BN_CLICKED(IDC_BUTTON2, &CMMKDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMMKDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMMKDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO1, &CMMKDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMMKDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// Обработчики сообщений CMMKDlg

BOOL CMMKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	srand(time(NULL));
	SetUnlimited();
	animation = false;
	r_anim_no.SetCheck(BST_CHECKED);
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMMKDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMMKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD __stdcall CMMKDlg::MyThreadFunction(LPVOID lpParam)
{
	CMMKDlg* my_process = (CMMKDlg*)lpParam;
	srand(my_process->random_my);
	my_process->material.Main(my_process->maxT, my_process->maxY, my_process->xmax, std::vector<int>({ my_process->t1, my_process->t2, my_process->t3 }), my_process->period, my_process->part_wnd, my_process->cond);
	my_process->MessageBox(L"123", L"123");
	return 0;
}



void CMMKDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	KillTimer(my_timer);
	if (part_wnd >= 1)
	{
		MessageBox(L"Доля окна должна быть меньше единицы", L"Ошибка!");
		return;
	}
	evolution.SetParam(maxY, xmax, 0.5, 15);
	if (animation)
		my_timer = SetTimer(123, 15, NULL);
	random_my = rand();
	CreateThread(NULL, NULL, MyThreadFunction, this, NULL, NULL);
	if (cond == limited)
	{
		int center = floor(xmax / 2);
		x1.resize(xmax);
		for (int i = 0; i < xmax; i++)
		{
			x1[i] = i - center;
		}
	}
}

void CMMKDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	EnterCriticalSection(&material.cs);
	evolution.atoms = material.GetPosition();
	LeaveCriticalSection(&material.cs);
	evolution.Invalidate(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}

void CMMKDlg::SetUnlimited()
{
	r_unlimited.SetCheck(BST_CHECKED);
	r_limited.SetCheck(BST_UNCHECKED);
	r_wind.SetCheck(BST_UNCHECKED);
	ed_part.ShowWindow(SW_HIDE);
	ed_period.ShowWindow(SW_SHOW);
	text_part.ShowWindow(SW_HIDE);
	text_period.ShowWindow(SW_SHOW);
	cond = unlimitedNotAll;
}

void CMMKDlg::SetWind()
{
	r_unlimited.SetCheck(BST_UNCHECKED);
	r_limited.SetCheck(BST_UNCHECKED);
	r_wind.SetCheck(BST_CHECKED);
	ed_part.ShowWindow(SW_SHOW);
	ed_period.ShowWindow(SW_HIDE);
	text_part.ShowWindow(SW_SHOW);
	text_period.ShowWindow(SW_HIDE);
	cond = wind;
}

void CMMKDlg::SetLimited()
{
	r_unlimited.SetCheck(BST_UNCHECKED);
	r_limited.SetCheck(BST_CHECKED);
	r_wind.SetCheck(BST_UNCHECKED);
	ed_part.ShowWindow(SW_HIDE);
	ed_period.ShowWindow(SW_HIDE);
	text_part.ShowWindow(SW_HIDE);
	text_period.ShowWindow(SW_HIDE);
	cond = limited;
}


void CMMKDlg::OnBnClickedRunlimited()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetUnlimited();
}


void CMMKDlg::OnBnClickedRwind()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetWind();
}


void CMMKDlg::OnBnClickedRlimited()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetLimited();
}


void CMMKDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	material.pause = !material.pause;
}


void CMMKDlg::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
	EnterCriticalSection(&material.cs_stop);
	material.stop = !material.stop;
	LeaveCriticalSection(&material.cs_stop);
}


void CMMKDlg::OnBnClickedButton4()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	std::vector<std::vector<double>> grPrac;
	std::vector<std::vector<double>> grTheor;
	CString str1, str2, str3;
	switch (cond)
	{
	case unlimitedNotAll:
		grTheor = material.GetCxtTheor(xmax, std::vector<double>({ D1, D2, D3 }), std::vector<int>({ t1, t2, t3 }), num_sloy1);
		grPrac = material.GetCxtPrac();
		if (grPrac.size() != 3 || grTheor.size() != 3)
		{
			MessageBox(L"Не получилось трёх измерений концентрации", L"Ошибка");
			return;
		}

		str1.Format(L"%f", material.mark1);
		ed_mark1.SetWindowTextW(str1);
		str2.Format(L"%f", material.mark2);
		ed_mark2.SetWindowTextW(str2);
		str3.Format(L"%f", material.mark3);
		ed_mark3.SetWindowTextW(str3);

		material.printCxt(std::vector<int>({ t1, t2, t3 }));
		gr1.SetParam(std::vector<std::vector<double>>({ grPrac[0], grTheor[0] }), true, std::vector<Gdiplus::Color>({ Gdiplus::Color::Red, Gdiplus::Color::Blue }));
		gr2.SetParam(std::vector<std::vector<double>>({ grPrac[1], grTheor[1] }), true, std::vector<Gdiplus::Color>({ Gdiplus::Color::Red, Gdiplus::Color::Blue }));
		gr3.SetParam(std::vector<std::vector<double>>({ grPrac[2], grTheor[2] }), true, std::vector<Gdiplus::Color>({ Gdiplus::Color::Red, Gdiplus::Color::Blue }));
		gr1.Invalidate(FALSE);
		gr2.Invalidate(FALSE);
		gr3.Invalidate(FALSE); break;
	case wind:
		pic_izoline.draw = true;
		pic_izoline.setka = material.CxtWind.back();
		pic_izoline.size_izoline = 4;
		pic_izoline.Invalidate(FALSE);
		material.printCxtWind(); break;
	case limited:
		grPrac = material.GetCxtPrac();
		grTheor = material.GetCxtTheorLimited(xmax, std::vector<double>({ D1, D2, D3 }), std::vector<int>({ t1, t2, t3 }));
		if (grPrac.size() != 3 || grTheor.size() != 3)
		{
			MessageBox(L"Не получилось трёх измерений концентрации", L"Ошибка");
			return;
		}

		str1.Format(L"%f", material.mark1);
		ed_mark1.SetWindowTextW(str1);
		str2.Format(L"%f", material.mark2);
		ed_mark2.SetWindowTextW(str2);
		str3.Format(L"%f", material.mark3);
		ed_mark3.SetWindowTextW(str3);

		material.printCxtLimited(std::vector<int>({ t1, t2, t3 }));
		gr1.SetParam(std::vector<std::vector<double>>({ grPrac[0], grTheor[0] }), true, std::vector<Gdiplus::Color>({ Gdiplus::Color::Red, Gdiplus::Color::Blue }), std::vector<std::vector<double>>({ x1, x1 }));
		gr2.SetParam(std::vector<std::vector<double>>({ grPrac[1], grTheor[1] }), true, std::vector<Gdiplus::Color>({ Gdiplus::Color::Red, Gdiplus::Color::Blue }), std::vector<std::vector<double>>({ x1, x1 }));
		gr3.SetParam(std::vector<std::vector<double>>({ grPrac[2], grTheor[2] }), true, std::vector<Gdiplus::Color>({ Gdiplus::Color::Red, Gdiplus::Color::Blue }), std::vector<std::vector<double>>({ x1, x1 }));
		gr1.Invalidate(FALSE);
		gr2.Invalidate(FALSE);
		gr3.Invalidate(FALSE); break;
	}
}


void CMMKDlg::OnBnClickedRadio1()
{
	// TODO: добавьте свой код обработчика уведомлений
	r_anim_yes.SetCheck(BST_CHECKED);
	r_anim_no.SetCheck(BST_UNCHECKED);
	animation = true;
}


void CMMKDlg::OnBnClickedRadio2()
{
	// TODO: добавьте свой код обработчика уведомлений
	KillTimer(my_timer);
	r_anim_yes.SetCheck(BST_UNCHECKED);
	r_anim_no.SetCheck(BST_CHECKED);
	animation = false;
}
