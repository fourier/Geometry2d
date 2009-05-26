// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Geometry2d.h"

#include "MainFrm.h"
#include ".\mainfrm.h"
#include ".\Geometry2dDoc.h"
#include ".\ChildFrm.h"

#include <ExtToolControlBar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND_EX(IDR_Geometry2dTYPE, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_Geometry2dTYPE, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDR_TRIANGLE, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_TRIANGLE, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDR_BOUNDARY_BAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_BOUNDARY_BAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDR_PRIMITIVES, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_PRIMITIVES, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_WIZARD_SHEET_PLACER, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIZARD_SHEET_PLACER, OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDR_OPTIONS, OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDR_OPTIONS, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(ID_INDICATORXY, OnIndicatorXY)
	ON_UPDATE_COMMAND_UI(ID_INDICATORMODE,OnIndicatorMode)
	ON_COMMAND_EX(ID_NODEOPTIONS_HOLDER, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_NODEOPTIONS_HOLDER, OnUpdateControlBarMenu)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATORMODE,
	ID_INDICATORXY,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

void CMainFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CExtControlBar::DoFrameBarCheckUpdate( this, pCmdUI, true );
}

BOOL CMainFrame::OnBarCheck(UINT nID)
{
	return CExtControlBar::DoFrameBarCheckCmd( this, nID, true );
}

// CMainFrame construction/destruction

CMainFrame::CMainFrame() 
{
	g_PaintManager.InstallPaintManager( RUNTIME_CLASS(CExtPaintManagerStudio2005) );

	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	ASSERT( pApp->m_pszProfileName != NULL );


	g_CmdManager->ProfileSetup(
		pApp->m_pszProfileName,
		GetSafeHwnd()
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MAINFRAME
			)
		);
	
	


	if(!m_wndMenuBar.Create(
		NULL, // _T("Menu Bar"),
		this,
		IDR_Geometry2dTYPE
		))
	{
	    TRACE0("Failed to create menubar\n");
		return -1; 
	}

	if( !m_wndToolBar.Create(
			_T("ToolBar"),
			this,
			AFX_IDW_TOOLBAR
			)
		||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if( !m_wndTriangleToolBar.Create(
			_T("Triangle"),
			this,
			IDR_TRIANGLE
			)
		||
		!m_wndTriangleToolBar.LoadToolBar(IDR_TRIANGLE)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if( !m_wndBoundaryToolBar.Create(
			_T("Boundary conditions"),
			this,
			IDR_BOUNDARY_BAR
			)
		||
		!m_wndBoundaryToolBar.LoadToolBar(IDR_BOUNDARY_BAR)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if( !m_wndPrimitivesToolBar.Create(
			_T("Primitives"),
			this,
			IDR_PRIMITIVES
			)
		||
		!m_wndPrimitivesToolBar.LoadToolBar(IDR_PRIMITIVES)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


    CExtCmdItem * pCmdItem = NULL;

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_NODE
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Add nodes");

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_SEGMENT
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Add segments");

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_MARKER1
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Set marker to inner");

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_MARKER2
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Set marker to outer");

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_HOLE
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Set hole field");
	
	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_NODE_CHECK
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Select node");

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_BOUNDARY_LINE
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Symmetry conditions");

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_BOUNDARY_CURVE
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Boundary conditions");

	if( !m_wndPropertiesToolBar.Create(
			_T("Properties"),
			this,
			IDR_OPTIONS
			)
		||
		!m_wndPropertiesToolBar.LoadToolBar(IDR_OPTIONS)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_PROPERTIES
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Properties");

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_TRIANGULATE
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Triangulate");

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			ID_BUTTON_NODE_OPTIONS
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = _T("Node options");

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndProperties.SetInitDesiredSizeVertical(
		CSize( 200, 400 )
		);
	m_wndProperties.SetInitDesiredSizeHorizontal(
		CSize( 400, 200 )
		);

	m_wndNodeOptions.SetInitDesiredSizeVertical(
		CSize( 200, 400 )
		);
	m_wndNodeOptions.SetInitDesiredSizeHorizontal(
		CSize( 400, 200 )
		);

	if(	!m_wndProperties.Create(
		_T("Properties"),
		this,
		ID_VIEW_WIZARD_SHEET_PLACER	)
		)
	{
		TRACE0("Failed to create m_wndProperties\n");
		return -1;		// fail to create
	}

	if(	!m_wndNodeOptions.Create(
		_T("Node options"),
		this,
		ID_NODEOPTIONS_HOLDER)
		)
	{
		TRACE0("Failed to create m_wndProperties\n");
		return -1;		// fail to create
	}


	if( !m_wndInBarProperties.Create(
		IDD_PROPERTIES_DLG,
		&m_wndProperties)
		)
	{
		TRACE0("Failed to create m_wndInBarProperties\n");
		return -1;		// fail to create
	}
	m_wndInBarProperties.ShowSizeGrip( FALSE );

	if( !m_wndInBarNodeProperties.Create(
		IDD_NODE_PROPERTIES,
		&m_wndNodeOptions)
		)
	{
		TRACE0("Failed to create m_wndInBarProperties\n");
		return -1;		// fail to create
	}
	m_wndInBarProperties.ShowSizeGrip( FALSE );


	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndTriangleToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPropertiesToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndBoundaryToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPrimitivesToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	m_wndNodeOptions.EnableDocking(CBRS_ALIGN_ANY);
	RecalcLayout();
	
	
	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
	DockControlBar(&m_wndMenuBar);	
	DockControlBar(&m_wndToolBar);	
	DockControlBar(&m_wndTriangleToolBar);	
	DockControlBar(&m_wndBoundaryToolBar);	
	DockControlBar(&m_wndPropertiesToolBar);	
	DockControlBar(&m_wndPrimitivesToolBar);	
	DockControlBar(&m_wndProperties);	
	DockControlBar(&m_wndNodeOptions);	

	m_wndProperties.DockControlBar( AFX_IDW_DOCKBAR_LEFT, 2, this, false );
	m_wndNodeOptions.DockControlBar( AFX_IDW_DOCKBAR_RIGHT, 2, this, false );
	
	int nIndicator=m_wndStatusBar.CommandToIndex(ID_INDICATORXY);
	m_wndStatusBar.SetPaneInfo(nIndicator, ID_INDICATORXY, SBPS_NORMAL, 100);
	
	nIndicator=m_wndStatusBar.CommandToIndex(ID_INDICATORMODE);
	m_wndStatusBar.SetPaneInfo(nIndicator, ID_INDICATORMODE, SBPS_NORMAL, 60);

	RecalcLayout();
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


void CMainFrame::ActivateFrame(int nCmdShow)
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CFrameWnd::ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	
	CMDIFrameWnd::ActivateFrame(nCmdShow);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::ChangeXY(double x, double y)
{
	m_strIndicatorXY.Format("(%.4f,%.4f",x,y);
}

void CMainFrame::ChangeXY(const CString& str)
{
	m_strIndicatorXY = str;
}

void CMainFrame::OnIndicatorXY (CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetText(m_strIndicatorXY);
}

void CMainFrame::ChangeProperties(short scale, CGeometry2dView::ENodeDrawingMode mode,
	LGeometryData::EBoundaryCondition cond)
{
	CGeometry2dView* pView = NULL;
	CGeometry2dDoc* pDoc = NULL;

	POSITION pos;
		
	CChildFrame* pChild = static_cast<CChildFrame*>(MDIGetActive());
	if ( pChild )
	{
		pDoc = static_cast<CGeometry2dDoc*>(pChild->GetActiveDocument());
		if ( pDoc )
		{
			pos = pDoc->GetFirstViewPosition();
			if ( pos )
			{
				pView = static_cast<CGeometry2dView*>(pDoc->GetNextView(pos));
				if ( pView )
				{
					m_wndInBarProperties.SetScale(scale);
					m_wndInBarProperties.SetNodeDrawingMode(mode);
					m_wndInBarProperties.SetBoundaryConditions(cond);
					pView->GeometryProperties(scale,mode,cond);
				}
			}
		}
	}
}

void CMainFrame::ApplyNodesChange(LGeometryData::NodesConstIterator it,
	double x, double y, LGeometryData::EBoundaryType bnd, 
	bool delete_all_containing )
{
	CGeometry2dView* pView = NULL;
	CGeometry2dDoc* pDoc = NULL;

	POSITION pos;
		
	CChildFrame* pChild = static_cast<CChildFrame*>(MDIGetActive());
	if ( pChild )
	{
		pDoc = static_cast<CGeometry2dDoc*>(pChild->GetActiveDocument());
		if ( pDoc )
		{
			pos = pDoc->GetFirstViewPosition();
			if ( pos )
			{
				pView = static_cast<CGeometry2dView*>(pDoc->GetNextView(pos));
				if ( pView )
				{
					pDoc->PrepareUndo();
					pDoc->GetData().SetCoordinates(it,x,y);
					pDoc->GetData().SetBoundary(it,bnd);
					if ( delete_all_containing )
						pDoc->GetData().RemoveAllContainingNode(it);
					pView->PrepareBuffer();
					pView->RedrawWindow();
					return;
				}
			}
		}
	}	
	UnsetNodeForProperties();
}

void CMainFrame::ApplyNodesChange(LGeometryData::NodesConstIterator it,
	double x, double y, LGeometryData::EBoundaryType bnd, 
	bool delete_all_containing, double Z1, double Z2,int doftype )
{
	CGeometry2dView* pView = NULL;
	CGeometry2dDoc* pDoc = NULL;

	POSITION pos;
		
	CChildFrame* pChild = static_cast<CChildFrame*>(MDIGetActive());
	if ( pChild )
	{
		pDoc = static_cast<CGeometry2dDoc*>(pChild->GetActiveDocument());
		if ( pDoc )
		{
			pos = pDoc->GetFirstViewPosition();
			if ( pos )
			{
				pView = static_cast<CGeometry2dView*>(pDoc->GetNextView(pos));
				if ( pView )
				{
					pDoc->PrepareUndo();
					pDoc->GetData().SetCoordinates(it,x,y);
					pDoc->GetData().SetBoundary(it,bnd);
					if ( delete_all_containing )
						pDoc->GetData().RemoveAllContainingNode(it);
					pDoc->GetData().ChangeCondition(it,Z1,Z2,doftype);
					UnsetNodeForProperties();
					pView->PrepareBuffer();
					pView->RedrawWindow();
					return;
				}
			}
		}
	}	
	UnsetNodeForProperties();
}



void CMainFrame::ActivateProperties()
{
	CExtControlBar::DoFrameBarCheckCmd(
		this, ID_VIEW_WIZARD_SHEET_PLACER, false);
}

void CMainFrame::ActivateNodeProperties()
{
	CExtControlBar::DoFrameBarCheckCmd(
		this, ID_NODEOPTIONS_HOLDER, false);
}

void CMainFrame::SetNodeForProperties(LGeometryData::NodesConstIterator it,bool bSymmetric,
	bool bBoundary, bool bStressed, double Z1, double Z2,int doftype)
{
	m_wndInBarNodeProperties.SetNode(it,bSymmetric,bBoundary,bStressed,Z1,Z2,doftype);
}

void CMainFrame::UnsetNodeForProperties()
{
	m_wndInBarNodeProperties.DisableAll();
	m_wndNodeOptions.m_pDockSite->ShowControlBar( &m_wndNodeOptions, FALSE, FALSE );
}

void CMainFrame::OnIndicatorMode(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetText(m_strIndicatorMode);
}

void CMainFrame::ChangeMode(const CString& str)
{
	m_strIndicatorMode = str;	
}
