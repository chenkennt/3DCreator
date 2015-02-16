// Surface.h: interface for the CSurface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SURFACE_H__INCLUDED_)
#define SURFACE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vertex.h"
#include "Edge.h"
#include "Matrix.h"

struct SurfaceTreeNode
{
	double formula[4];
	SurfaceTreeNode *lChild;
	SurfaceTreeNode *rChild;
	CEdge *edge;
};

struct VertexLink
{
	CVertex *vertex;
	VertexLink *next;
};

class CSurface  
{
public:
	friend class CSurfaceModel;
	friend class CShape;
	friend class CCube;
	friend class CCircleLoftObject;
	friend class CLineLoftObject;

	CSurface();
	CSurface(const CSurface &s);
	virtual ~CSurface();

	CSurface &operator<<(CEdge *e);
	void setFormula(double a, double b, double c, double d);

	static void getSurfaceFormula(const CVertex &v1, const CVertex &v2, const CVertex &v3, const CVertex &v4, double formula[4]);
	static void rotateSurface(double formula[4], const CMatrix &rm);

private:
	CEdge *firstEdge;
	double formula[4];

	void combine(CSurface &s);
	CSurface *cut(const CSurface &cs);
	CSurface *cross(const CSurface &cs);
	SurfaceTreeNode *createSurfaceTree() const;
	static CSurface *recursiveCut(SurfaceTreeNode *tree, CSurface *s);
	static void recursiveCreateTree(SurfaceTreeNode *tree);
	static void recursiveDeleteTree(SurfaceTreeNode *tree);

};

#endif // !defined(SURFACE_H__INCLUDED_)
