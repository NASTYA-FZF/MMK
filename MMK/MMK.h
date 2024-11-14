
// MMK.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMMKApp:
// Сведения о реализации этого класса: MMK.cpp
//

class CMMKApp : public CWinApp
{
public:
	CMMKApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMMKApp theApp;
