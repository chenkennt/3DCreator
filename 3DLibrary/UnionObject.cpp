// UnionObject.cpp: implementation of the CUnionObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnionObject.h"
#include "Constants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnionObject::CUnionObject()
{

}

CUnionObject::CUnionObject(CCSGModel *po, CCSGModel *so) : CBoolObject(po, so)
{
}

CUnionObject::~CUnionObject()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

CrossLink *CUnionObject::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	CrossLink *l1 = primaryObject->calcCross(ver, vec, b);
	CrossLink *l2 = secondaryObject->calcCross(ver, vec, b);

	if (l1 == NULL) return l2;
	else
	{
		CrossLink *p = l1;
		while (p->next != NULL) p = p->next;
		p->next = l2;
		return l1;
	}
}

int CUnionObject::getLocation(const CVertex &v)
{
	int l1 = primaryObject->getLocation(v);
	int l2 = secondaryObject->getLocation(v);
	if (l1 < 0 && l2 < 0) return -1;
	if (l1 <= 0 && l2 <= 0) return 0;
	return 1;
}

CCSGModel *CUnionObject::alterView(const CUserView &view)
{
	CUnionObject *result = new CUnionObject;
	result->primaryObject = primaryObject->alterView(view);
	result->secondaryObject = secondaryObject->alterView(view);

	return result;
}

CCSGModel *CUnionObject::copy()
{
	CUnionObject *result = new CUnionObject(*this);
	result->primaryObject = primaryObject->copy();
	result->secondaryObject = secondaryObject->copy();
	return result;
}

CSurfaceModel *CUnionObject::createSurfaceModel()
{
	CSurfaceModel *m1 = primaryObject->createSurfaceModel();
	CSurfaceModel *m2 = secondaryObject->createSurfaceModel();
	CSurfaceModel *model = m1->unionModel(*m2);
	delete m1;
	delete m2;
	return model;
}

void CUnionObject::write(CArchive &ar)
{
	int type = TYPE_UNION;
	ar.Write(&type, sizeof(int));

	primaryObject->write(ar);
	secondaryObject->write(ar);
}

void CUnionObject::read(CArchive &ar)
{
	primaryObject = CCSGModel::createFromFile(ar);
	secondaryObject = CCSGModel::createFromFile(ar);

	updateParameters();
}