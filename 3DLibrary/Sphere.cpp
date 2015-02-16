// Sphere.cpp: implementation of the CSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sphere.h"
#include "Matrix.h"
#include "Constants.h"
#include "Geometry.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSphere::CSphere()
{

}

CSphere::CSphere(const CVertex &c, double r)
{
	center = c;
	radius = r;

	ym = center + CVector(0, 1, 0);
	xm = center + CVector(1, 0, 0);
}

CSphere::~CSphere()
{

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CSphere::move(const CVector &d)
{
	center = center + d;
	xm = xm + d;
	ym = ym + d;
}

void CSphere::scale(double s)
{
	center.x *= s;
	center.y *= s;
	center.z *= s;
	radius *= s;

	xm.x *= s;
	xm.y *= s;
	xm.z *= s;
	ym.x *= s;
	ym.y *= s;
	ym.z *= s;
}

void CSphere::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	CMatrix rm = CGeometry::getRotateMatrix(ver, vec, theta);
	CGeometry::executeTransform(center, rm);

	CGeometry::executeTransform(xm, rm);
	CGeometry::executeTransform(ym, rm);
}

CSurfaceModel *CSphere::createSurfaceModel()
{
	CSurfaceModel *model = new CSurfaceModel;
	CVertex tv(center.x, center.y + radius, center.z);
	CVertex bv(center.x, center.y - radius, center.z);

	for (int i = 0; i < BALL_HORIZONTAL_PIECES; i++)
	{
		double d = radius * sin(PI / BALL_VERTICAL_PIECES);
		double y = radius * cos(PI / BALL_VERTICAL_PIECES);
		double x1 = d * sin(i * 2 * PI / BALL_HORIZONTAL_PIECES);
		double z1 = d * cos(i * 2 * PI / BALL_HORIZONTAL_PIECES);
		double x2 = d * sin((i + 1) * 2 * PI / BALL_HORIZONTAL_PIECES);
		double z2 = d * cos((i + 1) * 2 * PI / BALL_HORIZONTAL_PIECES);
		CVertex v1(tv);
		CVertex v2(center.x + x1, center.y + y, center.z + z1);
		CVertex v3(center.x + x2, center.y + y, center.z + z2);
		CVertex v4(bv);
		CVertex v5(center.x + x1, center.y - y, center.z + z1);
		CVertex v6(center.x + x2, center.y - y, center.z + z2);
		CSurface *s = new CSurface;
		double formu[4];
		CSurface::getSurfaceFormula(v1, v2, center, v3, formu);
		*s<<new CEdge(v1, v2, formu);
		CSurface::getSurfaceFormula(v2, v3, center, v1, formu);
		*s<<new CEdge(v2, v3, formu);
		CSurface::getSurfaceFormula(v3, v1, center, v2, formu);
		*s<<new CEdge(v3, v1, formu);
		double a =
			(v1.y - v2.y) * (v1.z + v2.z) +
			(v2.y - v3.y) * (v2.z + v3.z) +
			(v3.y - v1.y) * (v3.z + v1.z);
		double b =
			(v1.z - v2.z) * (v1.x + v2.x) +
			(v2.z - v3.z) * (v2.x + v3.x) +
			(v3.z - v1.z) * (v3.x + v1.x);
		double c =
			(v1.x - v2.x) * (v1.y + v2.y) +
			(v2.x - v3.x) * (v2.y + v3.y) +
			(v3.x - v1.x) * (v3.y + v1.y);
		double dd = -(a * v1.x + b * v1.y + c * v1.z);
		if (center.x * a + center.y * b + center.z * c + dd < 0) s->setFormula(-a, -b, -c, -dd);
		else s->setFormula(a, b, c, dd);
		*model<<s;
		s = new CSurface;
		CSurface::getSurfaceFormula(v4, v5, center, v6, formu);
		*s<<new CEdge(v4, v5, formu);
		CSurface::getSurfaceFormula(v5, v6, center, v4, formu);
		*s<<new CEdge(v5, v6, formu);
		CSurface::getSurfaceFormula(v6, v4, center, v5, formu);
		*s<<new CEdge(v6, v4, formu);
		a = (v4.y - v5.y) * (v4.z + v5.z) +
			(v5.y - v6.y) * (v5.z + v6.z) +
			(v6.y - v4.y) * (v6.z + v4.z);
		b = (v4.z - v5.z) * (v4.x + v5.x) +
			(v5.z - v6.z) * (v5.x + v6.x) +
			(v6.z - v4.z) * (v6.x + v4.x);
		c = (v4.x - v5.x) * (v4.y + v5.y) +
			(v5.x - v6.x) * (v5.y + v6.y) +
			(v6.x - v4.x) * (v6.y + v4.y);
		dd = -(a * v4.x + b * v4.y + c * v4.z);
		if (center.x * a + center.y * b + center.z * c + dd < 0) s->setFormula(-a, -b, -c, -dd);
		else s->setFormula(a, b, c, dd);
		*model<<s;
	}
	for (i = 1; i < BALL_VERTICAL_PIECES - 1; i++)
		for (int j = 0; j < BALL_HORIZONTAL_PIECES; j++)
		{
			double d1 = radius * sin(i * PI / BALL_VERTICAL_PIECES);
			double d2 = radius * sin((i + 1) * PI / BALL_VERTICAL_PIECES);
			double y1 = radius * cos(i * PI / BALL_VERTICAL_PIECES);
			double y2 = radius * cos((i + 1) * PI / BALL_VERTICAL_PIECES);
			double x1 = d1 * sin(j * 2 * PI / BALL_HORIZONTAL_PIECES);
			double z1 = d1 * cos(j * 2 * PI / BALL_HORIZONTAL_PIECES);
			double x2 = d1 * sin((j + 1) * 2 * PI / BALL_HORIZONTAL_PIECES);
			double z2 = d1 * cos((j + 1) * 2 * PI / BALL_HORIZONTAL_PIECES);
			double x3 = d2 * sin(j * 2 * PI / BALL_HORIZONTAL_PIECES);
			double z3 = d2 * cos(j * 2 * PI / BALL_HORIZONTAL_PIECES);
			double x4 = d2 * sin((j + 1) * 2 * PI / BALL_HORIZONTAL_PIECES);
			double z4 = d2 * cos((j + 1) * 2 * PI / BALL_HORIZONTAL_PIECES);
			CVertex v1(center.x + x1, center.y + y1, center.z + z1);
			CVertex v2(center.x + x2, center.y + y1, center.z + z2);
			CVertex v3(center.x + x3, center.y + y2, center.z + z3);
			CVertex v4(center.x + x4, center.y + y2, center.z + z4);
			CSurface *s = new CSurface;
			double formu[4];
			CSurface::getSurfaceFormula(v1, v2, center, v4, formu);
			*s<<new CEdge(v1, v2, formu);
			CSurface::getSurfaceFormula(v2, v4, center, v3, formu);
			*s<<new CEdge(v2, v4, formu);
			CSurface::getSurfaceFormula(v4, v3, center, v1, formu);
			*s<<new CEdge(v4, v3, formu);
			CSurface::getSurfaceFormula(v3, v1, center, v2, formu);
			*s<<new CEdge(v3, v1, formu);
			double a =
				(v1.y - v2.y) * (v1.z + v2.z) +
				(v2.y - v3.y) * (v2.z + v3.z) +
				(v3.y - v4.y) * (v3.z + v4.z) +
				(v4.y - v1.y) * (v4.z + v1.z);
			double b =
				(v1.z - v2.z) * (v1.x + v2.x) +
				(v2.z - v3.z) * (v2.x + v3.x) +
				(v3.z - v4.z) * (v3.x + v4.x) +
				(v4.z - v1.z) * (v4.x + v1.x);
			double c =
				(v1.x - v2.x) * (v1.y + v2.y) +
				(v2.x - v3.x) * (v2.y + v3.y) +
				(v3.x - v4.x) * (v3.y + v4.y) +
				(v4.x - v1.x) * (v4.y + v1.y);
			double d = -(a * v1.x + b * v1.y + c * v1.z);
			if (center.x * a + center.y * b + center.z * c + d < 0) s->setFormula(-a, -b, -c, -d);
			else s->setFormula(a, b, c, d);
			*model<<s;
		}

	model->center = center;
	return model;
}

void CSphere::updateParameters()
{
}

void CSphere::setRadius(double r)
{
	radius = r;
}

void CSphere::setCenter(const CVertex &c)
{
	center = c;
}

CrossLink *CSphere::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	double t =
		(vec.x * (center.x - ver.x)	+ vec.y * (center.y - ver.y) + vec.z * (center.z - ver.z))
		/ (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	double d =
		sqrt((ver.x + vec.x * t - center.x) * (ver.x + vec.x * t - center.x)
		   + (ver.y + vec.y * t - center.y) * (ver.y + vec.y * t - center.y)
		   + (ver.z + vec.z * t - center.z) * (ver.z + vec.z * t - center.z));
	if (d > radius) return NULL;

	CVector p = ver - center;
	double pa = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	double pb = 2 * (p.x * vec.x + p.y * vec.y + p.z * vec.z);
	double pc = p.x * p.x + p.y * p.y + p.z * p.z - radius * radius;
	double tt[2];
	int n = solveFormula(pa, pb, pc, tt);

	CrossLink *head = NULL;
	for (int i = 0; i < n; i++)
	{
		if (fabs(tt[i]) < ZERO || tt[i] < 0 || (b > 0 && (tt[i] > b || fabs(b - tt[i]) < ZERO))) continue;
		CrossLink *l = new CrossLink(tt[i], p + tt[i] * vec, this);
		l->next = head;
		head = l;
	}
	return head;
}

CCSGModel *CSphere::alterView(const CUserView &view)
{
	CSphere *sphere = new CSphere(*this);

	CMatrix rm = CGeometry::getAlterViewMatrix(view);
	CGeometry::executeTransform(sphere->center, rm);

	CGeometry::executeTransform(sphere->xm, rm);
	CGeometry::executeTransform(sphere->ym, rm);

	return sphere;
}

int CSphere::getLocation(const CVertex &v)
{
	double d = (v - center).getLength() - radius;
	if (fabs(d) < ZERO) return 0;
	if (d > 0) return -1;
	return 1;
}

COLORREF CSphere::getColor(const CVertex &v, const CMaterial &m, CVector &nv)
{
	if (m.texture == NULL) return m.color;

	CVector vec = v - center;
	CVector vx = xm - center;
	CVector vy = ym - center;
	CVector v1 = vec - (vy & vec) * vy;

	CVector vz = vx * vy;
	double alpha = vx ^ v1;
	if ((vz & v1) < 0) alpha = 2 * PI - alpha;
	double dx = alpha * radius;
	double dy = 2 * PI - (vy ^ vec) * radius;

	if (m.texture->getBump())
	{
		double bp[2];
		m.texture->getBumpPixel(dx, dy, bp);
		v1.standardize();
		double xz = v1 & vec;
		double y = -(vy & vec);
		if (xz < 0)
		{
			xz = -xz;
			y = -y;
		}
		CVector by = xz * vy + y * v1;
		CVector bx = by * nv;
		bx.standardize();
		by.standardize();
		nv = bx * by;
		double l = nv.getLength();
		nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
	}

	if (m.texture->getTexture()) return m.texture->getPixel(dx, dy);
	else return m.color;
}

int CSphere::getType()
{
	return TYPE_SPHERE;
}

CCSGModel *CSphere::copy()
{
	return new CSphere(*this);
}

void CSphere::write(CArchive &ar)
{
	int type = TYPE_SPHERE;
	ar.Write(&type, sizeof(int));

	center.write(ar);
	xm.write(ar);
	ym.write(ar);
	ar.Write(&radius, sizeof(double));
}

void CSphere::read(CArchive &ar)
{
	center.read(ar);
	xm.read(ar);
	ym.read(ar);
	ar.Read(&radius, sizeof(double));

	updateParameters();
}