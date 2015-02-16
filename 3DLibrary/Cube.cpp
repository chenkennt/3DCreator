// Cube.cpp: implementation of the CCube class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cube.h"
#include "math.h"
#include "Matrix.h"
#include "Geometry.h"
#include "Constants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCube::CCube()
{

}

CCube::CCube(const CVertex &c, const CVector &yd, const CVector &xd, double l, double w, double h)
{
	center = c;
	yAxis = yd;
	yAxis.standardize();
	xAxis = xd - (xd & yAxis) * yAxis;
	xAxis.standardize();
	zAxis = xAxis * yAxis;
	zAxis.standardize();
	length = l;
	width = w;
	height = h;
	updateParameters();
}

CCube::~CCube()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CCube::move(const CVector &d)
{
	center = center + d;

	updateParameters();
}

void CCube::scale(double s)
{
	center.x *= s;
	center.y *= s;
	center.z *= s;
	length *= s;
	width *= s;
	height *= s;

	radius *= s;

	updateParameters();
}

void CCube::rotate(const CVertex &ver, const CVector &vec, double theta)
{
	CVertex vx = center + xAxis;
	CVertex vy = center + yAxis;
	CVertex vz = center + zAxis;

	CMatrix rm = CGeometry::getRotateMatrix(ver, vec, theta);
	CGeometry::executeTransform(center, rm);
	CGeometry::executeTransform(vx, rm);
	CGeometry::executeTransform(vy, rm);
	CGeometry::executeTransform(vz, rm);

	xAxis = vx - center;
	yAxis = vy - center;
	zAxis = vz - center;

	updateParameters();
}

CSurfaceModel *CCube::createSurfaceModel()
{
	CSurfaceModel *model = new CSurfaceModel;
	CSurface *s1 = new CSurface;
	CSurface *s2 = new CSurface;
	CSurface *s3 = new CSurface;
	CSurface *s4 = new CSurface;
	CSurface *s5 = new CSurface;
	CSurface *s6 = new CSurface;
	s3->setFormula(face[0][0], face[0][1], face[0][2], face[0][3]);
	s4->setFormula(face[1][0], face[1][1], face[1][2], face[1][3]);
	s1->setFormula(face[2][0], face[2][1], face[2][2], face[2][3]);
	s2->setFormula(face[3][0], face[3][1], face[3][2], face[3][3]);
	s5->setFormula(face[4][0], face[4][1], face[4][2], face[4][3]);
	s6->setFormula(face[5][0], face[5][1], face[5][2], face[5][3]);
	*s1<<new CEdge(vertex[0], vertex[1], s3->formula);
	*s1<<new CEdge(vertex[1], vertex[2], s6->formula);
	*s1<<new CEdge(vertex[2], vertex[3], s4->formula);
	*s1<<new CEdge(vertex[3], vertex[0], s5->formula);
	*s2<<new CEdge(vertex[4], vertex[5], s3->formula);
	*s2<<new CEdge(vertex[5], vertex[6], s6->formula);
	*s2<<new CEdge(vertex[6], vertex[7], s4->formula);
	*s2<<new CEdge(vertex[7], vertex[4], s5->formula);
	*s3<<new CEdge(vertex[0], vertex[4], s5->formula);
	*s3<<new CEdge(vertex[4], vertex[5], s2->formula);
	*s3<<new CEdge(vertex[5], vertex[1], s6->formula);
	*s3<<new CEdge(vertex[1], vertex[0], s1->formula);
	*s4<<new CEdge(vertex[3], vertex[7], s5->formula);
	*s4<<new CEdge(vertex[7], vertex[6], s2->formula);
	*s4<<new CEdge(vertex[6], vertex[2], s6->formula);
	*s4<<new CEdge(vertex[2], vertex[3], s1->formula);
	*s5<<new CEdge(vertex[0], vertex[4], s3->formula);
	*s5<<new CEdge(vertex[4], vertex[7], s2->formula);
	*s5<<new CEdge(vertex[7], vertex[3], s4->formula);
	*s5<<new CEdge(vertex[3], vertex[0], s1->formula);
	*s6<<new CEdge(vertex[1], vertex[5], s3->formula);
	*s6<<new CEdge(vertex[5], vertex[6], s2->formula);
	*s6<<new CEdge(vertex[6], vertex[2], s4->formula);
	*s6<<new CEdge(vertex[2], vertex[1], s1->formula);

	*model<<s1<<s2<<s3<<s4<<s5<<s6;
	model->center = center;
	return model;
}

void CCube::updateParameters()
{
	face[0][0] = face[1][0] = xAxis.x;
	face[0][1] = face[1][1] = xAxis.y;
	face[0][2] = face[1][2] = xAxis.z;
	face[2][0] = face[3][0] = yAxis.x;
	face[2][1] = face[3][1] = yAxis.y;
	face[2][2] = face[3][2] = yAxis.z;
	face[4][0] = face[5][0] = zAxis.x;
	face[4][1] = face[5][1] = zAxis.y;
	face[4][2] = face[5][2] = zAxis.z;
	CVertex vs[6];
	vs[0] = center + 0.5 * width * xAxis;
	vs[1] = center - 0.5 * width * xAxis;
	vs[2] = center + 0.5 * height * yAxis;
	vs[3] = center - 0.5 * height * yAxis;
	vs[4] = center + 0.5 * length * zAxis;
	vs[5] = center - 0.5 * length * zAxis;
	for (int i = 0; i < 6; i++)
	{
		face[i][3] = -(face[i][0] * vs[i].x + face[i][1] * vs[i].y + face[i][2] * vs[i].z);
		if (face[i][0] * center.x + face[i][1] * center.y + face[i][2] * center.z + face[i][3] < 0)
		{
			face[i][0] = -face[i][0];
			face[i][1] = -face[i][1];
			face[i][2] = -face[i][2];
			face[i][3] = -face[i][3];
		}
	}

	vertex[0] = CVertex(center + 0.5 * height * yAxis + 0.5 * width * xAxis + 0.5 * length * zAxis);
	vertex[1] = CVertex(center + 0.5 * height * yAxis + 0.5 * width * xAxis - 0.5 * length * zAxis);
	vertex[2] = CVertex(center + 0.5 * height * yAxis - 0.5 * width * xAxis - 0.5 * length * zAxis);
	vertex[3] = CVertex(center + 0.5 * height * yAxis - 0.5 * width * xAxis + 0.5 * length * zAxis);
	vertex[4] = CVertex(center - 0.5 * height * yAxis + 0.5 * width * xAxis + 0.5 * length * zAxis);
	vertex[5] = CVertex(center - 0.5 * height * yAxis + 0.5 * width * xAxis - 0.5 * length * zAxis);
	vertex[6] = CVertex(center - 0.5 * height * yAxis - 0.5 * width * xAxis - 0.5 * length * zAxis);
	vertex[7] = CVertex(center - 0.5 * height * yAxis - 0.5 * width * xAxis + 0.5 * length * zAxis);

	radius = sqrt(length * length + width * width + height * height) / 2;
}

CrossLink *CCube::calcCross(const CVertex &ver, const CVector &vec, double b)
{
	double t =
		(vec.x * (center.x - ver.x)	+ vec.y * (center.y - ver.y) + vec.z * (center.z - ver.z))
		/ (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	double d =
		sqrt((ver.x + vec.x * t - center.x) * (ver.x + vec.x * t - center.x)
		   + (ver.y + vec.y * t - center.y) * (ver.y + vec.y * t - center.y)
		   + (ver.z + vec.z * t - center.z) * (ver.z + vec.z * t - center.z));
	if (d > radius) return NULL;

	CrossLink *head = NULL;

	for (int i = 0; i < 6; i++)
	{
		double r = vec.x * face[i][0] + vec.y * face[i][1] + vec.z * face[i][2];
		if (fabs(r) < ZERO) continue;
		double t = -(ver.x * face[i][0] + ver.y * face[i][1] + ver.z * face[i][2] + face[i][3]) / r;
		if (fabs(t) < ZERO || t < 0 || (b > 0 && (t > b || fabs(b - t) < ZERO))) continue;
		CVertex cross = ver + t * vec;

		bool out = false;
		for (int j = 0; j < 6; j++)
			if (i != j)
			{
				double p = cross.x * face[j][0] + cross.y * face[j][1] + cross.z * face[j][2] + face[j][3];
				if (p < 0 && fabs(p) > ZERO)
				{
					out = true;
					break;
				}
			}
		if (!out)
		{
			CrossLink *nv = new CrossLink(t, CVector(-face[i][0], -face[i][1], -face[i][2]), this);
			nv->next = head;
			head = nv;
		}
	}
	return head;
}

int CCube::getLocation(const CVertex &v)
{
	int p = 0;
	for (int i = 0; i < 6; i++)
	{
		double d = face[i][0] * v.x + face[i][1] * v.y + face[i][2] * v.z + face[i][3];
		if (fabs(d)< ZERO) p++;
		else if (d < 0) return -1;
	}

	if (p > 0) return 0;
	else return 1;
}

COLORREF CCube::getColor(const CVertex &v, const CMaterial &m, CVector &nv)
{
	if (m.texture == NULL) return m.color;
	
	CVertex pcenter = center - width / 2 * xAxis - height / 2 * yAxis - length / 2 * zAxis;
	CVector vec = v - pcenter;
	double x = xAxis & vec;
	double y = yAxis & vec;
	double z = zAxis & vec;

	if (fabs(x) < ZERO || fabs(x - width) < ZERO)
	{
		CVector bx = yAxis;
		CVector by = zAxis;
		if (fabs(x - width) < ZERO) bx = -bx;
		if (m.texture->getBump())
		{
			double bp[2];
			m.texture->getBumpPixel(vec & bx, vec & by, bp);
			nv = bx * by;
			double l = nv.getLength();
			nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
		}
		
		if (m.texture->getTexture()) return m.texture->getPixel(vec & bx, vec & by);
		else return m.color;
	}

	if (fabs(y) < ZERO || fabs(y - height) < ZERO)
	{
		CVector bx = zAxis;
		CVector by = xAxis;
		if (fabs(y - height) < ZERO) bx = -bx;
		if (m.texture->getBump())
		{
			double bp[2];
			m.texture->getBumpPixel(vec & bx, vec & by, bp);
			nv = bx * by;
			double l = nv.getLength();
			nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
		}
		
		if (m.texture->getTexture()) return m.texture->getPixel(vec & bx, vec & by);
		else return m.color;
	}

	if (fabs(z) < ZERO || fabs(z - length) < ZERO)
	{
		CVector bx = xAxis;
		CVector by = yAxis;
		if (fabs(z - length) < ZERO) bx = -bx;
		if (m.texture->getBump())
		{
			double bp[2];
			m.texture->getBumpPixel(vec & bx, vec & by, bp);
			nv = bx * by;
			double l = nv.getLength();
			nv = nv + bp[0] / l * (nv * by) + bp[1] / l * (bx * nv);
		}
		
		if (m.texture->getTexture()) return m.texture->getPixel(vec & bx, vec & by);
		else return m.color;
	}

	return RGB(0, 0, 0);
}

CCSGModel *CCube::alterView(const CUserView &view)
{
	CCube *cube = new CCube(*this);

	CVertex vx = center + xAxis;
	CVertex vy = center + yAxis;
	CVertex vz = center + zAxis;

	CMatrix rm = CGeometry::getAlterViewMatrix(view);
	CGeometry::executeTransform(cube->center, rm);
	CGeometry::executeTransform(vx, rm);
	CGeometry::executeTransform(vy, rm);
	CGeometry::executeTransform(vz, rm);

	cube->xAxis = vx - cube->center;
	cube->yAxis = vy - cube->center;
	cube->zAxis = vz - cube->center;

	cube->updateParameters();

	return cube;
}

int CCube::getType()
{
	return TYPE_CUBE;
}

void CCube::setLength(double l)
{
	length = l;
	updateParameters();
}

void CCube::setHeight(double h)
{
	height = h;
	updateParameters();
}

void CCube::setWidth(double w)
{
	width = w;
	updateParameters();
}

double CCube::getLength() const
{
	return length;
}

double CCube::getHeight() const
{
	return height;
}

double CCube::getWidth() const
{
	return width;
}

CCSGModel *CCube::copy()
{
	return new CCube(*this);
}

void CCube::write(CArchive &ar)
{
	int type = TYPE_CUBE;
	ar.Write(&type, sizeof(int));

	center.write(ar);
	xAxis.write(ar);
	yAxis.write(ar);
	zAxis.write(ar);
	ar.Write(&length, sizeof(double));
	ar.Write(&width, sizeof(double));
	ar.Write(&height, sizeof(double));
}

void CCube::read(CArchive &ar)
{
	center.read(ar);
	xAxis.read(ar);
	yAxis.read(ar);
	zAxis.read(ar);
	ar.Read(&length, sizeof(double));
	ar.Read(&width, sizeof(double));
	ar.Read(&height, sizeof(double));

	updateParameters();
}