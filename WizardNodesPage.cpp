// WizardNodesPage.cpp : implementation file
//

#include "stdafx.h"
#include "Geometry2d.h"
#include "WizardNodesPage.h"
#include ".\wizardnodespage.h"
#include ".\wizardgeometrysheet.h"
#include ".\Geometry2dView.h"

// CWizardNodesPage dialog

IMPLEMENT_DYNAMIC(CWizardNodesPage, CExtResizablePropertyPage )
CWizardNodesPage::CWizardNodesPage()
	: CExtResizablePropertyPage (CWizardNodesPage::IDD)
{
}

CWizardNodesPage::~CWizardNodesPage()
{
}

void CWizardNodesPage::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage ::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardNodesPage, CExtResizablePropertyPage )
END_MESSAGE_MAP()


// CWizardNodesPage message handlers

BOOL CWizardNodesPage::OnSetActive()
{
	CWizardGeometrySheet* pSheet = static_cast<CWizardGeometrySheet*>(GetParent());

	pSheet->SetWizardButtons(PSWIZB_NEXT);
//	pSheet->GetView()->SetMode(CGeometry2dView::ENodes);
	return CExtResizablePropertyPage ::OnSetActive();
}
