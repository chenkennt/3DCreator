// Scene.cpp: implementation of the CScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Scene.h"
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

CScene::CScene()
{
	firstGeometry = NULL;
	firstLight = NULL;
	ambientLight = CLight(CVertex(0, 0, 0), 0.4, RGB(255, 255, 255), true);
	background.color = RGB(0, 0, 0);
}

CScene::~CScene()
{
	clear();
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CScene::add(CGeometry *geometry, const CString &name)
{
	GeometryLink *ng = new GeometryLink;
	ng->geometry = geometry;
	ng->geometry->name = name;
	ng->next = firstGeometry;
	firstGeometry = ng;
}

void CScene::add(CLight *light, const CString &name)
{
	LightLink *nl = new LightLink;
	nl->light = light;
	nl->light->name = name;
	nl->next = firstLight;
	firstLight = nl;
}

void CScene::drawWireframe(CDC *pDC, const CRect &viewRect, const CUserView &view, bool eliminate)
{
	CMatrix rm = CGeometry::getAlterViewMatrix(view);
	GeometryLink *p = firstGeometry;
	while (p != NULL)
	{
		p->geometry->draw(pDC, viewRect, view, rm, p->geometry->material.color, eliminate);
		p = p->next;
	}
	LightLink *l = firstLight;
	while (l != NULL)
	{
		l->light->draw(pDC, viewRect, view, rm, RGB(255, 255, 128), eliminate);
		l = l->next;
	}
}

void CScene::render(CDC *pDC, const CRect &viewRect, const CUserView &view, int antialias)
{
	double d = viewRect.Width() / view.getWidth();
	CSGLink *altered = NULL;
	GeometryLink *g = firstGeometry;
	while (g != NULL)
	{
		CSGLink *l = new CSGLink;
		l->csg = g->geometry->csgModel->alterView(view);
		l->material = &g->geometry->material;
		l->next = altered;
		altered = l;
		g = g->next;
	}

	CMatrix rm = CGeometry::getAlterViewMatrix(view);
	LightLink *alight = NULL;
	LightLink *l = firstLight;
	while (l != NULL)
	{
		LightLink *nl = new LightLink;
		nl->light = new CLight(*l->light);
		CGeometry::executeTransform(nl->light->position, rm);
		nl->next = alight;
		alight = nl;
		l = l->next;
	}

	CVertex origin(0, 0, 0);
	CVector light(0, 0, 1);
	double step;
	int nstep;
	switch (antialias)
	{
	case NO_ANTI_ALIAS:
		step = 0;
		nstep = 1;
		break;
	case ANTI_ALIAS_4X:
		step = 1.0 / 2;
		nstep = 2;
		break;
	case ANTI_ALIAS_9X:
		step = 1.0 / 3;
		nstep = 3;
		break;
	}

	for (int k = 0; k <= viewRect.Width(); k++)
		for (int j = 0; j <= viewRect.Height(); j++)
		{
			double x = k - viewRect.Width() / 2;
			double y = j - viewRect.Height() / 2;
			int r = 0;
			int g = 0;
			int b = 0;
			for (int p = 0; p < nstep; p++, x += step)
				for (int q = 0; q < nstep; q++, y += step)
				{
					if (view.getType() == PROJECTION_PARALLEL) origin = CVertex(x / d, y / d, 0);
					else if (view.getType() == PROJECTION_PERSPECTIVE)
					{
						light = CVector(x, y, view.getDistance() * d);
						light.standardize();
					}
					double dis;
					COLORREF color = getSurfaceColor(origin, light, altered, alight, ambientLight, dis, 0);
					if (dis == -1)
					{
						if (background.texture) color = background.texture->getPixel(k, j);
						else color = background.color;
					}
					r += GetRValue(color);
					g += GetGValue(color);
					b += GetBValue(color);
				}
			r /= nstep * nstep;
			g /= nstep * nstep;
			b /= nstep * nstep;

			pDC->SetPixel(viewRect.left + k, viewRect.bottom - j, RGB(r, g, b));
		}

	while (altered != NULL)
	{
		CSGLink *temp = altered;
		altered = altered->next;
		delete temp->csg;
		delete temp;
	}
	while (alight != NULL)
	{
		LightLink *temp = alight;
		alight = alight->next;
		delete temp->light;
		delete temp;
	}
}

COLORREF CScene::getSurfaceColor(const CVertex &origin, const CVector &light, CSGLink *csglist, LightLink *lightlist, const CLight &al, double &dis, int depth)
{
	if (depth == MAX_DEPTH) return RGB(0, 0, 0);

	CSGLink *g = csglist;
	CSGLink *minc = NULL;
	CrossLink *mind = NULL;

	while (g != NULL)
	{
		CrossLink *cross = g->csg->calcCross(origin, light, -1);
		while (cross != NULL)
			if (mind == NULL || cross->distance < mind->distance)
			{
				if (mind != NULL) delete mind;
				mind = cross;
				minc = g;
				cross = cross->next;
			}
			else
			{
				CrossLink *t = cross;
				cross = cross->next;
				delete t;
			}
		g = g->next;
	}

	if (mind == NULL)
	{
		dis = -1;
		return RGB(0, 0, 0);
	}
	else dis = mind->distance;

	COLORREF alcolor = luminance(al.getIntension() * minc->material->ka, al.getColor());
	LightLink *l = lightlist;
	CVertex cp = origin + mind->distance * light;
	COLORREF scolor = mind->model->getColor(cp, *minc->material, mind->vertical);
	COLORREF color = lightColor(scolor, alcolor);
	CVector rrlight = calcReflectLight(light, mind->vertical);
	bool oti = true;
	if ((light ^ mind->vertical) < PI / 2)
	{
		mind->vertical = -mind->vertical;
		oti = false;
	}

	while (l != NULL)
	{
		CVertex o = l->light->getPosition();
		CVector lig = o - cp;
		double b = lig.getLength();
		lig.standardize();
		bool cr = false;

		if (l->light->getCastShadow())
		{
			g = csglist;
			while (g != NULL)
			{
				CrossLink *cross = g->csg->calcCross(cp, lig, b);
				if (cross != NULL)
				{
					while (cross != NULL)
					{
						CrossLink *t = cross;
						cross = cross->next;
						delete t;
					}
					cr = true;
					break;
				}
				g = g->next;
			}
		}
		if (!cr)
		{
			double angle = lig ^ mind->vertical;
			if (angle < PI / 2)
			{
				double alpha = rrlight ^ lig;
				if (alpha > PI / 2) alpha = PI / 2;
				double decline = b * DK + K;
				COLORREF lc = luminance(l->light->getIntension() * minc->material->kd * cos(angle) / decline, l->light->getColor());
				COLORREF dc = lightColor(scolor, lc);
				color = addColor(color, dc);
				COLORREF sc = luminance(l->light->getIntension() * minc->material->ks * pow(cos(alpha), minc->material->n) / decline, l->light->getColor());
				color = addColor(color, sc);
			}
		}

		l = l->next;
	}

	if (minc->material->kr != 0)
	{
		COLORREF rc;
		rrlight.standardize();
		double dis;
		rc = getSurfaceColor(cp, rrlight, csglist, lightlist, al, dis, depth + 1);
		rc = luminance(minc->material->kr, rc);
		color = addColor(color, rc);
	}

	if (minc->material->kt != 0)
	{
		COLORREF tc;
		CVector tlight;
		if (oti) tlight = calcRefractLight(light, mind->vertical, 1, minc->material->rr);
		else tlight = calcRefractLight(light, mind->vertical, minc->material->rr, 1);
		tlight.standardize();
		double dis;
		tc = getSurfaceColor(cp, tlight, csglist, lightlist, al, dis, depth + 1);
		tc = lightColor(scolor, tc);
		tc = luminance(minc->material->kt, tc);
		color = addColor(color, tc);
	}

	delete mind;
	return color;
}

COLORREF CScene::luminance(double k, COLORREF c)
{
	if (k > 1) k = 1;
	return RGB(k * GetRValue(c), k * GetGValue(c), k * GetBValue(c));
}

COLORREF CScene::addColor(COLORREF a, COLORREF c)
{
	int r = GetRValue(a) + GetRValue(c);
	int g = GetGValue(a) + GetGValue(c);
	int b = GetBValue(a) + GetBValue(c);
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;

	return RGB(r, g, b);
}

COLORREF CScene::lightColor(COLORREF c, COLORREF l)
{
	return RGB(GetRValue(c) * GetRValue(l) / 255, GetGValue(c) * GetGValue(l) / 255, GetBValue(c) * GetBValue(l) / 255);
}

CVector CScene::calcReflectLight(const CVector &l, const CVector &v)
{
	CVector vv = v;
	vv.standardize();
	return 1 / fabs(vv & l) * l + 2 * vv;
}

CVector CScene::calcRefractLight(const CVector &l, const CVector &v, double n1, double n2)
{
	CVector vv = v;
	vv.standardize();
	CVector ll = 1 / fabs(vv & l) * l;
	double tt = (n2 / n1) * (n2 / n1) * ll.getLength() * ll.getLength() - (ll + vv).getLength() * (ll + vv).getLength();
	if (tt < 0) return calcReflectLight(l, v);
	double kf = 1 / sqrt(tt);
	return kf * (vv + ll) - vv;
}

CGraphicObject *CScene::getSelection(const CPoint &pt, const CUserView &view, const CRect &viewRect, bool eliminate, bool smooth)
{
	LightLink *l = firstLight;
	while (l != NULL)
	{
		if (l->light->calcSelected(pt, viewRect, view, eliminate)) return l->light;
		l = l->next;
	}

	if (smooth)
	{
		CMatrix rm = CGeometry::getAlterViewMatrix(view);

		double d = viewRect.Width() / view.getWidth();
		CSGLink *altered = NULL;
		CSGLink *f = NULL;
		GeometryLink *gl = firstGeometry;
		while (gl != NULL)
		{
			CSGLink *l = new CSGLink;
			l->csg = gl->geometry->csgModel->alterView(view);
			l->material = &gl->geometry->material;
			l->next = NULL;
			if (f == NULL) altered = f = l;
			else
			{
				f->next = l;
				f = l;
			}
			gl = gl->next;
		}

		double x = pt.x - viewRect.CenterPoint().x;
		double y = viewRect.CenterPoint().y - pt.y;
		CVertex origin(0, 0, 0);
		CVector light(0, 0, 1);
		if (view.getType() == PROJECTION_PARALLEL) origin = CVertex(x / d, y / d, 0);
		else if (view.getType() == PROJECTION_PERSPECTIVE)
		{
			light = CVector(x, y, view.getDistance() * d);
			light.standardize();
		}
		CSGLink *g = altered;
		gl = firstGeometry;
		CGraphicObject *minc = NULL;
		CrossLink *mind = NULL;

		while (g != NULL)
		{
			CrossLink *cross = g->csg->calcCross(origin, light, -1);
			while (cross != NULL)
				if (mind == NULL || cross->distance < mind->distance)
				{
					if (mind != NULL) delete mind;
					mind = cross;
					minc = gl->geometry;
					cross = cross->next;
				}
				else
				{
					CrossLink *t = cross;
					cross = cross->next;
					delete t;
				}
			g = g->next;
			gl = gl->next;
		}

		while (altered != NULL)
		{
			CSGLink *temp = altered;
			altered = altered->next;
			delete temp->csg;
			delete temp;
		}

		if (mind) delete mind;
		if (minc) return minc;
	}
	else
	{
		GeometryLink *g = firstGeometry;
		while (g != NULL)
		{
			if (g->geometry->surfaceModel->calcSelected(pt, viewRect, view, eliminate)) return g->geometry;
			g = g->next;
		}
	}

	return NULL;
}

void CScene::clear()
{
	while (firstGeometry != NULL)
	{
		GeometryLink *temp = firstGeometry;
		firstGeometry = firstGeometry->next;
		delete temp->geometry;
		delete temp;
	}

	while (firstLight != NULL)
	{
		LightLink *temp = firstLight;
		firstLight = firstLight->next;
		delete temp->light;
		delete temp;
	}

	ambientLight = CLight(CVertex(0, 0, 0), 0.4, RGB(255, 255, 255), true);
	background.color = RGB(0, 0, 0);
	if (background.texture)
	{
		delete background.texture;
		background.texture = NULL;
	}
}

void CScene::remove(CGraphicObject *object, bool erase)
{
	GeometryLink *g = firstGeometry;
	GeometryLink *f = NULL;
	while (g != NULL)
	{
		if (g->geometry == object)
		{
			if (g == firstGeometry) firstGeometry = g->next;
			else f->next = g->next;
			if (erase) delete g->geometry;
			delete g;
			return;
		}
		f = g;
		g = g->next;
	}

	LightLink *l = firstLight;
	LightLink *fl = NULL;
	while (l != NULL)
	{
		if (l->light == object)
		{
			if (l == firstLight) firstLight = l->next;
			else fl->next = l->next;
			if (erase) delete l->light;
			delete l;
			return;
		}
		fl = l;
		l = l->next;
	}
}

void CScene::intersectObject(CGeometry *g1, CGeometry *g2)
{
	if (g1 == g2) return;

	GeometryLink *g = firstGeometry;
	GeometryLink *f = NULL;
	while (g != NULL)
	{
		if (g->geometry == g2)
		{
			g1->intersectGeometry(g2);

			if (g == firstGeometry) firstGeometry = g->next;
			else f->next = g->next;
			delete g;
			return;
		}
		f = g;
		g = g->next;
	}
}

void CScene::unionObject(CGeometry *g1, CGeometry *g2)
{
	if (g1 == g2) return;

	GeometryLink *g = firstGeometry;
	GeometryLink *f = NULL;
	while (g != NULL)
	{
		if (g->geometry == g2)
		{
			g1->unionGeometry(g2);

			if (g == firstGeometry) firstGeometry = g->next;
			else f->next = g->next;
			delete g;
			return;
		}
		f = g;
		g = g->next;
	}
}

void CScene::differenceObject(CGeometry *g1, CGeometry *g2)
{
	if (g1 == g2) return;

	GeometryLink *g = firstGeometry;
	GeometryLink *f = NULL;
	while (g != NULL)
	{
		if (g->geometry == g2)
		{
			g1->differenceGeometry(g2);

			if (g == firstGeometry) firstGeometry = g->next;
			else f->next = g->next;
			delete g;
			return;
		}
		f = g;
		g = g->next;
	}
}

void CScene::read(CArchive &ar)
{
	clear();

	ambientLight.read(ar);
	background.read(ar);

	int nl;
	int ng;

	ar.Read(&nl, sizeof(nl));
	for (int i = 0; i < nl; i++)
	{
		CLight *l = new CLight;
		l->read(ar);
		add(l, l->name);
	}
	ar.Read(&ng, sizeof(ng));
	for (i = 0; i < ng; i++)
	{
		CGeometry *g = new CGeometry;
		g->read(ar);
		add(g, g->name);
	}
}

void CScene::write(CArchive &ar)
{
	ambientLight.write(ar);
	background.write(ar);

	int nl = 0;
	LightLink *l = firstLight;
	while (l != NULL)
	{
		nl++;
		l = l->next;
	}
	int ng = 0;
	GeometryLink *g = firstGeometry;
	while (g != NULL)
	{
		ng++;
		g = g->next;
	}

	ar.Write(&nl, sizeof(nl));
	l = firstLight;
	while (l != NULL)
	{
		l->light->write(ar);
		l = l->next;
	}

	ar.Write(&ng, sizeof(ng));
	g = firstGeometry;
	while (g != NULL)
	{
		g->geometry->write(ar);
		g = g->next;
	}
}

void CScene::simpleRender(CDC *pDC, const CRect &viewRect, const CUserView &view, COLORREF bk)
{
	CMatrix rm = CGeometry::getAlterViewMatrix(view);

	double d = viewRect.Width() / view.getWidth();
	CSGLink *altered = NULL;
	GeometryLink *g = firstGeometry;
	while (g != NULL)
	{
		CSGLink *l = new CSGLink;
		l->csg = g->geometry->csgModel->alterView(view);
		l->material = &g->geometry->material;
		l->next = altered;
		altered = l;
		g = g->next;
	}

	LightLink *alight = NULL;
	LightLink *l = firstLight;
	while (l != NULL)
	{
		LightLink *nl = new LightLink;
		nl->light = new CLight(*l->light);
		CGeometry::executeTransform(nl->light->position, rm);
		nl->next = alight;
		alight = nl;
		l = l->next;
	}

	CVertex origin(0, 0, 0);
	CVector light(0, 0, 1);
//	COLORREF *pline = new COLORREF[viewRect.Height() + 1 + SIMPLE_RENDER_GAP * 2];
//	COLORREF *nline = new COLORREF[viewRect.Height() + 1 + SIMPLE_RENDER_GAP * 2];
	COLORREF pline[2048];
	COLORREF nline[2048];
	COLORREF pcolor;

	for (int k = 0; k <= viewRect.Width() + SIMPLE_RENDER_GAP; k += SIMPLE_RENDER_GAP)
	{
		for (int j = 0; j <= viewRect.Height() + SIMPLE_RENDER_GAP; j += SIMPLE_RENDER_GAP)
		{
			double x = k - viewRect.Width() / 2;
			double y = j - viewRect.Height() / 2;

			if (view.getType() == PROJECTION_PARALLEL) origin = CVertex(x / d, y / d, 0);
			else if (view.getType() == PROJECTION_PERSPECTIVE)
			{
				light = CVector(x, y, view.getDistance() * d);
				light.standardize();
			}
			double dis;
			COLORREF color = getSimpleSurfaceColor(origin, light, altered, alight, ambientLight, dis);
			if ((dis != -1 || pcolor != bk || pline[j] != bk || pline[j - SIMPLE_RENDER_GAP] != bk) && k != 0 && j != 0)
			{
				int dr1 = GetRValue(color) - GetRValue(pline[j - SIMPLE_RENDER_GAP]);
				int dg1 = GetGValue(color) - GetGValue(pline[j - SIMPLE_RENDER_GAP]);
				int db1 = GetBValue(color) - GetBValue(pline[j - SIMPLE_RENDER_GAP]);
				int dr2 = GetRValue(pcolor) - GetRValue(pline[j - SIMPLE_RENDER_GAP]);
				int dg2 = GetGValue(pcolor) - GetGValue(pline[j - SIMPLE_RENDER_GAP]);
				int db2 = GetBValue(pcolor) - GetBValue(pline[j - SIMPLE_RENDER_GAP]);
				int dr3 = GetRValue(pline[j]) - GetRValue(pline[j - SIMPLE_RENDER_GAP]);
				int dg3 = GetGValue(pline[j]) - GetGValue(pline[j - SIMPLE_RENDER_GAP]);
				int db3 = GetBValue(pline[j]) - GetBValue(pline[j - SIMPLE_RENDER_GAP]);
				if (fabs(dr1) > MAX_COLOR_DIFFERENCE || fabs(dg1) > MAX_COLOR_DIFFERENCE || fabs(db1) > MAX_COLOR_DIFFERENCE
					|| fabs(dr2) > MAX_COLOR_DIFFERENCE || fabs(dg2) > MAX_COLOR_DIFFERENCE || fabs(db2) > MAX_COLOR_DIFFERENCE
					|| fabs(dr3) > MAX_COLOR_DIFFERENCE || fabs(dg3) > MAX_COLOR_DIFFERENCE || fabs(db3) > MAX_COLOR_DIFFERENCE)
					for (int p = 0; p < SIMPLE_RENDER_GAP; p++)
						for (int q = 0; q < SIMPLE_RENDER_GAP; q++)
						{
							double x = k - SIMPLE_RENDER_GAP + p - viewRect.Width() / 2;
							double y = j - SIMPLE_RENDER_GAP + q - viewRect.Height() / 2;

							if (view.getType() == PROJECTION_PARALLEL) origin = CVertex(x / d, y / d, 0);
							else if (view.getType() == PROJECTION_PERSPECTIVE)
							{
								light = CVector(x, y, view.getDistance() * d);
								light.standardize();
							}
							double dis;
							COLORREF color = getSimpleSurfaceColor(origin, light, altered, alight, ambientLight, dis);
							if (dis != -1) pDC->SetPixel(viewRect.left + k - SIMPLE_RENDER_GAP + p, viewRect.bottom - j + SIMPLE_RENDER_GAP - q, color);
						}
				else
					for (int p = 0; p < SIMPLE_RENDER_GAP; p++)
					{
						int r = GetRValue(pline[j - SIMPLE_RENDER_GAP]) + p * (GetRValue(pcolor) - GetRValue(pline[j - SIMPLE_RENDER_GAP])) / SIMPLE_RENDER_GAP;
						int g = GetGValue(pline[j - SIMPLE_RENDER_GAP]) + p * (GetGValue(pcolor) - GetGValue(pline[j - SIMPLE_RENDER_GAP])) / SIMPLE_RENDER_GAP;
						int b = GetBValue(pline[j - SIMPLE_RENDER_GAP]) + p * (GetBValue(pcolor) - GetBValue(pline[j - SIMPLE_RENDER_GAP])) / SIMPLE_RENDER_GAP;
						int r1 = GetRValue(pline[j]) + p * (GetRValue(color) - GetRValue(pline[j])) / SIMPLE_RENDER_GAP;
						int g1 = GetGValue(pline[j]) + p * (GetGValue(color) - GetGValue(pline[j])) / SIMPLE_RENDER_GAP;
						int b1 = GetBValue(pline[j]) + p * (GetBValue(color) - GetBValue(pline[j])) / SIMPLE_RENDER_GAP;
						double dr = (r1 - r) / SIMPLE_RENDER_GAP;
						double dg = (g1 - g) / SIMPLE_RENDER_GAP;
						double db = (b1 - b) / SIMPLE_RENDER_GAP;
						for (int q = 0; q < SIMPLE_RENDER_GAP; q++)
							pDC->SetPixel(viewRect.left + k - SIMPLE_RENDER_GAP + p, viewRect.bottom - j + SIMPLE_RENDER_GAP - q, RGB(r + q * dr, g + q * dg, b + q * db));
					}
			}
			if (dis != -1) nline[j] = pcolor = color;
			else nline[j] = pcolor = bk;
		}
		memcpy(pline, nline, sizeof(COLORREF) * (viewRect.Height() + 1 + SIMPLE_RENDER_GAP));
	}

//	delete[] pline;
//	delete[] nline;

	while (altered != NULL)
	{
		CSGLink *temp = altered;
		altered = altered->next;
		delete temp->csg;
		delete temp;
	}
	while (alight != NULL)
	{
		LightLink *temp = alight;
		alight = alight->next;
		delete temp->light;
		delete temp;
	}

	l = firstLight;
	while (l != NULL)
	{
		l->light->draw(pDC, viewRect, view, rm, RGB(255, 255, 128), true);
		l = l->next;
	}
}

COLORREF CScene::getSimpleSurfaceColor(const CVertex &origin, const CVector &light, CSGLink *csglist, LightLink *lightlist, const CLight &al, double &dis)
{
	CSGLink *g = csglist;
	CSGLink *minc = NULL;
	CrossLink *mind = NULL;

	while (g != NULL)
	{
		CrossLink *cross = g->csg->calcCross(origin, light, -1);
		while (cross != NULL)
			if (mind == NULL || cross->distance < mind->distance)
			{
				if (mind != NULL) delete mind;
				mind = cross;
				minc = g;
				cross = cross->next;
			}
			else
			{
				CrossLink *t = cross;
				cross = cross->next;
				delete t;
			}
		g = g->next;
	}

	if (mind == NULL)
	{
		dis = -1;
		return RGB(0, 0, 0);
	}
	else dis = mind->distance;

	COLORREF alcolor = luminance(al.getIntension() * minc->material->ka, al.getColor());
	LightLink *l = lightlist;
	CVertex cp = origin + mind->distance * light;
	COLORREF scolor = minc->material->color;
	COLORREF color = lightColor(scolor, alcolor);
	CVector rrlight = calcReflectLight(light, mind->vertical);
	if ((light ^ mind->vertical) < PI / 2) mind->vertical = -mind->vertical;

	while (l != NULL)
	{
		CVertex o = l->light->getPosition();
		CVector lig = o - cp;
		double b = lig.getLength();
		lig.standardize();
		bool cr = false;

/*		if (l->light->getCastShadow())
		{
			g = csglist;
			while (g != NULL)
			{
				CrossLink *cross = g->csg->calcCross(cp, lig, b);
				if (cross != NULL)
				{
					while (cross != NULL)
					{
						CrossLink *t = cross;
						cross = cross->next;
						delete t;
					}
					cr = true;
					break;
				}
				g = g->next;
			}
		}*/
		if (!cr)
		{
			double angle = lig ^ mind->vertical;
			if (angle < PI / 2)
			{
				double alpha = rrlight ^ lig;
				if (alpha > PI / 2) alpha = PI / 2;
				double decline = b * DK + K;
				COLORREF lc = luminance(l->light->getIntension() * minc->material->kd * cos(angle) / decline, l->light->getColor());
				COLORREF dc = lightColor(scolor, lc);
				color = addColor(color, dc);
				COLORREF sc = luminance(l->light->getIntension() * minc->material->ks * pow(cos(alpha), minc->material->n) / decline, l->light->getColor());
				color = addColor(color, sc);
			}
		}

		l = l->next;
	}

	delete mind;
	return color;
}