// Scene.h: interface for the CScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SCENE_H__INCLUDED_)
#define SCENE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Geometry.h"
#include "Light.h"

struct GeometryLink
{
	CGeometry *geometry;
	GeometryLink *next;
};

struct LightLink
{
	CLight *light;
	LightLink *next;
};

struct CSGLink
{
	CCSGModel *csg;
	CMaterial *material;
	CSGLink *next;
};

class CScene  
{
public:
	friend class CUserView;

	CLight ambientLight;
	CMaterial background;

	CScene();
	virtual ~CScene();

	void add(CGeometry *geometry, const CString &name);
	void add(CLight *geometry, const CString &name);
	void drawWireframe(CDC *pDC, const CRect &viewRect, const CUserView &view, bool eliminate = true);
	void render(CDC *pDC, const CRect &viewRect, const CUserView &view, int antialias);
	CGraphicObject *getSelection(const CPoint &pt, const CUserView &view, const CRect &viewRect, bool eliminate, bool smooth);
	void clear();
	void remove(CGraphicObject *object, bool erase = true);
	void intersectObject(CGeometry *g1, CGeometry *g2);
	void unionObject(CGeometry *g1, CGeometry *g2);
	void differenceObject(CGeometry *g1, CGeometry *g2);
	void read(CArchive &ar);
	void write(CArchive &ar);
	void simpleRender(CDC *pDC, const CRect &viewRect, const CUserView &view, COLORREF bk);

private:
	GeometryLink *firstGeometry;
	LightLink *firstLight;

	static COLORREF getSurfaceColor(const CVertex &origin, const CVector &light, CSGLink *csglist, LightLink *lightlist, const CLight &al, double &dis, int depth);
	static COLORREF getSimpleSurfaceColor(const CVertex &origin, const CVector &light, CSGLink *csglist, LightLink *lightlist, const CLight &al, double &dis);
	static COLORREF luminance(double k, COLORREF c);
	static COLORREF addColor(COLORREF a, COLORREF c);
	static COLORREF lightColor(COLORREF c, COLORREF l);
	static CVector calcReflectLight(const CVector &l, const CVector &v);
	static CVector calcRefractLight(const CVector &l, const CVector &v, double n1, double n2);

};

#endif // !defined(SCENE_H__INCLUDED_)
