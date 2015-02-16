// Vector.h: interface for the CVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(VECTOR_H__INCLUDED_)
#define VECTOR_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVector  
{
public:
	double x;
	double y;
	double z;

	CVector();
	CVector(double x, double y, double z);
	virtual ~CVector();

	void standardize();
	CVector operator-() const;
	double getLength() const;
	void write(CArchive &ar);
	void read(CArchive &ar);

};

CVector operator+(const CVector &v1, const CVector &v2);
CVector operator-(const CVector &v1, const CVector &v2);
CVector operator*(const CVector &v1, const CVector &v2);
CVector operator*(double d, const CVector &v);
double operator&(const CVector &v1, const CVector &v2);
double operator^(const CVector &v1, const CVector &v2);


#endif // !defined(VECTOR_H__INCLUDED_)
