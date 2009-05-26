// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "Geometry2d.h"

#include "ChildFrm.h"
#include ".\childfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_NCPAINT()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers

void CChildFrame::OnNcPaint()
{
	// OnWindowPosChanged() and OnNcPaint() methods
	// implemented to fix WinXP MDI interface flicker bug
	CMDIFrameWnd * pMdiFrame =
			STATIC_DOWNCAST( CMDIFrameWnd, GetParentFrame() );
		ASSERT_VALID( pMdiFrame );
	BOOL bMax = FALSE;
	CMDIChildWnd * pActive = pMdiFrame->MDIGetActive( &bMax );
		pActive;
		if( bMax )
			return;
		CMDIChildWnd::OnNcPaint();
}

void CChildFrame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
// OnWindowPosChanged() and OnNcPaint() methods
	// implemented to fix WinXP MDI interface flicker bug
	CMDIFrameWnd * pMdiFrame =
			STATIC_DOWNCAST( CMDIFrameWnd, GetParentFrame() );
		ASSERT_VALID( pMdiFrame );
	BOOL bMax = FALSE;
	CMDIChildWnd * pActive = pMdiFrame->MDIGetActive( &bMax );
		pActive;
		if( ! bMax )
		{
			CMDIChildWnd::OnWindowPosChanged(lpwndpos);
			return;
		} // if( ! bMax )
		SetRedraw( FALSE );
		CMDIChildWnd::OnWindowPosChanged(lpwndpos);
		SetRedraw( TRUE );
		RedrawWindow(
			NULL, NULL,
			RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME
			);
}
