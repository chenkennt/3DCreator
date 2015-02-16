// UnionObject.h: interface for the CUnionObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNIONOBJECT_H__INCLUDED_)
#define UNIONOBJECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoolObject.h"

class CUnionObject : public CBoolObject  
{
public:
	CUnionObject();
	CUnionObject(CCSGModel *po, CCSGModel *so);
	virtual ~CUnionObject();

	CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b);
	int getLocation(const CVertex &v);
	CCSGModel *alterView(const CUserView &view);
	CCSGModel *copy();
	CSurfaceModel *createSurfaceModel();
	void write(CArchive &ar);
	void read(CArchive &ar);

};

#endif // !defined(UNIONOBJECT_H__INCLUDED_)
