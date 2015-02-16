// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector.h"
#include "math.h"
#include "Constants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVector::CVector()
{
	x = y = z = 0;
}

CVector::CVector(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

CVector::~CVector()
{

}

void CVector::standardize()
{
	double l = sqrt(x * x + y * y + z * z);
	x /= l;
	y /= l;
	z /= l;
}

double CVector::getLength() const
{
	return sqrt(x * x + y * y + z * z);
}

void CVector::write(CArchive &ar)
{
	ar.Write(&x, sizeof(double));
	ar.Write(&y, sizeof(double));
	ar.Write(&z, sizeof(double));
}

void CVector::read(CArchive &ar)
{
	ar.Read(&x, sizeof(double));
	ar.Read(&y, sizeof(double));
	ar.Read(&z, sizeof(double));
}

CVector CVector::operator-() const
{
	return CVector(-x, -y, -z);
}

CVector operator+(const CVector &v1, const CVector &v2)
{
	return CVector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

CVector operator-(const CVector &v1, const CVector &v2)
{
	return CVector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

CVector operator*(const CVector &v1, const CVector &v2)
{
	return CVector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

CVector operator*(double d, const CVector &v)
{
	return CVector(d * v.x, d * v.y, d * v.z);
}

double operator&(const CVector &v1, const CVector &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double operator^(const CVector &v1, const CVector &v2)
{
	double l = v1.getLength() * v2.getLength();
	if (fabs(l) < ZERO) return 0;
	else return acos((v1 & v2) / l);
}