#pragma once

#include <Extresizabledialog.h>

// CWizardNodesPage dialog

class CWizardNodesPage : public CExtResizablePropertyPage 
{
	DECLARE_DYNAMIC(CWizardNodesPage)

public:
	CWizardNodesPage();
	virtual ~CWizardNodesPage();

// Dialog Data
	enum { IDD = IDD_GEOMETRY_WIZARD_NODES_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
};
