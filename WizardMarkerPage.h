#pragma once


#include <Extresizabledialog.h>
// CWizardMarkerPage dialog

class CWizardMarkerPage : public CExtResizablePropertyPage
{
	DECLARE_DYNAMIC(CWizardMarkerPage)

public:
	CWizardMarkerPage();
	virtual ~CWizardMarkerPage();

// Dialog Data
	enum { IDD = IDD_GEOMETRY_WIZARD_MARKER_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedRadioMarker1();
	afx_msg void OnBnClickedRadioMarker2();
	virtual BOOL OnKillActive();
};
