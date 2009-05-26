#pragma once

#include <Extresizabledialog.h>

// CWizardLinesPage dialog

class CWizardLinesPage : public CExtResizablePropertyPage 
{
	DECLARE_DYNAMIC(CWizardLinesPage)

public:
	CWizardLinesPage();
	virtual ~CWizardLinesPage();

// Dialog Data
	enum { IDD = IDD_GEOMETRY_WIZARD_LINES_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
};
