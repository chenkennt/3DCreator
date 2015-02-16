// Cylinder.cpp: implementation of the CCylinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cylinder.h"
#include "Geometry.h"
#include "Constants.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCylinder::CCylinder()
{

}

CCylinder::CCylinder(const CVertex &c, const CVector &a, double h, double r)
{
	center = c;
	axis = a;
	axis.standardize();
	CVector vv(1, 0, 0);
	if ((vv * a).getLength() < ZERO) vv = CVector(0, 1, 0);
	xaxis = vv - (vv & axis) * axis;
	xaxis.standardize();
	height = h;
	pradius = r;

	updateParameters();
}

CCylinder::~CCylinder()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CCylinder::move(const CVector &d)
{
	center = center + d;
	updateParameters();
}

void CCylinder::scale(double s)
{
	center.x = s * center.x;
	center.y = s * center.y;
	center.z = s * center.z;
	pradius *= s;
	height *= s;

	updateParameters();
}

void CCylinder::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	CMatrix rm = CGeometry::getRotateMatrix(ver, vec, theta);
	CVertex t = center + axis;
	CVertex xt = center + xaxis;
	CGeometry::executeTransform(center, rm);
	CGeometry::executeTransform(t, rm);
	CGeometry::executeTransform(xt, rm);
	axis = t - center;
	xaxis = xt - center;

	updateParameters();
}

CSurfaceModel *CCylinder::createSurfaceModel()
{
	CSurfaceModel *model = new CSurfaceModel;

	CSurface *ts = new CSurface;
	CSurface *bs = new CSurface;
	CVertex bv(0, 0, 0);
	CVertex tv(0, height, 0);

	for (int i = 0; i < CYLINDER_PIECES; i++)
	{
		CSurface *s = new CSurface;

		double x1 = pradius * sin(i * 2 * PI / CYLINDER_PIECES);
		double x2 = pradius * sin((i + 1) * 2 * PI / CYLINDER_PIECES);
		double z1 = pradius * cos(i * 2 * PI / CYLINDER_PIECES);
		double z2 = pradius * cos((i + 1) * 2 * PI / CYLINDER_PIECES);
		CVertex v1(x1, 0, z1);
		CVertex v2(x2, 0, z2);
		CVertex v3(x1, height, z1);
		CVertex v4(x2, height, z2);
		double formu[4];
		CSurface::getSurfaceFormula(v1, v2, bv, v4, formu);
		*s<<new CEdge(v1, v2, formu);
		CSurface::getSurfaceFormula(v2, v4, bv, v3, formu);
		*s<<new CEdge(v2, v4, formu);
		CSurface::getSurfaceFormula(v4, v3, tv, v1, formu);
		*s<<new CEdge(v4, v3, formu);
		CSurface::getSurfaceFormula(v3, v1, tv, v2, formu);
		*s<<new CEdge(v3, v1, formu);
		CSurface::getSurfaceFormula(v1, v2, v3, bv, formu);
		s->setFormula(formu[0], formu[1], formu[2], formu[3]);
		*model<<s;

		CSurface::getSurfaceFormula(v1, v2, v3, bv, formu);
		*bs<<new CEdge(v1, v2, formu);
		CSurface::getSurfaceFormula(v3, v4, v1, bv, formu);
		*ts<<new CEdge(v3, v4, formu);
	}
	bs->setFormula(0, 1, 0, 0);
	ts->setFormula(0, -1, 0, height);
	*model<<ts<<bs;

	model->center = CVertex(0, height / 2, 0);
	CVector raxis = CVector(0, 1, 0) * axis;
	if (raxis.getLength() < ZERO) raxis = CVector(0, 0, 1);
	model->rotate(model->center, raxis, axis ^ CVector(0, 1, 0));
	model->move(center - model->center);

	return model;
}

CrossLink *CCylinder::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	double t =
		(vec.x * (center.x - ver.x)	+ vec.y * (center.y - ver.y) + vec.z * (center.z - ver.z))
		/ (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	double d =
		sqrt((ver.x + vec.x * t - center.x) * (ver.x + vec.x * t - center.x)
		   + (ver.y + vec.y * t - center.y) * (ver.y + vec.y * t - center.y)
		   + (ver.z + vec.z * t - center.z) * (ver.z + vec.z * t - center.z));
	if (d > radius) return NULL;

	CVector hv = vec - (axis & vec) * axis;
	CVertex hc = center + ((ver - center) & axis) * axis;

	CVector p = ver - hc;
	double pa = hv.x * hv.x + hv.y * hv.y + hv.z * hv.z;
	double pb = 2 * (p.x * hv.x + p.y * hv.y + p.z * hv.z);
	double pc = p.x * p.x + p.y * p.y + p.z * p.z - pradius * pradius;
	double tt[2];
	int n = solveFormula(pa, pb, pc, tt);
	
	CrossLink *head = NULL;
	for (int i = 0; i < n; i++)
	{
		CVertex cr = ver + tt[i] * vec;
		bool out = false;
		for (int j = 0; j < 2; j++)
		{
			double k = cr.x * face[j][0] + cr.y * face[j][1] + cr.z * face[j][2] + face[j][3];
			if (fabs(k) > 0 && k < 0)
			{
				out = true;
				break;
			}
		}
		if (out) continue;
		if (fabs(tt[i]) < ZERO || tt[i] < 0 || (b > 0 && (tt[i] > b || fabs(b - tt[i]) < ZERO))) continue;
		CrossLink *l = new CrossLink(tt[i], p + tt[i] * hv, this);
		l->next = head;
		head = l;
	}

	for (i = 0; i < 2; i++)
	{
		double r = vec.x * face[i][0] + vec.y * face[i][1] + vec.z * face[i][2];
		if (fabs(r) < ZERO) continue;
		double t = -(ver.x * face[i][0] + ver.y * face[i][1] + ver.z * face[i][2] + face[i][3]) / r;
		if (fabs(t) < ZERO || t < 0 || (b > 0 && (t > b || fabs(b - t) < ZERO))) continue;
		CVertex cross = ver + t * vec;
		double pr = ((cross - center) - (axis & (cross - center)) * axis).getLength();
		if (fabs(pr - pradius) > ZERO && pr > pradius) continue;
		CrossLink *l;
		if (i == 0) l = new CrossLink(t, axis, this);
		else l = new CrossLink(t, -axis, this);
		l->next = head;
		head = l;
	}

	return head;
}

int CCylinder::getLocation(const CVertex &v)
{
	CVector p = v - center;
	double d = (p - (p & axis) * axis).getLength();
	if (fabs(d - pradius) > ZERO && d > pradius) return -1;
	for (int i = 0; i < 2; i++)
	{
		double k = face[i][0] * v.x + face[i][1] * v.y + face[i][2] * v.z + face[i][3];
		if (fabs(k) < ZERO) return 0;
		if (k < 0) return -1;
	}
	if (fabs(d - pradius) < ZERO) return 0;
	return 1;
}

COLORREF CCylinder::getColor(const CVertex &v, const CMaterial &m, CVector &nv)
{
	if (m.texture == NULL) return m.color;

	double y = (v - bottom) & axis;
	CVector zaxis = xaxis * axis;
	zaxis.standardize();

	CVector xa = v - center - ((v - center) & axis) * axis;
	if (fabs(y) < ZERO || fabs(y - height) < ZERO)
	{
		CVector by = zaxis;
		CVector bx = xaxis;
		if (fabs(y - height) < ZERO) bx = -bx;
		if (m.texture->getBump())
		{
			double bp[2];
			m.texture->getBumpPixel(xa & bx, xa & by, bp);
			nv = bx * by;
			double l = nv.getLength();
			nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
		}
		
		if (m.texture->getTexture()) return m.texture->getPixel(xa & bx, xa & by);
		else return m.color;
	}

	double alpha = xa ^ xaxis;
	if ((xa & zaxis) < 0) alpha = 2 * PI - alpha;

	if (m.texture->getBump())
	{
		double bp[2];
		m.texture->getBumpPixel(alpha * pradius, y, bp);
		CVector by = axis;
		CVector bx = by * nv;
		bx.standardize();
		by.standardize();
		nv = bx * by;
		double l = nv.getLength();
		nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
	}

	if (m.texture->getTexture()) return m.texture->getPixel(alpha * pradius, y);
	else return m.color;
}

CCSGModel *CCylinder::alterView(const CUserView &view)
{
	CCylinder *cylinder = new CCylinder(*this);
	CMatrix rm = CGeometry::getAlterViewMatrix(view);

	CVertex t = center + axis;
	CVertex xt = center + xaxis;
	CGeometry::executeTransform(cylinder->center, rm);
	CGeometry::executeTransform(t, rm);
	CGeometry::executeTransform(xt, rm);
	cylinder->axis = t - cylinder->center;
	cylinder->xaxis = xt - cylinder->center;

	cylinder->updateParameters();

	return cylinder;
}

void CCylinder::updateParameters()
{
	top = center + height / 2 * axis;
	bottom = center - height / 2 * axis;
	face[0][0] = -axis.x;
	face[1][0] = axis.x;
	face[0][1] = -axis.y;
	face[1][1] = axis.y;
	face[0][2] = -axis.z;
	face[1][2] = axis.z;
	face[0][3] = -(top.x * face[0][0] + top.y * face[0][1] + top.z * face[0][2]);
	face[1][3] = -(bottom.x * face[1][0] + bottom.y * face[1][1] + bottom.z * face[1][2]);

	radius = sqrt((height / 2) * (height / 2) + pradius * pradius);
}

int CCylinder::getType()
{
	return TYPE_CYLINDER;
}

void CCylinder::setPRadius(double r)
{
	pradius = r;
	updateParameters();
}

void CCylinder::setCenter(const CVertex &c)
{
	center = c;
	updateParameters();
}

void CCylinder::setHeight(double h)
{
	height = h;
	updateParameters();
}

double CCylinder::getHeight() const
{
	return height;
}

double CCylinder::getPRadius() const
{
	return pradius;
}

CCSGModel *CCylinder::copy()
{
	return new CCylinder(*this);
}

void CCylinder::write(CArchive &ar)
{
	int type = TYPE_CYLINDER;
	ar.Write(&type, sizeof(int));

	center.write(ar);
	axis.write(ar);
	xaxis.write(ar);
	ar.Write(&pradius, sizeof(double));
	ar.Write(&height, sizeof(double));
}

void CCylinder::read(CArchive &ar)
{
	center.read(ar);
	axis.read(ar);
	xaxis.read(ar);
	ar.Read(&pradius, sizeof(double));
	ar.Read(&height, sizeof(double));

	updateParameters();
}