
// ObrazyPr1.h : main header file for the ObrazyPr1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CObrazyPr1App:
// See ObrazyPr1.cpp for the implementation of this class
//

class CObrazyPr1App : public CWinApp
{
public:
	CObrazyPr1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CObrazyPr1App theApp;
