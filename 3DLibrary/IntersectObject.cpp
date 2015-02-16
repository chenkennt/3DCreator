// IntersectObject.cpp: implementation of the CIntersectObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IntersectObject.h"
#include "Constants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntersectObject::CIntersectObject()
{

}

CIntersectObject::CIntersectObject(CCSGModel *po, CCSGModel *so) : CBoolObject(po, so)
{
}

CIntersectObject::~CIntersectObject()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

CrossLink *CIntersectObject::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	CrossLink *l1 = primaryObject->calcCross(ver, vec, b);
	CrossLink *l2 = secondaryObject->calcCross(ver, vec, b);

	CrossLink *p = l1;
	CrossLink *ff = NULL;
	while (p != NULL)
		if (secondaryObject->getLocation(ver + p->distance * vec) < 0)
		{
			CrossLink *t = p;
			p = p->next;
			if (ff == NULL) l1 = p;
			else ff->next = p;
			delete t;
		}
		else
		{
			ff = p;
			p = p->next;
		}
	if (l1 == NULL) l1 = l2;
	else ff->next = l2;
	p = l2;
	while (p != NULL)
		if (primaryObject->getLocation(ver + p->distance * vec) < 0)
		{
			CrossLink *t = p;
			p = p->next;
			if (ff == NULL) l1 = p;
			else ff->next = p;
			delete t;
		}
		else
		{
			ff = p;
			p = p->next;
		}

	return l1;
}

int CIntersectObject::getLocation(const CVertex &v)
{
	int l1 = primaryObject->getLocation(v);
	int l2 = secondaryObject->getLocation(v);
	if (l1 > 0 && l2 > 0) return 1;
	if (l1 >= 0 && l2 >= 0) return 0;
	return -1;
}

CCSGModel *CIntersectObject::alterView(const CUserView &view)
{
	CIntersectObject *result = new CIntersectObject;
	result->primaryObject = primaryObject->alterView(view);
	result->secondaryObject = secondaryObject->alterView(view);

	return result;
}

CCSGModel *CIntersectObject::copy()
{
	CIntersectObject *result = new CIntersectObject(*this);
	result->primaryObject = primaryObject->copy();
	result->secondaryObject = secondaryObject->copy();
	return result;
}

CSurfaceModel *CIntersectObject::createSurfaceModel()
{
	CSurfaceModel *m1 = primaryObject->createSurfaceModel();
	CSurfaceModel *m2 = secondaryObject->createSurfaceModel();
	CSurfaceModel *model = m1->intersectModel(*m2);
	delete m1;
	delete m2;
	return model;
}

void CIntersectObject::write(CArchive &ar)
{
	int type = TYPE_INTERSECT;
	ar.Write(&type, sizeof(int));

	primaryObject->write(ar);
	secondaryObject->write(ar);
}

void CIntersectObject::read(CArchive &ar)
{
	primaryObject = CCSGModel::createFromFile(ar);
	secondaryObject = CCSGModel::createFromFile(ar);

	updateParameters();
}