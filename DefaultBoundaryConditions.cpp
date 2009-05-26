// DefaultBoundaryConditions.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "DefaultBoundaryConditions.h"
#include ".\defaultboundaryconditions.h"



// CDefaultBoundaryConditions dialog

IMPLEMENT_DYNAMIC(CDefaultBoundaryConditions, CExtResizableDialog)
CDefaultBoundaryConditions::CDefaultBoundaryConditions(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CDefaultBoundaryConditions::IDD, pParent),
	m_bIsStress(false)
{
}

CDefaultBoundaryConditions::~CDefaultBoundaryConditions()
{
}

void CDefaultBoundaryConditions::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_STATIC_BOUNDARY_TYPE,m_wndStatic);
	DDX_Control(pDX,IDC_EDIT_Z1_,m_wndZ1);
	DDX_Control(pDX,IDC_EDIT_Z2_,m_wndZ2);
	DDX_Control(pDX,IDOK,m_btOk);
	DDX_Control(pDX,IDC_PRESCX,m_wndPrescX);
	DDX_Control(pDX,IDC_PRESCY,m_wndPrescY);

	CExtResizableDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDefaultBoundaryConditions, CExtResizableDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_PRESCX, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_PRESCY, OnBnClickedCheck2)
END_MESSAGE_MAP()


// CDefaultBoundaryConditions message handlers

void CDefaultBoundaryConditions::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CString str;
	
	m_wndZ1.GetWindowText(str);
	m_z1 = atof(str);

	m_wndZ2.GetWindowText(str);
	m_z2 = atof(str);

	if ( m_wndPrescX.GetCheck() == BST_UNCHECKED && m_wndPrescY.GetCheck() == BST_UNCHECKED )
		m_doffixed = 0;
	else if ( m_wndPrescX.GetCheck() == BST_CHECKED && m_wndPrescY.GetCheck() == BST_CHECKED )
		m_doffixed = 3;
	else if ( m_wndPrescX.GetCheck() == BST_CHECKED && m_wndPrescY.GetCheck() == BST_UNCHECKED )
		m_doffixed = 1;
	else
		m_doffixed = 2;

	OnOK();
}

BOOL CDefaultBoundaryConditions::OnInitDialog()
{
	CExtResizableDialog::OnInitDialog();

	if ( m_bIsStress )
		m_wndStatic.SetWindowText("Select default stresss, S1 and S2");
	else 
		m_wndStatic.SetWindowText("Select default displacements, u1 and u2");
	
	m_wndZ1.SetWindowText("0.0");
	m_wndZ2.SetWindowText("0.0");
	m_wndPrescX.SetCheck(BST_CHECKED);
	m_wndPrescY.SetCheck(BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDefaultBoundaryConditions::OnBnClickedCheck1()
{
	m_wndZ1.EnableWindow(m_wndPrescX.GetCheck() == BST_CHECKED);
}

void CDefaultBoundaryConditions::OnBnClickedCheck2()
{
	m_wndZ2.EnableWindow(m_wndPrescY.GetCheck() == BST_CHECKED);
}
