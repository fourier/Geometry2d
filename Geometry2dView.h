// Geometry2dView.h : interface of the CGeometry2dView class
//


#pragma once

#include ".\Geometrydata.h"

class CGeometry2dDoc;

class CGeometry2dView : public CView
{
protected: // create from serialization only
	CGeometry2dView();
	DECLARE_DYNCREATE(CGeometry2dView)
	
// Attributes
public:
	CGeometry2dDoc* GetDocument() const;
	void PrepareBuffer();
// Operations
public:
	enum EMode 
	{
		ENodes,
		ELines,
		EBoundary,
		ESymmetry,
		EMarker,
		EHoles,
		ESelectNode,
		EDrawing,
		ENone
	};

	enum EDrawingMode 
	{
		EPrimitives,
		ESK,
		ENotDrawingMode
	};

	enum EPrimitivesMode
	{
		ELinePrimitive,
		ECurvePrimitive,
		ENotPrimitivesMode
	};

	enum ENodeDrawingMode 
	{
		EBox,
		ECross,
		ECircle,
		EDot,
		ENotDraw
	};

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

	void GeometryProperties(short scale,ENodeDrawingMode mode,
		LGeometryData::EBoundaryCondition cond);
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// Implementation
public:
	virtual ~CGeometry2dView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// Getters/Setters
	inline EMode GetMode() const { return m_eMode; };
	void SetMode(EMode eMode);
	
	inline LGeometryData::EBoundaryType GetMarker() const { return m_nMarker; };
	inline void SetMarker(LGeometryData::EBoundaryType marker) 
	{
		m_nMarker = marker;
	};
	inline void FlushMode()
	{
		m_eDrawingMode = ENotDrawingMode;
		m_ePrimitivesMode = ENotPrimitivesMode;
//		m_PrimitivesModePressed.m_bPressed = false;
	};

protected:
	EMode m_eMode;
	ENodeDrawingMode m_eNodesDrawingMode;
	EDrawingMode m_eDrawingMode;
	EPrimitivesMode m_ePrimitivesMode;

	struct DefaultBoundaryValues 
	{
		double z1;
		double z2;
		int doftype;
		DefaultBoundaryValues(double Z1=0,double Z2=0,int doftyp=3)
			: z1(Z1),z2(Z2),doftype(doftyp){};
	};

	DefaultBoundaryValues m_defBndValues;

	LGeometryData::NodesConstIterator m_nodeSelected;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
private:
	void DrawSK(CDC* pDC);
	void DrawNodes(CDC* pDC);
	void DrawLines(CDC* pDC);
	void DrawCross(CDC* pDC,const CPoint& center);
	void DrawNode(CDC* pDC,LGeometryData::NodesConstIterator it,COLORREF rgbBox);
	void DrawLine(CDC* pDC,CPoint from,CPoint to,COLORREF color);
	void DrawTriangles(CDC* pDC);
	void DrawHoles(CDC *pDC);
	void DrawHole(CDC *pDC,LGeometryData::NodesConstIterator it, COLORREF rgbClr);
	void DrawSymmetry(CDC* pDC);
	void DrawBoundary(CDC* pDC);

	void CreateTsarevCurve(const CPoint& point);

	inline CPoint LNode2CPoint(LGeometryData::LNode node)
	{
		CPoint result;
		result.x = int(node.x*m_nScaling) - m_orgSK.x;
		result.y = int(node.y*m_nScaling) - m_orgSK.y;
		return result;
	};

	inline LGeometryData::LNode CPoint2LNode(CPoint point)
	{
		LGeometryData::LNode result;
		result.x = double((double)(point.x+m_orgSK.x)/(double)m_nScaling);
		result.y = double((double)(point.y+m_orgSK.y)/(double)m_nScaling);
		return result;
	};

	struct PRESSED
	{
		PRESSED(CPoint pnt=CPoint(0,0),bool isPressed=false):
			m_point(pnt),m_bPressed(isPressed)
		{
		};
		CPoint m_point;
		bool m_bPressed;
	};

	PRESSED m_LinesModePressed;
	PRESSED m_SymmetryModePressed;
	PRESSED m_BoundaryModePressed;
	PRESSED m_PrimitivesModePressed;

	CPoint m_currentPoint;
	CPoint m_orgSK;
	
	CDC m_dcNodes;
	CDC m_dcLines;

	CBitmap m_bmpNodes;
	CBitmap m_bmpLines;

	LGeometryData::EBoundaryType m_nMarker;

	int m_nScaling;

	static void DrawPointer(CDC* pDC, const CPoint& pnt1, const CPoint& pnt2,
		COLORREF color);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnGeometryAddNode();
	afx_msg void OnGeometryAddSegment();
	afx_msg void OnGeometrySetMarker1();
	afx_msg void OnGeometrySetMarker2();
	afx_msg void OnGeometrySetHoles();
	afx_msg void OnTriangulateCurrentView();
	afx_msg void OnGeometryProperties();
	afx_msg void SetSelectNodesMode();
	afx_msg void OnBoundaryLine();
	afx_msg void OnBoundaryCurve();
	afx_msg void OnLinePrimitive();
	afx_msg void OnCurvePrimitive();
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnGeometryNodeoptions();
	afx_msg void OnGeometryExporttomodule();
	afx_msg void OnGeometryChangesk();
};

#ifndef _DEBUG  // debug version in Geometry2dView.cpp
inline CGeometry2dDoc* CGeometry2dView::GetDocument() const
   { return reinterpret_cast<CGeometry2dDoc*>(m_pDocument); }
#endif

