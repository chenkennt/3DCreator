// Cylinder.h: interface for the CCylinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CYLINDER_H__INCLUDED_)
#define CYLINDER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSGModel.h"

class CCylinder : public CCSGModel  
{
public:
	CCylinder();
	CCylinder(const CVertex &c, const CVector &a, double h, double r);
	virtual ~CCylinder();

	void move(const CVector &d);
	void scale(double s);
	void rotate(const CVertex &ver, const CVector &vec, double theta);
	CSurfaceModel *createSurfaceModel();
	CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b);
	int getLocation(const CVertex &v);
	COLORREF getColor(const CVertex &v, const CMaterial &m, CVector &nv);
	CCSGModel *alterView(const CUserView &view);
	int getType();
	void setPRadius(double r);
	void setCenter(const CVertex &c);
	void setHeight(double h);
	double getHeight() const;
	double getPRadius() const;
	CCSGModel *copy();
	void write(CArchive &ar);
	void read(CArchive &ar);

private:
	double pradius;
	double height;
	CVector axis;
	CVector xaxis;
	CVertex top;
	CVertex bottom;
	double face[2][4];

	void updateParameters();

};

#endif // !defined(CYLINDER_H__INCLUDED_)
