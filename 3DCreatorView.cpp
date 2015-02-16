// 3DCreatorView.cpp : implementation of the CMy3DCreatorView class
//

#include "stdafx.h"
#include "3DCreator.h"

#include "3DCreatorDoc.h"
#include "3DCreatorView.h"
#include "MainFrm.h"
#include "ToolForm.h"
#include "3DLibrary\Constants.h"
#include "3DLibrary\Cube.h"
#include "3DLibrary\Sphere.h"
#include "3DLibrary\Cylinder.h"
#include "3DLibrary\Cone.h"
#include "3DLibrary\SurfaceModel.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorView

IMPLEMENT_DYNCREATE(CMy3DCreatorView, CView)

BEGIN_MESSAGE_MAP(CMy3DCreatorView, CView)
	//{{AFX_MSG_MAP(CMy3DCreatorView)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ELIMINATE, OnEliminate)
	ON_COMMAND(ID_ZOOM, OnZoom)
	ON_COMMAND(ID_TOP, OnTop)
	ON_COMMAND(ID_BACK, OnBack)
	ON_COMMAND(ID_BOTTOM, OnBottom)
	ON_COMMAND(ID_LEFT, OnLeft)
	ON_COMMAND(ID_FRONT, OnFront)
	ON_COMMAND(ID_RIGHT, OnRight)
	ON_COMMAND(ID_PERSPECTIVE, OnPerspective)
	ON_COMMAND(ID_SMOOTH, OnSmooth)
	ON_COMMAND(ID_WIREFRAME, OnWireframe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorView construction/destruction

CMy3DCreatorView::CMy3DCreatorView()
{
	view[0] = CUserView::createStockView(FRONT_VIEW);
	view[1] = CUserView::createStockView(LEFT_VIEW);
	view[2] = CUserView::createStockView(TOP_VIEW);
	view[3] = CUserView::createStockView(PERSPECTIVE_VIEW);
	viewName[0] = "Front";
	viewName[1] = "Left";
	viewName[2] = "Top";
	viewName[3] = "Perspective";
	zoom = false;
	eliminate[0] = eliminate[1] = eliminate[2] = eliminate[3] = true;
	smooth[0] = smooth[1] = smooth[2] = false;
	smooth[3] = true;
	activeView = 3;
	oldpos.x = -1;
}

CMy3DCreatorView::~CMy3DCreatorView()
{
}

BOOL CMy3DCreatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorView drawing

void CMy3DCreatorView::OnDraw(CDC* pDC)
{
	CMy3DCreatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	redrawScreen(pDC, DRAW_ALLVIEW);
	redrawScreen(pDC, DRAW_BORDER);
}

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorView diagnostics

#ifdef _DEBUG
void CMy3DCreatorView::AssertValid() const
{
	CView::AssertValid();
}

void CMy3DCreatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy3DCreatorDoc* CMy3DCreatorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy3DCreatorDoc)));
	return (CMy3DCreatorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorView message handlers

void CMy3DCreatorView::redrawScreen(CDC *pDC, int drawmethod)
{
	CRect crc;
	GetClientRect(crc);

	int w = (crc.Width() - 3 * VIEW_GAP) / 2;
	int h = (crc.Height() - 3 * VIEW_GAP) / 2;
	CRect rc[4];
	if (!zoom)
	{
		rc[0] = CRect(VIEW_GAP, VIEW_GAP, w + VIEW_GAP, h + VIEW_GAP);
		rc[1] = CRect(w + 2 * VIEW_GAP, VIEW_GAP,  2 * w + 2 * VIEW_GAP, h + VIEW_GAP);
		rc[2] = CRect(VIEW_GAP, h + 2 * VIEW_GAP, w + VIEW_GAP, 2 * h + 2 * VIEW_GAP);
		rc[3] = CRect(w + 2 * VIEW_GAP, h + 2 * VIEW_GAP, 2 * w + 2 * VIEW_GAP, 2 * h + 2 * VIEW_GAP);
	}
	else rc[0] = rc[1] = rc[2] = rc[3] = CRect(VIEW_GAP, VIEW_GAP, 2 * w + 2 * VIEW_GAP, 2 * h + 2 * VIEW_GAP);

	if (drawmethod == DRAW_BORDER)
	{
		CPen bpen(PS_SOLID, 1, GAPCOLOR);
		CPen *oldpen = pDC->SelectObject(&bpen);
		CBrush bbrush(GAPCOLOR);
		CBrush *oldbrush = pDC->SelectObject(&bbrush);
		pDC->Rectangle(0, 0, crc.Width(), VIEW_GAP);
		pDC->Rectangle(0, 0, VIEW_GAP, crc.Height());
		pDC->Rectangle(0, 2 * h + 2 * VIEW_GAP, crc.Width(), crc.Height());
		pDC->Rectangle(2 * w + 2 * VIEW_GAP, 0, crc.Width(), crc.Height());
		if (!zoom)
		{
			pDC->Rectangle(w + VIEW_GAP, 0, w + 2 * VIEW_GAP, crc.Height());
			pDC->Rectangle(0, h + VIEW_GAP, crc.Width(), h + 2 * VIEW_GAP);
		}

		CRect arc = rc[activeView];
		CPen wpen(PS_SOLID, 2, ACTIVECOLOR);
		pDC->SelectObject(&wpen);
		CBrush wbrush(ACTIVECOLOR);
		pDC->SelectObject(&wbrush);
		pDC->MoveTo(arc.left - 1, arc.top - 1);
		pDC->LineTo(arc.right + 1, arc.top - 1);
		pDC->LineTo(arc.right + 1, arc.bottom + 1);
		pDC->LineTo(arc.left - 1, arc.bottom + 1);
		pDC->LineTo(arc.left - 1, arc.top - 2);

		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
		bbrush.DeleteObject();
		bpen.DeleteObject();
		wpen.DeleteObject();
		wbrush.DeleteObject();
		return;
	}

	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, crc.Width(), crc.Height());
	CBitmap *oldbmp = dc.SelectObject(&bmp);

	CBrush brush(BKCOLOR);
	CBrush *oldbrush = dc.SelectObject(&brush);
	dc.SetTextColor(TEXTCOLOR);
	dc.SetBkColor(BKCOLOR);

	CSurfaceModel *model = NULL;
	if (GetDocument()->temp != NULL) model = GetDocument()->temp->createSurfaceModel();

	for (int i = 0; i < 4; i++)
		if ((!zoom && drawmethod == DRAW_ALLVIEW) || i == activeView)
		{
			dc.Rectangle(rc[i]);
			view[i].drawHorizontalFace(&dc, rc[i], GRIDCOLOR, AXISCOLOR);
			if (!smooth[i])	GetDocument()->scene.drawWireframe(&dc, rc[i], view[i], eliminate[i]);
			else GetDocument()->scene.simpleRender(&dc, rc[i], view[i], BKCOLOR);
			dc.TextOut(rc[i].left + 3, rc[i].top + 3, viewName[i]);
			if (model != NULL)
			{
				CMatrix rm = CGeometry::getAlterViewMatrix(view[i]);
				model->draw(&dc, rc[i], view[i], rm, SELECTEDCOLOR, eliminate[i]);
			}
			if (GetDocument()->selection != NULL)
			{
				CMatrix rm = CGeometry::getAlterViewMatrix(view[i]);
				GetDocument()->selection->draw(&dc, rc[i], view[i], rm, SELECTEDCOLOR, eliminate[i]);
			}
			pDC->BitBlt(rc[i].left, rc[i].top, rc[i].Width(), rc[i].Height(), &dc, rc[i].left, rc[i].top, SRCCOPY);
		}

	if (model != NULL) delete model;

	dc.SelectObject(oldbmp);
	dc.SelectObject(oldbrush);
	brush.DeleteObject();
	dc.DeleteDC();
	bmp.DeleteObject();
}

void CMy3DCreatorView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	int w = (cx - 3 * VIEW_GAP);
	int h = (cy - 3 * VIEW_GAP);
	for (int i = 0; i < 4; i++)
		view[i].setWidth(view[i].getHeight() * w / h);
}

void CMy3DCreatorView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect crc;
	GetClientRect(crc);
	if (!zoom)
	{
		activeView = 0;
		if (point.x > crc.Width() / 2) activeView += 1;
		if (point.y > crc.Height() / 2) activeView += 2;
	}
	CDC *pDC = GetDC();

	int w = (crc.Width() - 3 * VIEW_GAP) / 2;
	int h = (crc.Height() - 3 * VIEW_GAP) / 2;
	CRect rc[4];
	if (!zoom)
	{
		rc[0] = CRect(VIEW_GAP, VIEW_GAP, w + VIEW_GAP, h + VIEW_GAP);
		rc[1] = CRect(w + 2 * VIEW_GAP, VIEW_GAP,  2 * w + 2 * VIEW_GAP, h + VIEW_GAP);
		rc[2] = CRect(VIEW_GAP, h + 2 * VIEW_GAP, w + VIEW_GAP, 2 * h + 2 * VIEW_GAP);
		rc[3] = CRect(w + 2 * VIEW_GAP, h + 2 * VIEW_GAP, 2 * w + 2 * VIEW_GAP, 2 * h + 2 * VIEW_GAP);
	}
	else rc[0] = rc[1] = rc[2] = rc[3] = CRect(VIEW_GAP, VIEW_GAP, 2 * w + 2 * VIEW_GAP, 2 * h + 2 * VIEW_GAP);

	CVertex v = view[activeView].getVertexOnHF(point, rc[activeView]);

	switch (GetDocument()->status)
	{
	case STATUS_SELECTING:
		{
			GetDocument()->selection = GetDocument()->scene.getSelection(point, view[activeView], rc[activeView], eliminate[activeView], smooth[activeView]);
			CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			if (pFrame)
			{
				CToolForm *form = (CToolForm*)pFrame->m_wndSplitter.GetPane(0, 1);
				if (form->m_tab.GetCurSel() == 1) form->refreshButtons();
			}
			redrawScreen(pDC, DRAW_ALLVIEW);
			break;
		}
	case STATUS_INTERSECT:
	case STATUS_UNION:
	case STATUS_DIFFERENCE:
		if (GetDocument()->createstep == 0)
		{
			GetDocument()->selection = GetDocument()->scene.getSelection(point, view[activeView], rc[activeView], eliminate[activeView], smooth[activeView]);
			redrawScreen(pDC, DRAW_ALLVIEW);
			break;
		}
		else
		{
			if (!GetDocument()->selection || GetDocument()->selection->getType() == TYPE_LIGHT) break;
			CGraphicObject *g = GetDocument()->scene.getSelection(point, view[activeView], rc[activeView], eliminate[activeView], smooth[activeView]);
			if (g == NULL || g == GetDocument()->selection || g->getType() == TYPE_LIGHT) break;
			CGeometry *g1 = (CGeometry*)GetDocument()->selection;
			CGeometry *g2 = (CGeometry*)g;
			if (GetDocument()->status == STATUS_INTERSECT) GetDocument()->scene.intersectObject(g1, g2);
			else if (GetDocument()->status == STATUS_UNION) GetDocument()->scene.unionObject(g1, g2);
			else if (GetDocument()->status == STATUS_DIFFERENCE) GetDocument()->scene.differenceObject(g1, g2);
			GetDocument()->SetModifiedFlag();
			GetDocument()->createstep = 0;

			CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			if (pFrame)
			{
				CToolForm *form = (CToolForm*)pFrame->m_wndSplitter.GetPane(0, 1);
				form->refreshButtons();
			}
			redrawScreen(pDC, DRAW_ALLVIEW);
		}
		break;
	case STATUS_MOVE:
	case STATUS_ROTATE:
	case STATUS_SCALE:
		{
			CGraphicObject *g = GetDocument()->scene.getSelection(point, view[activeView], rc[activeView], eliminate[activeView], smooth[activeView]);
			if (g)
			{
				GetDocument()->selection = g;
				CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
				if (pFrame)
				{
					CToolForm *form = (CToolForm*)pFrame->m_wndSplitter.GetPane(0, 1);
					if (form->m_tab.GetCurSel() == 1) form->refreshButtons();
				}
				redrawScreen(pDC, DRAW_ALLVIEW);
			}

			if (GetDocument()->selection) oldver = view[activeView].getVertexOnHF(point, rc[activeView]);
			break;
		}
	case STATUS_NEW_CUBE:
		switch (GetDocument()->createstep)
		{
		case 0:
			{
				CVector zaxis = view[activeView].getHoriX() * view[activeView].getHoriY();
				GetDocument()->temp = new CCube(v, zaxis, view[activeView].getHoriX(), 0.1, 0.1, 0.1);
				GetDocument()->createstep = 1;
				GetDocument()->ov = v;
				GetDocument()->selection = NULL;
				break;
			}
		}
		break;
	case STATUS_NEW_SPHERE:
		switch (GetDocument()->createstep)
		{
		case 0:
			{
				GetDocument()->temp = new CSphere(v, 0);
				GetDocument()->createstep = 1;
				GetDocument()->ov = v;
				GetDocument()->selection = NULL;
				break;
			}
		}
		break;
	case STATUS_NEW_CYLINDER:
		switch (GetDocument()->createstep)
		{
		case 0:
			{
				GetDocument()->temp = new CCylinder(v, view[activeView].getHoriX() * view[activeView].getHoriY(), 0.1, 0.1);
				GetDocument()->createstep = 1;
				GetDocument()->ov = v;
				GetDocument()->selection = NULL;
				break;
			}
		}
		break;
	case STATUS_NEW_CONE:
		switch (GetDocument()->createstep)
		{
		case 0:
			{
				GetDocument()->temp = new CCone(v, view[activeView].getHoriX() * view[activeView].getHoriY(), 0.1, 0.1, 0.1);
				GetDocument()->createstep = 1;
				GetDocument()->ov = v;
				GetDocument()->selection = NULL;
				break;
			}
		}
		break;
	case STATUS_NEW_LIGHT:
		{
			CLight *l = new CLight(v);
			CString name;
			name.Format("Light%d", GetDocument()->currlight);
			GetDocument()->currlight++;
			GetDocument()->scene.add(l, name);
			GetDocument()->selection = l;
			GetDocument()->SetModifiedFlag();
			redrawScreen(pDC, DRAW_ALLVIEW);
		}
		break;
	}

	redrawScreen(pDC, DRAW_BORDER);
	ReleaseDC(pDC);

	CView::OnLButtonDown(nFlags, point);
}

void CMy3DCreatorView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect crc;
	GetClientRect(crc);
	CDC *pDC = GetDC();

	int w = (crc.Width() - 3 * VIEW_GAP) / 2;
	int h = (crc.Height() - 3 * VIEW_GAP) / 2;
	CRect rc[4];
	if (!zoom)
	{
		rc[0] = CRect(VIEW_GAP, VIEW_GAP, w + VIEW_GAP, h + VIEW_GAP);
		rc[1] = CRect(w + 2 * VIEW_GAP, VIEW_GAP,  2 * w + 2 * VIEW_GAP, h + VIEW_GAP);
		rc[2] = CRect(VIEW_GAP, h + 2 * VIEW_GAP, w + VIEW_GAP, 2 * h + 2 * VIEW_GAP);
		rc[3] = CRect(w + 2 * VIEW_GAP, h + 2 * VIEW_GAP, 2 * w + 2 * VIEW_GAP, 2 * h + 2 * VIEW_GAP);
	}
	else rc[0] = rc[1] = rc[2] = rc[3] = CRect(VIEW_GAP, VIEW_GAP, 2 * w + 2 * VIEW_GAP, 2 * h + 2 * VIEW_GAP);

	CVertex v = view[activeView].getVertexOnHF(point, rc[activeView]);
	
	switch (GetDocument()->status)
	{
	case STATUS_NEW_CUBE:
		switch (GetDocument()->createstep)
		{
		case 1:
			{
				GetDocument()->createstep = 2;
				GetDocument()->ov = v;
				break;
			}
		case 2:
			{
				GetDocument()->createstep = 0;
				CGeometry *g = new CGeometry(GetDocument()->temp);
				g->material.color = Colors[abs(rand()) % 32];
				CString name;
				name.Format("Object%d", GetDocument()->currname);
				GetDocument()->currname++;
				GetDocument()->scene.add(g, name);
				GetDocument()->temp = NULL;
				GetDocument()->selection = g;
				GetDocument()->SetModifiedFlag();
				if (smooth[0] || smooth[1] || smooth[2] || smooth[3]) redrawScreen(pDC, DRAW_ALLVIEW);
			}
		}
		break;
	case STATUS_NEW_SPHERE:
		switch (GetDocument()->createstep)
		{
		case 1:
			{
				GetDocument()->createstep = 0;
				CGeometry *g = new CGeometry(GetDocument()->temp);
				g->material.color = Colors[abs(rand()) % 32];
				CString name;
				name.Format("Object%d", GetDocument()->currname);
				GetDocument()->currname++;
				GetDocument()->scene.add(g, name);
				GetDocument()->temp = NULL;
				GetDocument()->selection = g;
				GetDocument()->SetModifiedFlag();
				if (smooth[0] || smooth[1] || smooth[2] || smooth[3]) redrawScreen(pDC, DRAW_ALLVIEW);
			}
		}
		break;
	case STATUS_NEW_CYLINDER:
		switch (GetDocument()->createstep)
		{
		case 1:
			{
				GetDocument()->createstep = 2;
				GetDocument()->ov = v;
				break;
			}
		case 2:
			{
				GetDocument()->createstep = 0;
				CGeometry *g = new CGeometry(GetDocument()->temp);
				g->material.color = Colors[abs(rand()) % 32];
				CString name;
				name.Format("Object%d", GetDocument()->currname);
				GetDocument()->currname++;
				GetDocument()->scene.add(g, name);
				GetDocument()->temp = NULL;
				GetDocument()->selection = g;
				GetDocument()->SetModifiedFlag();
				if (smooth[0] || smooth[1] || smooth[2] || smooth[3]) redrawScreen(pDC, DRAW_ALLVIEW);
			}
		}
		break;
	case STATUS_NEW_CONE:
		switch (GetDocument()->createstep)
		{
		case 1:
			{
				GetDocument()->createstep = 2;
				GetDocument()->ov = v;
				break;
			}
		case 2:
			{
				GetDocument()->createstep = 3;
				GetDocument()->ov = CVertex(point.y, 0, 0);
				break;
			}
		case 3:
			{
				GetDocument()->createstep = 0;
				CGeometry *g = new CGeometry(GetDocument()->temp);
				g->material.color = Colors[abs(rand()) % 32];
				CString name;
				name.Format("Object%d", GetDocument()->currname);
				GetDocument()->currname++;
				GetDocument()->scene.add(g, name);
				GetDocument()->temp = NULL;
				GetDocument()->selection = g;
				GetDocument()->SetModifiedFlag();
				if (smooth[0] || smooth[1] || smooth[2] || smooth[3]) redrawScreen(pDC, DRAW_ALLVIEW);
			}
		}
		break;
	}

	ReleaseDC(pDC);

	CView::OnLButtonUp(nFlags, point);
}

void CMy3DCreatorView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect crc;
	GetClientRect(crc);
	CDC *pDC = GetDC();

	int w = (crc.Width() - 3 * VIEW_GAP) / 2;
	int h = (crc.Height() - 3 * VIEW_GAP) / 2;
	CRect rc[4];
	if (!zoom)
	{
		rc[0] = CRect(VIEW_GAP, VIEW_GAP, w + VIEW_GAP, h + VIEW_GAP);
		rc[1] = CRect(w + 2 * VIEW_GAP, VIEW_GAP,  2 * w + 2 * VIEW_GAP, h + VIEW_GAP);
		rc[2] = CRect(VIEW_GAP, h + 2 * VIEW_GAP, w + VIEW_GAP, 2 * h + 2 * VIEW_GAP);
		rc[3] = CRect(w + 2 * VIEW_GAP, h + 2 * VIEW_GAP, 2 * w + 2 * VIEW_GAP, 2 * h + 2 * VIEW_GAP);
	}
	else rc[0] = rc[1] = rc[2] = rc[3] = CRect(VIEW_GAP, VIEW_GAP, 2 * w + 2 * VIEW_GAP, 2 * h + 2 * VIEW_GAP);

	CVertex v = view[activeView].getVertexOnHF(point, rc[activeView]);
	
	if (oldpos.x != -1 && nFlags & MK_LBUTTON)
	{
		switch (GetDocument()->status)
		{
		case STATUS_SCALEVIEW:
			view[activeView].scale(1 - (double)(point.y - oldpos.y) / 100);
			redrawScreen(pDC, DRAW_ACTIVEVIEW);
			break;
		case STATUS_ZOOMVIEW:
			view[activeView].zoom(1 - (double)(point.y - oldpos.y) / 100);
			redrawScreen(pDC, DRAW_ACTIVEVIEW);
			break;
		case STATUS_MOVEVIEW:
			{
				double dx = rc[activeView].Width() / view[activeView].getWidth();
				double dy = rc[activeView].Height() / view[activeView].getHeight();
				view[activeView].move((oldpos.x - point.x) / dx, (point.y - oldpos.y) / dy);
				redrawScreen(pDC, DRAW_ACTIVEVIEW);
				break;
			}
		case STATUS_ROTATEVIEW:
			view[activeView].verticalRotate((point.y - oldpos.y) * PI / 180);
			view[activeView].horizontalRotate((point.x - oldpos.x) * PI / 180);
			redrawScreen(pDC, DRAW_ACTIVEVIEW);
			if (view[activeView].getType() == PROJECTION_PARALLEL) viewName[activeView] = "User";
			break;
		case STATUS_MOVE:
			{
				if (!GetDocument()->selection) break;

				CVertex nv = view[activeView].getVertexOnHF(point, rc[activeView]);
				CVector l = nv - oldver;
				oldver = nv;
				GetDocument()->selection->move(l);
				CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
				if (pFrame)
				{
					CToolForm *form = (CToolForm*)pFrame->m_wndSplitter.GetPane(0, 1);
					if (form->m_tab.GetCurSel() == 1) form->refreshButtons();
				}
				redrawScreen(pDC, DRAW_ALLVIEW);
				GetDocument()->SetModifiedFlag();
				break;
			}
		case STATUS_SCALE:
			{
				if (!GetDocument()->selection || GetDocument()->selection->getType() == TYPE_LIGHT) break;

				CGeometry *g = (CGeometry*)GetDocument()->selection;
				CVertex c = g->getCenter();
				g->move(CVertex(0, 0, 0) - c);
				g->scale(1 - (double)(point.y - oldpos.y) / 100);
				g->move(c - CVertex(0, 0, 0));
				CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
				if (pFrame)
				{
					CToolForm *form = (CToolForm*)pFrame->m_wndSplitter.GetPane(0, 1);
					if (form->m_tab.GetCurSel() == 1) form->refreshButtons();
				}
				redrawScreen(pDC, DRAW_ALLVIEW);
				GetDocument()->SetModifiedFlag();
				break;
			}
		case STATUS_ROTATE:
			{
				if (!GetDocument()->selection || GetDocument()->selection->getType() == TYPE_LIGHT) break;

				CGeometry *g = (CGeometry*)GetDocument()->selection;
				CVector axis = view[activeView].getHoriX() * view[activeView].getHoriY();
				g->rotate(g->getCenter(), axis, (point.x - oldpos.x) * PI / 180);
				redrawScreen(pDC, DRAW_ALLVIEW);
				GetDocument()->SetModifiedFlag();
				break;
			}
		}
	}

	switch (GetDocument()->status)
	{
	case STATUS_NEW_CUBE:
		switch (GetDocument()->createstep)
		{
		case 1:
			{
				CCube *cube = (CCube*)GetDocument()->temp;
				double w = (v - GetDocument()->ov) & view[activeView].getHoriX();
				double l = (v - GetDocument()->ov) & view[activeView].getHoriY();
				cube->setWidth(fabs(w));
				cube->setLength(fabs(l));
				cube->setCenter(GetDocument()->ov + w / 2 * view[activeView].getHoriX() + l / 2 * view[activeView].getHoriY());
				redrawScreen(pDC, DRAW_ALLVIEW);
				break;
			}
		case 2:
			{
				CCube *cube = (CCube*)GetDocument()->temp;
				double h;
				if (view[activeView].getType() == PROJECTION_PERSPECTIVE)
				{
					CVertex nv = view[activeView].getVertexOnFace(point, rc[activeView], GetDocument()->ov, view[activeView].getHoriX());
					h = (nv - GetDocument()->ov) & (view[activeView].getHoriX() * view[activeView].getHoriY());
				}
				else
				{
					CVertex nv = view[activeView].getVertexOnHF(point, rc[activeView]);
					h = (nv - GetDocument()->ov) & view[activeView].getHoriY();
				}
				double h1 = (cube->getCenter() - GetDocument()->ov) & (view[activeView].getHoriX() * view[activeView].getHoriY());
				cube->setCenter(cube->getCenter() + (h / 2 - h1) * (view[activeView].getHoriX() * view[activeView].getHoriY()));
				cube->setHeight(fabs(h));
				redrawScreen(pDC, DRAW_ALLVIEW);
				break;
			}
		}
		break;
	case STATUS_NEW_SPHERE:
		switch (GetDocument()->createstep)
		{
		case 1:
			{
				CSphere *sphere = (CSphere*)GetDocument()->temp;
				sphere->setRadius((v - GetDocument()->ov).getLength());
				redrawScreen(pDC, DRAW_ALLVIEW);
				break;
			}
		}
		break;
	case STATUS_NEW_CYLINDER:
		switch (GetDocument()->createstep)
		{
		case 1:
			{
				CCylinder *cylinder = (CCylinder*)GetDocument()->temp;
				cylinder->setPRadius((v - GetDocument()->ov).getLength());
				redrawScreen(pDC, DRAW_ALLVIEW);
				break;
			}
		case 2:
			{
				CCylinder *cylinder = (CCylinder*)GetDocument()->temp;
				double h;
				if (view[activeView].getType() == PROJECTION_PERSPECTIVE)
				{
					CVertex nv = view[activeView].getVertexOnFace(point, rc[activeView], GetDocument()->ov, view[activeView].getHoriX());
					h = (nv - GetDocument()->ov) & (view[activeView].getHoriX() * view[activeView].getHoriY());
				}
				else
				{
					CVertex nv = view[activeView].getVertexOnHF(point, rc[activeView]);
					h = (nv - GetDocument()->ov) & view[activeView].getHoriY();
				}
				double h1 = (cylinder->getCenter() - GetDocument()->ov) & (view[activeView].getHoriX() * view[activeView].getHoriY());
				cylinder->setCenter(cylinder->getCenter() + (h / 2 - h1) * (view[activeView].getHoriX() * view[activeView].getHoriY()));
				cylinder->setHeight(fabs(h));
				redrawScreen(pDC, DRAW_ALLVIEW);
				break;
			}
		}
		break;
	case STATUS_NEW_CONE:
		switch (GetDocument()->createstep)
		{
		case 1:
			{
				CCone *cone = (CCone*)GetDocument()->temp;
				cone->setTRadius((v - GetDocument()->ov).getLength());
				cone->setBRadius((v - GetDocument()->ov).getLength());
				redrawScreen(pDC, DRAW_ALLVIEW);
				break;
			}
		case 2:
			{
				CCone *cone = (CCone*)GetDocument()->temp;
				double h;
				if (view[activeView].getType() == PROJECTION_PERSPECTIVE)
				{
					CVertex nv = view[activeView].getVertexOnFace(point, rc[activeView], GetDocument()->ov, view[activeView].getHoriX());
					h = (nv - GetDocument()->ov) & (view[activeView].getHoriX() * view[activeView].getHoriY());
				}
				else
				{
					CVertex nv = view[activeView].getVertexOnHF(point, rc[activeView]);
					h = (nv - GetDocument()->ov) & view[activeView].getHoriY();
				}
				CVector v1 = view[activeView].getHoriX() * view[activeView].getHoriY();
				if (h < 0) v1 = -v1;
				h = fabs(h);
				if ((cone->getAxis() & v1) < 0) h = -h;
				cone->setHeight(h);
				redrawScreen(pDC, DRAW_ALLVIEW);
				break;
			}
		case 3:
			{
				CCone *cone = (CCone*)GetDocument()->temp;
				double r = cone->getTRadius() - (point.y - GetDocument()->ov.x) * cone->getBRadius() / 100;
				if (r < 0) r = 0;
				cone->setTRadius(r);
				GetDocument()->ov = CVertex(point.y, 0, 0);
				redrawScreen(pDC, DRAW_ALLVIEW);
				break;
			}
		}
		break;
	}

	ReleaseDC(pDC);
	oldpos = point;

	CStatusBar *pBar = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	if (pBar)
	{
		CString s;
		s.Format("X:%4.2f", v.x);
		pBar->SetPaneText(1, s);
		s.Format("Y:%4.2f", v.y);
		pBar->SetPaneText(2, s);
		s.Format("Z:%4.2f", v.z);
		pBar->SetPaneText(3, s);
	}

	CView::OnMouseMove(nFlags, point);
}

void CMy3DCreatorView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CRect crc;
	GetClientRect(crc);
	CDC *pDC = GetDC();

	int x = 0;
	int y = 0;
	int na = 0;
	if (!zoom)
	{
		if (point.x > crc.Width() / 2)
		{
			x += crc.Width() / 2;
			na += 1;
		}
		if (point.y > crc.Height() / 2)
		{
			y += crc.Height() / 2;
			na += 2;
		}
	}
	else na = activeView;

	CSize size = pDC->GetTextExtent(viewName[na]);
	if (point.x - x < size.cx + 4 && point.y - y < size.cy + 4)
	{
		if (na != activeView)
		{
			activeView = na;
			redrawScreen(pDC, DRAW_BORDER);
		}
		CMenu menu;
		menu.LoadMenu(IDR_POPUP_MENU);
		CMenu* view = menu.GetSubMenu(0);
		CPoint pos = point;
		ClientToScreen(&pos);
		if (smooth[activeView])
		{
			view->CheckMenuItem(0, MF_BYPOSITION | MF_UNCHECKED);
			view->CheckMenuItem(1, MF_BYPOSITION | MF_CHECKED);
		}
		else
		{
			view->CheckMenuItem(0, MF_BYPOSITION | MF_CHECKED);
			view->CheckMenuItem(1, MF_BYPOSITION | MF_UNCHECKED);
		}
		if (zoom) view->CheckMenuItem(3, MF_BYPOSITION | MF_CHECKED);
		else view->CheckMenuItem(3, MF_BYPOSITION | MF_UNCHECKED);
		if (eliminate[activeView]) view->CheckMenuItem(4, MF_BYPOSITION | MF_CHECKED);
		else view->CheckMenuItem(4, MF_BYPOSITION | MF_UNCHECKED);
		view->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);
		CView::OnRButtonDown(nFlags, point);
		return;
	}
		
	if (na != activeView)
	{
		activeView = na;
		redrawScreen(pDC, DRAW_BORDER);
		CView::OnRButtonDown(nFlags, point);
		return;
	}

	switch (GetDocument()->status)
	{
	case STATUS_NEW_CUBE:
	case STATUS_NEW_SPHERE:
	case STATUS_NEW_CYLINDER:
	case STATUS_NEW_CONE:
		if (GetDocument()->createstep == 0)
		{
			GetDocument()->switchStatus(STATUS_SELECTING);
			CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			if (pFrame)
			{
				CToolForm *form = (CToolForm*)pFrame->m_wndSplitter.GetPane(0, 1);
				form->refreshButtons();
			}
		}
		else
		{
			GetDocument()->createstep = 0;
			if (GetDocument()->temp != NULL)
			{
				delete GetDocument()->temp;
				GetDocument()->temp =NULL;
				redrawScreen(pDC, DRAW_ALLVIEW);
			}
		}
		break;
	case STATUS_INTERSECT:
	case STATUS_UNION:
	case STATUS_DIFFERENCE:
		{
			if (GetDocument()->createstep == 0) GetDocument()->switchStatus(STATUS_SELECTING);
			else GetDocument()->createstep = 0;
			CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			if (pFrame)
			{
				CToolForm *form = (CToolForm*)pFrame->m_wndSplitter.GetPane(0, 1);
				form->refreshButtons();
			}
			break;
		}
	case STATUS_NEW_LIGHT:
		GetDocument()->switchStatus(STATUS_SELECTING);
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		if (pFrame)
		{
			CToolForm *form = (CToolForm*)pFrame->m_wndSplitter.GetPane(0, 1);
			form->refreshButtons();
		}
		break;
	}

	ReleaseDC(pDC);
	CView::OnRButtonDown(nFlags, point);
}

void CMy3DCreatorView::OnEliminate() 
{
	eliminate[activeView] = !eliminate[activeView];
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnZoom() 
{
	zoom = !zoom;
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ALLVIEW);
	redrawScreen(pDC, DRAW_BORDER);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnTop() 
{
	CRect rc;
	GetClientRect(rc);
	int w = (rc.Width() - 3 * VIEW_GAP);
	int h = (rc.Height() - 3 * VIEW_GAP);

	view[activeView] = CUserView::createStockView(TOP_VIEW);
	viewName[activeView] = "Top";
	view[activeView].setWidth(view[activeView].getHeight() * w / h);
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnBack() 
{
	CRect rc;
	GetClientRect(rc);
	int w = (rc.Width() - 3 * VIEW_GAP);
	int h = (rc.Height() - 3 * VIEW_GAP);

	view[activeView] = CUserView::createStockView(BACK_VIEW);
	viewName[activeView] = "Back";
	view[activeView].setWidth(view[activeView].getHeight() * w / h);
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnBottom() 
{
	CRect rc;
	GetClientRect(rc);
	int w = (rc.Width() - 3 * VIEW_GAP);
	int h = (rc.Height() - 3 * VIEW_GAP);

	view[activeView] = CUserView::createStockView(BOTTOM_VIEW);
	viewName[activeView] = "Bottom";
	view[activeView].setWidth(view[activeView].getHeight() * w / h);
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnLeft() 
{
	CRect rc;
	GetClientRect(rc);
	int w = (rc.Width() - 3 * VIEW_GAP);
	int h = (rc.Height() - 3 * VIEW_GAP);

	view[activeView] = CUserView::createStockView(LEFT_VIEW);
	viewName[activeView] = "Left";
	view[activeView].setWidth(view[activeView].getHeight() * w / h);
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnFront() 
{
	CRect rc;
	GetClientRect(rc);
	int w = (rc.Width() - 3 * VIEW_GAP);
	int h = (rc.Height() - 3 * VIEW_GAP);

	view[activeView] = CUserView::createStockView(FRONT_VIEW);
	viewName[activeView] = "Front";
	view[activeView].setWidth(view[activeView].getHeight() * w / h);
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnRight() 
{
	CRect rc;
	GetClientRect(rc);
	int w = (rc.Width() - 3 * VIEW_GAP);
	int h = (rc.Height() - 3 * VIEW_GAP);

	view[activeView] = CUserView::createStockView(RIGHT_VIEW);
	viewName[activeView] = "Right";
	view[activeView].setWidth(view[activeView].getHeight() * w / h);
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnPerspective() 
{
	CRect rc;
	GetClientRect(rc);
	int w = (rc.Width() - 3 * VIEW_GAP);
	int h = (rc.Height() - 3 * VIEW_GAP);

	view[activeView] = CUserView::createStockView(PERSPECTIVE_VIEW);
	viewName[activeView] = "Perspective";
	view[activeView].setWidth(view[activeView].getHeight() * w / h);
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnSmooth() 
{
	smooth[activeView] = true;
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}

void CMy3DCreatorView::OnWireframe() 
{
	smooth[activeView] = false;
	CDC *pDC = GetDC();
	redrawScreen(pDC, DRAW_ACTIVEVIEW);
	ReleaseDC(pDC);
}
