#pragma once

#include <Extresizabledialog.h>
#include <ExtEdit.h>
#include <Extcombobox.h>

#include ".\Geometry2dview.h"
#include ".\GeometryData.h"

// CPropertiesDlg dialog

class CPropertiesDlg : public CExtResizableDialog 
{
	DECLARE_DYNAMIC(CPropertiesDlg)

public:
	CPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropertiesDlg();

// Dialog Data
	enum { IDD = IDD_PROPERTIES_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	afx_msg void OnScaligSpinChange(NMHDR* pNMHDR, LRESULT* pResult);

	CExtEdit m_wndScalingEdit;
	CSpinButtonCtrl m_wndScalingSpin;
	CExtComboBox m_wndNodeDrawingOptions;	
	CExtComboBox m_wndBoundaryConditions;

	static short s_nInititalScale;
	short m_shortScale;

	LGeometryData::EBoundaryCondition m_eBoundaryCondition;

	CGeometry2dView::ENodeDrawingMode m_eMode;
public:
	
	virtual BOOL OnInitDialog();
	
	inline const short GetScale(void) const
	{
		return m_shortScale;
	};
	
	inline const CGeometry2dView::ENodeDrawingMode GetNodeDrawingMode(void) const
	{
		return m_eMode;
	};

	void SetScale(short scale);
	void SetNodeDrawingMode(CGeometry2dView::ENodeDrawingMode mode);
	void SetBoundaryConditions(LGeometryData::EBoundaryCondition cond);

protected:
	virtual void OnOK();
};
