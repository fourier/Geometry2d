#pragma once

#include <ExtEdit.h>
#include <Extresizabledialog.h>
#include <Extcombobox.h>
#include ".\geometrydata.h"

// CTriangulateOptionsDlg dialog

class CTriangulateOptionsDlg : public CExtResizableDialog
{
	DECLARE_DYNAMIC(CTriangulateOptionsDlg)

public:
	CTriangulateOptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTriangulateOptionsDlg();

// Dialog Data
	enum { IDD = IDD_TRIANGULATE_OPTIONS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

	CExtEdit m_wndMinAngleEdit;
	CExtEdit m_wndMaxSquareEdit;
	CExtComboBox m_wndNodesPerTriangle;

	double m_fMinAngle;
	double m_fMaxSquare;
	LGeometryData::EDegree m_nDegree;

	static double s_fMinAngle;
	static double s_fMaxSquare;
public:
	virtual BOOL OnInitDialog();

	inline const double GetMinAngle() const { return m_fMinAngle;};
	inline const double GetMaxSquare() const { return m_fMaxSquare;};
	inline const LGeometryData::EDegree GetDegree() const { return m_nDegree;};
};
