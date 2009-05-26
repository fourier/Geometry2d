// NodePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "NodePropertiesDlg.h"
#include ".\nodepropertiesdlg.h"
#include ".\mainfrm.h"

// CNodePropertiesDlg dialog

IMPLEMENT_DYNAMIC(CNodePropertiesDlg, CExtResizableDialog)
CNodePropertiesDlg::CNodePropertiesDlg(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CNodePropertiesDlg::IDD, pParent)
{

	m_fX = 0;
	m_fY = 0;
	m_fZ1 = 0;
	m_fZ2 = 0;
	m_ndoftype= 3;
	m_bSymmetric = false;
	m_bBoundary = false;
	m_bStressed = false;
	m_bDeleteAllContaining = false;
}

CNodePropertiesDlg::~CNodePropertiesDlg()
{
}

void CNodePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_NODE_X_COORD,m_wndEditX);
	DDX_Control(pDX,IDC_NODE_Y_COORD,m_wndEditY);
	DDX_Control(pDX,IDC_REMOVE_NODE_FROM_ALL,m_wndCheckDeleteAll);
	DDX_Control(pDX,IDC_COMBO_BOUNDARY_TYPE,m_wndBoundaryCombo);
	DDX_Control(pDX,IDC_APPLY_BUTTON,m_wndApplyButton);
	DDX_Control(pDX,IDC_CHECK_SYMMETRY_CONDITION,m_wndSymmetricCondition);
	DDX_Control(pDX,IDC_STATIC_BOUNDARY_CONDITION_TEXT,m_wndStatic);
	DDX_Control(pDX,IDC_EDIT_Z1,m_wndZ1);
	DDX_Control(pDX,IDC_EDIT_Z2,m_wndZ2);
	DDX_Control(pDX,IDC_USE1,m_wndPrescX);
	DDX_Control(pDX,IDC_USE2,m_wndPrescY);

	CExtResizableDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNodePropertiesDlg, CExtResizableDialog)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, OnBnClickedApplyButton)
	ON_BN_CLICKED(IDC_USE1, OnBnClickedUse1)
	ON_BN_CLICKED(IDC_USE2, OnBnClickedUse2)
END_MESSAGE_MAP()


// CNodePropertiesDlg message handlers

BOOL CNodePropertiesDlg::OnInitDialog()
{
	CExtResizableDialog::OnInitDialog();

	int nPos = m_wndBoundaryCombo.AddString("Inner");
	m_wndBoundaryCombo.SetItemData(nPos,LGeometryData::EInner);
	nPos = m_wndBoundaryCombo.AddString("Outer");
	m_wndBoundaryCombo.SetItemData(nPos,LGeometryData::EOuter);
	
	m_wndBoundaryCombo.SetCurSel(0);

	m_wndCheckDeleteAll.EnableWindow(FALSE);
	m_wndEditX.EnableWindow(FALSE);
	m_wndEditY.EnableWindow(FALSE);
	m_wndBoundaryCombo.EnableWindow(FALSE);
	m_wndApplyButton.EnableWindow(FALSE);

	m_wndSymmetricCondition.EnableWindow(FALSE);

	m_wndStatic.SetWindowText("No conditions");
	
	m_wndZ1.EnableWindow(FALSE);
	m_wndZ2.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNodePropertiesDlg::DisableAll()
{
	m_wndCheckDeleteAll.SetCheck(0);
	m_wndBoundaryCombo.SetCurSel(0);
	m_wndBoundaryCombo.EnableWindow(FALSE);
	m_wndCheckDeleteAll.SetCheck(0);
	m_wndCheckDeleteAll.EnableWindow(FALSE);
	m_wndEditX.SetWindowText("");
	m_wndEditY.SetWindowText("");
	m_wndEditX.EnableWindow(FALSE);
	m_wndEditY.EnableWindow(FALSE);
	m_wndApplyButton.EnableWindow(FALSE);

	m_wndStatic.SetWindowText("No conditions");
	m_wndZ1.SetWindowText("");
	m_wndZ2.SetWindowText("");
	m_wndZ1.EnableWindow(FALSE);
	m_wndZ2.EnableWindow(FALSE);
	m_wndPrescX.EnableWindow(FALSE);
	m_wndPrescY.EnableWindow(FALSE);

	m_nodeCurrent = NULL;
}

void CNodePropertiesDlg::EnableAll()
{
	m_wndBoundaryCombo.EnableWindow(TRUE);
	m_wndCheckDeleteAll.EnableWindow(TRUE);
	m_wndEditX.EnableWindow(TRUE);
	m_wndEditY.EnableWindow(TRUE);
	m_wndApplyButton.EnableWindow(TRUE);
	m_wndCheckDeleteAll.SetCheck(0);
	m_wndPrescX.EnableWindow(TRUE);
	m_wndPrescY.EnableWindow(TRUE);
}


void CNodePropertiesDlg::SetNode(LGeometryData::NodesConstIterator it, bool bIsInSymmetric,
		bool bBoundary, bool bIsStresses, double Z1,double Z2,int doftype)
{
	m_nodeCurrent = it;
	
	m_bSymmetric = bIsInSymmetric;

	if ( (m_bBoundary = bBoundary ) == true )
	{
		m_bStressed = bIsStresses;
		m_fZ1 = Z1;
		m_fZ2 = Z2;
		m_ndoftype = doftype;
	}
	UpdateAll();
}

void CNodePropertiesDlg::UpdateAll() // ATTENTION!!! Dangerous function!!!
{
	if ( m_nodeCurrent != NULL )
	{
		CString strText;

		EnableAll();
		for ( int nPos = 0; nPos < m_wndBoundaryCombo.GetCount(); nPos++ )
		{
			if ( m_wndBoundaryCombo.GetItemData(nPos) == m_nodeCurrent->bnd )
			{
				m_wndBoundaryCombo.SetCurSel(nPos);
				break;
			}
		}

		if ( m_bSymmetric )
			m_wndSymmetricCondition.SetCheck(1);
		else
			m_wndSymmetricCondition.SetCheck(0);

		if ( !m_bBoundary )
		{
			m_wndZ1.EnableWindow(FALSE);
			m_wndZ1.SetWindowText("");
			m_wndZ2.EnableWindow(FALSE);
			m_wndZ2.SetWindowText("");
			m_wndStatic.SetWindowText("No conditions");
			m_wndPrescX.EnableWindow(FALSE);
			m_wndPrescY.EnableWindow(FALSE);
		}
		else
		{
			m_wndZ1.EnableWindow(TRUE);
			m_wndZ2.EnableWindow(TRUE);
			
			strText.Format("%.4f",m_fZ1);
			m_wndZ1.SetWindowText(strText);
			strText.Format("%.4f",m_fZ2);
			m_wndZ2.SetWindowText(strText);

			if ( m_bStressed )
				m_wndStatic.SetWindowText("Stresses, S1 and S2:");
			else
				m_wndStatic.SetWindowText("Displacements, u1 and u2:");
			m_wndPrescX.EnableWindow(TRUE);
			m_wndPrescY.EnableWindow(TRUE);

			switch (m_ndoftype)
			{
			case 0:
				m_wndPrescX.SetCheck(BST_UNCHECKED);
				m_wndPrescY.SetCheck(BST_UNCHECKED);
				break;
			case 1:
				m_wndPrescX.SetCheck(BST_CHECKED);
				m_wndPrescY.SetCheck(BST_UNCHECKED);
				break;
			case 2:
				m_wndPrescX.SetCheck(BST_UNCHECKED);
				m_wndPrescY.SetCheck(BST_CHECKED);
				break;
			case 3:
				m_wndPrescX.SetCheck(BST_CHECKED);
				m_wndPrescY.SetCheck(BST_CHECKED);
				break;
			};

		}
		
		strText.Format("%.4f",m_nodeCurrent->x);
		m_wndEditX.SetWindowText((LPCTSTR)strText);
		strText.Format("%.4f",m_nodeCurrent->y);
		m_wndEditY.SetWindowText((LPCTSTR)strText);

		m_wndCheckDeleteAll.SetCheck(0);

	}
}


void CNodePropertiesDlg::OnBnClickedApplyButton()
{
	CString strText;
	
	m_wndEditX.GetWindowText(strText);
	m_fX = atof(strText);
	m_wndEditY.GetWindowText(strText);
	m_fY = atof(strText);
	
	m_wndZ1.GetWindowText(strText);
	m_fZ1 = atof(strText);
	m_wndZ2.GetWindowText(strText);
	m_fZ2 = atof(strText);
	SetBoundaryTypeState();

	m_bDeleteAllContaining = m_wndCheckDeleteAll.GetCheck() == 1 ? true:false;

	m_eBoundary = (LGeometryData::EBoundaryType)
		m_wndBoundaryCombo.GetItemData(m_wndBoundaryCombo.GetCurSel());

	if ( !m_bBoundary )
		static_cast<CMainFrame*>(theApp.m_pMainWnd)->ApplyNodesChange(
			m_nodeCurrent,m_fX,m_fY,m_eBoundary,m_bDeleteAllContaining);
	else
		static_cast<CMainFrame*>(theApp.m_pMainWnd)->ApplyNodesChange(
			m_nodeCurrent,m_fX,m_fY,m_eBoundary,m_bDeleteAllContaining,m_fZ1,m_fZ2,m_ndoftype);

	m_wndCheckDeleteAll.SetCheck(0);
}



void CNodePropertiesDlg::OnBnClickedUse1()
{
	m_wndZ1.EnableWindow(m_wndPrescX.GetCheck() == BST_CHECKED);
}

void CNodePropertiesDlg::OnBnClickedUse2()
{
	m_wndZ2.EnableWindow(m_wndPrescY.GetCheck() == BST_CHECKED);
}


void CNodePropertiesDlg::SetBoundaryTypeState(void)
{
	if ( m_wndPrescX.GetCheck() == BST_UNCHECKED && m_wndPrescY.GetCheck() == BST_UNCHECKED )
		m_ndoftype = 0;
	else if ( m_wndPrescX.GetCheck() == BST_CHECKED && m_wndPrescY.GetCheck() == BST_CHECKED )
		m_ndoftype = 3;
	else if ( m_wndPrescX.GetCheck() == BST_CHECKED && m_wndPrescY.GetCheck() == BST_UNCHECKED )
		m_ndoftype = 1;
	else
		m_ndoftype = 2;
}