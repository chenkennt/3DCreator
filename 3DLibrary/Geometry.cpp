// Geometry.cpp: implementation of the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Geometry.h"
#include "Constants.h"
#include "IntersectObject.h"
#include "UnionObject.h"
#include "DifferenceObject.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeometry::CGeometry()
{
}

CGeometry::CGeometry(CCSGModel *csg, COLORREF color)
{
	csgModel = csg;
	surfaceModel = csg->createSurfaceModel();
	material = CMaterial(color);
}

CGeometry::~CGeometry()
{
	if (csgModel != NULL) delete csgModel;
	if (surfaceModel != NULL) delete surfaceModel;
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CGeometry::move(const CVector &d)
{
	csgModel->move(d);
	surfaceModel->move(d);
}

CVertex CGeometry::getCenter()
{
	return csgModel->getCenter();
}

void CGeometry::scale(double s)
{
	csgModel->scale(s);
	surfaceModel->scale(s);
}

void CGeometry::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	csgModel->rotate(ver, vec, theta);
	surfaceModel->rotate(ver, vec, theta);
}

CMatrix CGeometry::getRotateMatrix(const CVertex &ver, const CVector &vec, double theta)
{
	double a = vec.x;
	double b = vec.y;
	double c = vec.z;
	double d = sqrt(b * b + c * c);
	double r = sqrt(a * a + b * b + c * c);

	CMatrix tr(4, 4);
	tr[0][0] = tr[1][1] = tr[2][2] = tr[3][3] = 1;
	tr[3][0] = -ver.x;
	tr[3][1] = -ver.y;
	tr[3][2] = -ver.z;

	CMatrix rx(4, 4);
	rx[0][0] = rx[3][3] = 1;
	if (d == 0) rx[1][1] = rx[2][2] = 1;
	else
	{
		rx[1][1] = rx[2][2] = c / d;
		rx[1][2] = b / d;
		rx[2][1] = -rx[1][2];
	}

	CMatrix ry(4, 4);
	ry[1][1] = ry[3][3] = 1;
	ry[0][0] = ry[2][2] = d / r;
	ry[2][0] = -a / r;
	ry[0][2] = -ry[2][0];

	CMatrix rz(4, 4);
	rz[2][2] = rz[3][3] = 1;
	rz[0][0] = rz[1][1] = cos(theta);
	rz[0][1] = sin(theta);
	rz[1][0] = -rz[0][1];

	CMatrix pre = tr * rx * ry;

	tr[3][0] = -tr[3][0];
	tr[3][1] = -tr[3][1];
	tr[3][2] = -tr[3][2];

	rx[1][2] = -rx[1][2];
	rx[2][1] = -rx[2][1];
	
	ry[2][0] = -ry[2][0];
	ry[0][2] = -ry[0][2];

	CMatrix post = ry * rx * tr;

	CMatrix rm = pre * rz * post;

	return rm;
}

CMatrix CGeometry::getAlterViewMatrix(const CUserView &view)
{
	CVector vpn = view.getVPN();
	CVector v = view.getVUP();
	CVector u = vpn * v;
	vpn.standardize();
	v.standardize();
	u.standardize();

	CMatrix t(4, 4);
	t[0][0] = t[1][1] = t[2][2] = t[3][3] = 1;
	t[3][0] = -view.getCOP().x;
	t[3][1] = -view.getCOP().y;
	t[3][2] = -view.getCOP().z;

	CMatrix r(4, 4);
	r[0][0] = u.x;
	r[0][1] = v.x;
	r[0][2] = vpn.x;
	r[1][0] = u.y;
	r[1][1] = v.y;
	r[1][2] = vpn.y;
	r[2][0] = u.z;
	r[2][1] = v.z;
	r[2][2] = vpn.z;
	r[3][3] = 1;

	return t * r;
}

void CGeometry::executeTransform(CVertex &v, const CMatrix &m)
{
	CMatrix vm(1, 4);
	vm[0][0] = v.x;
	vm[0][1] = v.y;
	vm[0][2] = v.z;
	vm[0][3] = 1;
	CMatrix tvm = vm * m;
	v.x = tvm[0][0];
	v.y = tvm[0][1];
	v.z = tvm[0][2];
}

void CGeometry::drawEdge(CDC *pDC, const CVertex &v1, const CVertex &v2, const CUserView &view, const CRect viewRect, const CMatrix &rm)
{
	double faces[5][4];
	view.getFaceParameters(faces);
	CVertex vv1 = v1;
	CVertex vv2 = v2;
	executeTransform(vv1, rm);
	executeTransform(vv2, rm);

	for (int i = 0; i < 5; i++)
	{
		double p = vv1.x * faces[i][0] + vv1.y * faces[i][1] + vv1.z * faces[i][2] + faces[i][3];
		double q = vv2.x * faces[i][0] + vv2.y * faces[i][1] + vv2.z * faces[i][2] + faces[i][3];
		if (p < 0 && q < 0) return;
		if (p >= 0 && q >= 0) continue;
		CVector k = vv2 - vv1;
		double t = p / (p - q);
		if (p < 0) vv1 = vv1 + t * k;
		else vv2 = vv1 + t * k;
	}

	double dx = viewRect.Width() / view.getWidth();
	double dy = viewRect.Height() / view.getHeight();
	if (view.getType() == PROJECTION_PERSPECTIVE && fabs(vv1.z) > ZERO)
	{
		vv1.x = vv1.x * view.getDistance() / vv1.z;
		vv1.y = vv1.y * view.getDistance() / vv1.z;
		vv2.x = vv2.x * view.getDistance() / vv2.z;
		vv2.y = vv2.y * view.getDistance() / vv2.z;
	}
	pDC->MoveTo(viewRect.CenterPoint().x + (int)(vv1.x * dx), viewRect.CenterPoint().y - (int)(vv1.y * dy));
	pDC->LineTo(viewRect.CenterPoint().x + (int)(vv2.x * dx), viewRect.CenterPoint().y - (int)(vv2.y * dy));
}

void CGeometry::unionGeometry(CGeometry *g)
{
	csgModel = new CUnionObject(csgModel, g->csgModel);
	CSurfaceModel *model = surfaceModel;
	surfaceModel = surfaceModel->unionModel(*g->surfaceModel);
	delete model;
	g->csgModel = NULL;
	delete g;
}

void CGeometry::intersectGeometry(CGeometry *g)
{
	csgModel = new CIntersectObject(csgModel, g->csgModel);
	CSurfaceModel *model = surfaceModel;
	surfaceModel = surfaceModel->intersectModel(*g->surfaceModel);
	delete model;
	g->csgModel = NULL;
	delete g;
}

void CGeometry::differenceGeometry(CGeometry *g)
{
	csgModel = new CDifferenceObject(csgModel, g->csgModel);
	CSurfaceModel *model = surfaceModel;
	surfaceModel = surfaceModel->differenceModel(*g->surfaceModel);
	delete model;
	g->csgModel = NULL;
	delete g;
}

int CGeometry::getType()
{
	return csgModel->getType();
}

void CGeometry::draw(CDC *pDC, const CRect &viewRect, const CUserView &view, const CMatrix &rm, COLORREF color, bool eliminate)
{
	surfaceModel->draw(pDC, viewRect, view, rm, color, eliminate);
}

CCSGModel *CGeometry::getCSGModel()
{
	return csgModel;
}

CGraphicObject *CGeometry::copy()
{
	CGeometry *g = new CGeometry;
	g->name = name;
	g->material = material;
	g->csgModel = csgModel->copy();
	g->surfaceModel = new CSurfaceModel(*surfaceModel);

	return g;
}

void CGeometry::recreateSurfaceModel()
{
	if (surfaceModel != NULL) delete surfaceModel;
	
	surfaceModel = csgModel->createSurfaceModel();
}

void CGeometry::read(CArchive &ar)
{
	ar>>name;
	csgModel = CCSGModel::createFromFile(ar);
	material.read(ar);
	surfaceModel = csgModel->createSurfaceModel();
}

void CGeometry::write(CArchive &ar)
{
	ar<<name;
	csgModel->write(ar);
	material.write(ar);
}