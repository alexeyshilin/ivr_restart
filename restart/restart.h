// restart.h : main header file for the restart application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CrestartApp:
// See restart.cpp for the implementation of this class
//

class CrestartApp : public CWinApp
{
public:
	CrestartApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CrestartApp theApp;