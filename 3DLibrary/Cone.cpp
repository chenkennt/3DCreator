// Cone.cpp: implementation of the CCone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cone.h"
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

CCone::CCone()
{

}

CCone::CCone(const CVertex &c, const CVector &a, double h, double tr, double br)
{
	bcenter = c;
	axis = a;
	axis.standardize();
	tradius = tr;
	bradius = br;
	height = h;

	CVector vv(1, 0, 0);
	if ((vv * a).getLength() < ZERO) vv = CVector(0, 1, 0);
	xaxis = vv - (vv & axis) * axis;
	xaxis.standardize();

	updateParameters();
}

CCone::~CCone()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CCone::move(const CVector &d)
{
	bcenter = bcenter + d;

	updateParameters();
}

void CCone::scale(double s)
{
	bcenter.x = s * bcenter.x;
	bcenter.y = s * bcenter.y;
	bcenter.z = s * bcenter.z;
	bradius *= s;
	tradius *= s;
	height *= s;

	updateParameters();
}

void CCone::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	CMatrix rm = CGeometry::getRotateMatrix(ver, vec, theta);
	CVertex t = bcenter + axis;
	CVertex xt = bcenter + xaxis;
	CGeometry::executeTransform(bcenter, rm);
	CGeometry::executeTransform(t, rm);
	CGeometry::executeTransform(xt, rm);
	axis = t - bcenter;
	xaxis = xt - bcenter;

	updateParameters();
}

CSurfaceModel *CCone::createSurfaceModel()
{
	CSurfaceModel *model = new CSurfaceModel;

	CSurface *ts = new CSurface;
	CSurface *bs = new CSurface;
	CVertex bv(0, 0, 0);
	CVertex tv(0, height, 0);

	for (int i = 0; i < CONE_PIECES; i++)
	{
		CSurface *s = new CSurface;

		double x1 = bradius * sin(i * 2 * PI / CYLINDER_PIECES);
		double x2 = bradius * sin((i + 1) * 2 * PI / CYLINDER_PIECES);
		double x3 = tradius * sin(i * 2 * PI / CYLINDER_PIECES);
		double x4 = tradius * sin((i + 1) * 2 * PI / CYLINDER_PIECES);
		double z1 = bradius * cos(i * 2 * PI / CYLINDER_PIECES);
		double z2 = bradius * cos((i + 1) * 2 * PI / CYLINDER_PIECES);
		double z3 = tradius * cos(i * 2 * PI / CYLINDER_PIECES);
		double z4 = tradius * cos((i + 1) * 2 * PI / CYLINDER_PIECES);
		CVertex v1(x1, 0, z1);
		CVertex v2(x2, 0, z2);
		CVertex v3(x3, height, z3);
		CVertex v4(x4, height, z4);
		double formu[4];
		if (fabs(tradius) > ZERO && fabs(bradius) > ZERO)
		{
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
		else if (fabs(tradius) < ZERO)
		{
			CSurface::getSurfaceFormula(v1, v2, bv, v4, formu);
			*s<<new CEdge(v1, v2, formu);
			CSurface::getSurfaceFormula(v2, v4, bv, v1, formu);
			*s<<new CEdge(v2, v4, formu);
			CSurface::getSurfaceFormula(v4, v1, bv, v2, formu);
			*s<<new CEdge(v4, v1, formu);
			CSurface::getSurfaceFormula(v1, v2, v4, bv, formu);
			s->setFormula(formu[0], formu[1], formu[2], formu[3]);
			*model<<s;

			CSurface::getSurfaceFormula(v1, v2, v4, bv, formu);
			*bs<<new CEdge(v1, v2, formu);
		}
		else
		{
			CSurface::getSurfaceFormula(v1, v4, tv, v3, formu);
			*s<<new CEdge(v1, v4, formu);
			CSurface::getSurfaceFormula(v4, v3, tv, v1, formu);
			*s<<new CEdge(v4, v3, formu);
			CSurface::getSurfaceFormula(v3, v1, tv, v4, formu);
			*s<<new CEdge(v3, v1, formu);
			CSurface::getSurfaceFormula(v1, v4, v3, tv, formu);
			s->setFormula(formu[0], formu[1], formu[2], formu[3]);
			*model<<s;

			CSurface::getSurfaceFormula(v3, v4, v1, tv, formu);
			*ts<<new CEdge(v3, v4, formu);
		}
	}
	if (fabs(bradius) > ZERO) bs->setFormula(0, 1, 0, 0);
	if (fabs(tradius) > ZERO) ts->setFormula(0, -1, 0, height);
	*model<<ts<<bs;

	double x = (tradius * tradius + height * height - bradius * bradius) / (2 * height);
	model->center = CVertex(0, x, 0);
	CVector raxis = CVector(0, 1, 0) * axis;
	if (raxis.getLength() < ZERO) raxis = CVector(0, 0, 1);
	model->rotate(model->center, raxis, axis ^ CVector(0, 1, 0));
	model->move(center - model->center);

	return model;
}

CrossLink *CCone::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	double t =
		(vec.x * (center.x - ver.x)	+ vec.y * (center.y - ver.y) + vec.z * (center.z - ver.z))
		/ (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	double d =
		sqrt((ver.x + vec.x * t - center.x) * (ver.x + vec.x * t - center.x)
		   + (ver.y + vec.y * t - center.y) * (ver.y + vec.y * t - center.y)
		   + (ver.z + vec.z * t - center.z) * (ver.z + vec.z * t - center.z));
	if (d > radius) return NULL;

	CVector zaxis = axis * xaxis;
	zaxis.standardize();
	CrossLink *head = NULL;

	double kx = vec & xaxis;
	double ky = vec & axis;
	double kz = vec & zaxis;
	double x = (ver - bcenter) & xaxis;
	double y = (ver - bcenter) & axis;
	double z = (ver - bcenter) & zaxis;
	double kr = (tradius - bradius) / height;
	double br = bradius;

	double pa = kx * kx + kz * kz - kr * kr * ky * ky;
	double pb = 2 * (x * kx + z * kz - (y * kr + br) * kr * ky);
	double pc = x * x + z * z - (y * kr + br) * (y * kr + br);

	double tt[2];
	int n = solveFormula(pa, pb, pc, tt);
	for (int i = 0; i < n; i++)
	{
		if (fabs(tt[i]) < ZERO || tt[i] < 0 || (b > 0 && (tt[i] > b || fabs(b - tt[i]) < ZERO))) continue;

		double yy = y + ky * tt[i];
		if ((fabs(yy) > ZERO && yy < 0) || (fabs(yy - height) > ZERO && yy > height)) continue;

		CVertex v = ver + tt[i] * vec;
		CVector xa = v - center - ((v - center) & axis) * axis;
		xa.standardize();
		CrossLink *l = new CrossLink(tt[i], height * xa + (bradius - tradius) * axis, this);
		l->next = head;
		head = l;
	}

	if (fabs(ky) > ZERO)
	{
		tt[0] = -y / ky;
		tt[1] = (height - y) / ky;
		double r[2] = {bradius, tradius};
		for (int i = 0; i < 2; i++)
		{
			if (fabs(tt[i]) < ZERO || tt[i] < 0 || (b > 0 && (tt[i] > b || fabs(b - tt[i]) < ZERO))) continue;

			double xx = x + tt[i] * kx;
			double zz = z + tt[i] * kz;
			if (xx * xx + zz * zz > r[i] * r[i] && fabs(xx * xx + zz * zz - r[i] * r[i]) > ZERO) continue;

			CVector v = axis;
			if (i == 0) v = -axis;
			CrossLink *l = new CrossLink(tt[i], v, this);
			l->next = head;
			head = l;
		}
	}

	return head;
}

int CCone::getLocation(const CVertex &v)
{
	double y = (v - bcenter) & axis;
	if ((y < 0 && fabs(y) > ZERO) || (y > height && fabs(y - height) > ZERO)) return -1;

	double r = (v - bcenter - ((v - bcenter) & axis) * axis).getLength();
	double kr = (tradius - bradius) / height;
	double br = bradius;
	if (r > y * kr + br && fabs(y * kr + br - r) > ZERO) return -1;

	if (fabs(y) < ZERO || fabs(y - height) < ZERO || fabs(y * kr + br - r) < ZERO) return 0;

	return 1;
}

COLORREF CCone::getColor(const CVertex &v, const CMaterial &m, CVector &nv)
{
	if (m.texture == NULL) return m.color;

	double y = (v - bcenter) & axis;
	CVector zaxis = xaxis * axis;
	zaxis.standardize();

	CVector xa = v - bcenter - ((v - bcenter) & axis) * axis;
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
	double r = bradius;
	if (tradius > bradius) r = tradius;

	if (m.texture->getBump())
	{
		xa.standardize();
		double bp[2];
		m.texture->getBumpPixel(alpha * r, y, bp);
		CVector by = (tradius - bradius) * xa + height * axis;
		CVector bx = by * nv;
		bx.standardize();
		by.standardize();
		nv = bx * by;
		double l = nv.getLength();
		nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
	}

	if (m.texture->getTexture()) return m.texture->getPixel(alpha * r, y);
	else return m.color;
}

CCSGModel *CCone::alterView(const CUserView &view)
{
	CCone *cone = new CCone(*this);
	CMatrix rm = CGeometry::getAlterViewMatrix(view);

	CVertex t = bcenter + axis;
	CVertex xt = bcenter + xaxis;
	CGeometry::executeTransform(cone->bcenter, rm);
	CGeometry::executeTransform(t, rm);
	CGeometry::executeTransform(xt, rm);
	cone->axis = t - cone->bcenter;
	cone->xaxis = xt - cone->bcenter;

	cone->updateParameters();

	return cone;
}

void CCone::updateParameters()
{
	if (fabs(height) < 0)
	{
		center = bcenter;
		if (tradius > bradius) radius = tradius;
		else radius = bradius;
	}
	else
	{
		double x = (tradius * tradius + height * height - bradius * bradius) / (2 * height);
		center = bcenter + x * axis;
		radius = sqrt(x * x + bradius * bradius);
	}
}

int CCone::getType()
{
	return TYPE_CONE;
}

void CCone::setBCenter(const CVertex &c)
{
	center = c;
	updateParameters();
}

void CCone::setTRadius(double r)
{
	tradius = r;
	updateParameters();
}

void CCone::setBRadius(double r)
{
	bradius = r;
	updateParameters();
}

void CCone::setHeight(double h)
{
	if (h < 0)
	{
		height = -h;
		axis = -axis;
		xaxis = -xaxis;
	}
	else height = h;

	updateParameters();
}

CVector CCone::getAxis()
{
	return axis;
}

double CCone::getTRadius() const
{
	return tradius;
}

double CCone::getBRadius() const
{
	return bradius;
}

CVertex CCone::getBCenter() const
{
	return bcenter;
}

double CCone::getHeight() const
{
	return height;
}

CCSGModel *CCone::copy()
{
	return new CCone(*this);
}

void CCone::write(CArchive &ar)
{
	int type = TYPE_CONE;
	ar.Write(&type, sizeof(int));
	
	axis.write(ar);
	xaxis.write(ar);
	bcenter.write(ar);
	ar.Write(&bradius, sizeof(double));
	ar.Write(&tradius, sizeof(double));
	ar.Write(&height, sizeof(double));
}

void CCone::read(CArchive &ar)
{
	axis.read(ar);
	xaxis.read(ar);
	bcenter.read(ar);
	ar.Read(&bradius, sizeof(double));
	ar.Read(&tradius, sizeof(double));
	ar.Read(&height, sizeof(double));

	updateParameters();
}