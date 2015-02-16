// CSGModel.h: interface for the CCSGModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CSGMODEL_H__INCLUDED_)
#define CSGMODEL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector.h"
#include "Vertex.h"
#include "SurfaceModel.h"
#include "Material.h"
#include "UserView.h"

class CCSGModel;

struct CrossLink
{
	double distance;
	CVector vertical;
	CrossLink *next;
	CCSGModel *model;

	CrossLink(double d, const CVector &v, CCSGModel *m) {distance = d; vertical = v; model = m; next = NULL;}

};

class CCSGModel  
{
public:
	CCSGModel();
	virtual ~CCSGModel();

	virtual void move(const CVector &d) = 0;
	virtual void scale(double s) = 0;
	virtual void rotate(const CVertex &ver, const CVector &vec, double theta) = 0;
	virtual CSurfaceModel *createSurfaceModel() = 0;
	virtual CrossLink *calcCross(const CVertex &ver, const CVector &vec, double b) = 0;
	virtual int getLocation(const CVertex &v) = 0;
	virtual COLORREF getColor(const CVertex &v, const CMaterial &m, CVector &nv) = 0;
	virtual CCSGModel *alterView(const CUserView &view) = 0;
	virtual int getType() = 0;
	virtual CCSGModel *copy() = 0;
	virtual void write(CArchive &ar) = 0;
	virtual void read(CArchive &ar) = 0;
	CVertex getCenter() const;
	void setCenter(const CVertex &v);
	double getRadius() const;

	static CCSGModel *createFromFile(CArchive &ar);

protected:
	CVertex center;
	double radius;

	virtual void updateParameters() = 0;

	static int solveFormula(double pa, double pb, double pc, double answer[2]);

};

#endif // !defined(CSGMODEL_H__INCLUDED_)
