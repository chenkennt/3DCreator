// Edge.h: interface for the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(EDGE_H__INCLUDED_)
#define EDGE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vertex.h"

class CEdge  
{
public:
	friend class CSurface;
	friend class CSurfaceModel;
	friend class CShape;
	friend class CCircleLoftObject;
	friend class CLineLoftObject;

	CEdge();
	CEdge(const CVertex &v1, const CVertex &v2, double formu[4]);
	virtual ~CEdge();

private:
	CVertex sv;
	CVertex ev;
	CEdge *next;
	double formula[4];

	int getLocation(double formu[4]);
	CEdge *cut(double formu[4]);

};

#endif // !defined(EDGE_H__INCLUDED_)
