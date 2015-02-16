// Cube.h: interface for the CCube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CUBE_H__INCLUDED_)
#define CUBE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSGModel.h"

class CCube : public CCSGModel  
{
public:
	CCube();
	CCube(const CVertex &c, const CVector &yd, const CVector &xd, double l, double w, double h);
	virtual ~CCube();

	void move(const CVector &d);
	void scale(double s);
	void rotate(const CVertex &ver, const CVector &vec, double theta);
	CSurfaceModel *createSurfaceModel();
	CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b);
	int getLocation(const CVertex &v);
	COLORREF getColor(const CVertex &v, const CMaterial &m, CVector &nv);
	CCSGModel *alterView(const CUserView &view);
	int getType();
	void setLength(double l);
	void setHeight(double h);
	void setWidth(double w);
	double getLength() const;
	double getHeight() const;
	double getWidth() const;
	CCSGModel *copy();
	void write(CArchive &ar);
	void read(CArchive &ar);

private:
	CVector xAxis;
	CVector yAxis;
	CVector zAxis;
	double length;
	double width;
	double height;

	double face[6][4];
	CVertex vertex[8];

	void updateParameters();
};

#endif // !defined(CUBE_H__INCLUDED_)
