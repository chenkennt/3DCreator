// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MATRIX_H__INCLUDED_)
#define MATRIX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMatrix  
{
public:
	CMatrix();
	CMatrix(int r, int c, double *elements = NULL);
	CMatrix(CMatrix &m);
	virtual ~CMatrix();

	int getRowCount() const;
	int getColumnCount() const;

	double *operator[](int x) const;
	CMatrix &operator=(const CMatrix &m);
	friend CMatrix operator*(const CMatrix &m1, const CMatrix &m2);

private:
	int row;
	int col;
	double *elements;

};

#endif // !defined(MATRIX_H__INCLUDED_)
