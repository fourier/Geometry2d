// WizardLinesPage.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "WizardLinesPage.h"
#include ".\wizardlinespage.h"
#include ".\wizardgeometrysheet.h"
#include ".\Geometry2dView.h"

// CWizardLinesPage dialog

IMPLEMENT_DYNAMIC(CWizardLinesPage, CExtResizablePropertyPage )
CWizardLinesPage::CWizardLinesPage()
	: CExtResizablePropertyPage (CWizardLinesPage::IDD)
{
}

CWizardLinesPage::~CWizardLinesPage()
{
}

void CWizardLinesPage::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage ::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardLinesPage, CExtResizablePropertyPage )
END_MESSAGE_MAP()


// CWizardLinesPage message handlers

BOOL CWizardLinesPage::OnSetActive()
{
	CWizardGeometrySheet* pSheet = static_cast<CWizardGeometrySheet*>(GetParent());

	pSheet->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK );
	
//	pSheet->GetView()->SetMode(CGeometry2dView::ELines);
	
	return CExtResizablePropertyPage ::OnSetActive();
}
