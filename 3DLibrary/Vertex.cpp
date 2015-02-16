// Vertex.cpp: implementation of the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vertex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVertex::CVertex()
{
	x = y = z = 0;
}

CVertex::CVertex(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

CVertex::~CVertex()
{

}

void CVertex::write(CArchive &ar)
{
	ar.Write(&x, sizeof(double));
	ar.Write(&y, sizeof(double));
	ar.Write(&z, sizeof(double));
}

void CVertex::read(CArchive &ar)
{
	ar.Read(&x, sizeof(double));
	ar.Read(&y, sizeof(double));
	ar.Read(&z, sizeof(double));
}

CVector operator-(const CVertex &v1, const CVertex &v2)
{
	return CVector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

CVertex operator+(const CVertex &ver, const CVector &vec)
{
	return CVertex(ver.x + vec.x, ver.y + vec.y, ver.z + vec.z);
}

CVertex operator-(const CVertex &ver, const CVector &vec)
{
	return CVertex(ver.x - vec.x, ver.y - vec.y, ver.z - vec.z);
}