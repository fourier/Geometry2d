#pragma once

#include <Extresizabledialog.h>
#include <ExtEdit.h>
#include <ExtButton.h>
#include <ExtBtnOnFlat.h>
#include <ExtLabel.h>

#include ".\geometrydata.h"

// CNodePropertiesDlg dialog


class CNodePropertiesDlg : public CExtResizableDialog
{
	DECLARE_DYNAMIC(CNodePropertiesDlg)

public:
	CNodePropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNodePropertiesDlg();

	void SetNode(LGeometryData::NodesConstIterator it, bool bIsInSymmetric,
		bool bBoundary, bool bIsStresses, double Z1=0,double Z2=0,int doftype = 3);
	

	void DisableAll();
	void UpdateAll();

	const double GetX() const { return m_fX;};
	const double GetY() const { return m_fY;};
	const LGeometryData::EBoundaryType GetBoundary() const { return m_eBoundary;};
	const bool GetDeleteAllContaining() const { return m_bDeleteAllContaining;};

// Dialog Data
	enum { IDD = IDD_NODE_PROPERTIES };

protected:
	CExtEdit m_wndEditX;
	CExtEdit m_wndEditY;
	CExtCheckBox m_wndCheckDeleteAll;
	CExtBtnOnFlat m_wndApplyButton;
	CExtComboBox m_wndBoundaryCombo;
//  Boundary condition controls
	CExtCheckBox m_wndSymmetricCondition;
//	CExtComboBox m_wndBoundaryConditionCombo;
	CExtLabel m_wndStatic;
	CExtEdit m_wndZ1;
	CExtEdit m_wndZ2;
	CExtCheckBox m_wndPrescX;
	CExtCheckBox m_wndPrescY;

	LGeometryData::NodesConstIterator m_nodeCurrent;

	double m_fX;
	double m_fY;
	LGeometryData::EBoundaryType m_eBoundary;
	
	bool m_bDeleteAllContaining;

	bool m_bBoundary;

	bool m_bSymmetric;

	bool m_bStressed;

	double m_fZ1;

	double m_fZ2;

	int m_ndoftype;

	void EnableAll();
	void SetBoundaryTypeState(void);	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApplyButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedUse1();
	afx_msg void OnBnClickedUse2();
};
