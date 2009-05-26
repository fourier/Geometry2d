// TriangulateOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "TriangulateOptionsDlg.h"
#include ".\triangulateoptionsdlg.h"
#include ".\geometrydata.h"

// CTriangulateOptionsDlg dialog

double CTriangulateOptionsDlg::s_fMinAngle = 25.2;
double CTriangulateOptionsDlg::s_fMaxSquare = 10;

IMPLEMENT_DYNAMIC(CTriangulateOptionsDlg, CExtResizableDialog)
CTriangulateOptionsDlg::CTriangulateOptionsDlg(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CTriangulateOptionsDlg::IDD, pParent),
	m_fMaxSquare(s_fMaxSquare),
	m_fMinAngle(s_fMinAngle),
	m_nDegree(LGeometryData::ESixNodes)
{
}

CTriangulateOptionsDlg::~CTriangulateOptionsDlg()
{
}

void CTriangulateOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_MIN_ANGLE, m_wndMinAngleEdit);
	DDX_Control(pDX, IDC_MAX_SQUARE, m_wndMaxSquareEdit);
	DDX_Control(pDX, IDC_COMBO_NODES_COUNT, m_wndNodesPerTriangle);
	CExtResizableDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTriangulateOptionsDlg, CExtResizableDialog)
END_MESSAGE_MAP()


// CTriangulateOptionsDlg message handlers

void CTriangulateOptionsDlg::OnOK()
{
	CString strText;

	m_wndMaxSquareEdit.GetWindowText(strText);

	m_fMaxSquare = atof((LPCTSTR)strText);
	if ( m_fMaxSquare == 0 || m_fMaxSquare < 0 )
	{
		AfxMessageBox("Wrong square size");
		return;
	}
	
	m_wndMinAngleEdit.GetWindowText(strText);

    m_fMinAngle = atof((LPCTSTR)strText);
	if ( m_fMinAngle > 33.5 || m_fMinAngle < 1 )
	{
		AfxMessageBox("Wrong angle");
		return;
	}

	m_nDegree = LGeometryData::EDegree(m_wndNodesPerTriangle.GetItemData(
		m_wndNodesPerTriangle.GetCurSel()));

	CExtResizableDialog::OnOK();
}

BOOL CTriangulateOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString strText;

	strText.Format("%f",m_fMaxSquare);

	m_wndMaxSquareEdit.SetWindowText((LPCTSTR)strText);
	
	strText.Format("%f",m_fMinAngle);

	m_wndMinAngleEdit.SetWindowText((LPCTSTR)strText);

	int nPos = m_wndNodesPerTriangle.AddString("6 nodes");
	m_wndNodesPerTriangle.SetItemData(nPos,LGeometryData::ESixNodes);
	nPos = m_wndNodesPerTriangle.AddString("3 nodes");
	m_wndNodesPerTriangle.SetItemData(nPos,LGeometryData::EThreeNodes);

	m_wndNodesPerTriangle.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

