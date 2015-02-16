// Shape.h: interface for the CShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SHAPE_H__INCLUDED_)
#define SHAPE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector.h"
#include "Vertex.h"
#include "UserView.h"
#include "Surface.h"

struct ShapeElement;

struct ShapeLink
{
	ShapeElement *linked;
	CVector vector;
};

struct ShapeElement
{
	CVertex vertex;
	ShapeLink *next;
	ShapeLink *prev;

	ShapeElement() {next = prev = NULL;}
};

struct ShapeRenderStruct
{
	double b;
	double k;
	double s;
	double i;
	double start;
	double end;
	CVertex v1;
	CVertex v2;
	CVertex v3;
	CVertex v4;
	bool h;
	bool r;
	double rad;
	double length;
	double tlength;

	ShapeRenderStruct *next;
};

class CShape
{
public:
	CShape();
	CShape(const CShape &shape);
	virtual ~CShape();

	void add(const CVertex &next, const CVector &d1, const CVector &d2, bool close = false);
	void add(const CVertex &first);
	void move(const CVector &d);
	void scale(double s);
	void rotate(double theta);
	CVertex getCenter();
	void setCenter(const CVertex &c);
	void drawShape(CDC *pDC, const CUserView &view, const CRect &viewRect, COLORREF color, bool drawvector = false);
	bool prepareCircleLoft();
	bool prepareLineLoft();
	CSurface *createSurface();
	ShapeRenderStruct *prepareRender();
	double getRadius();
	void clear();
	ShapeElement *selectVertex(const CPoint &pt, const CRect &viewRect, const CUserView &view);
	ShapeLink *selectVector(const CPoint &pt, const CRect &viewRect, const CUserView &view);
	bool selectCenter(const CPoint &pt, const CRect &viewRect, const CUserView &view);
	ShapeElement *getFirstVertex();
	void write(CArchive &ar);
	void read(CArchive &ar);

	static int recursiveGetLocation(const CVertex &v, SurfaceTreeNode *tree);
	static void recursiveDrawShape(CDC *pDC, const CRect &viewRect, const CUserView &view, const CVertex &v1, const CVertex &v2, const CVertex &v3, const CVertex &v4, double s, double e);
	static double getPosition(const CVertex &v, ShapeRenderStruct *srs);

private:
	CVertex center;
	bool closed;
	ShapeElement *firstElement;
	ShapeElement *lastElement;

	CSurface *createPolyline();
	bool checkDirection();

	static void recursiveCreatePolyline(CSurface &surface, const CVertex &v1, const CVertex &v2, const CVertex &v3, const CVertex &v4, double s, double e);
	static void recursivePrepareRender(ShapeRenderStruct *&srs, const CVertex &v1, const CVertex &v2, const CVertex &v3, const CVertex &v4, double s, double e, bool r, double &tl);
	static bool checkCross(const CSurface &surface);

};

#endif // !defined(SHAPE_H__INCLUDED_)
