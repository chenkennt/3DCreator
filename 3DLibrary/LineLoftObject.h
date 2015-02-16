// LineLoftObject.h: interface for the CLineLoftObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LINELOFTOBJECT_H__INCLUDED_)
#define LINELOFTOBJECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSGModel.h"
#include "Shape.h"

class CLineLoftObject : public CCSGModel  
{
public:
	CLineLoftObject();
	CLineLoftObject(const CLineLoftObject &loft);
	virtual ~CLineLoftObject();

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

	static CLineLoftObject *create(const CShape &s, const CVertex &c, const CVector &za, const CVector &ya, double h);

private:
	CShape *shape;
	CVector zaxis;
	CVector yaxis;
	double height;
	CVertex pcenter;
	ShapeRenderStruct *srs;
	SurfaceTreeNode *stree;

	void updateParameters();

};

#endif // !defined(LINELOFTOBJECT_H__INCLUDED_)
