// BoolObject.h: interface for the CBoolObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(BOOLOBJECT_H__INCLUDED_)
#define BOOLOBJECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSGModel.h"

class CBoolObject : public CCSGModel  
{
public:
	CBoolObject();
	CBoolObject(CCSGModel *po, CCSGModel *so);
	virtual ~CBoolObject();

	void move(const CVector &d);
	void scale(double s);
	void rotate(const CVertex &ver, const CVector &vec, double theta);
	COLORREF getColor(const CVertex &v, const CMaterial &m, CVector &nv);
	int getType();

protected:
	CCSGModel *primaryObject;
	CCSGModel *secondaryObject;

	void updateParameters();

};

#endif // !defined(BOOLOBJECT_H__INCLUDED_)
