// Edge.cpp: implementation of the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Edge.h"
#include "Constants.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdge::CEdge()
{

}

CEdge::CEdge(const CVertex &v1, const CVertex &v2, double formu[4])
{
	sv = v1;
	ev = v2;
	formula[0] = formu[0];
	formula[1] = formu[1];
	formula[2] = formu[2];
	formula[3] = formu[3];
	next = NULL;
}

CEdge::~CEdge()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

int CEdge::getLocation(double formu[4])
{
	double z1 = sv.x * formu[0] + sv.y * formu[1] + sv.z * formu[2] + formu[3];
	double z2 = ev.x * formu[0] + ev.y * formu[1] + ev.z * formu[2] + formu[3];
	int k1, k2;
	if (fabs(z1) < ZERO_FOR_SURFACEMODEL) k1 = 0;
	else if (z1 > 0) k1 = 1;
	else k1 = -1;
	if (fabs(z2) < ZERO_FOR_SURFACEMODEL) k2 = 0;
	else if (z2 > 0) k2 = 1;
	else k2 = -1;

	if (k1 == 0 && k2 == 0) return 1;
	else if (k1 * k2 == 0) return k1 + k2;
	else if (k1 == 1 && k2 == 1) return 1;
	else if (k1 == -1 && k2 == -1) return -1;
	else return 0;
}

CEdge *CEdge::cut(double formu[4])
{
	double z = sv.x * formu[0] + sv.y * formu[1] + sv.z * formu[2] + formu[3];
	int p;
	if (fabs(z) < ZERO_FOR_SURFACEMODEL || z > 0) p = 1;
	else p = -1;

	CVector k = ev - sv;
	double t = -(sv.x * formu[0] + sv.y * formu[1] + sv.z * formu[2] + formu[3]) / (k.x * formu[0] + k.y * formu[1] + k.z * formu[2]);
	CVertex c(sv + t * k);
	CEdge *result;
	if (p == 1)
	{
		result = new CEdge(c, ev, formula);
		ev = c;
	}
	else
	{
		result = new CEdge(sv, c, formula);
		sv = c;
	}
	return result;
}