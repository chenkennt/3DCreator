// Geometry.h: interface for the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GEOMETRY_H__INCLUDED_)
#define GEOMETRY_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSGModel.h"
#include "SurfaceModel.h"
#include "Matrix.h"
#include "Material.h"
#include "GraphicObject.h"

class CGeometry : public CGraphicObject
{
public:
	friend class CScene;

	CMaterial material;

	CGeometry();
	CGeometry(CCSGModel *csg, COLORREF color = RGB(255, 255, 255));
	virtual ~CGeometry();

	void move(const CVector &d);
	void scale(double s);
	void rotate(const CVertex &ver, const CVector &vec, double theta);
	CVertex getCenter();
	void intersectGeometry(CGeometry *g);
	void unionGeometry(CGeometry *g);
	void differenceGeometry(CGeometry *g);
	int getType();
	void draw(CDC *pDC, const CRect &viewRect, const CUserView &view, const CMatrix &rm, COLORREF color, bool eliminate = true);
	CCSGModel *getCSGModel();
	CGraphicObject *copy();
	void recreateSurfaceModel();
	void read(CArchive &ar);
	void write(CArchive &ar);

	static CMatrix getRotateMatrix(const CVertex &ver, const CVector &vec, double theta);
	static CMatrix getAlterViewMatrix(const CUserView &view);
	static void executeTransform(CVertex &v, const CMatrix &m);
	static void drawEdge(CDC *pDC, const CVertex &v1, const CVertex &v2, const CUserView &view, const CRect viewRect, const CMatrix &rm);

private:
	CCSGModel *csgModel;
	CSurfaceModel *surfaceModel;

};

#endif // !defined(GEOMETRY_H__INCLUDED_)
