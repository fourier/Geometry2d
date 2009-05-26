#pragma once

#include <Extresizabledialog.h>

// CWizardBoundaryPage dialog

class CWizardBoundaryPage : public CExtResizablePropertyPage 
{
	DECLARE_DYNAMIC(CWizardBoundaryPage)

public:
	CWizardBoundaryPage();
	virtual ~CWizardBoundaryPage();

// Dialog Data
	enum { IDD = IDD_GEOMETRY_WIZARD_BOUNDARY_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
};
