// Geometry2dView.cpp : implementation of the CGeometry2dView class
//

#include "stdafx.h"
#include "winsock2.h"
#include "Geometry2d.h"


#include "Geometry2dDoc.h"
#include "Geometry2dView.h"
#include ".\geometry2dview.h"
#include ".\TriangulateOptionsDlg.h"
#include ".\PropertiesDlg.h"
#include ".\MainFrm.h"
#include ".\DefaultBoundaryConditions.h"


#include <vector>
#include <cmath>
#include <sstream>
#include <fstream>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ABSOLUTE_PATH
// CGeometry2dView

IMPLEMENT_DYNCREATE(CGeometry2dView, CView)

BEGIN_MESSAGE_MAP(CGeometry2dView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BUTTON_NODE,OnGeometryAddNode)
	ON_COMMAND(ID_BUTTON_SEGMENT,OnGeometryAddSegment)
	ON_COMMAND(ID_BUTTON_MARKER1,OnGeometrySetMarker1)
	ON_COMMAND(ID_BUTTON_MARKER2,OnGeometrySetMarker2)
	ON_COMMAND(ID_BUTTON_HOLE,OnGeometrySetHoles)
	ON_COMMAND(ID_TRIANGULATE_CURRENT_VIEW, OnTriangulateCurrentView)
	ON_COMMAND(ID_BUTTON_TRIANGULATE, OnTriangulateCurrentView)
	ON_COMMAND(ID_GEOMETRY_PROPERTIES, OnGeometryProperties)
	ON_COMMAND(ID_BUTTON_PROPERTIES,OnGeometryProperties)
	ON_COMMAND(ID_GEOMETRY_NODEOPTIONS, OnGeometryNodeoptions)
	ON_COMMAND(ID_BUTTON_NODE_OPTIONS, OnGeometryNodeoptions)
	ON_COMMAND(ID_BUTTON_NODE_CHECK, SetSelectNodesMode)
	ON_COMMAND(ID_BUTTON_BOUNDARY_LINE, OnBoundaryLine)
	ON_COMMAND(ID_BUTTON_BOUNDARY_CURVE, OnBoundaryCurve)
	ON_COMMAND(ID_GEOMETRY_EXPORTTOMODULE, OnGeometryExporttomodule)
	ON_COMMAND(ID_BUTTON_LINE_PRIMITIVE, OnLinePrimitive)
	ON_COMMAND(ID_BUTTON_CURVE_PRIMITIVE, OnCurvePrimitive)
	ON_COMMAND(ID_GEOMETRY_CHANGESK, OnGeometryChangesk)
END_MESSAGE_MAP()
// CGeometry2dView construction/destruction

CGeometry2dView::CGeometry2dView()
	: m_eMode(ENone), 
	  m_eNodesDrawingMode(EBox),
	  m_defBndValues(0,0),
	  m_nMarker(LGeometryData::ENotMarked),
	  m_nScaling(10),
	  m_eDrawingMode(ENotDrawingMode),
	  m_ePrimitivesMode(ENotPrimitivesMode)
{

	m_LinesModePressed.m_bPressed = false;
	m_SymmetryModePressed.m_bPressed = false;
	m_BoundaryModePressed.m_bPressed = false;
	m_orgSK.x = -100;
	m_orgSK.y = -100;
}

CGeometry2dView::~CGeometry2dView()
{
}

BOOL CGeometry2dView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGeometry2dView drawing

void CGeometry2dView::OnDraw(CDC* pDC)
{
	
	CGeometry2dDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
    GetClientRect(&rect);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcNodes, 
		0, 0, SRCCOPY);	

	switch(m_eMode)
	{
	case EBoundary:
		{
			if ( m_BoundaryModePressed.m_bPressed )
			{
				DrawNode(pDC,pDoc->GetData().GetCorrespondingNode(CPoint2LNode(
					m_BoundaryModePressed.m_point)),RGB(0,50,255));
				LGeometryData::NodesConstIterator it = 
					pDoc->GetData().GetCorrespondingNode(
						CPoint2LNode(m_currentPoint));
				if ( it != pDoc->GetData().GetNodesArray().end() )
					DrawNode(pDC,it,RGB(0,50,255));
			}	
		}
		break;
	case ELines:
		{
			if ( m_LinesModePressed.m_bPressed )
			{
				DrawLine(pDC,m_LinesModePressed.m_point,m_currentPoint,
					RGB(0,0,255));
				LGeometryData::NodesConstIterator it = 
					pDoc->GetData().GetCorrespondingNode(
						CPoint2LNode(m_currentPoint));
				if ( it != pDoc->GetData().GetNodesArray().end() )
					DrawNode(pDC,it,RGB(255,0,0));
			}
		}
		break;
	case ENodes:
		{
			LGeometryData::NodesConstIterator it = 
				pDoc->GetData().GetCorrespondingNode(
					CPoint2LNode(m_currentPoint));
			if ( it != pDoc->GetData().GetNodesArray().end() )
				DrawNode(pDC,it,RGB(255,0,0));
		}
		break;
	case ESelectNode:
		if ( m_nodeSelected != pDoc->GetData().GetNodesArray().end() )
		{
			CPoint center = LNode2CPoint(*m_nodeSelected);
			DrawCross(pDC,center);
		}
		break;
	case ESymmetry:
		{
			if ( m_SymmetryModePressed.m_bPressed )
			{
				DrawNode(pDC,pDoc->GetData().GetCorrespondingNode(CPoint2LNode(
				m_SymmetryModePressed.m_point)),RGB(255,255,0));
			LGeometryData::NodesConstIterator it = 
				pDoc->GetData().GetCorrespondingNode(
					CPoint2LNode(m_currentPoint));
			if ( it != pDoc->GetData().GetNodesArray().end() )
				DrawNode(pDC,it,RGB(255,255,0));
			}
		}
		break;
	case EDrawing:
		{
			switch (m_eDrawingMode)
			{
			case EPrimitives:
				{
					switch (m_ePrimitivesMode)
					{
					case ELinePrimitive:
						{
							if ( m_PrimitivesModePressed.m_bPressed )
							{
								DrawLine(pDC,m_PrimitivesModePressed.m_point,m_currentPoint,
									RGB(255,0,0));
							/*
								LGeometryData::NodesConstIterator it = 
									pDoc->GetData().GetCorrespondingNode(
										CPoint2LNode(m_currentPoint));
								if ( it != pDoc->GetData().GetNodesArray().end() )
									DrawNode(pDC,it,RGB(255,0,0));
							*/
							}
						}
						break;
					case ECurvePrimitive:
						{
						}
						break;
					default:
						break;
					};
				}
				break;
			default:
				break;
			};
		}
	case ENone:
	default:
		break;
	};
	
}

void CGeometry2dView::DrawNodes(CDC* pDC)
{
	CGeometry2dDoc* pDoc = GetDocument();
	
	const LGeometryData::NodesVector vec = pDoc->GetData().GetNodesArray();
// ordinal draw
	LGeometryData::NodesConstIterator it = vec.begin();
	for ( ; it != vec.end(); it++ )
	{
		DrawNode(pDC,it,it->bnd == 0 ? RGB(0,200,50):RGB(0,50,200));
	}
}

void CGeometry2dView::DrawNode(CDC* pDC,
	LGeometryData::NodesConstIterator it,COLORREF rgbBox)
{
	CPen pen(PS_SOLID,1,rgbBox);

	CPen* pOldPen = pDC->SelectObject(&pen);
	
	CPoint point = LNode2CPoint(*it);

	int dx = int(LGeometryData::s_delta_x*m_nScaling);
	int dy = int(LGeometryData::s_delta_y*m_nScaling);

	switch ( m_eNodesDrawingMode )
	{
	case EBox:		
		pDC->Rectangle(int(point.x-dx),
			int(point.y-dy),
			int(point.x+dx),
			int(point.y+dy));
		break;	
	case ECross:
		DrawLine(pDC,CPoint(point.x+dx,point.y),CPoint(point.x-dx,point.y),rgbBox);
		DrawLine(pDC,CPoint(point.x,point.y+dy),CPoint(point.x,point.y-dy),rgbBox);
		break;
	case ECircle:
		pDC->Ellipse(point.x-dx,point.y-dy,point.x+dx,point.y+dy);
		break;
	case EDot:
	case ENotDraw:
	default:
		break;
	}	

	if ( m_eNodesDrawingMode != ENotDraw )
		pDC->SetPixel(point,rgbBox);
	
	pDC->SelectObject(pOldPen);
}

void CGeometry2dView::DrawLines(CDC* pDC)
{
	CGeometry2dDoc* pDoc = GetDocument();

	const LGeometryData::NodesVector nodes = 
		pDoc->GetData().GetNodesArray();
	const LGeometryData::SegmentsVector segments = 
		pDoc->GetData().GetSegmentsArray();

	LGeometryData::SegmentsConstIterator it =
		segments.begin();

	for ( ; it != segments.end(); it++ )
	{
		CPoint from,to;
		from = LNode2CPoint(nodes[it->n1]);
		to   = LNode2CPoint(nodes[it->n2]);
		/*
		if ( ( pDoc->GetData().GetNodesArray()[it->n1].bnd == 
			 pDoc->GetData().GetNodesArray()[it->n1].bnd )
			 == 0 )*/
		DrawLine(pDC,from,to,RGB(0,255,0));
	}
}

void CGeometry2dView::DrawLine(CDC* pDC,CPoint from,CPoint to,COLORREF color)
{
	POINT arr[2];
	arr[0] = from;
	arr[1] = to;
	if ( from != to )
	{
		CPen pen(PS_SOLID,1,color);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->Polyline(arr,2);
		pDC->SelectObject(pOldPen);
	}
}

void CGeometry2dView::DrawSK(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(10,10);
//	DrawLine(pDC,CPoint(m_orgSK.x,m_orgSK.y),CPoint(m_orgSK.x+10,m_orgSK.y),RGB(0,255,0));
//	DrawLine(pDC,CPoint(m_orgSK.x,m_orgSK.y),CPoint(m_orgSK.x,m_orgSK.y+10),RGB(0,255,0));
}

void CGeometry2dView::DrawBoundary(CDC* pDC)
{
	CGeometry2dDoc* pDoc = GetDocument();

	const LGeometryData::NodesVector vec = pDoc->GetData().GetNodesArray();
	
	const LGeometryData::BoundaryVector bnd = pDoc->GetData().GetBoundaryArray();
	
	CPoint pnt;

	LGeometryData::BoundaryConstIterator it = bnd.begin();
	if ( it != bnd.end() )
	{
		for ( ; it != bnd.end(); it++ )
		{
			pnt = LNode2CPoint(vec[it->index]);
			CPen pen(PS_SOLID|PS_COSMETIC,1,RGB(50,50,0));
			CPen* pOldPen = pDC->SelectObject(&pen);
			pDC->Ellipse(pnt.x-3,pnt.y-3,pnt.x+3,pnt.y+3);
			pDC->SelectObject(pOldPen);
			if ( pDoc->GetData().GetBoundaryCondition() ==
				LGeometryData::ESymAndDisplacement ||
				 pDoc->GetData().GetBoundaryCondition() ==
				 LGeometryData::EDisplacement )
			{
				double x = vec[it->index].x;
				double y = vec[it->index].y;
				switch(it->doftype)
				{
				case 0:
					break;
				case 1:
					x += it->Z1;
					break;
				case 2: 
					y += it->Z2;
					break;
				case 3: 
					x += it->Z1;
					y += it->Z2;
					break;
				default:
					continue;
				}
				LGeometryData::LNode nod2(x,y);
				CPoint pnt2 = LNode2CPoint(nod2);
				DrawPointer(pDC,pnt,pnt2,RGB(250,30,0));
			}
		}
	}
}

void CGeometry2dView::DrawPointer(CDC* pDC, const CPoint& pnt1, const CPoint& pnt2,
		COLORREF color)
{
	CPen pen(PS_SOLID,1,color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	POINT arr[5];

	const double distance = 8.0;

	CPoint pnt3,pnt4;

	if ( pnt1.x == pnt2.x )
	{
		pnt3.x = int(pnt1.x - (abs(pnt2.y-pnt1.y)/distance));
		pnt3.y = int((pnt1.y+pnt2.y)/2.0);
		pnt4.x = int(pnt1.x + (abs(pnt2.y-pnt1.y)/distance));
		pnt4.y = int((pnt1.y+pnt2.y)/2.0);
	}
	else 
	if ( pnt1.y == pnt2.y )
	{
		pnt3.x = int((pnt1.x+pnt2.x)/2.0);
		pnt3.y = int(pnt1.y + (abs(pnt2.x-pnt1.x)/distance));
		pnt4.x = int((pnt1.x+pnt2.x)/2.0);
		pnt4.y = int(pnt1.y - (abs(pnt2.x-pnt1.x)/distance));
	}
	else
	{
		CPoint point1 = pnt1.y < pnt2.y ? pnt1 : pnt2;
		CPoint point2 = pnt2.y > pnt1.y ? pnt2 : pnt1;

		int x1 = point1.x;
		int x2 = point2.x;
		int y1 = point1.y;
		int y2 = point2.y;

		double a1 = (x2+x1)/2.0;
		double a2 = (y2+y1)/2.0;
		double b = sqrt(pow(x2-x1,2)+pow(y2-y1,2));
		double atan_ = atan((y2-y1)/(double)(x2-x1));
		double sin_ = sin(atan_)/distance;
		double cos_ = cos(atan_)/distance;
		double c1 = b * sin_;
		double c2 = b * cos_;

		pnt3.x = int( a1 - c1 );
		pnt3.y = int( a2 + c2 );
		pnt4.x = int( a1 + c1 );
		pnt4.y = int( a2 - c2 );
	}
	
	arr[0] = pnt1;
	arr[1] = pnt2;
	arr[2] = pnt3;
	arr[3] = pnt4;
	arr[4] = pnt2;
	
	pDC->Polyline(&arr[0],5);
	pDC->Polygon(&arr[2],3);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

// CGeometry2dView printing

BOOL CGeometry2dView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGeometry2dView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGeometry2dView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGeometry2dView diagnostics

#ifdef _DEBUG
void CGeometry2dView::AssertValid() const
{
	CView::AssertValid();
}

void CGeometry2dView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGeometry2dDoc* CGeometry2dView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeometry2dDoc)));
	return (CGeometry2dDoc*)m_pDocument;
}
#endif //_DEBUG


// CGeometry2dView message handlers

void CGeometry2dView::OnRButtonUp(UINT nFlags, CPoint point)
{
	FlushMode();
	SetMode(ENone);
	RedrawWindow();
//	CView::OnRButtonUp(nFlags,point);
}

void CGeometry2dView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CGeometry2dDoc* pDoc = GetDocument();
	
	switch( m_eMode )
	{
	case EDrawing:
		{
			if ( m_eDrawingMode == ESK )
			{
//				m_orgSK = point;
				CreateTsarevCurve(point);
				FlushMode();
			}
			else
			if ( m_eDrawingMode  == EPrimitives )
			{
				switch ( m_ePrimitivesMode )
				{
				case ELinePrimitive:
					{
						CPoint last(0,0);
						if ( m_PrimitivesModePressed.m_bPressed )
							last = m_PrimitivesModePressed.m_point;

						LGeometryData::NodesConstIterator it =
							pDoc->GetData().GetCorrespondingNode(CPoint2LNode(point));
						if ( it != pDoc->GetData().GetNodesArray().end() )
						{
							m_PrimitivesModePressed.m_point = 
								LNode2CPoint(*it);
						}
						else 
						{
							pDoc->GetData().AddNode(CPoint2LNode(point));
							m_PrimitivesModePressed.m_point = point;
						}
						it = pDoc->GetData().GetCorrespondingNode(CPoint2LNode(point));
						if ( it != pDoc->GetData().GetNodesArray().end() )
						{
							if ( m_PrimitivesModePressed.m_bPressed )
							{
								pDoc->GetData().AddSegment(
									CPoint2LNode(last),
									CPoint2LNode(m_PrimitivesModePressed.m_point));
							}
						}
						m_PrimitivesModePressed.m_bPressed = true;
					}
					break;
				case ECurvePrimitive:
					break;
				};
			}
		}
		break;
	case ENodes:
		{
			pDoc->GetData().AddNode(CPoint2LNode(point));
		}
		break;
	case ELines:
		{
			LGeometryData::NodesConstIterator it =
				pDoc->GetData().GetCorrespondingNode(CPoint2LNode(point));
			if ( it != pDoc->GetData().GetNodesArray().end() )
			{
				if ( !m_LinesModePressed.m_bPressed ) 
				{
					m_LinesModePressed.m_bPressed = true;
					m_LinesModePressed.m_point = 
						LNode2CPoint(*it);
					break;
				}
				else
				{
					pDoc->GetData().AddSegment(
						CPoint2LNode(m_LinesModePressed.m_point),
						CPoint2LNode(point));
					
					m_LinesModePressed.m_point = point;
					break;
				}
			}
			else
			{
				it =
					pDoc->GetData().GetCorrespondingNode(CPoint2LNode(m_currentPoint));
				if ( it != pDoc->GetData().GetNodesArray().end() )
				{
					if ( !m_LinesModePressed.m_bPressed ) 
					{
						m_LinesModePressed.m_bPressed = true;
						m_LinesModePressed.m_point = 
							LNode2CPoint(*it);
						break;
					}
					else
					{
						pDoc->GetData().AddSegment(
							CPoint2LNode(m_LinesModePressed.m_point),
							CPoint2LNode(m_currentPoint));
						
						m_LinesModePressed.m_point = m_currentPoint;
						break;
					}
				}
			}
			
			m_LinesModePressed.m_bPressed = false;
		}
		break;
	case EMarker:
		if ( m_nMarker != -1 )
		{
			pDoc->GetData().SetBoundary(pDoc->
				GetData().GetCorrespondingNode(CPoint2LNode(point)),m_nMarker);
		}
		break;
	case EHoles:
		pDoc->GetData().AddHole(CPoint2LNode(point));
		break;
	case ESelectNode:
		{
			LGeometryData::NodesConstIterator it =
				pDoc->GetData().GetCorrespondingNode(CPoint2LNode(point));
			if ( it != pDoc->GetData().GetNodesArray().end() )
			{
				m_nodeSelected = it;
				bool bSym = false;
				LGeometryData::SymmetricConstIterator iter =
					pDoc->GetData().GetSymmetricArray().begin();
				for ( ; iter != pDoc->GetData().GetSymmetricArray().end();
					iter++ )
				{
					if ( it->x == pDoc->GetData().GetNodesArray()[*iter].x &&
						 it->y == pDoc->GetData().GetNodesArray()[*iter].y )
					{
						bSym = true;
						break;
					}
				}

				bool bFound = false;
				LGeometryData::BoundaryConstIterator iter1 = 
					pDoc->GetData().GetBoundaryArray().begin();
				for ( ; iter1 != pDoc->GetData().GetBoundaryArray().end(); iter1++ )
				{
					if ( pDoc->GetData().GetNodesArray()[iter1->index].x == it->x 
						&& pDoc->GetData().GetNodesArray()[iter1->index].y == it->y )
					{
						bFound = true;
						break;
					}
					
				}
				if ( !bFound || pDoc->GetData().GetBoundaryCondition() == LGeometryData::ENoConditions )
					static_cast<CMainFrame*>(theApp.m_pMainWnd)->SetNodeForProperties(m_nodeSelected, bSym);
				else
				{
					bool bStressed = 
						( pDoc->GetData().GetBoundaryCondition() ==
						LGeometryData::EStress || 
						  pDoc->GetData().GetBoundaryCondition() ==
						  LGeometryData::ESymAndStress ) ? true : false;
					static_cast<CMainFrame*>(theApp.m_pMainWnd)->
						SetNodeForProperties(m_nodeSelected, bSym, true, bStressed,
						iter1->Z1,iter1->Z2,iter1->doftype);
				}	
				OnGeometryNodeoptions();
			}
			else
			{
				m_nodeSelected = pDoc->GetData().GetNodesArray().end();
				static_cast<CMainFrame*>(theApp.m_pMainWnd)->UnsetNodeForProperties();
			}
		}
		break;
	case ESymmetry:
		{
			LGeometryData::NodesConstIterator it =
				pDoc->GetData().GetCorrespondingNode(CPoint2LNode(point));
			if ( it != pDoc->GetData().GetNodesArray().end() )
			{
				if ( !m_SymmetryModePressed.m_bPressed ) 
				{
					m_SymmetryModePressed.m_bPressed = true;
					m_SymmetryModePressed.m_point = point;
//						LNode2CPoint(*it);
					break;
				}
				else
				{
					LGeometryData::NodesConstIterator it1 =
						pDoc->GetData().GetCorrespondingNode(CPoint2LNode(point));
					LGeometryData::NodesConstIterator it2 =
						pDoc->GetData().GetCorrespondingNode(CPoint2LNode(m_SymmetryModePressed.m_point));
			
					m_SymmetryModePressed.m_point = point;

					pDoc->GetData().SetSymmetricArray(it1,it2);
				}
			}
			m_SymmetryModePressed.m_bPressed = false;
		}
        break;	
	case EBoundary:
		{
			LGeometryData::NodesConstIterator it =
				pDoc->GetData().GetCorrespondingNode(CPoint2LNode(point));
			if ( it != pDoc->GetData().GetNodesArray().end() )
			{
				if ( !m_BoundaryModePressed.m_bPressed ) 
				{
					m_BoundaryModePressed.m_bPressed = true;
					m_BoundaryModePressed.m_point = point;
					break;
				}
				else
				{
					LGeometryData::NodesConstIterator it1 =
						pDoc->GetData().GetCorrespondingNode(CPoint2LNode(point));
					LGeometryData::NodesConstIterator it2 =
						pDoc->GetData().GetCorrespondingNode(
						CPoint2LNode(m_BoundaryModePressed.m_point));
			
					m_BoundaryModePressed.m_point = point;

					pDoc->GetData().AddToBoundaryArray(it1,it2,
						m_defBndValues.z1,m_defBndValues.z2,m_defBndValues.doftype);
				}
			}
			m_BoundaryModePressed.m_bPressed = false;
		}
        break;	
	case ENone:
		break;
	default:
		TRACE0("OnLButtonUp: Unknown mode\n");
		ASSERT(FALSE);
		break;
	};
	PrepareBuffer();
	RedrawWindow();
	CView::OnLButtonUp(nFlags, point);
}


void CGeometry2dView::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
		
	GetClientRect(rect);

	if ( rect.PtInRect(point) )
	{
		LGeometryData::LNode node = CPoint2LNode(point);	
		static_cast<CMainFrame*>(theApp.m_pMainWnd)->ChangeXY(node.x,node.y);
	}
	CGeometry2dDoc* pDoc = GetDocument();

	m_currentPoint = point;

	LGeometryData::NodesConstIterator it = 
		pDoc->GetData().GetCorrespondingNode(CPoint2LNode(m_currentPoint));

	if ( it != pDoc->GetData().GetNodesArray().end() && ( m_LinesModePressed.m_bPressed ||
		m_SymmetryModePressed.m_bPressed || m_BoundaryModePressed.m_bPressed || m_PrimitivesModePressed.m_bPressed ) )
	{
		m_currentPoint = LNode2CPoint(*it);
		RedrawWindow();
	}
	if ( m_eMode != ENone )
		RedrawWindow();
	CView::OnMouseMove(nFlags, point);
}

void CGeometry2dView::OnInitialUpdate()
{
	CGeometry2dDoc* pDoc = GetDocument();
	m_nodeSelected = pDoc->GetData().GetNodesArray().end();

	int nx = GetSystemMetrics(SM_CXSCREEN);
	int ny = GetSystemMetrics(SM_CYSCREEN);

	m_dcNodes.CreateCompatibleDC(GetDC());
	m_dcLines.CreateCompatibleDC(GetDC());

	
	m_bmpNodes.CreateCompatibleBitmap(GetDC(),nx,ny);
	m_bmpLines.CreateCompatibleBitmap(GetDC(),nx,ny);
	
	m_dcNodes.SelectObject(m_bmpNodes);
	m_dcLines.SelectObject(m_bmpLines);

	CBrush brush;
	CRect rect(0,0,nx,ny);
	
	brush.CreateSolidBrush(RGB(255,255,255));
	m_dcNodes.FillRect(rect,&brush);
	m_dcLines.FillRect(rect,&brush);
}


void CGeometry2dView::DrawTriangles(CDC* pDC)
{
	CGeometry2dDoc* pDoc = GetDocument();
	const LGeometryData::TrianglesVector trns = 
		pDoc->GetData().GetTrianglesArray();
	const LGeometryData::NodesVector nds = 
		pDoc->GetData().GetNodesArray();

	for ( LGeometryData::TrianglesConstIterator it = 
		trns.begin();
		it != trns.end(); it++ )
	{
		DrawLine(pDC,
			LNode2CPoint(nds[it->ind1]),LNode2CPoint(nds[it->ind2]),
			RGB(0,200,100));
		DrawLine(pDC,
			LNode2CPoint(nds[it->ind2]),LNode2CPoint(nds[it->ind3]),
			RGB(0,200,100));
		DrawLine(pDC,
			LNode2CPoint(nds[it->ind3]),LNode2CPoint(nds[it->ind1]),
			RGB(0,200,100));
	};
}


void CGeometry2dView::DrawHoles(CDC *pDC)
{
	CGeometry2dDoc* pDoc = GetDocument();
	const LGeometryData::NodesVector hls = 
		pDoc->GetData().GetHolesArray();
	for ( LGeometryData::NodesConstIterator it = hls.begin();
		it != hls.end();
		it++ )
		DrawHole(pDC,it,RGB(255,0,0));
	
}

void CGeometry2dView::DrawHole(CDC *pDC, 
	LGeometryData::NodesConstIterator it, COLORREF rgbClr)
{
	CPoint point = LNode2CPoint(*it);
	DrawLine(pDC,
		CPoint(point.x-7,point.y),CPoint(point.x+7,point.y),
		rgbClr);
	DrawLine(pDC,
		CPoint(point.x,point.y-7),CPoint(point.x,point.y+7),
		rgbClr);
}

void CGeometry2dView::PrepareBuffer()
{
	int nx = GetSystemMetrics(SM_CXSCREEN);
	int ny = GetSystemMetrics(SM_CYSCREEN);
	
	CBrush brush;
	CRect rect(0,0,nx,ny);
	
	brush.CreateSolidBrush(RGB(255,255,255));
	m_dcNodes.FillRect(rect,&brush);

	DrawSK(&m_dcNodes);
	DrawNodes(&m_dcNodes);
	DrawLines(&m_dcNodes);
	DrawTriangles(&m_dcNodes);
	DrawHoles(&m_dcNodes);
	DrawBoundary(&m_dcNodes);
	DrawSymmetry(&m_dcNodes);
};

void CGeometry2dView::OnGeometryAddNode()
{
	SetMode(ENodes);
}

void CGeometry2dView::OnGeometryAddSegment()
{
	SetMode(ELines);
}

void CGeometry2dView::OnGeometrySetMarker1()
{
	SetMode(EMarker);
	m_nMarker = LGeometryData::EInner;
}

void CGeometry2dView::OnGeometrySetMarker2()
{
	SetMode(EMarker);
	m_nMarker = LGeometryData::EOuter;
}

void CGeometry2dView::OnGeometrySetHoles()
{
	SetMode(EHoles);
}

void CGeometry2dView::OnTriangulateCurrentView()
{
	CTriangulateOptionsDlg dlg;
	
	INT_PTR nResult;
	
	CGeometry2dDoc* pDoc = GetDocument();
	
	if ( ( nResult = dlg.DoModal() ) == IDOK )
	{
		pDoc->PrepareUndo();
		const double minAngle  = dlg.GetMinAngle();	
		const double maxSquare = dlg.GetMaxSquare();
		const LGeometryData::EDegree degree	= dlg.GetDegree();
			
		if ( !pDoc->GetData().Triangulate( minAngle, maxSquare, degree ) )
			AfxMessageBox("Wrong triangulation params"); 
		else
		{
			PrepareBuffer();
			RedrawWindow();
			SetMode(ENone);
		}
	}

}

void CGeometry2dView::OnGeometryProperties()
{
	CMainFrame* theFrame = static_cast<CMainFrame*>(theApp.m_pMainWnd);
	theFrame->ActivateProperties();
}

void CGeometry2dView::GeometryProperties(short scale,ENodeDrawingMode mode,
	LGeometryData::EBoundaryCondition cond)
{
	CGeometry2dDoc* pDoc = GetDocument();
	m_nScaling = scale;
	m_eNodesDrawingMode = mode;
	pDoc->GetData().SetBoundaryCondition(cond);
	PrepareBuffer();
	RedrawWindow();
//	SetMode(ENone);
}
void CGeometry2dView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CGeometry2dDoc* pDoc = GetDocument();

	if ( bActivate )
	{
		CMainFrame* theFrame = static_cast<CMainFrame*>(theApp.m_pMainWnd);
		theFrame->ChangeProperties( m_nScaling, m_eNodesDrawingMode,
			pDoc->GetData().GetBoundaryCondition() );	
		SetMode(m_eMode);
	}
	else 
	{
		CMainFrame* theFrame = static_cast<CMainFrame*>(theApp.m_pMainWnd);
		theFrame->UnsetNodeForProperties();
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CGeometry2dView::OnGeometryNodeoptions()
{
	CMainFrame* theFrame = static_cast<CMainFrame*>(theApp.m_pMainWnd);
	theFrame->ActivateNodeProperties();
}

void CGeometry2dView::SetSelectNodesMode()
{
	SetMode(ESelectNode);
}

void CGeometry2dView::DrawCross(CDC* pDC, const CPoint& center)
{
	if ( ESelectNode != m_eMode )
		return;

	int dx = int(LGeometryData::s_delta_x*m_nScaling);
	int dy = int(LGeometryData::s_delta_y*m_nScaling);
	
	DrawLine(pDC,
		CPoint(center.x+dx+10,center.y),
		CPoint(center.x+dx,center.y),
		RGB(255,0,0));
	
	DrawLine(pDC,
		CPoint(center.x-dx-10,center.y),
		CPoint(center.x-dx,center.y),
		RGB(255,0,0));
	
	DrawLine(pDC,
		CPoint(center.x,center.y+dy+10),
		CPoint(center.x,center.y+dy),
		RGB(255,0,0));
	
	DrawLine(pDC,
		CPoint(center.x,center.y-dy-10),
		CPoint(center.x,center.y-dy),
		RGB(255,0,0));

}

void CGeometry2dView::SetMode(EMode eMode)
{ 
	if ( eMode != EDrawing )
		FlushMode();
	
	CGeometry2dDoc* pDoc = GetDocument();

	if ( eMode == ESymmetry )
	{
		if ( pDoc->GetData().GetBoundaryCondition() == LGeometryData::ENoConditions ||
			pDoc->GetData().GetBoundaryCondition() == LGeometryData::EStress ||
			pDoc->GetData().GetBoundaryCondition() == LGeometryData::EDisplacement )
			return;
	}
	
	if ( eMode == EBoundary )
	{
		if ( pDoc->GetData().GetBoundaryCondition() == LGeometryData::ENoConditions )
			return;
	}

	CMainFrame* theFrame = static_cast<CMainFrame*>(theApp.m_pMainWnd);

	switch ( eMode )
	{
	case ENodes:
		theFrame->ChangeMode("Add node");
		break;
	case ELines:
		theFrame->ChangeMode("Segments");
		break;
	case EMarker:
		theFrame->ChangeMode("Marker");
		break;
	case EHoles:
		theFrame->ChangeMode("Holes");
		break;
	case ESelectNode:
		theFrame->ChangeMode("Select node");
		break;
	case EBoundary:
		theFrame->ChangeMode("Boundary");
		break;
	case ESymmetry:
		theFrame->ChangeMode("Symmetry");
		break;
	case EDrawing:
		theFrame->ChangeMode("Drawing");
		break;
	case ENone:
	default:
		theFrame->ChangeMode("None");
		break;
	};
	

	m_eMode = eMode; 
	if ( m_eMode != ELines )
			m_LinesModePressed.m_bPressed = false;
	
	if ( m_eMode != ESymmetry )
		m_SymmetryModePressed.m_bPressed = false;
	
	if ( m_eMode != EBoundary )
		m_BoundaryModePressed.m_bPressed = false;
	
	if ( eMode != ESelectNode )
	{
		m_nodeSelected = pDoc->GetData().GetNodesArray().end();
		static_cast<CMainFrame*>(theApp.m_pMainWnd)->UnsetNodeForProperties();
	}

	if ( m_eMode != EDrawing )
		m_PrimitivesModePressed.m_bPressed = false;

	if ( eMode != EMarker )
		m_nMarker = LGeometryData::ENotMarked;
};

void CGeometry2dView::OnBoundaryCurve()
{
	CGeometry2dDoc* pDoc = GetDocument();
	
	if ( pDoc->GetData().GetBoundaryCondition() == 
		LGeometryData::ENoConditions )
		return;
	
	CDefaultBoundaryConditions dlg;
	dlg.SetConditionType( 
			( pDoc->GetData().GetBoundaryCondition() == LGeometryData::EStress || 
			pDoc->GetData().GetBoundaryCondition() == LGeometryData::ESymAndStress ) ?
			true:false);
	if ( dlg.DoModal() == IDOK )
	{
		m_defBndValues.z1 = dlg.GetZ1();
		m_defBndValues.z2 = dlg.GetZ2();
		m_defBndValues.doftype = dlg.GetDofType();
	}
	else 
		return;

	SetMode(EBoundary);
}

void CGeometry2dView::OnBoundaryLine()
{
	CGeometry2dDoc* pDoc = GetDocument();

	if ( pDoc->GetData().GetBoundaryCondition() == 
		LGeometryData::ENoConditions || 
		pDoc->GetData().GetBoundaryCondition() == 
		LGeometryData::EDisplacement ||
		pDoc->GetData().GetBoundaryCondition() == 
		LGeometryData::EStress )
		return;

	SetMode(ESymmetry);
}

void CGeometry2dView::DrawSymmetry(CDC* pDC)
{
	CGeometry2dDoc* pDoc = GetDocument();
	if ( pDoc->GetData().GetBoundaryCondition() ==  
		LGeometryData::ENoConditions || 
		pDoc->GetData().GetBoundaryCondition() == 
		LGeometryData::EDisplacement ||
		pDoc->GetData().GetBoundaryCondition() == 
		LGeometryData::EStress )
		return;

	if ( pDoc->GetData().GetSymmetricArray().size() == 0 )
		return;

	LGeometryData::SymmetricConstIterator it =
		pDoc->GetData().GetSymmetricArray().begin();

	std::vector<POINT> points;
	points.reserve(pDoc->GetData().GetSymmetricArray().size());

	for ( ; it != pDoc->GetData().GetSymmetricArray().end(); it++ )
	{
		points.push_back( LNode2CPoint(pDoc->GetData().GetNodesArray()[*it]));
	}
	
	CPen pen(PS_DASHDOT, 1, RGB(0,100,255));

	CPen* oldPen = pDC->SelectObject(&pen);
	pDC->Polyline(&points[0],int(points.size()));
	pDC->SelectObject(oldPen);

}
void CGeometry2dView::OnGeometryExporttomodule()
{
	CGeometry2dDoc* pDoc = GetDocument();

	std::stringstream stream;

	stream << pDoc->GetData();
#ifdef ABSOLUTE_PATH
#ifdef _DEBUG
	std::ofstream file("c:\\mclab\\data\\geometry.node");
#else
	std::ofstream file("c:\\mclab\\data\\geometry.node");
#endif
#else
	std::ofstream file("..\\data\\geometry.node");
#endif
	file << stream.str() << std::flush;

	file.close();
/*
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);

	SOCKET s;
	sockaddr_in service;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(27015);
	
	if ( connect(s,(sockaddr*)&service,sizeof(service)) != -1 )
	{
		int len = stream.str().length();
		
		short siz = htons(len);
		if ( send(s,(char*)&siz,sizeof(short),0) < 0 )
			AfxMessageBox("Error");

		if ( ( len = send(s,stream.str().c_str(),stream.str().length(),0) ) < 0 )
			AfxMessageBox("Error");
		CString str;
		str.Format("sent = %d",len);
		AfxMessageBox(str);
	}

	closesocket(s);

	WSACleanup();

	*/

}

void CGeometry2dView::OnLinePrimitive()
{
	CGeometry2dDoc* pDoc = GetDocument();
	SetMode(EDrawing);
	m_eDrawingMode = EPrimitives;
	m_ePrimitivesMode = ELinePrimitive;
}

void CGeometry2dView::OnCurvePrimitive()
{
	CGeometry2dDoc* pDoc = GetDocument();
}


void CGeometry2dView::OnGeometryChangesk()
{
	m_eMode = EDrawing;
	m_eDrawingMode = ESK;
	m_ePrimitivesMode = ENotPrimitivesMode;
}

#define M_PI 3.14159265359
#define R 100
#define Ro 50
#define L 100
#define alfa M_PI/4

double g( double x )
{
	return (Ro+R)/2+((R-Ro)/2)*cos(M_PI+M_PI*x/(L));
}

void CGeometry2dView::CreateTsarevCurve(const CPoint& point) // upper corner
{
	CGeometry2dDoc* pDoc = GetDocument();

	LGeometryData::LNode first = CPoint2LNode(point);
/*
	int K = 16;

	double step = L/(double)K;

	for ( int i = 0; i < K+1; i++ )
	{
		LGeometryData::LNode node;
//		node.y = first.y + step*i;
		node.y = i*step;
		node.x = 0.5*g(-Ro+i*step)-7.5;
		pDoc->GetData().AddNode(node);
	}
	for ( int i = 0; i < K+1; i++ )
	{
		LGeometryData::LNode node;
//		node.y = first.y + step*i;
		node.y = i*step;
		node.x = 0.5*g(-Ro+i*step)-17.5;
		pDoc->GetData().AddNode(node);
	}
*/
	LGeometryData::LNode node1,node2,node3,node4;
	node1.x = 15;node1.y = 0;
	node2.x = 30;node2.y = 100;
	node3.x = 20;node3.y = 100;
	node4.x = 5;node4.y = 00;
	pDoc->GetData().AddNode(node1);
	pDoc->GetData().AddNode(node2);
	pDoc->GetData().AddNode(node3);
	pDoc->GetData().AddNode(node4);
	
//	node.x = 
}