// Geometry2dDoc.h : interface of the CGeometry2dDoc class
//


#pragma once
#include ".\GeometryData.h"

class CGeometry2dDoc : public CDocument
{
protected: // create from serialization only
	CGeometry2dDoc();
	DECLARE_DYNCREATE(CGeometry2dDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGeometry2dDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	inline LGeometryData& GetData() { return m_geomData;};

	void PrepareUndo();
	void RestoreUndo();
protected:
	LGeometryData m_geomData;
	LGeometryData m_geomUndoData;
	CString m_strPathName;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileOpen();
	afx_msg void OnEditUndo();
};


