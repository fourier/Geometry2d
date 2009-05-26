#pragma once


#include <Extresizabledialog.h>
// CWizardHolesPage dialog

class CWizardHolesPage : public CExtResizablePropertyPage
{
	DECLARE_DYNAMIC(CWizardHolesPage)

public:
	CWizardHolesPage();
	virtual ~CWizardHolesPage();

// Dialog Data
	enum { IDD = IDD_GEOMETRY_WIZARD_HOLE_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
