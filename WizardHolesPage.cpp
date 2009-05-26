// WizardHolesPage.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "WizardHolesPage.h"
#include ".\wizardgeometrysheet.h"
#include ".\Geometry2dView.h"
#include ".\wizardholespage.h"


// CWizardHolesPage dialog

IMPLEMENT_DYNAMIC(CWizardHolesPage, CExtResizablePropertyPage)
CWizardHolesPage::CWizardHolesPage()
	: CExtResizablePropertyPage(CWizardHolesPage::IDD)
{
}

CWizardHolesPage::~CWizardHolesPage()
{
}

void CWizardHolesPage::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardHolesPage, CExtResizablePropertyPage)
END_MESSAGE_MAP()


// CWizardHolesPage message handlers

BOOL CWizardHolesPage::OnSetActive()
{
	CWizardGeometrySheet* pSheet = static_cast<CWizardGeometrySheet*>(GetParent());

	pSheet->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK );
	
//	pSheet->GetView()->SetMode(CGeometry2dView::EHoles);
	
	return CExtResizablePropertyPage ::OnSetActive();
}

BOOL CWizardHolesPage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CExtResizablePropertyPage::OnNotify(wParam,lParam,pResult);
}
