// Vertex.h: interface for the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(VERTEX_H__INCLUDED_)
#define VERTEX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector.h"

class CVertex  
{
public:
	double x;
	double y;
	double z;

	CVertex();
	CVertex(double x, double y, double z);
	virtual ~CVertex();

	void write(CArchive &ar);
	void read(CArchive &ar);

};

CVector operator-(const CVertex &v1, const CVertex &v2);
CVertex operator+(const CVertex &ver, const CVector &vec);
CVertex operator-(const CVertex &ver, const CVector &vec);

#endif // !defined(VERTEX_H__INCLUDED_)
