// Cone.h: interface for the CCone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CONE_H__INCLUDED_)
#define CONE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSGModel.h"

class CCone : public CCSGModel  
{
public:
	CCone();
	CCone(const CVertex &c, const CVector &a, double h, double tr, double br);
	virtual ~CCone();

	void move(const CVector &d);
	void scale(double s);
	void rotate(const CVertex &ver, const CVector &vec, double theta);
	CSurfaceModel *createSurfaceModel();
	CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b);
	int getLocation(const CVertex &v);
	COLORREF getColor(const CVertex &v, const CMaterial &m, CVector &nv);
	CCSGModel *alterView(const CUserView &view);
	int getType();
	void setBCenter(const CVertex &c);
	void setTRadius(double r);
	void setBRadius(double r);
	void setHeight(double h);
	CVector getAxis();
	double getTRadius() const;
	double getBRadius() const;
	CVertex getBCenter() const;
	double getHeight() const;
	CCSGModel *copy();
	void write(CArchive &ar);
	void read(CArchive &ar);

private:
	double height;
	double tradius;
	double bradius;
	CVector axis;
	CVector xaxis;
	CVertex bcenter;

	void updateParameters();

};

#endif // !defined(CONE_H__INCLUDED_)
