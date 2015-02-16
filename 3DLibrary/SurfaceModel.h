// SurfaceModel.h: interface for the CSurfaceModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SURFACEMODEL_H__INCLUDED_)
#define SURFACEMODEL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Surface.h"
#include "UserView.h"

struct SurfaceLink
{
	CSurface *surface;
	SurfaceLink *next;
};

class CSurfaceModel  
{
public:
	friend class CCube;
	friend class CSphere;
	friend class CCylinder;
	friend class CCone;
	friend class CCircleLoftObject;
	friend class CLineLoftObject;

	CSurfaceModel();
	CSurfaceModel(const CSurfaceModel &model);
	virtual ~CSurfaceModel();

	void move(const CVector &d);
	void scale(double s);
	void rotate(const CVertex &ver, const CVector &vec, double theta);
	CSurfaceModel &operator<<(CSurface *surface);
	void draw(CDC *pDC, const CRect &viewRect, const CUserView &view, const CMatrix &rm, COLORREF color, bool eliminate = true);
	CSurfaceModel *unionModel(CSurfaceModel &model);
	CSurfaceModel *intersectModel(CSurfaceModel &model);
	CSurfaceModel *differenceModel(CSurfaceModel &model);
	bool calcSelected(const CPoint &pt, const CRect &viewRect, const CUserView &view, bool eliminate);

private:
	SurfaceLink *firstSurface;
	CVertex center;

	CSurfaceModel *cut(const CSurfaceModel &csm);
	CSurface *cut(const CSurface &s) const;
	void combine(CSurfaceModel &model);

};

#endif // !defined(SURFACEMODEL_H__INCLUDED_)
