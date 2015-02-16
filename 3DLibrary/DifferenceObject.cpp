// DifferenceObject.cpp: implementation of the CDifferenceObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DifferenceObject.h"
#include "Constants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDifferenceObject::CDifferenceObject()
{

}

CDifferenceObject::CDifferenceObject(CCSGModel *po, CCSGModel *so) : CBoolObject(po, so)
{
}

CDifferenceObject::~CDifferenceObject()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

CrossLink *CDifferenceObject::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	CrossLink *l1 = primaryObject->calcCross(ver, vec, b);
	CrossLink *l2 = secondaryObject->calcCross(ver, vec, b);

	CrossLink *p = l1;
	CrossLink *ff = NULL;
	while (p != NULL)
		if (secondaryObject->getLocation(ver + p->distance * vec) > 0)
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
			p->vertical.x = -p->vertical.x;
			p->vertical.y = -p->vertical.y;
			p->vertical.z = -p->vertical.z;
			p = p->next;
		}

	return l1;
}

int CDifferenceObject::getLocation(const CVertex &v)
{
	int l1 = primaryObject->getLocation(v);
	int l2 = secondaryObject->getLocation(v);
	if (l1 > 0 && l2 < 0) return 1;
	if (l1 >= 0 && l2 <= 0) return 0;
	return -1;
}

CCSGModel *CDifferenceObject::alterView(const CUserView &view)
{
	CDifferenceObject *result = new CDifferenceObject;
	result->primaryObject = primaryObject->alterView(view);
	result->secondaryObject = secondaryObject->alterView(view);

	return result;
}

CCSGModel *CDifferenceObject::copy()
{
	CDifferenceObject *result = new CDifferenceObject(*this);
	result->primaryObject = primaryObject->copy();
	result->secondaryObject = secondaryObject->copy();
	return result;
}

CSurfaceModel *CDifferenceObject::createSurfaceModel()
{
	CSurfaceModel *m1 = primaryObject->createSurfaceModel();
	CSurfaceModel *m2 = secondaryObject->createSurfaceModel();
	CSurfaceModel *model = m1->differenceModel(*m2);
	delete m1;
	delete m2;
	return model;
}

void CDifferenceObject::write(CArchive &ar)
{
	int type = TYPE_DIFFERENCE;
	ar.Write(&type, sizeof(int));

	primaryObject->write(ar);
	secondaryObject->write(ar);
}

void CDifferenceObject::read(CArchive &ar)
{
	primaryObject = CCSGModel::createFromFile(ar);
	secondaryObject = CCSGModel::createFromFile(ar);

	updateParameters();
}