
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
	, maxY(100)
	, maxT(100)
	, xmax(50)
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
}

BEGIN_MESSAGE_MAP(CMMKDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMMKDlg::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений CMMKDlg

BOOL CMMKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

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
	my_process->material.Main(my_process->maxT, my_process->maxY, my_process->xmax);
	return 0;
}



void CMMKDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData();
	evolution.SetParam(maxY, xmax, 0.5, 15);
	my_timer = SetTimer(123, 15, NULL);
	CreateThread(NULL, NULL, MyThreadFunction, this, NULL, NULL);
}

void CMMKDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	EnterCriticalSection(&material.cs);
	evolution.atoms = material.GetPosition();
	LeaveCriticalSection(&material.cs);
	Invalidate(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}
