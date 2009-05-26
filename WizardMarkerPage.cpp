// WizardMarkerPage.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "WizardMarkerPage.h"
#include ".\wizardgeometrysheet.h"
#include ".\Geometry2dView.h"
#include ".\wizardmarkerpage.h"


// CWizardMarkerPage dialog

IMPLEMENT_DYNAMIC(CWizardMarkerPage, CExtResizablePropertyPage)
CWizardMarkerPage::CWizardMarkerPage()
	: CExtResizablePropertyPage(CWizardMarkerPage::IDD)
{
}

CWizardMarkerPage::~CWizardMarkerPage()
{
}

void CWizardMarkerPage::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardMarkerPage, CExtResizablePropertyPage)
	ON_BN_CLICKED(IDC_RADIO_MARKER1, OnBnClickedRadioMarker1)
	ON_BN_CLICKED(IDC_RADIO_MARKER2, OnBnClickedRadioMarker2)
END_MESSAGE_MAP()


// CWizardMarkerPage message handlers

BOOL CWizardMarkerPage::OnSetActive()
{
	CWnd * wnd = GetDlgItem(IDC_RADIO_MARKER1);

	CWizardGeometrySheet* pSheet = static_cast<CWizardGeometrySheet*>(GetParent());

	pSheet->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK );
	
	pSheet->GetView()->SetMode(CGeometry2dView::EMarker);
	
	return CExtResizablePropertyPage::OnSetActive();
}


void CWizardMarkerPage::OnBnClickedRadioMarker1()
{
	CWizardGeometrySheet* pSheet = static_cast<CWizardGeometrySheet*>(GetParent());
	pSheet->GetView()->SetMarker(0);
}

void CWizardMarkerPage::OnBnClickedRadioMarker2()
{
	CWizardGeometrySheet* pSheet = static_cast<CWizardGeometrySheet*>(GetParent());
	pSheet->GetView()->SetMarker(1);
}

BOOL CWizardMarkerPage::OnKillActive()
{
	CWizardGeometrySheet* pSheet = static_cast<CWizardGeometrySheet*>(GetParent());
//	pSheet->GetView()->SetMarker(-1);
	return CExtResizablePropertyPage::OnKillActive(); 
}
