// BoolObject.cpp: implementation of the CBoolObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoolObject.h"
#include "Constants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoolObject::CBoolObject()
{

}

CBoolObject::CBoolObject(CCSGModel *po, CCSGModel *so)
{
	center = po->getCenter() + 0.5 * (so->getCenter() - po->getCenter());
	primaryObject = po;
	secondaryObject = so;
}

CBoolObject::~CBoolObject()
{
	delete primaryObject;
	delete secondaryObject;
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CBoolObject::move(const CVector &d)
{
	primaryObject->move(d);
	secondaryObject->move(d);
	center = center + d;
}

void CBoolObject::scale(double s)
{
	primaryObject->scale(s);
	secondaryObject->scale(s);
}

void CBoolObject::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	primaryObject->rotate(ver, vec, theta);
	secondaryObject->rotate(ver, vec, theta);
}

COLORREF CBoolObject::getColor(const CVertex &v, const CMaterial &m, CVector &nv)
{
	if (primaryObject->getLocation(v) == 0) return primaryObject->getColor(v, m, nv);
	else return secondaryObject->getColor(v, m, nv);
}

void CBoolObject::updateParameters()
{
}

int CBoolObject::getType()
{
	return TYPE_BOOLEAN;
}