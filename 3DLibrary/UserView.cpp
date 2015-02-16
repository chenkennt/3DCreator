// UserView.cpp: implementation of the CUserView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserView.h"
#include "Matrix.h"
#include "Constants.h"
#include "Geometry.h"
#include "Scene.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserView::CUserView()
{
}

CUserView::CUserView(int type, const CVertex &cop, const CVertex &center, const CVector &vup, double d, double w, double h)
{
	this->type = type;
	this->center = center;
	this->cop = cop;
	distance = d;
	width = w;
	height = h;

	CVector vpn = center - cop;
	vpn.standardize();
	this->vup = vup - (vpn & vup) * vpn;
	this->vup.standardize();
}

CUserView::~CUserView()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

int CUserView::getType() const
{
	return type;
}

CVertex CUserView::getCOP() const
{
	return cop;
}

CVector CUserView::getVUP() const
{
	return vup;
}

CVector CUserView::getVPN() const
{
	return center - cop;
}

double CUserView::getDistance() const
{
	return distance;
}

double CUserView::getWidth() const
{
	return width;
}

double CUserView::getHeight() const
{
	return height;
}

void CUserView::move(double dx, double dy)
{
	CVector v = vup;
	v.standardize();
	CVector u = (center - cop) * vup;
	u.standardize();

	double l;
	if (type == PROJECTION_PERSPECTIVE) l = (center - cop).getLength();
	else if (type == PROJECTION_PARALLEL) l = distance;
	dx *= l / distance;
	dy *= l / distance;
	center = center + dx * u + dy * v;
	cop = cop + dx * u + dy * v;
}

void CUserView::verticalRotate(double theta)
{
	CVertex v(cop + vup);
	CVector u = (center - cop) * vup;

	CMatrix rm = CGeometry::getRotateMatrix(center, u, theta);
	CGeometry::executeTransform(cop, rm);
	CGeometry::executeTransform(v, rm);
	vup = v - cop;
}

void CUserView::horizontalRotate(double theta)
{
	CVertex v(cop + vup);

	CMatrix rm = CGeometry::getRotateMatrix(center, CVector(0, 1, 0), theta);
	CGeometry::executeTransform(cop, rm);
	CGeometry::executeTransform(v, rm);
	vup = v - cop;
}

void CUserView::scale(double d)
{
	if (type == PROJECTION_PERSPECTIVE)
	{
		CVector v = cop - center;
		v = d * v;
		cop = center + v;
	}
}

void CUserView::getFaceParameters(double par[5][4]) const
{
	double parper[5][4] = {
		{2 * distance / width, 0, 1, 0},
		{-2 * distance / width, 0, 1, 0},
		{0, 2 * distance / height, 1, 0},
		{0, -2 * distance / height, 1, 0},
		{0, 0, 1, 0}};
	double parpar[5][4] = {
		{-1, 0, 0, width / 2},
		{1, 0, 0, width / 2},
		{0, -1, 0, height / 2},
		{0, 1, 0, height / 2},
		{0, 0, 0, 0}};
	if (type == PROJECTION_PARALLEL) memcpy(par, parpar, sizeof(double) * 20);
	else if (type == PROJECTION_PERSPECTIVE) memcpy(par, parper, sizeof(double) * 20);
}

CUserView CUserView::createStockView(int viewtype)
{
	switch (viewtype)
	{
	case TOP_VIEW:
		{
			CUserView view(PROJECTION_PARALLEL, CVertex(0, 1000, 0), CVertex(0, 0, 0), CVector(0, 0, -1), 200, 400, 400);
			view.horix = CVector(1, 0, 0);
			view.horiy = CVector(0, 0, -1);
			return view;
		}
	case BOTTOM_VIEW:
		{
			CUserView view(PROJECTION_PARALLEL, CVertex(0, -1000, 0), CVertex(0, 0, 0), CVector(0, 0, 1), 200, 400, 400);
			view.horix = CVector(1, 0, 0);
			view.horiy = CVector(0, 0, 1);
			return view;
		}
	case LEFT_VIEW:
		{
			CUserView view(PROJECTION_PARALLEL, CVertex(-1000, 0, 0), CVertex(0, 0, 0), CVector(0, 1, 0), 200, 400, 400);
			view.horix = CVector(0, 1, 0);
			view.horiy = CVector(0, 0, 1);
			return view;
		}
	case RIGHT_VIEW:
		{
			CUserView view(PROJECTION_PARALLEL, CVertex(1000, 0, 0), CVertex(0, 0, 0), CVector(0, 1, 0), 200, 400, 400);
			view.horix = CVector(0, 1, 0);
			view.horiy = CVector(0, 0, -1);
			return view;
		}
	case FRONT_VIEW:
		{
			CUserView view(PROJECTION_PARALLEL, CVertex(0, 0, 1000), CVertex(0, 0, 0), CVector(0, 1, 0), 200, 400, 400);
			view.horix = CVector(1, 0, 0);
			view.horiy = CVector(0, 1, 0);
			return view;
		}
	case BACK_VIEW:
		{
			CUserView view(PROJECTION_PARALLEL, CVertex(0, 0, -1000), CVertex(0, 0, 0), CVector(0, 1, 0), 200, 400, 400);
			view.horix = CVector(1, 0, 0);
			view.horiy = CVector(0, -1, 0);
			return view;
		}
	case PERSPECTIVE_VIEW:
		{
			CUserView view(PROJECTION_PERSPECTIVE, CVertex(600, 600, 600), CVertex(0, 0, 0), CVector(0, 1, 0), 200, 100, 100);
			view.horix = CVector(1, 0, 0);
			view.horiy = CVector(0, 0, -1);
			return view;
		}
	default:
		{
			CUserView view(PROJECTION_PERSPECTIVE, CVertex(0, 200, 800), CVertex(0, 0, 0), CVector(0, 1, 0), 200, 100, 100);
			view.horix = CVector(1, 0, 0);
			view.horiy = CVector(0, 0, -1);
			return view;
		}
	}
}

void CUserView::drawHorizontalFace(CDC *pDC, const CRect &viewRect, COLORREF pc, COLORREF sc)
{
	CMatrix rm = CGeometry::getAlterViewMatrix(*this);
	CPen pen(PS_SOLID, 1, pc);
	CPen *oldpen = pDC->SelectObject(&pen);
	CPen spen(PS_SOLID, 1, sc);

	CVertex origin(0, 0, 0);
	for (int i = -MAX_HORIZONTAL_FACE_PIECES; i <= MAX_HORIZONTAL_FACE_PIECES; i++)
	{
		CVertex vs = origin + MAX_HORIZONTAL_FACE_GAP * i * horix + MAX_HORIZONTAL_FACE_GAP * MAX_HORIZONTAL_FACE_PIECES * horiy;
		CVertex ve = origin + MAX_HORIZONTAL_FACE_GAP * i * horix + (-MAX_HORIZONTAL_FACE_GAP) * MAX_HORIZONTAL_FACE_PIECES * horiy;
		CGeometry::drawEdge(pDC, vs, ve, *this, viewRect, rm);
		vs = origin + MAX_HORIZONTAL_FACE_GAP * i * horiy + MAX_HORIZONTAL_FACE_GAP * MAX_HORIZONTAL_FACE_PIECES * horix;
		ve = origin + MAX_HORIZONTAL_FACE_GAP * i * horiy + (-MAX_HORIZONTAL_FACE_GAP) * MAX_HORIZONTAL_FACE_PIECES * horix;
		CGeometry::drawEdge(pDC, vs, ve, *this, viewRect, rm);
	}

	pDC->SelectObject(&spen);
	CVertex vs = origin + MAX_HORIZONTAL_FACE_GAP * MAX_HORIZONTAL_FACE_PIECES * horiy;
	CVertex ve = origin + (-MAX_HORIZONTAL_FACE_GAP) * MAX_HORIZONTAL_FACE_PIECES * horiy;
	CGeometry::drawEdge(pDC, vs, ve, *this, viewRect, rm);
	vs = origin + MAX_HORIZONTAL_FACE_GAP * MAX_HORIZONTAL_FACE_PIECES * horix;
	ve = origin + (-MAX_HORIZONTAL_FACE_GAP) * MAX_HORIZONTAL_FACE_PIECES * horix;
	CGeometry::drawEdge(pDC, vs, ve, *this, viewRect, rm);

	pDC->SelectObject(oldpen);
	pen.DeleteObject();
	spen.DeleteObject();
}

void CUserView::setWidth(double w)
{
	width = w;
}

void CUserView::setHeight(double h)
{
	height = h;
}

CVertex CUserView::getVertexOnHF(const CPoint &pt, const CRect &viewRect)
{
	return getVertexOnFace(pt, viewRect, CVertex(0, 0, 0), horix * horiy);
}

CVector CUserView::getHoriX()
{
	return horix;
}

CVector CUserView::getHoriY()
{
	return horiy;
}

CVertex CUserView::getVertexOnFace(const CPoint &pt, const CRect &viewRect, const CVertex &v, const CVector &n)
{
	double a = n.x;
	double b = n.y;
	double c = n.z;
	double d = -(a * v.x + b * v.y + c * v.z);

	double dx = viewRect.Width() / width;
	double dy = viewRect.Height() / height;
	double x = (pt.x - viewRect.CenterPoint().x) / dx;
	double y = (viewRect.CenterPoint().y - pt.y) / dy;

	CVector vpn = center - cop;
	CVector u = vpn * vup;
	u.standardize();
	vpn.standardize();
	CVertex vc = cop + distance * vpn;

	CVector l;
	CVertex o;
	if (type == PROJECTION_PERSPECTIVE)
	{
		l = vc + x * u + y * vup - cop;
		o = cop;
	}
	else if (type == PROJECTION_PARALLEL)
	{
		l = vpn;
		o = vc + x * u + y * vup;
	}

	double k = a * l.x + b * l.y + c * l.z;
	if (fabs(k) < ZERO) return CVertex(0, 0, 0);
	double t = -(a * o.x + b * o.y + c * o.z + d) / k;
	return o + t * l;
}

void CUserView::zoom(double d)
{
	width *= d;
	height *= d;
}

void CUserView::read(CArchive &ar)
{
	ar.Read(&type, sizeof(type));
	cop.read(ar);
	center.read(ar);
	vup.read(ar);
	ar.Read(&distance, sizeof(distance));
	ar.Read(&width, sizeof(width));
	ar.Read(&height, sizeof(height));
	horix.read(ar);
	horiy.read(ar);
}

void CUserView::write(CArchive &ar)
{
	ar.Write(&type, sizeof(type));
	cop.write(ar);
	center.write(ar);
	vup.write(ar);
	ar.Write(&distance, sizeof(distance));
	ar.Write(&width, sizeof(width));
	ar.Write(&height, sizeof(height));
	horix.write(ar);
	horiy.write(ar);
}

void CUserView::align(const CScene &scene)
{
	if (type == PROJECTION_PERSPECTIVE) return;

	CVector v = cop - center;
	double l = v.getLength();
	v.standardize();
	GeometryLink *g = scene.firstGeometry;
	while (g != NULL)
	{
		CCSGModel *csg = g->geometry->getCSGModel();
		double k = ((csg->getCenter() - center) & v) + csg->getRadius();
		if (k + 100 > l) l = k + 100;
		g = g->next;
	}
	cop = center + l * v;
}