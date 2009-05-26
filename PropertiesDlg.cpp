// PropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "PropertiesDlg.h"
#include ".\propertiesdlg.h"
#include ".\mainfrm.h"

// CPropertiesDlg dialog

short CPropertiesDlg::s_nInititalScale = 10;

IMPLEMENT_DYNAMIC(CPropertiesDlg, CExtResizableDialog)
CPropertiesDlg::CPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CPropertiesDlg::IDD, pParent),
	m_shortScale(s_nInititalScale),
	m_eMode(CGeometry2dView::EBox)
{
}

CPropertiesDlg::~CPropertiesDlg()
{
}

void CPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_SCALING_EDIT,m_wndScalingEdit);
	DDX_Control(pDX,IDC_SCALING_SPIN,m_wndScalingSpin);
	DDX_Control(pDX,IDC_NODE_DRAWING_MODES,m_wndNodeDrawingOptions);
	DDX_Control(pDX,IDC_COMBO_BOUNDARY_CONDITIONS,m_wndBoundaryConditions);
	CExtResizableDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropertiesDlg, CExtResizableDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCALING_SPIN, OnScaligSpinChange)
END_MESSAGE_MAP()


// CPropertiesDlg message handlers

BOOL CPropertiesDlg::OnInitDialog()
{
	CExtResizableDialog::OnInitDialog();

	m_wndScalingSpin.SetBuddy(&m_wndScalingEdit);
	m_wndScalingSpin.SetRange(1,100);

	CString strText;
	strText.Format("%d:1",s_nInititalScale);

	m_wndScalingSpin.SetPos(s_nInititalScale);
	m_wndScalingEdit.SetWindowText((LPCTSTR)strText);

	// Node drawing mode

	int nPos = m_wndNodeDrawingOptions.AddString("As a box");
	m_wndNodeDrawingOptions.SetItemData(nPos,CGeometry2dView::EBox);
	
	nPos = m_wndNodeDrawingOptions.AddString("As a cross");
	m_wndNodeDrawingOptions.SetItemData(nPos,CGeometry2dView::ECross);
	
	nPos = m_wndNodeDrawingOptions.AddString("As a circle");
	m_wndNodeDrawingOptions.SetItemData(nPos,CGeometry2dView::ECircle);
	
	nPos = m_wndNodeDrawingOptions.AddString("As a dot");
	m_wndNodeDrawingOptions.SetItemData(nPos,CGeometry2dView::EDot);

	nPos = m_wndNodeDrawingOptions.AddString("Do not draw");
	m_wndNodeDrawingOptions.SetItemData(nPos,CGeometry2dView::ENotDraw);

	m_wndNodeDrawingOptions.SetCurSel(0);
	
	// Boundary condition type

	nPos = m_wndBoundaryConditions.InsertString(-1,"No conditions");
	m_wndBoundaryConditions.SetItemData(nPos,LGeometryData::ENoConditions);
	nPos = m_wndBoundaryConditions.InsertString(-1,"As displacements");
	m_wndBoundaryConditions.SetItemData(nPos,LGeometryData::EDisplacement);
	nPos = m_wndBoundaryConditions.InsertString(-1,"As stresses");
	m_wndBoundaryConditions.SetItemData(nPos,LGeometryData::EStress);
	nPos = m_wndBoundaryConditions.InsertString(-1,"Sym. and displacements");
	m_wndBoundaryConditions.SetItemData(nPos,LGeometryData::ESymAndDisplacement);
	nPos = m_wndBoundaryConditions.InsertString(-1,"Sym. and stresses");
	m_wndBoundaryConditions.SetItemData(nPos,LGeometryData::ESymAndStress);
	
	m_wndBoundaryConditions.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesDlg::OnScaligSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	m_shortScale = m_wndScalingSpin.GetPos();

	CString strText;
	strText.Format("%d:1",m_shortScale);

	m_wndScalingEdit.SetWindowText((LPCTSTR)strText);
	
	*pResult = 0;
}


void CPropertiesDlg::OnOK()
{
	m_eMode = static_cast<CGeometry2dView::ENodeDrawingMode>
		(m_wndNodeDrawingOptions.GetItemData(m_wndNodeDrawingOptions.GetCurSel()));	

	m_eBoundaryCondition = static_cast<LGeometryData::EBoundaryCondition>
		(m_wndBoundaryConditions.GetItemData(m_wndBoundaryConditions.GetCurSel()));

	static_cast<CMainFrame*>(theApp.m_pMainWnd)
		->ChangeProperties(m_shortScale,m_eMode,m_eBoundaryCondition);

	CExtResizableDialog::OnOK();
}

void CPropertiesDlg::SetScale(short scale)
{
	m_shortScale = scale;
	CString strText;
	strText.Format("%d:1",m_shortScale);
	m_wndScalingEdit.SetWindowText((LPCTSTR)strText);
}

void CPropertiesDlg::SetNodeDrawingMode(CGeometry2dView::ENodeDrawingMode mode)
{
	bool bFound = false;

	for ( int i = 0; i < m_wndNodeDrawingOptions.GetCount(); i++ )
	{
		if ( (CGeometry2dView::ENodeDrawingMode)m_wndNodeDrawingOptions.GetItemData(i) 
			== mode )
		{
			bFound = true;
			break;
		}
	}
	
	ASSERT(bFound);
	
	m_wndNodeDrawingOptions.SetCurSel(i);
	m_eMode = mode;
}

void CPropertiesDlg::SetBoundaryConditions(LGeometryData::EBoundaryCondition cond)
{
	m_eBoundaryCondition = cond;
	for ( int i = 0; i < m_wndBoundaryConditions.GetCount(); i++ )
		if ( cond == m_wndBoundaryConditions.GetItemData(i) )
		{
			m_wndBoundaryConditions.SetCurSel(i);
			return;
		}
}
