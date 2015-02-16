// Shape.cpp: implementation of the CShape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Shape.h"
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

CShape::CShape()
{
	firstElement = lastElement = NULL;
	closed = false;
	center = CVertex(0, 0, 0);
}

CShape::CShape(const CShape &shape)
{
	firstElement = lastElement = NULL;
	closed = shape.closed;
	center = shape.center;

	ShapeElement *p = shape.firstElement;
	ShapeElement *f = NULL;

	while (p != NULL)
	{
		ShapeElement *t = new ShapeElement;
		t->vertex = p->vertex;
		if (p->prev != NULL)
		{
			t->prev = new ShapeLink;
			t->prev->vector = p->prev->vector;
		}
		if (p->next != NULL)
		{
			t->next = new ShapeLink;
			t->next->vector = p->next->vector;
		}
		if (firstElement == NULL) firstElement = t;
		else
		{
			t->prev->linked = f;
			f->next->linked = t;
		}
		f = t;
		if (p->next == NULL) p = NULL;
		else if (p->next->linked == shape.firstElement) p = NULL;
		else p = p->next->linked;
	}
	if (closed)
	{
		f->next->linked = firstElement;
		firstElement->prev->linked = f;
	}
	lastElement = f;
}

CShape::~CShape()
{
	clear();
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CShape::add(const CVertex &next, const CVector &d1, const CVector &d2, bool close)
{
	if (firstElement == NULL || closed) return;

	if (!close)
	{
		lastElement->next = new ShapeLink;
		ShapeElement *se = new ShapeElement;
		lastElement->next->linked = se;
		lastElement->next->vector = d1;
		se->vertex = next;
		se->prev = new ShapeLink;
		se->prev->linked = lastElement;
		se->prev->vector = d2;
		lastElement = se;
	}
	else
	{
		lastElement->next = new ShapeLink;
		lastElement->next->linked = firstElement;
		lastElement->next->vector = d1;
		firstElement->prev = new ShapeLink;
		firstElement->prev->linked = lastElement;
		firstElement->prev->vector = d2;
		closed = true;
	}
}

void CShape::add(const CVertex &first)
{
	if (firstElement != NULL || closed) return;
	firstElement = lastElement = new ShapeElement;
	firstElement->vertex = first;
}

void CShape::scale(double s)
{
	ShapeElement *p = firstElement;
	while (p != NULL)
	{
		ShapeElement *temp = p;
		if (p->next == NULL) p = NULL;
		else if (p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;
		temp->vertex = CVertex(0, 0, 0) + s * (temp->vertex - CVertex(0, 0, 0));
		if (temp->next != NULL) temp->next->vector = s * temp->next->vector;
		if (temp->prev != NULL) temp->prev->vector = s * temp->prev->vector;
	}
}

void CShape::rotate(double theta)
{
	CMatrix rm = CGeometry::getRotateMatrix(CVertex(0, 0, 0), CVector(0, 0, 1), theta);

	ShapeElement *p = firstElement;
	while (p != NULL)
	{
		ShapeElement *temp = p;
		if (p->next == NULL) p = NULL;
		else if (p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;
		CVertex nv, pv;
		if (temp->next != NULL) nv = temp->vertex + temp->next->vector;
		if (temp->prev != NULL) pv = temp->vertex + temp->prev->vector;
		CGeometry::executeTransform(temp->vertex, rm);
		CGeometry::executeTransform(nv, rm);
		CGeometry::executeTransform(pv, rm);
		if (temp->next != NULL) temp->next->vector = nv - temp->vertex;
		if (temp->prev != NULL) temp->prev->vector = pv - temp->vertex;
	}
}

CVertex CShape::getCenter()
{
	return center;
}

void CShape::move(const CVector &d)
{
	center = center + d;
	ShapeElement *p = firstElement;
	while (p != NULL)
	{
		ShapeElement *temp = p;
		if (p->next == NULL) p = NULL;
		else if (p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;
		temp->vertex = temp->vertex + d;
	}
}

void CShape::drawShape(CDC *pDC, const CUserView &view, const CRect &viewRect, COLORREF color, bool drawvector)
{

	ShapeElement *p = firstElement;
	if (p == NULL) return;

	double r = 6;
	double dx = 400 / view.getWidth();
	r /= dx;
	double c = 3 * r;
	CMatrix rm = CGeometry::getAlterViewMatrix(view);

	CPen pen(PS_SOLID, 1, color);
	CPen *oldpen = pDC->SelectObject(&pen);

	CGeometry::drawEdge(pDC, center + CVector(-c, 0, 0), center + CVector(c, 0, 0), view, viewRect, rm);
	CGeometry::drawEdge(pDC, center + CVector(0, -c, 0), center + CVector(0, c, 0), view, viewRect, rm);

	bool e = false;
	while (!e)
	{
		CGeometry::drawEdge(pDC, p->vertex + CVector(-r, 0, 0), p->vertex + CVector(r, 0, 0), view, viewRect, rm);
		CGeometry::drawEdge(pDC, p->vertex + CVector(0, -r, 0), p->vertex + CVector(0, r, 0), view, viewRect, rm);
		if (drawvector)
		{
			if (p->next && p->next->vector.getLength() > ZERO)
			{
				CGeometry::drawEdge(pDC, p->vertex, p->vertex + p->next->vector, view, viewRect, rm);
				CVector v1 = p->next->vector;
				double l = v1.getLength();
				v1.standardize();
				CVector v2 = v1 * CVector(0, 0, 1);
				v2.standardize();
				CVertex c = p->vertex + (l - 6) * v1;
				CGeometry::drawEdge(pDC, p->vertex + p->next->vector, c + 4 * v2, view, viewRect, rm);
				CGeometry::drawEdge(pDC, p->vertex + p->next->vector, c - 4 * v2, view, viewRect, rm);
			}
			if (p->prev && p->prev->vector.getLength() > ZERO)
			{
				CGeometry::drawEdge(pDC, p->vertex, p->vertex + p->prev->vector, view, viewRect, rm);
				CVector v1 = p->prev->vector;
				double l = v1.getLength();
				v1.standardize();
				CVector v2 = v1 * CVector(0, 0, 1);
				v2.standardize();
				CVertex c = p->vertex + (l - 6) * v1;
				CGeometry::drawEdge(pDC, p->vertex + p->prev->vector, c + 4 * v2, view, viewRect, rm);
				CGeometry::drawEdge(pDC, p->vertex + p->prev->vector, c - 4 * v2, view, viewRect, rm);
			}
		}
		ShapeElement *temp = p;
		if (p->next == NULL)
		{
			p = NULL;
			e = true;
		}
		else if (p->next->linked == firstElement)
		{
			e = true;
			p = p->next->linked;
		}
		else p = p->next->linked;
		if (p != NULL)
		{
			recursiveDrawShape(pDC, viewRect, view, temp->vertex, temp->vertex + temp->next->vector, p->vertex + p->prev->vector, p->vertex, 0, 0.5);
			recursiveDrawShape(pDC, viewRect, view, temp->vertex, temp->vertex + temp->next->vector, p->vertex + p->prev->vector, p->vertex, 0.5, 1);
		}
	}

	pDC->SelectObject(oldpen);
	pen.DeleteObject();
}

void CShape::setCenter(const CVertex &c)
{
	center = c;
}

void CShape::recursiveDrawShape(CDC *pDC, const CRect &viewRect, const CUserView &view, const CVertex &v1, const CVertex &v2, const CVertex &v3, const CVertex &v4, double s, double e)
{
	double m = (s + e) / 2;
	double xs = (1 - s) * (1 - s) * (1 - s) * v1.x +
				3 * s * (1 - s) * (1 - s) * v2.x +
				3 * s * s * (1 - s) * v3.x +
				s * s * s * v4.x;
	double ys = (1 - s) * (1 - s) * (1 - s) * v1.y +
				3 * s * (1 - s) * (1 - s) * v2.y +
				3 * s * s * (1 - s) * v3.y +
				s * s * s * v4.y;
	double xe = (1 - e) * (1 - e) * (1 - e) * v1.x +
				3 * e * (1 - e) * (1 - e) * v2.x +
				3 * e * e * (1 - e) * v3.x +
				e * e * e * v4.x;
	double ye = (1 - e) * (1 - e) * (1 - e) * v1.y +
				3 * e * (1 - e) * (1 - e) * v2.y +
				3 * e * e * (1 - e) * v3.y +
				e * e * e * v4.y;
	double xm = (1 - m) * (1 - m) * (1 - m) * v1.x +
				3 * m * (1 - m) * (1 - m) * v2.x +
				3 * m * m * (1 - m) * v3.x +
				m * m * m * v4.x;
	double ym = (1 - m) * (1 - m) * (1 - m) * v1.y +
				3 * m * (1 - m) * (1 - m) * v2.y +
				3 * m * m * (1 - m) * v3.y +
				m * m * m * v4.y;
	CVertex sv(xs, ys, 0);
	CVertex ev(xe, ye, 0);
	CVertex mv(xm, ym, 0);

	CMatrix rm = CGeometry::getAlterViewMatrix(view);
	if (((mv - sv) ^ (ev - mv)) < LINECURVE) CGeometry::drawEdge(pDC, sv, ev, view, viewRect, rm);
	else
	{
		recursiveDrawShape(pDC, viewRect, view, v1, v2, v3, v4, s, m);
		recursiveDrawShape(pDC, viewRect, view, v1, v2, v3, v4, m, e);
	}
}

CSurface *CShape::createPolyline()
{
	CSurface *result = new CSurface;

	ShapeElement *p = firstElement;
	bool e = false;
	while (!e)
	{
		ShapeElement *temp = p;
		if (p->next == NULL)
		{
			p = NULL;
			e = true;
		}
		else if (p->next->linked == firstElement)
		{
			e = true;
			p = p->next->linked;
		}
		else p = p->next->linked;
		if (p != NULL)
		{
			recursiveCreatePolyline(*result, temp->vertex, temp->vertex + temp->next->vector, p->vertex + p->prev->vector, p->vertex, 0, 0.5);
			recursiveCreatePolyline(*result, temp->vertex, temp->vertex + temp->next->vector, p->vertex + p->prev->vector, p->vertex, 0.5, 1);
		}
	}

	return result;
}

void CShape::recursiveCreatePolyline(CSurface &surface, const CVertex &v1, const CVertex &v2, const CVertex &v3, const CVertex &v4, double s, double e)
{
	double m = (s + e) / 2;
	double xs = (1 - s) * (1 - s) * (1 - s) * v1.x +
				3 * s * (1 - s) * (1 - s) * v2.x +
				3 * s * s * (1 - s) * v3.x +
				s * s * s * v4.x;
	double ys = (1 - s) * (1 - s) * (1 - s) * v1.y +
				3 * s * (1 - s) * (1 - s) * v2.y +
				3 * s * s * (1 - s) * v3.y +
				s * s * s * v4.y;
	double xe = (1 - e) * (1 - e) * (1 - e) * v1.x +
				3 * e * (1 - e) * (1 - e) * v2.x +
				3 * e * e * (1 - e) * v3.x +
				e * e * e * v4.x;
	double ye = (1 - e) * (1 - e) * (1 - e) * v1.y +
				3 * e * (1 - e) * (1 - e) * v2.y +
				3 * e * e * (1 - e) * v3.y +
				e * e * e * v4.y;
	double xm = (1 - m) * (1 - m) * (1 - m) * v1.x +
				3 * m * (1 - m) * (1 - m) * v2.x +
				3 * m * m * (1 - m) * v3.x +
				m * m * m * v4.x;
	double ym = (1 - m) * (1 - m) * (1 - m) * v1.y +
				3 * m * (1 - m) * (1 - m) * v2.y +
				3 * m * m * (1 - m) * v3.y +
				m * m * m * v4.y;
	CVertex sv(xs, ys, 0);
	CVertex ev(xe, ye, 0);
	CVertex mv(xm, ym, 0);

	if (((mv - sv) ^ (ev - mv)) < LINECURVE_FORLOFT)
	{
		double formu[4];
		surface<<new CEdge(sv, ev, formu);
	}
	else
	{
		recursiveCreatePolyline(surface, v1, v2, v3, v4, s, m);
		recursiveCreatePolyline(surface, v1, v2, v3, v4, m, e);
	}
}

double CShape::getRadius()
{
	double mr = 0;

	ShapeElement *p = firstElement;
	while (p != NULL)
	{
		ShapeElement *temp = p;
		if (p->next == NULL) p = NULL;
		else if (p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;
		CVector v = temp->vertex - CVertex(0, 0, 0);
		if (v.getLength() > mr) mr = v.getLength();
		if (temp->next != NULL && (v + temp->next->vector).getLength() > mr) mr = (v + temp->next->vector).getLength();
		if (temp->prev != NULL && (v + temp->prev->vector).getLength() > mr) mr = (v + temp->prev->vector).getLength();
	}

	return mr;
}

bool CShape::checkCross(const CSurface &surface)
{
	CEdge *e = surface.firstEdge;
	while (e != NULL)
	{
		CEdge *r = e->next;
		while (r != NULL)
		{
			CVertex v1 = e->sv;
			CVertex v2 = r->sv;
			CVector k1 = e->ev - e->sv;
			CVector k2 = r->ev - r->sv;

			double a = k1.x  * k2.y - k1.y * k2.x;
			double b1 = v2.x * k2.y + v1.y * k2.x - v2.y * k2.x - v1.x * k2.y;
			double b2 = v1.x * k1.y + v2.y * k1.x - v1.y * k1.x - v2.x * k1.y;
			if (fabs(a) < ZERO)
			{
				if (fabs(b1) < ZERO || fabs(b2) < ZERO)
				{
					double l1 = k1.getLength();
					double l2 = k2.getLength();
					k1.standardize();
					k2.standardize();
					double t1 = ((r->sv - v1) & k1) / l1;
					double t2 = ((r->ev - v1) & k1) / l1;
					double t3 = ((e->sv - v2) & k2) / l2;
					double t4 = ((e->sv - v2) & k2) / l2;
					if (t1 > 0 && fabs(t1) > ZERO && t1 < 1 && fabs(t1 - 1) > ZERO) return false;
					if (t2 > 0 && fabs(t2) > ZERO && t2 < 1 && fabs(t2 - 1) > ZERO) return false;
					if (t3 > 0 && fabs(t3) > ZERO && t3 < 1 && fabs(t3 - 1) > ZERO) return false;
					if (t4 > 0 && fabs(t4) > ZERO && t4 < 1 && fabs(t4 - 1) > ZERO) return false;
				}

				r = r->next;
				continue;
			}

			double t1 = b1 / a;
			double t2 = -b2 / a;
			if (t1 > 0 && fabs(t1) > ZERO && t1 < 1 && fabs(t1 - 1) > ZERO && t2 > 0 && fabs(t2) > ZERO && t2 < 1 && fabs(t2 - 1) > ZERO) return false;
			r = r->next;
		}
		e = e->next;
	}

	return true;
}

bool CShape::prepareCircleLoft()
{
	if (firstElement == NULL) return false;

	CSurface *result = createPolyline();

	if (!closed)
	{
		double formu[4];
		if (fabs(firstElement->vertex.x - center.x) > ZERO)
		{
			CVertex v(center.x, firstElement->vertex.y, 0);
			*result<<new CEdge(v, firstElement->vertex, formu);
		}
		if (fabs(lastElement->vertex.x - center.x) > ZERO)
		{
			CVertex v(center.x, lastElement->vertex.y, 0);
			*result<<new CEdge(lastElement->vertex, v, formu);
		}
	}

	int pos = 0;
	CEdge *p = result->firstEdge;
	while (p != NULL)
	{
		p->sv.x -= center.x;
		p->ev.x -= center.x;
		p->sv.y -= center.y;
		p->ev.y -= center.y;

		int t;
		bool f = false;

		if (fabs(p->sv.x) < ZERO && fabs(p->ev.x) < ZERO)
		{
			delete result;
			return false;
		}

		if (fabs(p->sv.x) < ZERO) t = 0;
		else if (p->sv.x > 0) t = 1;
		else t = -1;
		if (pos * t < 0) f = true;
		else pos = t;

		if (fabs(p->ev.x) < ZERO) t = 0;
		else if (p->ev.x > 0) t = 1;
		else t = -1;
		if (pos * t < 0) f = true;
		else pos = t;

		if (f)
		{
			delete result;
			return false;
		}

		p = p->next;
	}

	if (pos == 0)
	{
		delete result;
		return false;
	}

	if (!checkCross(*result))
	{
		delete result;
		return false;
	}

	move(CVertex(0, 0, 0) - center);
	if (pos == -1)
	{
		ShapeElement *p = firstElement;
		while (p != NULL)
		{
			ShapeElement *temp = p;
			if (p->next == NULL) p = NULL;
			else if (p->next->linked == firstElement) p = NULL;
			else p = p->next->linked;
			if (temp->next != NULL) temp->next->vector.x = -temp->next->vector.x;
			if (temp->prev != NULL) temp->prev->vector.x = -temp->prev->vector.x;
			temp->vertex.x = -temp->vertex.x;
		}
	}

	delete result;
	return true;
}

bool CShape::prepareLineLoft()
{
	if (firstElement == NULL) return false;
	if (!closed) return false;
	CSurface *result = createPolyline();

	bool r = checkCross(*result);
	if (r) move(CVertex(0, 0, 0) - center);
	delete result;
	return r;
}

CSurface *CShape::createSurface()
{
	CSurface *result = createPolyline();

	if (!closed)
	{
		double formu[4];
		if (fabs(firstElement->vertex.x - center.x) > ZERO)
		{
			CVertex v(center.x, firstElement->vertex.y, 0);
			*result<<new CEdge(v, firstElement->vertex, formu);
		}
		if (fabs(lastElement->vertex.x - center.x) > ZERO)
		{
			CVertex v(center.x, lastElement->vertex.y, 0);
			*result<<new CEdge(lastElement->vertex, v, formu);
		}
	}

	CVector vec;
	double miny = firstElement->vertex.y;
	double maxy = lastElement->vertex.y;
	if (miny > maxy)
	{
		double temp = miny;
		miny = maxy;
		maxy = temp;
	}

	CEdge *e = result->firstEdge;
	while (e != NULL)
	{
		double y = (e->sv.y + e->ev.y) / 2;
		
		CEdge *p = result->firstEdge;
		double mx = -getRadius() - 100;
		CEdge *me;
		bool r = true;
		while (p != NULL)
		{
			double k = p->ev.y - p->sv.y;
			if (fabs(k) < ZERO)
			{
				if (fabs(p->ev.y - y) < ZERO)
				{
					r = false;
					break;
				}
				else
				{
					p = p->next;
					continue;
				}
			}
			double t = (y - p->sv.y) / k;
			if ((fabs(t) < ZERO || t > 0) && (fabs(t - 1) < ZERO || t < 1))
			{
				double x = p->sv.x + t * (p->ev.x - p->sv.x);
				if (x > mx)
				{
					mx = x;
					me = p;
				}
			}
			p = p->next;
		}
		if (r)
		{
			CVector in;
			in = CVector(-1, 0, 0);
			vec = in * (me->ev - me->sv);
			break;
		}
		e = e->next;
	}

	e = result->firstEdge;
	while (e != NULL)
	{
		CVector vv = (e->ev - e->sv) * vec;
		vv.standardize();
		e->formula[0] = vv.x;
		e->formula[1] = vv.y;
		e->formula[2] = vv.z;
		e->formula[3] = -(vv.x * e->sv.x + vv.y * e->sv.y + vv.z * e->sv.z);
		e = e->next;
	}
	result->setFormula(0, 0, 1, 0);

	return result;
}

ShapeRenderStruct *CShape::prepareRender()
{
	bool r = checkDirection();
	ShapeRenderStruct *result = NULL;

	ShapeElement *p = firstElement;
	double tl = 0;
	if (!closed && fabs(firstElement->vertex.x - center.x) > ZERO) tl = firstElement->vertex.x - center.x;
	bool e = false;
	while (!e)
	{
		ShapeElement *temp = p;
		if (p->next == NULL)
		{
			p = NULL;
			e = true;
		}
		else if (p->next->linked == firstElement)
		{
			e = true;
			p = p->next->linked;
		}
		else p = p->next->linked;
		if (p != NULL)
		{
			recursivePrepareRender(result, temp->vertex, temp->vertex + temp->next->vector, p->vertex + p->prev->vector, p->vertex, 0, 0.5, r, tl);
			recursivePrepareRender(result, temp->vertex, temp->vertex + temp->next->vector, p->vertex + p->prev->vector, p->vertex, 0.5, 1, r, tl);
		}
	}

	if (!closed)
	{
		if (fabs(firstElement->vertex.x - center.x) > ZERO)
		{
			CVertex sv(center.x, firstElement->vertex.y, 0);
			CVertex ev = firstElement->vertex;
			double s = 0;
			double e = 1;

			ShapeRenderStruct *rs = new ShapeRenderStruct;
			rs->v1 = sv;
			rs->v2 = sv;
			rs->v3 = ev;
			rs->v4 = ev;
			rs->start = s;
			rs->end = e;
			rs->r = r;
			rs->length = (sv - ev).getLength();
			rs->tlength = 0;
			if (sv.x > ev.x) rs->rad = sv.x;
			else rs->rad = ev.x;
			if (fabs(sv.y - ev.y) < ZERO)
			{
				rs->h = true;
				rs->b = sv.y;
				rs->i = (s - e) / (sv.x - ev.x);
				rs->s = s - rs->i * sv.x;
			}
			else
			{
				rs->h = false;
				rs->k = (sv.x - ev.x) / (sv.y - ev.y);
				rs->b = sv.x - rs->k * sv.y;
				rs->i = (s - e) / (sv.y - ev.y);
				rs->s = s - rs->i * sv.y;
			}

			rs->next = result;
			result = rs;
		}
		if (fabs(lastElement->vertex.x - center.x) > ZERO)
		{
			CVertex sv = lastElement->vertex;
			CVertex ev(center.x, lastElement->vertex.y, 0);
			double s = 0;
			double e = 1;

			ShapeRenderStruct *rs = new ShapeRenderStruct;
			rs->v1 = sv;
			rs->v2 = sv;
			rs->v3 = ev;
			rs->v4 = ev;
			rs->start = s;
			rs->end = e;
			rs->r = r;
			rs->length = (sv - ev).getLength();
			rs->tlength = tl;
			if (sv.x > ev.x) rs->rad = sv.x;
			else rs->rad = ev.x;
			if (fabs(sv.y - ev.y) < ZERO)
			{
				rs->h = true;
				rs->b = sv.y;
				rs->i = (s - e) / (sv.x - ev.x);
				rs->s = s - rs->i * sv.x;
			}
			else
			{
				rs->h = false;
				rs->k = (sv.x - ev.x) / (sv.y - ev.y);
				rs->b = sv.x - rs->k * sv.y;
				rs->i = (s - e) / (sv.y - ev.y);
				rs->s = s - rs->i * sv.y;
			}

			rs->next = result;
			result = rs;
		}
	}

	return result;
}

void CShape::recursivePrepareRender(ShapeRenderStruct *&srs, const CVertex &v1, const CVertex &v2, const CVertex &v3, const CVertex &v4, double s, double e, bool r, double &tl)
{
	double m = (s + e) / 2;
	double xs = (1 - s) * (1 - s) * (1 - s) * v1.x +
				3 * s * (1 - s) * (1 - s) * v2.x +
				3 * s * s * (1 - s) * v3.x +
				s * s * s * v4.x;
	double ys = (1 - s) * (1 - s) * (1 - s) * v1.y +
				3 * s * (1 - s) * (1 - s) * v2.y +
				3 * s * s * (1 - s) * v3.y +
				s * s * s * v4.y;
	double xe = (1 - e) * (1 - e) * (1 - e) * v1.x +
				3 * e * (1 - e) * (1 - e) * v2.x +
				3 * e * e * (1 - e) * v3.x +
				e * e * e * v4.x;
	double ye = (1 - e) * (1 - e) * (1 - e) * v1.y +
				3 * e * (1 - e) * (1 - e) * v2.y +
				3 * e * e * (1 - e) * v3.y +
				e * e * e * v4.y;
	double xm = (1 - m) * (1 - m) * (1 - m) * v1.x +
				3 * m * (1 - m) * (1 - m) * v2.x +
				3 * m * m * (1 - m) * v3.x +
				m * m * m * v4.x;
	double ym = (1 - m) * (1 - m) * (1 - m) * v1.y +
				3 * m * (1 - m) * (1 - m) * v2.y +
				3 * m * m * (1 - m) * v3.y +
				m * m * m * v4.y;
	CVertex sv(xs, ys, 0);
	CVertex ev(xe, ye, 0);
	CVertex mv(xm, ym, 0);

	if (((mv - sv) ^ (ev - mv)) < LINECURVE_FORLOFT)
	{
		ShapeRenderStruct *rs = new ShapeRenderStruct;
		rs->v1 = v1;
		rs->v2 = v2;
		rs->v3 = v3;
		rs->v4 = v4;
		rs->start = s;
		rs->end = e;
		rs->r = r;
		rs->length = (sv - ev).getLength();
		rs->tlength = tl;
		tl += rs->length;
		if (sv.x > ev.x) rs->rad = sv.x;
		else rs->rad = ev.x;
		if (fabs(sv.y - ev.y) < ZERO)
		{
			rs->h = true;
			rs->b = sv.y;
			rs->i = (s - e) / (sv.x - ev.x);
			rs->s = s - rs->i * sv.x;
		}
		else
		{
			rs->h = false;
			rs->k = (sv.x - ev.x) / (sv.y - ev.y);
			rs->b = sv.x - rs->k * sv.y;
			rs->i = (s - e) / (sv.y - ev.y);
			rs->s = s - rs->i * sv.y;
		}

		rs->next = srs;
		srs = rs;
	}
	else
	{
		recursivePrepareRender(srs, v1, v2, v3, v4, s, m, r, tl);
		recursivePrepareRender(srs, v1, v2, v3, v4, m, e, r, tl);
	}
}

bool CShape::checkDirection()
{
	CSurface *result = createPolyline();

	if (!closed)
	{
		double formu[4];
		if (fabs(firstElement->vertex.x - center.x) > ZERO)
		{
			CVertex v(center.x, firstElement->vertex.y, 0);
			*result<<new CEdge(v, firstElement->vertex, formu);
		}
		if (fabs(lastElement->vertex.x - center.x) > ZERO)
		{
			CVertex v(center.x, lastElement->vertex.y, 0);
			*result<<new CEdge(lastElement->vertex, v, formu);
		}
	}

	CVector vec;
	double miny = firstElement->vertex.y;
	double maxy = lastElement->vertex.y;
	if (miny > maxy)
	{
		double temp = miny;
		miny = maxy;
		maxy = temp;
	}

	CEdge *e = result->firstEdge;
	while (e != NULL)
	{
		double y = (e->sv.y + e->ev.y) / 2;
		
		if (!closed && (y < miny || fabs(y - miny) < ZERO || y > maxy || fabs(y - maxy) < ZERO))
		{
			e = e->next;
			continue;
		}

		CEdge *p = result->firstEdge;
		double mx = getRadius() + 100;
		CEdge *me;
		bool r = true;
		while (p != NULL)
		{
			double k = p->ev.y - p->sv.y;
			if (fabs(k) < ZERO)
			{
				if (fabs(p->ev.y - y) < ZERO)
				{
					r = false;
					break;
				}
				else
				{
					p = p->next;
					continue;
				}
			}
			double t = (y - p->sv.y) / k;
			if ((fabs(t) < ZERO || t > 0) && (fabs(t - 1) < ZERO || t < 1))
			{
				double x = p->sv.x + t * (p->ev.x - p->sv.x);
				if (x < mx)
				{
					mx = x;
					me = p;
				}
			}
			p = p->next;
		}
		if (r)
		{
			CVector in;
			if (closed) in = CVector(1, 0, 0);
			else in = CVector(-1, 0, 0);
			vec = in * (me->ev - me->sv);
			break;
		}
		e = e->next;
	}

	bool r = true;
	if (vec.z < 0) r = false;
	delete result;
	return r;
}

int CShape::recursiveGetLocation(const CVertex &v, SurfaceTreeNode *tree)
{
	if (tree->lChild == NULL && tree->rChild == NULL)
	{
		CEdge *e = tree->edge;
		while (e != NULL)
		{
			double z = v.x * e->formula[0] + v.y * e->formula[1] + v.z * e->formula[2] + e->formula[3];
			if (z < 0 && fabs(z) > ZERO) return -1;
			e = e->next;
		}
		return 1;
	}
	else
	{
		double z = v.x * tree->formula[0] + v.y * tree->formula[1] + v.z * tree->formula[2] + tree->formula[3];
		if (z > 0 || fabs(z) < ZERO) return recursiveGetLocation(v, tree->lChild);
		else return recursiveGetLocation(v, tree->rChild);
	}
}

double CShape::getPosition(const CVertex &v, ShapeRenderStruct *srs)
{
	ShapeRenderStruct *rs = srs;

	while (rs != NULL)
	{
		if (rs->h)
		{
			if (fabs(v.y - rs->b) > ZERO)
			{
				rs = rs->next;
				continue;
			}

			double t = v.x * rs->i + rs->s;
			if (t > rs->start && t < rs->end) return rs->tlength + rs->length * (t - rs->start) / (rs->end - rs->start);
		}
		else
		{
			if (fabs(v.y * rs->k + rs->b - v.x) > ZERO)
			{
				rs = rs->next;
				continue;
			}

			double t = v.y * rs->i + rs->s;
			if (t > rs->start && t < rs->end) return rs->tlength + rs->length * (t - rs->start) / (rs->end - rs->start);
		}
		rs = rs->next;
	}

	return 0;
}

void CShape::clear()
{
	ShapeElement *p = firstElement;
	while (p != NULL)
	{
		ShapeElement *temp = p;
		if (p->next == NULL) p = NULL;
		else if (p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;
		if (temp->next != NULL) delete temp->next;
		if (temp->prev != NULL) delete temp->prev;
		delete temp;
	}
	firstElement = lastElement = NULL;
	closed = false;
	center = CVertex(0, 0, 0);
}

ShapeElement *CShape::selectVertex(const CPoint &pt, const CRect &viewRect, const CUserView &view)
{
	CMatrix rm = CGeometry::getAlterViewMatrix(view);

	ShapeElement *p = firstElement;
	while (p != NULL)
	{
		ShapeElement *temp = p;
		if (p->next == NULL) p = NULL;
		else if (p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;
		CVertex v = temp->vertex;
		CGeometry::executeTransform(v, rm);
		double dx = viewRect.Width() / view.getWidth();
		double dy = viewRect.Height() / view.getHeight();
		v.x *= dx;
		v.y *= dy;
		int x = pt.x - viewRect.CenterPoint().x;
		int y = viewRect.CenterPoint().y - pt.y;
		double d = (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y);
		if (d < DISTANCE_FORSELECT) return temp;
	}

	return NULL;
}

ShapeLink *CShape::selectVector(const CPoint &pt, const CRect &viewRect, const CUserView &view)
{
	CMatrix rm = CGeometry::getAlterViewMatrix(view);

	ShapeElement *p = firstElement;
	while (p != NULL)
	{
		ShapeElement *temp = p;
		if (p->next == NULL) p = NULL;
		else if (p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;

		if (temp->next)
		{
			CVertex v = temp->vertex + temp->next->vector;
			CGeometry::executeTransform(v, rm);
			double dx = viewRect.Width() / view.getWidth();
			double dy = viewRect.Height() / view.getHeight();
			v.x *= dx;
			v.y *= dy;
			int x = pt.x - viewRect.CenterPoint().x;
			int y = viewRect.CenterPoint().y - pt.y;
			double d = (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y);
			if (d < DISTANCE_FORSELECT) return temp->next;
		}
		if (temp->prev)
		{
			CVertex v = temp->vertex + temp->prev->vector;
			CGeometry::executeTransform(v, rm);
			double dx = viewRect.Width() / view.getWidth();
			double dy = viewRect.Height() / view.getHeight();
			v.x *= dx;
			v.y *= dy;
			int x = pt.x - viewRect.CenterPoint().x;
			int y = viewRect.CenterPoint().y - pt.y;
			double d = (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y);
			if (d < DISTANCE_FORSELECT) return temp->prev;
		}
	}

	return NULL;
}

ShapeElement *CShape::getFirstVertex()
{
	if (firstElement == NULL) return NULL;
	else return firstElement;
}

bool CShape::selectCenter(const CPoint &pt, const CRect &viewRect, const CUserView &view)
{
	CVertex v = center;
	CMatrix rm = CGeometry::getAlterViewMatrix(view);
	CGeometry::executeTransform(v, rm);
	double dx = viewRect.Width() / view.getWidth();
	double dy = viewRect.Height() / view.getHeight();
	v.x *= dx;
	v.y *= dy;
	int x = pt.x - viewRect.CenterPoint().x;
	int y = viewRect.CenterPoint().y - pt.y;
	double d = (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y);
	if (d < DISTANCE_FORSELECT) return true;
	else return false;
}

void CShape::write(CArchive &ar)
{
	center.write(ar);
	ar.Write(&closed, sizeof(closed));

	int n = 0;
	ShapeElement *p = firstElement;
	while (p != NULL)
	{
		if (p->next == NULL || p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;
		n++;
	}

	ar.Write(&n, sizeof(n));

	p = firstElement;
	while (p != NULL)
	{
		ShapeElement *temp = p;
		if (p->next == NULL || p->next->linked == firstElement) p = NULL;
		else p = p->next->linked;
		temp->vertex.write(ar);
		if (temp->prev) temp->prev->vector.write(ar);
		if (temp->next) temp->next->vector.write(ar);
	}
}

void CShape::read(CArchive &ar)
{
	clear();

	center.read(ar);
	ar.Read(&closed, sizeof(closed));

	int n;
	ar.Read(&n, sizeof(n));

	ShapeElement *p;
	for (int i = 0; i < n; i++)
	{
		p = new ShapeElement;
		p->vertex.read(ar);
		if (closed || i != 0)
		{
			p->prev = new ShapeLink;
			p->prev->vector.read(ar);
		}
		else p->prev = NULL;
		if (closed || i != n - 1)
		{
			p->next = new ShapeLink;
			p->next->vector.read(ar);
		}
		else p->next = NULL;
		if (i == 0) firstElement = lastElement = p;
		else
		{
			lastElement->next->linked = p;
			p->prev->linked = lastElement;
			lastElement = p;
		}
	}
	if (closed)
	{
		firstElement->prev->linked = lastElement;
		lastElement->next->linked = firstElement;
	}
}