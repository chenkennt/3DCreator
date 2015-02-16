// IntersectObject.h: interface for the CIntersectObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(INTERSECTOBJECT_H__INCLUDED_)
#define INTERSECTOBJECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoolObject.h"

class CIntersectObject : public CBoolObject  
{
public:
	CIntersectObject();
	CIntersectObject(CCSGModel *po, CCSGModel *so);
	virtual ~CIntersectObject();

	CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b);
	int getLocation(const CVertex &v);
	CCSGModel *alterView(const CUserView &view);
	CCSGModel *copy();
	CSurfaceModel *createSurfaceModel();
	void write(CArchive &ar);
	void read(CArchive &ar);

};

#endif // !defined(INTERSECTOBJECT_H__INCLUDED_)
