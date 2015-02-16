// LoftDialog.cpp : implementation file
//

#include "stdafx.h"
#include "3DCreator.h"
#include "LoftDialog.h"
#include "3DCreatorView.h"
#include "LoftLengthDialog.h"
#include "3DLibrary\Constants.h"
#include "3dLibrary\CircleLoftObject.h"
#include "3dLibrary\LineLoftObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STATUS_DRAWSHAPE 0
#define STATUS_EDITSHAPE 1
#define STATUS_IDLE 2
#define STATUS_LOFT_MOVEVIEW 3
#define STATUS_LOFT_ZOOMVIEW 4
#define STATUS_LOFT_MOVE 5
#define STATUS_LOFT_ROTATE 6
#define STATUS_LOFT_SCALE 7

/////////////////////////////////////////////////////////////////////////////
// CLoftDialog dialog


CLoftDialog::CLoftDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoftDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoftDialog)
	//}}AFX_DATA_INIT
}


void CLoftDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoftDialog)
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Control(pDX, IDC_POSY, m_posy);
	DDX_Control(pDX, IDC_POSX, m_posx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoftDialog, CDialog)
	//{{AFX_MSG_MAP(CLoftDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_LOFT_MOVEVIEW, OnLoftMoveview)
	ON_COMMAND(ID_LOFT_ZOOMVIEW, OnLoftZoomview)
	ON_COMMAND(ID_LOFT_DRAWSHAPE, OnLoftDrawshape)
	ON_COMMAND(ID_LOFT_EDIT, OnLoftEdit)
	ON_COMMAND(ID_LOFT_MOVE, OnLoftMove)
	ON_COMMAND(ID_LOFT_ROTATE, OnLoftRotate)
	ON_COMMAND(ID_LOFT_SCALE, OnLoftScale)
	ON_COMMAND(ID_CIRCLELOFT, OnCircleloft)
	ON_COMMAND(ID_LINELOFT, OnLineloft)
	ON_COMMAND(ID_EXITLOFT, OnExitloft)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoftDialog message handlers

void CLoftDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	redrawScreen(&dc);
}

BOOL CLoftDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	view = CUserView::createStockView(FRONT_VIEW);
	viewRect = CRect(0, 0, 400, 400);
	switchStatus(STATUS_DRAWSHAPE);
	select = NULL;
	slink = NULL;
	m_posx.pushed = true;
	m_posy.pushed = true;
	m_status.pushed = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoftDialog::redrawScreen(CDC *pDC)
{
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, viewRect.Width(), viewRect.Height());
	CBitmap *oldbmp = dc.SelectObject(&bmp);

	CBrush brush(BKCOLOR);
	CPen pen(PS_SOLID, 1, BKCOLOR);
	CBrush *oldbrush = dc.SelectObject(&brush);
	CPen *oldpen = dc.SelectObject(&pen);
	dc.SetBkColor(BKCOLOR);

	dc.Rectangle(viewRect);
	view.drawHorizontalFace(&dc, viewRect, GRIDCOLOR, AXISCOLOR);
	if (status == STATUS_EDITSHAPE) shape.drawShape(&dc, view, viewRect, SELECTEDCOLOR, true);
	else shape.drawShape(&dc, view, viewRect, SELECTEDCOLOR);
	CPen wpen(PS_SOLID, 1, SELECTEDCOLOR);
	dc.SelectObject(wpen);
	if (status == STATUS_DRAWSHAPE)
	{
		if (createstep != 0)
		{
			CShape::recursiveDrawShape(&dc, viewRect, view, v1, v2, v3, v4, 0, 0.5);
			CShape::recursiveDrawShape(&dc, viewRect, view, v1, v2, v3, v4, 0.5, 1);
		}
		if (createstep == 3) CShape::recursiveDrawShape(&dc, viewRect, view, v4, v4, v5, v5, 0, 1);
	}
	pDC->BitBlt(viewRect.left, viewRect.top, viewRect.Width(), viewRect.Height(), &dc, 0, 0, SRCCOPY);

	dc.SelectObject(oldbrush);
	dc.SelectObject(oldpen);
	brush.DeleteObject();
	pen.DeleteObject();
	wpen.DeleteObject();
}

void CLoftDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!viewRect.PtInRect(point))
	{
		CDialog::OnMouseMove(nFlags, point);
		return;
	}

	CVertex v = view.getVertexOnHF(point, viewRect);

	switch (status)
	{
	case STATUS_DRAWSHAPE:
		switch (createstep)
		{
		case 0:
			v1 = v2 = v3 = v4 = v;
			createstep = 1;
			break;
		case 2:
			{
				createstep = 3;
				v5 = v4;
				ShapeElement *se = shape.getFirstVertex();
				if (se && shape.selectVertex(point, viewRect, view) == se) close = true;
				else close = false;
			}
			break;
		}
		break;
	case STATUS_EDITSHAPE:
		select = shape.selectVertex(point, viewRect, view);
		slink = shape.selectVector(point, viewRect, view);
		scenter = shape.selectCenter(point, viewRect, view);
		break;
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CLoftDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!viewRect.PtInRect(point))
	{
		CDialog::OnMouseMove(nFlags, point);
		return;
	}

	switch (status)
	{
	case STATUS_DRAWSHAPE:
		switch (createstep)
		{
		case 1:
			createstep = 2;
			v2 = v3;
			break;
		case 3:
			{
				if (close)
				{
					ShapeElement *se = shape.getFirstVertex();
					if ((v3 - v4).getLength() < ZERO) shape.add(v4, v2 - v1, -se->next->vector, true);
					else shape.add(v4, v2 - v1, v3 - v4, true);
					switchStatus(STATUS_IDLE);
					CDC *pDC = GetDC();
					redrawScreen(pDC);
					ReleaseDC(pDC);
				}
				else
				{
					shape.add(v1);
					shape.add(v4, v2 - v1, v3 - v4);
					v1 = v4;
					v2 = v3 = v4 = v5;
					createstep = 2;
				}
				break;
			}
		}
		break;
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CLoftDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!viewRect.PtInRect(point))
	{
		CDialog::OnMouseMove(nFlags, point);
		return;
	}

	CVertex v = view.getVertexOnHF(point, viewRect);
	CDC *pDC = GetDC();

	if (oldpos.x != -1 && nFlags & MK_LBUTTON)
	{
		switch (status)
		{
		case STATUS_LOFT_ZOOMVIEW:
			view.zoom(1 - (double)(point.y - oldpos.y) / 100);
			redrawScreen(pDC);
			break;
		case STATUS_LOFT_MOVEVIEW:
			{
				double dx = viewRect.Width() / view.getWidth();
				double dy = viewRect.Height() / view.getHeight();
				view.move((oldpos.x - point.x) / dx, (point.y - oldpos.y) / dy);
				redrawScreen(pDC);
				break;
			}
		case STATUS_LOFT_MOVE:
			shape.move(view.getVertexOnHF(point, viewRect) - view.getVertexOnHF(oldpos, viewRect));
			redrawScreen(pDC);
			break;
		case STATUS_LOFT_ROTATE:
			shape.rotate((oldpos.x - point.x) * PI / 180);
			redrawScreen(pDC);
			break;
		case STATUS_LOFT_SCALE:
			{
				CVertex c = shape.getCenter();
				shape.move(CVertex(0, 0, 0) - c);
				shape.scale(1 - (double)(point.y - oldpos.y) / 100);
				shape.move(c - CVertex(0, 0, 0));
				redrawScreen(pDC);
				break;
			}
		}
	}

	switch (status)
	{
	case STATUS_DRAWSHAPE:
		switch (createstep)
		{
		case 1:
			v3 = v4 = v;
			redrawScreen(pDC);
			break;
		case 2:
			v3 = v4 = v;
			redrawScreen(pDC);
			break;
		case 3:
			v3 = v4 + (v4 - v);
			v5 = v;
			redrawScreen(pDC);
			break;
		}
		break;
	case STATUS_EDITSHAPE:
		if (nFlags & MK_LBUTTON)
		{
			if (select != NULL && !(nFlags & MK_CONTROL && slink != NULL))
			{
				select->vertex = v;
				redrawScreen(pDC);
			}
			else if (slink != NULL)
			{
				ShapeLink *link;
				bool l = true;
				CVertex ov;
				if (!slink->linked->prev || slink->linked->prev->linked->next != slink)
				{
					ShapeElement *se = slink->linked->next->linked;
					ov = se->vertex;
					if (se->next) link = se->next;
					else l = false;
				}
				else
				{
					ShapeElement *se = slink->linked->prev->linked;
					ov = se->vertex;
					if (se->prev) link = se->prev;
					else l = false;
				}
				if (l && (slink->vector + link->vector).getLength() < ZERO && !(nFlags & MK_SHIFT))
				{
					slink->vector = v - ov;
					link->vector = ov - v;
				}
				else slink->vector = v - ov;

				redrawScreen(pDC);
			}
			else if (scenter)
			{
				shape.setCenter(v);
				redrawScreen(pDC);
			}
		}
		break;
	}

	CString s;
	s.Format("X:%4.2f", v.x);
	m_posx.SetWindowText(s);
	s.Format("Y:%4.2f", v.y);
	m_posy.SetWindowText(s);
	ReleaseDC(pDC);
	oldpos = point;

	CDialog::OnMouseMove(nFlags, point);
}

void CLoftDialog::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (!viewRect.PtInRect(point))
	{
		CDialog::OnMouseMove(nFlags, point);
		return;
	}

	switch (status)
	{
	case STATUS_DRAWSHAPE:
		{
			switchStatus(STATUS_IDLE);
			CDC *pDC = GetDC();
			redrawScreen(pDC);
			ReleaseDC(pDC);
			break;
		}
	}

	CDialog::OnRButtonDown(nFlags, point);
}

void CLoftDialog::OnLoftMoveview() 
{
	switchStatus(STATUS_LOFT_MOVEVIEW);
}
void CLoftDialog::OnLoftZoomview() 
{
	switchStatus(STATUS_LOFT_ZOOMVIEW);
}

void CLoftDialog::switchStatus(int s)
{
	status = s;
	if (status == STATUS_DRAWSHAPE)
	{
		createstep = 0;
		shape.clear();
		select = NULL;
	}
	
	switch (status)
	{
	case STATUS_DRAWSHAPE:
		m_status.SetWindowText("绘制曲线");
		break;
	case STATUS_EDITSHAPE:
		m_status.SetWindowText("修改曲线");
		break;
	case STATUS_IDLE:
		m_status.SetWindowText("");
		break;
	case STATUS_LOFT_MOVEVIEW:
		m_status.SetWindowText("移动视图");
		break;
	case STATUS_LOFT_ZOOMVIEW:
		m_status.SetWindowText("视野缩放");
		break;
	case STATUS_LOFT_MOVE:
		m_status.SetWindowText("移动曲线");
		break;
	case STATUS_LOFT_ROTATE:
		m_status.SetWindowText("旋转曲线");
		break;
	case STATUS_LOFT_SCALE:
		m_status.SetWindowText("缩放曲线");
		break;
	}

	CDC *pDC = GetDC();
	redrawScreen(pDC);
	ReleaseDC(pDC);
}

void CLoftDialog::OnLoftDrawshape() 
{
	switchStatus(STATUS_DRAWSHAPE);
}

void CLoftDialog::OnLoftEdit() 
{
	switchStatus(STATUS_EDITSHAPE);	
}

void CLoftDialog::OnLoftMove() 
{
	switchStatus(STATUS_LOFT_MOVE);	
}

void CLoftDialog::OnLoftRotate() 
{
	switchStatus(STATUS_LOFT_ROTATE);	
}

void CLoftDialog::OnLoftScale() 
{
	switchStatus(STATUS_LOFT_SCALE);	
}

void CLoftDialog::OnCircleloft() 
{
	loft = CCircleLoftObject::create(shape, CVertex(0, 0, 0), CVector(0, 1, 0));
	if (loft == NULL) AfxMessageBox("放样图案错误！");
	else OnOK();
}

void CLoftDialog::OnLineloft() 
{
	CLoftLengthDialog dlg;
	dlg.m_length = 100;
	if (dlg.DoModal() == IDOK)
	{
		loft = CLineLoftObject::create(shape, CVertex(0, 0, 0), CVector(0, 0, 1), CVector(0, 1, 0), dlg.m_length);
		if (loft == NULL) AfxMessageBox("放样图案错误！");
		else OnOK();
	}
}

void CLoftDialog::OnExitloft() 
{
	OnCancel();
}
