// Geometry2dDoc.cpp : implementation of the CGeometry2dDoc class
//

#include "stdafx.h"
#include "Geometry2d.h"

#include "Geometry2dDoc.h"
#include ".\geometry2ddoc.h"
#include ".\Geometry2dView.h"

#include <fstream>
#include <stdexcept>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeometry2dDoc

IMPLEMENT_DYNCREATE(CGeometry2dDoc, CDocument)

BEGIN_MESSAGE_MAP(CGeometry2dDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
END_MESSAGE_MAP()


// CGeometry2dDoc construction/destruction

CGeometry2dDoc::CGeometry2dDoc()
{
	// TODO: add one-time construction code here

}

CGeometry2dDoc::~CGeometry2dDoc()
{
}

BOOL CGeometry2dDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CGeometry2dDoc serialization

void CGeometry2dDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CGeometry2dDoc diagnostics

#ifdef _DEBUG
void CGeometry2dDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGeometry2dDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGeometry2dDoc commands

BOOL CGeometry2dDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;


	return TRUE;
}

BOOL CGeometry2dDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	
//	std::ofstream file(lpszPathName);

//	return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}

void CGeometry2dDoc::OnFileSave()
{
	if ( !m_strPathName.GetLength() )
	{
		OnFileSaveAs();
	}
	else
	{
		std::ofstream file;
		
		file.open( (LPCTSTR)m_strPathName, std::ios_base::out | std::ios_base::trunc );

		file << m_geomData;

		file.flush();
		
		file.close();

	}
}

void CGeometry2dDoc::OnFileSaveAs()
{
	CFileDialog dlg(FALSE);
	char n[]="Node - Node text files\0*.,NOD;*.NODE\0All supported formats\0*.*\0";

	dlg.m_ofn.lpstrFilter=n;

	if ( dlg.DoModal() == IDOK )
	{	
		CString path = dlg.GetPathName();
		CString name = dlg.GetFileName();
		m_strPathName = path + ".node";
		
		OnFileSave();
	}

}

void CGeometry2dDoc::OnFileOpen()
{
	CFileDialog dlg(TRUE);

	char n[]="Node - Node text files\0*.,NOD;*.NODE\0All supported formats\0*.*\0";

	dlg.m_ofn.lpstrFilter=n;

	if ( dlg.DoModal() == IDOK )
	{	
		PrepareUndo();

		CString path = dlg.GetPathName();
	
		std::ifstream file;
		
		file.open( (LPCTSTR)path, std::ios_base::in );

		try 
		{

			file >> m_geomData;
		}
		catch ( std::logic_error& e)
		{
			AfxMessageBox(e.what());
		}
		file.close();

		POSITION pos = GetFirstViewPosition();
		if ( pos )
		{
			CGeometry2dView* pView = static_cast<CGeometry2dView*>(GetNextView(pos));
			if ( pView )
			{
				pView->PrepareBuffer();
				pView->RedrawWindow();
			}
		}
	}

}

void CGeometry2dDoc::PrepareUndo()
{
	m_geomUndoData = m_geomData;	
}

void CGeometry2dDoc::RestoreUndo()
{
	m_geomData = m_geomUndoData;

	POSITION pos = GetFirstViewPosition();
	if ( pos )
	{
		CGeometry2dView* pView = static_cast<CGeometry2dView*>(GetNextView(pos));
		if ( pView )
		{
			pView->PrepareBuffer();
			pView->RedrawWindow();
		}
	}
}

void CGeometry2dDoc::OnEditUndo()
{
	RestoreUndo();
}
