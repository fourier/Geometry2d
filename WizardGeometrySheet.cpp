// WizardGeometrySheet.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include ".\wizardgeometrysheet.h"

#include ".\Geometry2dView.h"
//class CGeometry2dView;

// CWizardGeometrySheet

IMPLEMENT_DYNAMIC(CWizardGeometrySheet, CExtResizablePropertySheet )
CWizardGeometrySheet::CWizardGeometrySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CExtResizablePropertySheet (nIDCaption, pParentWnd, iSelectPage)
{
}

CWizardGeometrySheet::CWizardGeometrySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CExtResizablePropertySheet (pszCaption, pParentWnd, iSelectPage)
{
}

CWizardGeometrySheet::~CWizardGeometrySheet()
{
	for ( int i = 0; i < GetPageCount(); i++ )
		if ( GetPage(i) )
			delete GetPage(i);
}


BEGIN_MESSAGE_MAP(CWizardGeometrySheet, CExtResizablePropertySheet )
END_MESSAGE_MAP()


// CWizardGeometrySheet message handlers

BOOL CWizardGeometrySheet::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	
	switch(nID) 
	{
	case ID_WIZFINISH:
		{
			GetView()->OnGeometryTriangulate();
		}
	case IDCANCEL:
		GetView()->SetMode(CGeometry2dView::ENone);
		DestroyWindow();
		break;
	case ID_WIZBACK:
	case ID_WIZNEXT:
		break;
	default:
		TRACE0("CWizardGeometrySheet::OnCmdMsg : Unknown case");
//		ASSERT(FALSE);
		break;
	}
	
	return CExtResizablePropertySheet ::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CWizardGeometrySheet::DestroyWindow()
{
	m_pView->PostMessage(WM_FLUSHWIZARD);
	return CExtResizablePropertySheet ::DestroyWindow();
}
