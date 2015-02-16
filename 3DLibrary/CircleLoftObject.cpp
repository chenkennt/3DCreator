// LoftObject.cpp: implementation of the CCircleLoftObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CircleLoftObject.h"
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

CCircleLoftObject::CCircleLoftObject()
{
	shape = NULL;
	srs = NULL;
	stree = NULL;
}

CCircleLoftObject::CCircleLoftObject(const CCircleLoftObject &loft)
{
	shape = new CShape(*loft.shape);
	srs = NULL;
	stree = NULL;
	axis = loft.axis;
	xaxis = loft.xaxis;
	center = loft.center;
	radius = loft.radius;
}

CCircleLoftObject::~CCircleLoftObject()
{
	if (shape != NULL) delete shape;
	while (srs != NULL)
	{
		ShapeRenderStruct *temp = srs;
		srs = srs->next;
		delete temp;
	}
	CSurface::recursiveDeleteTree(stree);
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CCircleLoftObject::move(const CVector &d)
{
	center = center + d;
}

void CCircleLoftObject::scale(double s)
{
	center = CVertex(0, 0, 0) + s * (center - CVertex(0, 0, 0));
	shape->scale(s);

	updateParameters();
}

void CCircleLoftObject::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	CMatrix rm = CGeometry::getRotateMatrix(ver, vec, theta);
	CVertex v = center + axis;
	CVertex vv = center + xaxis;
	CGeometry::executeTransform(center, rm);
	CGeometry::executeTransform(v, rm);
	CGeometry::executeTransform(vv, rm);
	axis = v - center;
	xaxis = vv - center;
}

CSurfaceModel *CCircleLoftObject::createSurfaceModel()
{
	CSurface *s = shape->createSurface();
	if (s == NULL) return NULL;

	CSurfaceModel *model = new CSurfaceModel;

	CEdge *e = s->firstEdge;
	while (e != NULL)
	{
		for (int i = 0; i < CIRCLELOFT_PIECES; i++)
		{
			double xp1 = sin(2 * i * PI / CIRCLELOFT_PIECES);
			double zp1 = cos(2 * i * PI / CIRCLELOFT_PIECES);
			double xp2 = sin(2 * (i + 1) * PI / CIRCLELOFT_PIECES);
			double zp2 = cos(2 * (i + 1) * PI / CIRCLELOFT_PIECES);
			double x1 = xp1 * e->sv.x;
			double z1 = zp1 * e->sv.x;
			double x2 = xp1 * e->ev.x;
			double z2 = zp1 * e->ev.x;
			double x3 = xp2 * e->sv.x;
			double z3 = zp2 * e->sv.x;
			double x4 = xp2 * e->ev.x;
			double z4 = zp2 * e->ev.x;
			double y1 = e->sv.y;
			double y2 = e->ev.y;
			double ix = xp1 * (e->sv.x + e->formula[0]);
			double iy = e->sv.y + e->formula[1];
			double iz = zp1 * (e->sv.x + e->formula[0]);
			CSurface *ns = new CSurface;
			CVertex v1(x1, y1, z1);
			CVertex v2(x2, y2, z2);
			CVertex v3(x3, y1, z3);
			CVertex v4(x4, y2, z4);
			CVertex iv(ix, iy, iz);
			double formu[4];
			if (fabs(e->sv.x) > ZERO && fabs(e->ev.x) > ZERO)
			{
				CSurface::getSurfaceFormula(v1, v2, iv, v4, formu);
				*ns<<new CEdge(v1, v2, formu);
				CSurface::getSurfaceFormula(v2, v4, iv, v3, formu);
				*ns<<new CEdge(v2, v4, formu);
				CSurface::getSurfaceFormula(v4, v3, iv, v1, formu);
				*ns<<new CEdge(v4, v3, formu);
				CSurface::getSurfaceFormula(v3, v1, iv, v2, formu);
				*ns<<new CEdge(v3, v1, formu);
			
				CSurface::getSurfaceFormula(v1, v2, v3, iv, formu);
				ns->setFormula(formu[0], formu[1], formu[2], formu[3]);
			}
			else if (fabs(e->sv.x) < ZERO)
			{
				CSurface::getSurfaceFormula(v1, v2, iv, v4, formu);
				*ns<<new CEdge(v1, v2, formu);
				CSurface::getSurfaceFormula(v2, v4, iv, v1, formu);
				*ns<<new CEdge(v2, v4, formu);
				CSurface::getSurfaceFormula(v4, v1, iv, v2, formu);
				*ns<<new CEdge(v4, v1, formu);
			
				CSurface::getSurfaceFormula(v1, v2, v4, iv, formu);
				ns->setFormula(formu[0], formu[1], formu[2], formu[3]);
			}
			else
			{
				CSurface::getSurfaceFormula(v1, v2, iv, v3, formu);
				*ns<<new CEdge(v1, v2, formu);
				CSurface::getSurfaceFormula(v2, v3, iv, v1, formu);
				*ns<<new CEdge(v2, v3, formu);
				CSurface::getSurfaceFormula(v3, v1, iv, v2, formu);
				*ns<<new CEdge(v3, v1, formu);
			
				CSurface::getSurfaceFormula(v1, v2, v3, iv, formu);
				ns->setFormula(formu[0], formu[1], formu[2], formu[3]);
			}
			*model<<ns;
		}
		e = e->next;
	}

	model->center = CVertex(0, 0, 0);
	CVector raxis = CVector(0, 1, 0) * axis;
	if (raxis.getLength() < ZERO)
	{
		if (axis.y < 0) model->scale(-1);
	}
	else model->rotate(CVertex(0, 0, 0), raxis, axis ^ CVector(0, 1, 0));
	model->move(center - CVertex(0, 0, 0));

	delete s;
	return model;
}

void CCircleLoftObject::updateParameters()
{
	radius = shape->getRadius();
}

CCircleLoftObject *CCircleLoftObject::create(const CShape &s, const CVertex &c, const CVector &v)
{
	CShape *ps = new CShape(s);

	if (!ps->prepareCircleLoft())
	{
		delete ps;
		return NULL;
	}

	CCircleLoftObject *result = new CCircleLoftObject;
	result->shape = ps;
	result->center = c;
	result->axis = v;
	result->axis.standardize();
	CVector vv(1, 0, 0);
	if (fabs((v * vv).getLength()) < ZERO) vv = CVector(0, 1, 0);
	result->xaxis = vv - (vv & result->axis) * result->axis;
	result->xaxis.standardize();
	result->updateParameters();

	return result;
}

CrossLink *CCircleLoftObject::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	double t =
		(vec.x * (center.x - ver.x)	+ vec.y * (center.y - ver.y) + vec.z * (center.z - ver.z))
		/ (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	double d =
		sqrt((ver.x + vec.x * t - center.x) * (ver.x + vec.x * t - center.x)
		   + (ver.y + vec.y * t - center.y) * (ver.y + vec.y * t - center.y)
		   + (ver.z + vec.z * t - center.z) * (ver.z + vec.z * t - center.z));
	if (d > radius) return NULL;

	CVector kx = vec - (vec & axis) * axis;
	double ky = vec & axis;
	CVertex a = center + ((ver - center) & axis) * axis;
	CVector p = ver - a;
	double y = (ver - center) & axis;

	CrossLink *result = NULL;
	ShapeRenderStruct *rs = srs;
	while (rs != NULL)
	{
		if (rs->h)
		{
			if (fabs(ky) < ZERO)
			{
				rs = rs->next;
				continue;
			}
			else
			{
				double tt = (rs->b - y) / ky;
				if (fabs(tt) < ZERO || tt < 0 || (b > 0 && (tt > b || fabs(b - tt) < ZERO)))
				{
					rs = rs->next;
					continue;
				}
				CVector xa = p + tt * kx;
				double x = xa.getLength();
				double tp = rs->s + x * rs->i;
				if (tp > rs->start && tp < rs->end)
				{
					double py = -3 * (1 - tp) * (1 - tp) * rs->v1.x +
								(3 - 12 * tp + 9 * tp * tp) * rs->v2.x +
								(6 * tp - 9 * tp * tp) * rs->v3.x +
								3 * tp * tp * rs->v4.x;
					double px = 3 * (1 - tp) * (1 - tp) * rs->v1.y -
								(3 - 12 * tp + 9 * tp * tp) * rs->v2.y -
								(6 * tp - 9 * tp * tp) * rs->v3.y -
								3 * tp * tp * rs->v4.y;
					if (!rs->r)
					{
						px = -px;
						py = -py;
					}
					xa.standardize();
					CrossLink *ns = new CrossLink(tt, px * xa + py * axis, this);
					ns->next = result;
					result = ns;
				}
			}
		}
		else
		{
			double pa = kx.x * kx.x + kx.y * kx.y + kx.z * kx.z - ky * ky * rs->k * rs->k;
			double pb = 2 * (p.x * kx.x + p.y * kx.y + p.z * kx.z - ky * rs->k * (y * rs->k + rs->b));
			double pc = p.x * p.x + p.y * p.y + p.z * p.z - (y * rs->k + rs->b) * (y * rs->k + rs->b);

			double tt[2];
			int n = solveFormula(pa, pb, pc, tt);
	
			for (int i = 0; i < n; i++)
			{
				if (fabs(tt[i]) < ZERO || tt[i] < 0 || (b > 0 && (tt[i] > b || fabs(b - tt[i]) < ZERO))) continue;
				double tp = rs->s + (ky * tt[i] + y) * rs->i;
				if (tp > rs->start && tp < rs->end)
				{
					double py = -3 * (1 - tp) * (1 - tp) * rs->v1.x +
								(3 - 12 * tp + 9 * tp * tp) * rs->v2.x +
								(6 * tp - 9 * tp * tp) * rs->v3.x +
								3 * tp * tp * rs->v4.x;
					double px = 3 * (1 - tp) * (1 - tp) * rs->v1.y -
								(3 - 12 * tp + 9 * tp * tp) * rs->v2.y -
								(6 * tp - 9 * tp * tp) * rs->v3.y -
								3 * tp * tp * rs->v4.y;
					if (!rs->r)
					{
						px = -px;
						py = -py;
					}
					CVector xa = p + tt[i] * kx;
					xa.standardize();
					CrossLink *ns = new CrossLink(tt[i], px * xa + py * axis, this);
					ns->next = result;
					result = ns;
				}
			}
		}

		rs = rs->next;
	}
	return result;
}

int CCircleLoftObject::getLocation(const CVertex &v)
{
	double y = (v - center) & axis;
	double x = (v - center - y * axis).getLength();

	return CShape::recursiveGetLocation(CVertex(x, y, 0), stree);
}

COLORREF CCircleLoftObject::getColor(const CVertex &v, const CMaterial &m, CVector &nv)
{
	if (m.texture == NULL) return m.color;

	double y = (v - center) & axis;
	CVector xa = v - center - y * axis;
	double py = -CShape::getPosition(CVertex(xa.getLength(), y, 0), srs);
	CVector za = xaxis * axis;
	double alpha = xa ^ xaxis;
	if ((xa & za) < 0) alpha = 2 * PI - alpha;

	if (m.texture->getBump())
	{
		double bp[2];
		m.texture->getBumpPixel(pradius * alpha, py, bp);
		xa.standardize();
		double xz = xa & nv;
		double y = -(axis & nv);
		if (xz < 0)
		{
			xz = -xz;
			y = -y;
		}
		CVector by = xz * axis + y * xa;
		CVector bx = by * nv;
		bx.standardize();
		by.standardize();
		nv = bx * by;
		double l = nv.getLength();
		nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
	}

	if (m.texture->getTexture()) return m.texture->getPixel(pradius * alpha, py);
	else return m.color;
}

CCSGModel *CCircleLoftObject::alterView(const CUserView &view)
{
	CCircleLoftObject *altered = new CCircleLoftObject(*this);

	CMatrix rm = CGeometry::getAlterViewMatrix(view);
	CVertex v = altered->center + altered->axis;
	CVertex vv = altered->center + altered->xaxis;
	CGeometry::executeTransform(altered->center, rm);
	CGeometry::executeTransform(v, rm);
	CGeometry::executeTransform(vv, rm);
	altered->axis = v - altered->center;
	altered->xaxis = vv - altered->center;
	altered->srs = altered->shape->prepareRender();
	CSurface *s = altered->shape->createSurface();
	altered->stree = s->createSurfaceTree();

	CSurface *surface = shape->createSurface();
	altered->pradius = 0;
	CEdge *e = surface->firstEdge;
	while (e != NULL)
	{
		if (e->sv.x > pradius) altered->pradius = e->sv.x;
		if (e->ev.x > pradius) altered->pradius = e->ev.x;
		e = e->next;
	}
	delete surface;

	delete s;

	return altered;
}

int CCircleLoftObject::getType()
{
	return TYPE_CIRCLELOFT;
}

CCSGModel *CCircleLoftObject::copy()
{
	return new CCircleLoftObject(*this);
}

void CCircleLoftObject::write(CArchive &ar)
{
	int type = TYPE_CIRCLELOFT;
	ar.Write(&type, sizeof(int));

	center.write(ar);
	axis.write(ar);
	xaxis.write(ar);
	shape->write(ar);
}

void CCircleLoftObject::read(CArchive &ar)
{
	center.read(ar);
	axis.read(ar);
	xaxis.read(ar);
	shape = new CShape;
	shape->read(ar);

	updateParameters();
}