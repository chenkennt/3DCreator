// CSGModel.cpp: implementation of the CCSGModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSGModel.h"
#include "Constants.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "LineLoftObject.h"
#include "CircleLoftObject.h"
#include "UnionObject.h"
#include "IntersectObject.h"
#include "DifferenceObject.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCSGModel::CCSGModel()
{

}

CCSGModel::~CCSGModel()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

CVertex CCSGModel::getCenter() const
{
	return center;
}

double CCSGModel::getRadius() const
{
	return radius;
}

int CCSGModel::solveFormula(double pa, double pb, double pc, double answer[2])
{
	if (fabs(pa) < ZERO)
	{
		if (fabs(pb) > ZERO)
		{
			answer[0] = -pc / pb;
			return 1;
		}
		else return 0;
	}
	else
	{
		double delta = pb * pb -  4 * pa * pc;
		if (fabs(delta) < ZERO)
		{
			answer[0] = -pb / (2 * pa);
			return 1;
		}
		else if (delta < 0) return 0;
		else
		{
			answer[0] = (-pb + sqrt(delta)) / (2 * pa);
			answer[1] = (-pb - sqrt(delta)) / (2 * pa);
			return 2;
		}
	}
}

void CCSGModel::setCenter(const CVertex &v)
{
	center = v;
	updateParameters();
}

CCSGModel *CCSGModel::createFromFile(CArchive &ar)
{
	int type;
	ar.Read(&type, sizeof(int));

	CCSGModel *result;
	switch (type)
	{
	case TYPE_CUBE:
		result = new CCube;
		break;
	case TYPE_SPHERE:
		result = new CSphere;
		break;
	case TYPE_CYLINDER:
		result = new CCylinder;
		break;
	case TYPE_CONE:
		result = new CCone;
		break;
	case TYPE_LINELOFT:
		result = new CLineLoftObject;
		break;
	case TYPE_CIRCLELOFT:
		result = new CCircleLoftObject;
		break;
	case TYPE_UNION:
		result = new CUnionObject;
		break;
	case TYPE_INTERSECT:
		result = new CIntersectObject;
		break;
	case TYPE_DIFFERENCE:
		result = new CDifferenceObject;
		break;
	default:
		return NULL;
	}

	result->read(ar);
	return result;
}