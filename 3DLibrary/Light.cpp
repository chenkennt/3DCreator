// Light.cpp: implementation of the CLight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Light.h"
#include "Constants.h"
#include "Geometry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLight::CLight()
{
}

CLight::CLight(CVertex pos)
{
	position = pos;
	intension = 0.5;
	color = RGB(255, 255, 255);
	castShadow = true;
}

CLight::CLight(CVertex pos, double i, COLORREF c, bool cs)
{
	position = pos;
	intension = i;
	color = c;
	castShadow = cs;
}

CLight::~CLight()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

CVertex CLight::getPosition()
{
	return position;
}

void CLight::setPosition(const CVertex &pos)
{
	position = pos;
}

double CLight::getIntension() const
{
	return intension;
}

void CLight::setIntension(double i)
{
	intension = i;
}

COLORREF CLight::getColor() const
{
	return color;
}

void CLight::setColor(COLORREF c)
{
	color = c;
}

bool CLight::getCastShadow()
{
	return castShadow;
}

void CLight::setCastShadow(bool cs)
{
	castShadow = cs;
}

int CLight::getType()
{
	return TYPE_LIGHT;
}

void CLight::draw(CDC *pDC, const CRect &viewRect, const CUserView &view, const CMatrix &rm, COLORREF color, bool eliminate)
{
	CPen pen(PS_SOLID, 1, color);
	CPen *oldpen = pDC->SelectObject(&pen);

	CSurfaceModel *m = createSurfaceModel(view);
	m->draw(pDC, viewRect, view, rm, color, eliminate);
	delete m;

	pDC->SelectObject(oldpen);
	pen.DeleteObject();
}

void CLight::move(const CVector &d)
{
	position = position + d;
}

CGraphicObject *CLight::copy()
{
	return new CLight(*this);
}

CSurfaceModel *CLight::createSurfaceModel(const CUserView &view)
{
	double r = 8;
	if (view.getType() == PROJECTION_PERSPECTIVE)
	{
		CVertex pos = position;
		CMatrix rm = CGeometry::getAlterViewMatrix(view);
		CGeometry::executeTransform(pos, rm);
		r *= pos.z / view.getDistance();
	}
	double dx = 400 / view.getWidth();
	r /= dx;

	CVertex cv[2];
	cv[0] = position + CVector(0, r, 0);
	cv[1] = position + CVector(0, -r, 0);
	CVertex bv[4];
	bv[0] = position + CVector(r, 0, 0);
	bv[1] = position + CVector(0, 0, r);
	bv[2] = position + CVector(-r, 0, 0);
	bv[3] = position + CVector(0, 0, -r);
	
	CSurfaceModel *model = new CSurfaceModel;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 4; j++)
		{
			double formu[4];
			int k = (j + 1) % 4;
			CSurface *s = new CSurface;
			CSurface::getSurfaceFormula(bv[j], bv[k], position, cv[i], formu);
			*s<<new CEdge(bv[j], bv[k], formu);
			CSurface::getSurfaceFormula(bv[k], cv[i], position, bv[j], formu);
			*s<<new CEdge(bv[k], cv[i], formu);
			CSurface::getSurfaceFormula(cv[i], bv[j], position, bv[k], formu);
			*s<<new CEdge(cv[i], bv[j], formu);
			CSurface::getSurfaceFormula(cv[i], bv[j], bv[k], position, formu);
			s->setFormula(formu[0], formu[1], formu[2], formu[3]);
			*model<<s;
		}

	return model;
}

bool CLight::calcSelected(const CPoint &pt, const CRect &viewRect, const CUserView &view, bool eliminate)
{
	CSurfaceModel *m = createSurfaceModel(view);
	bool r = m->calcSelected(pt, viewRect, view, eliminate);
	delete m;

	return r;
}

CVertex CLight::getCenter()
{
	return getPosition();
}

void CLight::read(CArchive &ar)
{
	ar>>name;
	ar.Read(&castShadow, sizeof(castShadow));
	ar.Read(&color, sizeof(color));
	ar.Read(&intension, sizeof(intension));
	position.read(ar);
}

void CLight::write(CArchive &ar)
{
	ar<<name;
	ar.Write(&castShadow, sizeof(castShadow));
	ar.Write(&color, sizeof(color));
	ar.Write(&intension, sizeof(intension));
	position.write(ar);
}