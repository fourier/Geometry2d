// Geometry2d.h : main header file for the Geometry2d application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CGeometry2dApp:
// See Geometry2d.cpp for the implementation of this class
//

class CGeometry2dApp : public CWinApp
{
public:
	CGeometry2dApp();


// Overrides
public:
	virtual BOOL InitInstance();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
public:
	CMultiDocTemplate* m_pDocTemplate;
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;
};

extern CGeometry2dApp theApp;