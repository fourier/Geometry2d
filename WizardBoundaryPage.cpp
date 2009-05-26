// WizardBoundaryPage.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "WizardBoundaryPage.h"
#include ".\wizardboundarypage.h"

#include ".\wizardgeometrysheet.h"
#include ".\Geometry2dView.h"

// CWizardBoundaryPage dialog

IMPLEMENT_DYNAMIC(CWizardBoundaryPage, CExtResizablePropertyPage )
CWizardBoundaryPage::CWizardBoundaryPage()
	: CExtResizablePropertyPage (CWizardBoundaryPage::IDD)
{
}

CWizardBoundaryPage::~CWizardBoundaryPage()
{
}

void CWizardBoundaryPage::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage ::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardBoundaryPage, CExtResizablePropertyPage )
END_MESSAGE_MAP()


// CWizardBoundaryPage message handlers

BOOL CWizardBoundaryPage::OnSetActive()
{
	CWizardGeometrySheet* pSheet = static_cast<CWizardGeometrySheet*>(GetParent());

	pSheet->SetWizardButtons(PSWIZB_FINISH | PSWIZB_BACK );

//	pSheet->GetView()->SetMode(CGeometry2dView::EBoundary);

	return CExtResizablePropertyPage ::OnSetActive();
}

BOOL CWizardBoundaryPage::OnWizardFinish()
{
	return CExtResizablePropertyPage::OnWizardFinish();
}
