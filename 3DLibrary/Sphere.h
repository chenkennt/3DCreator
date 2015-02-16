// Sphere.h: interface for the CSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SPHERE_H__INCLUDED_)
#define SPHERE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSGModel.h"

class CSphere : public CCSGModel  
{
public:
	CSphere();
	CSphere(const CVertex &c, double r);
	virtual ~CSphere();

	void move(const CVector &d);
	void scale(double s);
	void rotate(const CVertex &ver, const CVector &vec, double theta);
	CSurfaceModel *createSurfaceModel();
	void setRadius(double r);
	void setCenter(const CVertex &c);
	CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b);
	int getLocation(const CVertex &v);
	COLORREF getColor(const CVertex &v, const CMaterial &m, CVector &nv);
	CCSGModel *alterView(const CUserView &view);
	int getType();
	CCSGModel *copy();
	void write(CArchive &ar);
	void read(CArchive &ar);

private:
	CVertex xm;
	CVertex ym;

	void updateParameters();

};

#endif // !defined(SPHERE_H___INCLUDED_)
