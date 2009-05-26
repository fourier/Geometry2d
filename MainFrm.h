// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include <ExtTempl.h>
#include ".\propertiesdlg.h"
#include ".\nodepropertiesdlg.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CExtMenuControlBar    m_wndMenuBar;
protected: 
	// control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtToolControlBar    m_wndToolBar;
	CExtToolControlBar    m_wndTriangleToolBar;
	CExtToolControlBar    m_wndPropertiesToolBar;
	CExtToolControlBar    m_wndBoundaryToolBar;
	CExtToolControlBar    m_wndPrimitivesToolBar;
	CExtControlBar		  m_wndProperties;
	CExtControlBar		  m_wndNodeOptions;

	CExtWRB<CPropertiesDlg> m_wndInBarProperties;
	CExtWRB<CNodePropertiesDlg> m_wndInBarNodeProperties;

// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	afx_msg void OnIndicatorXY(CCmdUI *pCmdUI);
	afx_msg void OnIndicatorMode(CCmdUI *pCmdUI);

	CString m_strIndicatorXY;
	CString m_strIndicatorMode;
public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void ChangeProperties(short scale, CGeometry2dView::ENodeDrawingMode mode,
		LGeometryData::EBoundaryCondition cond);

	void ApplyNodesChange(LGeometryData::NodesConstIterator it,
		double x, double y, LGeometryData::EBoundaryType bnd, 
		bool delete_all_containing );
	void ApplyNodesChange(LGeometryData::NodesConstIterator it,
		double x, double y, LGeometryData::EBoundaryType bnd, 
		bool delete_all_containing, double Z1, double Z2,int doftype );



	void SetNodeForProperties(LGeometryData::NodesConstIterator it,bool bSymmetric,
		bool bBoundary=false, bool bStressed=false, double Z1=0, double Z2=0,int doftype = 0);
	void UnsetNodeForProperties();
	

	void ActivateProperties();
	void ActivateNodeProperties();

	void ChangeXY(double x, double y);
	void ChangeXY(const CString& str);
	
	void ChangeMode( const CString& str);
};


