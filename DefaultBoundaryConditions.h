#pragma once


#include <Extresizabledialog.h>
#include <ExtEdit.h>
#include <ExtLabel.h>
#include <ExtBtnOnFlat.h>

// CDefaultBoundaryConditions dialog

class CDefaultBoundaryConditions : public CExtResizableDialog
{
	DECLARE_DYNAMIC(CDefaultBoundaryConditions)

public:
	CDefaultBoundaryConditions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDefaultBoundaryConditions();

// Dialog Data
	enum { IDD = IDD_DEFAULT_BOUNDARY_OPTIONS };

	inline const double GetZ1(void) const { return m_z1;};
	inline const double GetZ2(void) const { return m_z2;};
	inline const int GetDofType(void) const { return m_doffixed;};

	inline void SetConditionType(bool bIsStress) { m_bIsStress = bIsStress;};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CExtLabel m_wndStatic;
	CExtEdit m_wndZ1;
	CExtEdit m_wndZ2;
	CExtBtnOnFlat m_btOk;

	CExtCheckBox m_wndPrescX;
	CExtCheckBox m_wndPrescY;


	double m_z1;
	double m_z2;
	int m_doffixed;
	bool m_bIsStress;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
};
