// Surface.cpp: implementation of the CSurface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surface.h"
#include "Geometry.h"
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

CSurface::CSurface()
{
	firstEdge = NULL;
}

CSurface::CSurface(const CSurface &s)
{
	firstEdge = NULL;

	CEdge *p = s.firstEdge;
	CEdge *np = NULL;
	while (p != NULL)
	{
		CEdge *e = new CEdge(*p);
		if (firstEdge == NULL) firstEdge = np = e;
		else
		{
			np->next = e;
			np = e;
		}
		p = p->next;
	}
	formula[0] = s.formula[0];
	formula[1] = s.formula[1];
	formula[2] = s.formula[2];
	formula[3] = s.formula[3];
}

CSurface::~CSurface()
{
	while (firstEdge != NULL)
	{
		CEdge *temp = firstEdge;
		firstEdge = firstEdge->next;
		delete temp;
	}
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

CSurface &CSurface::operator<<(CEdge *s)
{
	s->next = firstEdge;
	firstEdge = s;

	return *this;
}

void CSurface::setFormula(double a, double b, double c, double d)
{
	formula[0] = a;
	formula[1] = b;
	formula[2] = c;
	formula[3] = d;
}

void CSurface::combine(CSurface &s)
{
	if (firstEdge == NULL) firstEdge = s.firstEdge;
	else
	{
		CEdge *t = firstEdge;
		while (t->next != NULL) t = t->next;
		t->next = s.firstEdge;
	}
	s.firstEdge = NULL;
}

CSurface *CSurface::cut(const CSurface &cs)
{
	SurfaceTreeNode *tree1 = cs.createSurfaceTree();
	SurfaceTreeNode *tree2 = createSurfaceTree();
	CSurface *cs1 = new CSurface(cs);

	CSurface *out = recursiveCut(tree1, this);
	CSurface *out1 = recursiveCut(tree2, cs1);
	delete out1;
	CSurface *cs2 = new CSurface(*cs1);
	CEdge *t = cs2->firstEdge;
	while (t != NULL)
	{
		t->formula[0] = -t->formula[0];
		t->formula[1] = -t->formula[1];
		t->formula[2] = -t->formula[2];
		t->formula[3] = -t->formula[3];
		t = t->next;
	}
	combine(*cs1);
	delete cs1;
	out->combine(*cs2);
	delete cs2;
	recursiveDeleteTree(tree1);
	recursiveDeleteTree(tree2);

	out->setFormula(formula[0], formula[1], formula[2], formula[3]);
	return out;
}

CSurface *CSurface::cross(const CSurface &cs)
{
	CSurface *result = new CSurface;
	VertexLink *vlist = NULL;

	CEdge *e = firstEdge;
	while (e != NULL)
	{
		int ns, os;
		double z = e->sv.x * cs.formula[0] + e->sv.y * cs.formula[1] + e->sv.z * cs.formula[2] + cs.formula[3];
		if (z > 0 || fabs(z) < ZERO) os = 1;
		else os = -1;
		z = e->ev.x * cs.formula[0] + e->ev.y * cs.formula[1] + e->ev.z * cs.formula[2] + cs.formula[3];
		if (z > 0 || fabs(z) < ZERO) ns = 1;
		else ns = -1;

		if (os * ns < 0)
		{
			CVector k = e->ev - e->sv;
			double t = -(e->sv.x * cs.formula[0] + e->sv.y * cs.formula[1] + e->sv.z * cs.formula[2] + cs.formula[3]) / (k.x * cs.formula[0] + k.y * cs.formula[1] + k.z * cs.formula[2]);
			VertexLink *cv = new VertexLink;
			cv->vertex = new CVertex(e->sv + t * k);
			cv->next = vlist;
			vlist = cv;
		}
		e = e->next;
	}

	if (vlist == NULL) return result;
	
	VertexLink *v = vlist;
	while (v->next != NULL)
	{
		VertexLink *t = v->next;
		while (t != NULL)
		{
			bool ex = false;
			if (v->vertex->x > t->vertex->x) ex = true;
			else if (fabs(v->vertex->x - t->vertex->x) < ZERO && v->vertex->y > t->vertex->y) ex = true;
			else if (fabs(v->vertex->x - t->vertex->x) < ZERO && fabs(v->vertex->y - t->vertex->y) < ZERO && v->vertex->z > t->vertex->z) ex = true;
			if (ex)
			{
				CVertex *vp = v->vertex;
				v->vertex = t->vertex;
				t->vertex = vp;
			}
			t = t->next;
		}
		v = v->next;
	}
	v = vlist;
	while (v != NULL && v->next != NULL)
	{
		*result<<new CEdge(*v->vertex, *v->next->vertex, formula);
		v = v->next->next;
	}
	result->setFormula(cs.formula[0], cs.formula[1], cs.formula[2],cs.formula[3]);
	v = vlist;
	while (v != NULL)
	{
		VertexLink *temp = v;
		v = v->next;
		delete temp->vertex;
		delete temp;
	}

	return result;
}

SurfaceTreeNode *CSurface::createSurfaceTree() const
{
	SurfaceTreeNode *root = new SurfaceTreeNode;
	root->edge = NULL;
	root->lChild = root->rChild = NULL;
	CEdge *p = firstEdge;
	while (p != NULL)
	{
		CEdge *e = new CEdge(*p);
		e->next = root->edge;
		root->edge = e;
		p = p->next;
	}
	recursiveCreateTree(root);
	return root;
}

CSurface *CSurface::recursiveCut(SurfaceTreeNode *tree, CSurface *s)
{
	if (tree->lChild == NULL && tree->rChild == NULL)
	{
		CSurface *out = new CSurface;
		if (tree->edge == NULL)
		{
			out->firstEdge = s->firstEdge;
			s->firstEdge = NULL;
			return out;
		}

		out->setFormula(s->formula[0], s->formula[1], s->formula[2], s->formula[3]);
		CEdge *e = tree->edge;
		while (e != NULL)
		{
			CEdge *t = s->firstEdge;
			s->firstEdge = NULL;
			CEdge *temp;
			while (t != NULL)
			{
				switch (t->getLocation(e->formula))
				{
				case 1:
					temp = t;
					t = t->next;
					*s<<temp;
					break;
				case -1:
					temp = t;
					t = t->next;
					*out<<temp;
					break;
				case 0:
					CEdge *outs = t->cut(e->formula);
					temp = t;
					t = t->next;
					*s<<temp;
					*out<<outs;
					break;
				}
			}
			e = e->next;
		}
		return out;
	}
	else
	{
		CSurface *out = new CSurface;
		CEdge *e = s->firstEdge;
		s->firstEdge = NULL;
		CEdge *temp;
		while (e != NULL)
		{
			switch (e->getLocation(tree->formula))
			{
			case 1:
				temp = e;
				e = e->next;
				*s<<temp;
				break;
			case -1:
				temp = e;
				e = e->next;
				*out<<temp;
				break;
			case 0:
				CEdge *outs = e->cut(tree->formula);
				temp = e;
				e = e->next;
				*s<<temp;
				*out<<outs;
				break;
			}
		}
		CSurface *out1 = recursiveCut(tree->lChild, s);
		CSurface *out2 = recursiveCut(tree->rChild, out);
		s->combine(*out);
		out1->combine(*out2);
		delete out;
		delete out2;
		return out1;
	}
}

void CSurface::recursiveCreateTree(SurfaceTreeNode *tree)
{
	CEdge *p = tree->edge;
	while (p != NULL)
	{
		CEdge *t = tree->edge;
		while (t != NULL)
		{
			if (t != p && t->getLocation(p->formula) <= 0)
			{
				tree->lChild = new SurfaceTreeNode;
				tree->rChild = new SurfaceTreeNode;
				tree->lChild->lChild = tree->lChild->rChild = tree->rChild->lChild = tree->rChild->rChild = NULL;
				tree->lChild->edge = tree->rChild->edge = NULL;
				tree->formula[0] = p->formula[0];
				tree->formula[1] = p->formula[1];
				tree->formula[2] = p->formula[2];
				tree->formula[3] = p->formula[3];
				CEdge *s = tree->edge;
				CEdge *temp;
				while (s != NULL)
				{
					if (s == p)
					{
						s = s->next;
						continue;
					}
					switch (s->getLocation(p->formula))
					{
					case 1:
						temp = s;
						s = s->next;
						temp->next = tree->lChild->edge;
						tree->lChild->edge = temp;
						break;
					case -1:
						temp = s;
						s = s->next;
						temp->next = tree->rChild->edge;
						tree->rChild->edge = temp;
						break;
					case 0:
						CEdge *ne = s->cut(p->formula);
						temp = s;
						s = s->next;
						temp->next = tree->lChild->edge;
						tree->lChild->edge = temp;
						ne->next = tree->rChild->edge;
						tree->rChild->edge = ne;
						break;
					}
				}
				delete p;
				tree->edge = NULL;
				recursiveCreateTree(tree->lChild);
				recursiveCreateTree(tree->rChild);
				return;
			}
			t = t->next;
		}
		p = p->next;
	}
}

void CSurface::recursiveDeleteTree(SurfaceTreeNode *tree)
{
	if (tree == NULL) return;
	recursiveDeleteTree(tree->lChild);
	recursiveDeleteTree(tree->rChild);
	while (tree->edge != NULL)
	{
		CEdge *temp = tree->edge;
		tree->edge = tree->edge->next;
		delete temp;
	}
	delete tree;
}

void CSurface::getSurfaceFormula(const CVertex &v1, const CVertex &v2, const CVertex &v3, const CVertex &v4, double formula[4])
{
	formula[0] =
		(v1.y - v2.y) * (v1.z + v2.z) +
		(v2.y - v3.y) * (v2.z + v3.z) +
		(v3.y - v1.y) * (v3.z + v1.z);
	formula[1] =
		(v1.z - v2.z) * (v1.x + v2.x) +
		(v2.z - v3.z) * (v2.x + v3.x) +
		(v3.z - v1.z) * (v3.x + v1.x);
	formula[2] =
		(v1.x - v2.x) * (v1.y + v2.y) +
		(v2.x - v3.x) * (v2.y + v3.y) +
		(v3.x - v1.x) * (v3.y + v1.y);
	formula[3] = -(formula[0] * v1.x + formula[1] * v1.y + formula[2] * v1.z);
	if (v4.x * formula[0] + v4.y * formula[1] + v4.z * formula[2] + formula[3] < 0)
	{
		formula[0] = -formula[0];
		formula[1] = -formula[1];
		formula[2] = -formula[2];
		formula[3] = -formula[3];
	}
}

void CSurface::rotateSurface(double formula[4], const CMatrix &rm)
{
	CVector nv(formula[0], formula[1], formula[2]);
	double t = -formula[3] / (nv.x * nv.x + nv.y * nv.y + nv.z * nv.z);
	CVertex v(t * nv.x, t * nv.y, t * nv.z);
	CVertex vt = v + nv;
	CGeometry::executeTransform(v, rm);
	CGeometry::executeTransform(vt, rm);
	nv = vt - v;
	formula[0] = nv.x;
	formula[1] = nv.y;
	formula[2] = nv.z;
	formula[3] = -(v.x * nv.x + v.y * nv.y + v.z * nv.z);
}