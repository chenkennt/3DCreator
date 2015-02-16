// LineLoftObject.cpp: implementation of the CLineLoftObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineLoftObject.h"
#include "Shape.h"
#include "Constants.h"
#include "Matrix.h"
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

CLineLoftObject::CLineLoftObject()
{
	shape = NULL;
	srs = NULL;
	stree = NULL;
}

CLineLoftObject::CLineLoftObject(const CLineLoftObject &loft)
{
	shape = new CShape(*loft.shape);
	srs = NULL;
	stree = NULL;
	zaxis = loft.zaxis;
	yaxis = loft.yaxis;
	height = loft.height;
	pcenter = loft.pcenter;
	center = loft.center;
	radius = loft.radius;
}

CLineLoftObject::~CLineLoftObject()
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

void CLineLoftObject::move(const CVector &d)
{
	pcenter = pcenter + d;

	updateParameters();
}

void CLineLoftObject::scale(double s)
{
	pcenter = CVertex(0, 0, 0) + s * (pcenter - CVertex(0, 0, 0));
	height *= s;
	shape->scale(s);

	updateParameters();
}

void CLineLoftObject::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	CMatrix rm = CGeometry::getRotateMatrix(ver, vec, theta);
	CVertex v1 = pcenter + zaxis;
	CVertex v2 = pcenter + yaxis;
	CGeometry::executeTransform(pcenter, rm);
	CGeometry::executeTransform(v1, rm);
	CGeometry::executeTransform(v2, rm);
	zaxis = v1 - pcenter;
	yaxis = v2 - pcenter;

	updateParameters();
}

CSurfaceModel *CLineLoftObject::createSurfaceModel()
{
	CSurface *s = shape->createSurface();
	if (s == NULL) return NULL;

	CSurfaceModel *model = new CSurfaceModel;

	CEdge *e = s->firstEdge;
	while (e != NULL)
	{
		CSurface *ns = new CSurface;
		CVertex v1(e->sv.x, e->sv.y, 0);
		CVertex v2(e->ev.x, e->ev.y, 0);
		CVertex v3(e->sv.x, e->sv.y, height);
		CVertex v4(e->ev.x, e->ev.y, height);
		CVertex iv(e->sv.x + e->formula[0], e->sv.y + e->formula[1], height / 2);
		double formu[4];
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
		*model<<ns;
		e = e->next;
	}

	CSurface *bs = new CSurface(*s);
	e = bs->firstEdge;
	while (e != NULL)
	{
		e->sv.z = height;
		e->ev.z = height;
		e = e->next;
	}
	bs->formula[2] = -1;
	bs->formula[3] = height;
	*model<<s<<bs;

	model->center = CVertex(0, 0, height / 2);
	CVector raxis = CVector(0, 0, 1) * zaxis;
	if (raxis.getLength() < ZERO) raxis = CVector(0, 1, 0);

	model->rotate(CVertex(0, 0, 0), raxis, zaxis ^ CVector(0, 0, 1));
	CMatrix rm = CGeometry::getRotateMatrix(CVertex(0, 0, 0), raxis, zaxis ^ CVector(0, 0, 1));
	CVertex v(0, 1, 0);
	CGeometry::executeTransform(v, rm);
	CVector ya = v - CVertex(0, 0, 0);

	CVector rraxis = ya * yaxis;
	if (rraxis.getLength() < ZERO) rraxis = zaxis;
	model->rotate(CVertex(0, 0, 0), rraxis, yaxis ^ ya);

	model->move(center - model->center);

	return model;
}

CLineLoftObject *CLineLoftObject::create(const CShape &s, const CVertex &c, const CVector &za, const CVector &ya, double h)
{
	CShape *ps = new CShape(s);

	if (!ps->prepareLineLoft())
	{
		delete ps;
		return NULL;
	}

	CLineLoftObject *result = new CLineLoftObject;
	result->shape = ps;
	result->pcenter = c;
	result->zaxis = za;
	result->zaxis.standardize();
	result->yaxis = ya - (result->zaxis & ya) * result->zaxis;
	result->yaxis.standardize();
	result->height = h;
	result->updateParameters();

	return result;
}

void CLineLoftObject::updateParameters()
{
	center = pcenter + 0.5 * height * zaxis;
	radius = sqrt(height * height / 4 + shape->getRadius() * shape->getRadius());
}

CrossLink *CLineLoftObject::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	double t =
		(vec.x * (center.x - ver.x)	+ vec.y * (center.y - ver.y) + vec.z * (center.z - ver.z))
		/ (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	double d =
		sqrt((ver.x + vec.x * t - center.x) * (ver.x + vec.x * t - center.x)
		   + (ver.y + vec.y * t - center.y) * (ver.y + vec.y * t - center.y)
		   + (ver.z + vec.z * t - center.z) * (ver.z + vec.z * t - center.z));
	if (d > radius) return NULL;

	CVector xaxis = zaxis * yaxis;
	xaxis.standardize();

	double ky = vec & yaxis;
	double kx = vec & xaxis;
	double kz = vec & zaxis;
	double x = (ver - pcenter) & xaxis;
	double y = (ver - pcenter) & yaxis;
	double z = (ver - pcenter) & zaxis;

	ShapeRenderStruct *rs = srs;
	CrossLink *result = NULL;
	while (rs != NULL)
	{
		if (rs->h)
		{
			if (fabs(ky) > ZERO)
			{
				double tt = (rs->b - y) / ky;
				if (fabs(tt) < ZERO || tt < 0 || (b > 0 && (tt > b || fabs(b - tt) < ZERO)))
				{
					rs = rs->next;
					continue;
				}
				double tp = (x + kx * tt) * rs->i + rs->s;
				double tz = z + kz * tt;
				if (tp > rs->start && tp < rs->end && (tz > 0 || fabs(tz) < ZERO) && (tz < height || fabs(tz - height) < ZERO))
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
					CrossLink *ns = new CrossLink(tt, px * xaxis + py * yaxis, this);
					ns->next = result;
					result = ns;
				}
			}
		}
		else
		{
			double pa = kx - rs->k * ky;
			double pb = x - y * rs->k - rs->b;
			if (fabs(pa) > ZERO)
			{
				double tt = -pb / pa;
				if (fabs(tt) < ZERO || tt < 0 || (b > 0 && (tt > b || fabs(b - tt) < ZERO)))
				{
					rs = rs->next;
					continue;
				}
				double tp = (y + ky * tt) * rs->i + rs->s;
				double tz = z + kz * tt;
				if (tp > rs->start && tp < rs->end && (tz > 0 || fabs(tz) < ZERO) && (tz < height || fabs(tz - height) < ZERO))
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
					CrossLink *ns = new CrossLink(tt, px * xaxis + py * yaxis, this);
					ns->next = result;
					result = ns;
				}
			}
		}

		rs = rs->next;
	}

	if (fabs(kz) > ZERO)
	{
		double tt[2];
		tt[0] = -z / kz;
		tt[1] = (height - z) / kz;
		for (int i = 0; i < 2; i++)
		{
			if (fabs(tt[i]) < ZERO || tt[i] < 0 || (b > 0 && (tt[i] > b || fabs(b - tt[i]) < ZERO))) continue;
			double tx = x + kx * tt[i];
			double ty = y + ky * tt[i];
			if (CShape::recursiveGetLocation(CVertex(tx, ty, 0), stree) >= 0)
			{
				CVector vv = zaxis;
				if (i == 0) vv = -vv;
				CrossLink *ns = new CrossLink(tt[i], vv, this);
				ns->next = result;
				result = ns;
			}
		}
	}

	return result;
}

int CLineLoftObject::getLocation(const CVertex &v)
{
	CVector xaxis = zaxis * yaxis;
	xaxis.standardize();

	double x = (v - pcenter) & xaxis;
	double y = (v - pcenter) & yaxis;
	double z = (v - pcenter) & zaxis;

	if ((z < 0 && fabs(z) > ZERO) || (z > height && fabs(z - height) > ZERO)) return -1;
	return CShape::recursiveGetLocation(CVertex(x, y, 0), stree);
}

COLORREF CLineLoftObject::getColor(const CVertex &v, const CMaterial &m, CVector &nv)
{
	if (m.texture == NULL) return m.color;

	CVector xaxis = zaxis * yaxis;
	xaxis.standardize();

	CVector vec = v - pcenter;
	double x = vec & xaxis;
	double y = vec & yaxis;
	double z = vec & zaxis;

	if (fabs(z) < ZERO || fabs(z - height) < ZERO)
	{
		CVector by = yaxis;
		CVector bx = xaxis;
		if (fabs(z) < ZERO) bx = -bx;
		if (m.texture->getBump())
		{
			double bp[2];
			m.texture->getBumpPixel(vec & bx, vec & by, bp);
			nv = bx * by;
			double l = nv.getLength();
			nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
		}
		
		if (m.texture->getTexture()) return m.texture->getPixel(vec & bx, vec & by);
		else return m.color;
	}
	else
	{
		double l = CShape::getPosition(CVertex(x, y, 0), srs);

		if (m.texture->getBump())
		{
			double bp[2];
			m.texture->getBumpPixel(l, z, bp);
			CVector by = zaxis;
			CVector bx = by * nv;
			bx.standardize();
			by.standardize();
			nv = bx * by;
			double l = nv.getLength();
			nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
		}

		if (m.texture->getTexture()) return m.texture->getPixel(l, z);
		else return m.color;
	}
}

CCSGModel *CLineLoftObject::alterView(const CUserView &view)
{
	CLineLoftObject *altered = new CLineLoftObject(*this);

	CMatrix rm = CGeometry::getAlterViewMatrix(view);
	CVertex v1 = altered->pcenter + altered->zaxis;
	CVertex v2 = altered->pcenter + altered->yaxis;
	CGeometry::executeTransform(altered->pcenter, rm);
	CGeometry::executeTransform(v1, rm);
	CGeometry::executeTransform(v2, rm);
	altered->zaxis = v1 - altered->pcenter;
	altered->yaxis = v2 - altered->pcenter;
	altered->updateParameters();
	altered->srs = altered->shape->prepareRender();
	CSurface *s = altered->shape->createSurface();
	altered->stree = s->createSurfaceTree();
	delete s;

	return altered;
}

int CLineLoftObject::getType()
{
	return TYPE_LINELOFT;
}

CCSGModel *CLineLoftObject::copy()
{
	return new CLineLoftObject(*this);
}

void CLineLoftObject::write(CArchive &ar)
{
	int type = TYPE_LINELOFT;
	ar.Write(&type, sizeof(int));

	pcenter.write(ar);
	zaxis.write(ar);
	yaxis.write(ar);
	shape->write(ar);
	ar.Write(&height, sizeof(double));
}

void CLineLoftObject::read(CArchive &ar)
{
	pcenter.read(ar);
	zaxis.read(ar);
	yaxis.read(ar);
	shape = new CShape;
	shape->read(ar);
	ar.Read(&height, sizeof(double));

	updateParameters();
}