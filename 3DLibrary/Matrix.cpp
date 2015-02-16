// Matrix.cpp: implementation of the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrix.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{
	row = col = 0;
	elements = new double[0];
}

CMatrix::CMatrix(int r, int c, double *elements)
{
	row = r;
	col = c;
	this->elements = new double[r * c];
	if (elements == NULL) memset(this->elements, 0, sizeof(double) * r * c);
	else memcpy(this->elements, elements, sizeof(double) * r * c);
}

CMatrix::CMatrix(CMatrix &m)
{
	row = m.getRowCount();
	col = m.getColumnCount();
	elements = new double[row * col];
	memcpy(elements, m.elements, sizeof(double) * row * col);
}

CMatrix::~CMatrix()
{
	delete[] elements;
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

int CMatrix::getRowCount() const
{
	return row;
}

int CMatrix::getColumnCount() const
{
	return col;
}

double *CMatrix::operator[](int x) const
{
	return elements + x * col;
}

CMatrix &CMatrix::operator=(const CMatrix &m)
{
	if (elements != NULL) delete[] elements;
	row = m.getRowCount();
	col = m.getColumnCount();
	elements = new double[row * col];

	memcpy(elements, m.elements, sizeof(double) * row * col);
	return *this;
}

CMatrix operator*(const CMatrix &m1, const CMatrix &m2)
{
	if (m1.getColumnCount() != m2.getRowCount()) return CMatrix(0, 0);

	CMatrix result(m1.getRowCount(), m2.getColumnCount());
	for (int i = 0; i < result.row; i++)
		for (int j = 0; j < result.col; j++)
		{
			double r = 0;
			for (int k = 0; k < m1.col; k++) r += m1[i][k] * m2[k][j];
			result[i][j] = r;
		}
	return result;
}