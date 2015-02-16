// DifferenceObject.h: interface for the CDifferenceObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DIFFERENCEOBJECT_H__INCLUDED_)
#define DIFFERENCEOBJECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoolObject.h"

class CDifferenceObject : public CBoolObject  
{
public:
	CDifferenceObject();
	CDifferenceObject(CCSGModel *po, CCSGModel *so);
	virtual ~CDifferenceObject();

	CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b);
	int getLocation(const CVertex &v);
	CCSGModel *alterView(const CUserView &view);
	CCSGModel *copy();
	CSurfaceModel *createSurfaceModel();
	void write(CArchive &ar);
	void read(CArchive &ar);

};

#endif // !defined(DIFFERENCEOBJECT_H__INCLUDED_)
