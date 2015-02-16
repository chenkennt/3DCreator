// SurfaceModel.cpp: implementation of the CSurfaceModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SurfaceModel.h"
#include "Matrix.h"
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

CSurfaceModel::CSurfaceModel()
{
	firstSurface = NULL;
}

CSurfaceModel::CSurfaceModel(const CSurfaceModel &model)
{
	firstSurface = NULL;

	SurfaceLink *p = model.firstSurface;
	while (p != NULL)
	{
		*this<<new CSurface(*p->surface);
		p = p->next;
	}
	center = model.center;
}

CSurfaceModel::~CSurfaceModel()
{
	while (firstSurface != NULL)
	{
		SurfaceLink *temp = firstSurface;
		firstSurface = firstSurface->next;
		delete temp->surface;
		delete temp;
	}
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CSurfaceModel::move(const CVector &d)
{
	SurfaceLink *p = firstSurface;
	while (p != NULL)
	{
		CEdge *e = p->surface->firstEdge;
		while (e != NULL)
		{
			e->sv = e->sv + d;
			e->ev = e->ev + d;
			e->formula[3] -= e->formula[0] * d.x + e->formula[1] * d.y + e->formula[2] * d.z;
			e = e->next;
		}
		p->surface->formula[3] -= p->surface->formula[0] * d.x + p->surface->formula[1] * d.y + p->surface->formula[2] * d.z;
		p = p->next;
	}
	center = center + d;
}

void CSurfaceModel::scale(double s)
{
	SurfaceLink *p = firstSurface;
	while (p != NULL)
	{
		CEdge *e = p->surface->firstEdge;
		while (e != NULL)
		{
			e->sv = CVertex(0, 0, 0) + s * (e->sv - CVertex(0, 0, 0));
			e->ev = CVertex(0, 0, 0) + s * (e->ev - CVertex(0, 0, 0));
			e->formula[3] *= s;
			e = e->next;
		}
		p->surface->formula[3] *= s;
		p = p->next;
	}
	center = CVertex(0, 0, 0) + s * (center - CVertex(0, 0, 0));
}

void CSurfaceModel::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	CMatrix rm = CGeometry::getRotateMatrix(ver, vec, theta);
	SurfaceLink *p = firstSurface;
	while (p != NULL)
	{
		CEdge *e = p->surface->firstEdge;
		while (e != NULL)
		{
			CGeometry::executeTransform(e->sv, rm);
			CGeometry::executeTransform(e->ev, rm);
			CSurface::rotateSurface(e->formula, rm);
			e = e->next;
		}
		CSurface::rotateSurface(p->surface->formula, rm);
		p = p->next;
	}
	CGeometry::executeTransform(center, rm);
}

CSurfaceModel &CSurfaceModel::operator<<(CSurface *surface)
{
	SurfaceLink *ns = new SurfaceLink;
	ns->surface = surface;
	ns->next = firstSurface;
	firstSurface = ns;

	return *this;
}

void CSurfaceModel::draw(CDC *pDC, const CRect &viewRect, const CUserView &view, const CMatrix &rm, COLORREF color, bool eliminate)
{
	CPen pen(PS_SOLID, 1, color);
	CPen *oldpen = pDC->SelectObject(&pen);

	SurfaceLink *s = firstSurface;
	while (s != NULL)
	{
		bool r = true;

		if (eliminate)
		{
			CVector nv(s->surface->formula[0], s->surface->formula[1], s->surface->formula[2]);
			double t = -s->surface->formula[3] / (nv.x * nv.x + nv.y * nv.y + nv.z * nv.z);
			CVertex v(t * nv.x, t * nv.y, t * nv.z);
			CVertex vt = v + nv;
			CGeometry::executeTransform(v, rm);
			CGeometry::executeTransform(vt, rm);
			nv = vt - v;
			if (view.getType() == PROJECTION_PERSPECTIVE)
			{
				double z = -(v.x * nv.x + v.y * nv.y + v.z * nv.z);
				if (z < 0 || fabs(z) < ZERO) r = true;
				else r = false;
			}
			else
			{
				double z = CVector(0, 0, -1) & nv;
				if (z < 0 || fabs(z) < ZERO) r = true;
				else r = false;
			}
		}

		if (r)
		{
			CEdge *e = s->surface->firstEdge;
			while (e != NULL)
			{
				CGeometry::drawEdge(pDC, e->sv, e->ev, view, viewRect, rm);
				e = e->next;
			}
		}
		s = s->next;
	}

	pDC->SelectObject(oldpen);
	pen.DeleteObject();
}

CSurfaceModel *CSurfaceModel::cut(const CSurfaceModel &csm)
{
	CSurfaceModel *out = new CSurfaceModel;
	SurfaceLink *s = firstSurface;
	firstSurface = NULL;

	while (s != NULL)
	{
		CSurface *cs = csm.cut(*s->surface);
		CSurface *outs = s->surface->cut(*cs);
		delete cs;
		if (outs->firstEdge == NULL) delete outs;
		else *out<<outs;
		if (s->surface->firstEdge == NULL) delete s->surface;
		else *this<<s->surface;
		SurfaceLink *temp = s;
		s = s->next;
		delete temp;
	}
	return out;
}

CSurface *CSurfaceModel::cut(const CSurface &s) const
{
	CSurface *result = new CSurface;
	SurfaceLink *sl = firstSurface;
	while (sl != NULL)
	{
		CSurface *cross = sl->surface->cross(s);
		result->combine(*cross);
		delete cross;
		sl = sl->next;
	}
	result->setFormula(s.formula[0], s.formula[1], s.formula[2], s.formula[3]);
	return result;
}

CSurfaceModel *CSurfaceModel::unionModel(CSurfaceModel &model)
{
	CSurfaceModel *copy1 = new CSurfaceModel(*this);
	CSurfaceModel *copy2 = new CSurfaceModel(model);
	CSurfaceModel *out1 = copy1->cut(model);
	CSurfaceModel *out2 = copy2->cut(*this);

	delete copy1;
	delete copy2;
	out1->combine(*out2);
	delete out2;

	out1->center.x = (center.x + model.center.x) / 2;
	out1->center.y = (center.x + model.center.y) / 2;
	out1->center.z = (center.x + model.center.z) / 2;

	return out1;
}

CSurfaceModel *CSurfaceModel::intersectModel(CSurfaceModel &model)
{
	CSurfaceModel *copy1 = new CSurfaceModel(*this);
	CSurfaceModel *copy2 = new CSurfaceModel(model);
	CSurfaceModel *out1 = copy1->cut(model);
	CSurfaceModel *out2 = copy2->cut(*this);

	delete out1;
	delete out2;
	copy1->combine(*copy2);
	delete copy2;

	copy1->center.x = (center.x + model.center.x) / 2;
	copy1->center.y = (center.x + model.center.y) / 2;
	copy1->center.z = (center.x + model.center.z) / 2;

	return copy1;
}

CSurfaceModel *CSurfaceModel::differenceModel(CSurfaceModel &model)
{
	CSurfaceModel *copy1 = new CSurfaceModel(*this);
	CSurfaceModel *copy2 = new CSurfaceModel(model);
	CSurfaceModel *out1 = copy1->cut(model);
	CSurfaceModel *out2 = copy2->cut(*this);

	SurfaceLink *t = copy2->firstSurface;
	while (t != NULL)
	{
		t->surface->formula[0] = -t->surface->formula[0];
		t->surface->formula[1] = -t->surface->formula[1];
		t->surface->formula[2] = -t->surface->formula[2];
		t->surface->formula[3] = -t->surface->formula[3];
		t = t->next;
	}

	delete copy1;
	delete out2;
	out1->combine(*copy2);
	delete copy2;

	out1->center.x = (center.x + model.center.x) / 2;
	out1->center.y = (center.x + model.center.y) / 2;
	out1->center.z = (center.x + model.center.z) / 2;

	return out1;
}

void CSurfaceModel::combine(CSurfaceModel &model)
{
	if (firstSurface == NULL) firstSurface = model.firstSurface;
	else
	{
		SurfaceLink *t = firstSurface;
		while (t->next != NULL) t = t->next;
		t->next = model.firstSurface;
	}
	model.firstSurface = NULL;
}

bool CSurfaceModel::calcSelected(const CPoint &pt, const CRect &viewRect, const CUserView &view, bool eliminate)
{
	CMatrix rm = CGeometry::getAlterViewMatrix(view);

	SurfaceLink *s = firstSurface;
	while (s != NULL)
	{
		bool r = true;

		if (eliminate)
		{
			CVector nv(s->surface->formula[0], s->surface->formula[1], s->surface->formula[2]);
			double t = -s->surface->formula[3] / (nv.x * nv.x + nv.y * nv.y + nv.z * nv.z);
			CVertex v(t * nv.x, t * nv.y, t * nv.z);
			CVertex vt = v + nv;
			CGeometry::executeTransform(v, rm);
			CGeometry::executeTransform(vt, rm);
			nv = vt - v;
			if (view.getType() == PROJECTION_PERSPECTIVE)
			{
				double z = -(v.x * nv.x + v.y * nv.y + v.z * nv.z);
				if (z < 0 || fabs(z) < ZERO) r = true;
				else r = false;
			}
			else
			{
				double z = CVector(0, 0, -1) & nv;
				if (z < 0 || fabs(z) < ZERO) r = true;
				else r = false;
			}
		}

		if (!r)
		{
			s = s->next;
			continue;
		}

		CEdge *e = s->surface->firstEdge;
		while (e != NULL)
		{
			double faces[5][4];
			view.getFaceParameters(faces);
			CVertex vv1 = e->sv;
			CVertex vv2 = e->ev;
			CGeometry::executeTransform(vv1, rm);
			CGeometry::executeTransform(vv2, rm);

			bool vis = true;
			for (int i = 0; i < 5; i++)
			{
				double p = vv1.x * faces[i][0] + vv1.y * faces[i][1] + vv1.z * faces[i][2] + faces[i][3];
				double q = vv2.x * faces[i][0] + vv2.y * faces[i][1] + vv2.z * faces[i][2] + faces[i][3];
				if (p < 0 && q < 0)
				{
					vis = false;
					break;
				}
				if (p >= 0 && q >= 0) continue;
				CVector k = vv2 - vv1;
				double t = p / (p - q);
				if (p < 0) vv1 = vv1 + t * k;
				else vv2 = vv1 + t * k;
			}

			if (vis)
			{
				double dx = viewRect.Width() / view.getWidth();
				double dy = viewRect.Height() / view.getHeight();
				if (view.getType() == PROJECTION_PERSPECTIVE && fabs(vv1.z) > ZERO)
				{
					vv1.x = vv1.x * view.getDistance() / vv1.z;
					vv1.y = vv1.y * view.getDistance() / vv1.z;
					vv2.x = vv2.x * view.getDistance() / vv2.z;
					vv2.y = vv2.y * view.getDistance() / vv2.z;
				}
				double x = pt.x - viewRect.CenterPoint().x;
				double y = viewRect.CenterPoint().y - pt.y;
				vv1.x *= dx;
				vv1.y *= dy;
				vv2.x *= dx;
				vv2.y *= dy;

				double a = vv2.x - vv1.x;
				double b = vv2.y - vv1.y;
				double d = 100;
				if (fabs(a) < ZERO && fabs(b) < ZERO) d = (x - vv1.x) * (x - vv1.x) + (y - vv1.y) * (y - vv1.y);
				else
				{
					double c = -(a * x + b * y);
					double t = -(a * vv1.x + b * vv1.y + c) / (a * a + b * b);
					if (t >= 0 && t <= 1)
					{
						double cx = vv1.x + t * a;
						double cy = vv1.y + t * b;
						d = (cx - x) * (cx - x) + (cy - y) * (cy - y);
					}
				}
				if (d < DISTANCE_FORSELECT) return true;
			}

			e = e->next;
		}
		s = s->next;
	}

	return false;
}