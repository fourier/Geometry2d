#pragma once

#include <Extresizabledialog.h>


class CGeometry2dView;

// CWizardGeometrySheet

class CWizardGeometrySheet : public CExtResizablePropertySheet 
{
	DECLARE_DYNAMIC(CWizardGeometrySheet)

public:
	CWizardGeometrySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CWizardGeometrySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CWizardGeometrySheet();
	
	inline void SetView(CGeometry2dView* pView){ m_pView = pView; };
	inline CGeometry2dView* GetView() const { return m_pView; };
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL DestroyWindow();
protected:
	CGeometry2dView* m_pView;
};


