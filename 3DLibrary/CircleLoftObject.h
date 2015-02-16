// LoftObject.h: interface for the CCircleLoftObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LOFTOBJECT_H__INCLUDED_)
#define LOFTOBJECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSGModel.h"
#include "Shape.h"

class CCircleLoftObject : public CCSGModel  
{
public:
	CCircleLoftObject();
	CCircleLoftObject(const CCircleLoftObject &loft);
	virtual ~CCircleLoftObject();

	void move(const CVector &d);
	void scale(double s);
	void rotate(const CVertex &ver, const CVector &vec, double theta);
	CSurfaceModel *createSurfaceModel();
	CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b);
	int getLocation(const CVertex &v);
	COLORREF getColor(const CVertex &v, const CMaterial &m, CVector &nv);
	CCSGModel *alterView(const CUserView &view);
	int getType();
	CCSGModel *copy();
	void write(CArchive &ar);
	void read(CArchive &ar);

	static CCircleLoftObject *create(const CShape &s, const CVertex &c, const CVector &v);

private:
	CShape *shape;
	CVector axis;
	CVector xaxis;
	double pradius;
	ShapeRenderStruct *srs;
	SurfaceTreeNode *stree;

	void updateParameters();

};

#endif // !defined(LOFTOBJECT_H__INCLUDED_)
