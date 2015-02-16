// Light.h: interface for the CLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LIGHT_H__INCLUDED_)
#define LIGHT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vertex.h"
#include "GraphicObject.h"
#include "SurfaceModel.h"

class CLight : public CGraphicObject
{
public:
	friend class CScene;

	CLight();
	CLight(CVertex pos);
	CLight(CVertex pos, double i, COLORREF c, bool cs);
	virtual ~CLight();

	CVertex getPosition();
	void setPosition(const CVertex &pos);
	double getIntension() const;
	void setIntension(double i);
	COLORREF getColor() const;
	void setColor(COLORREF c);
	bool getCastShadow();
	void setCastShadow(bool cs);
	int getType();
	void draw(CDC *pDC, const CRect &viewRect, const CUserView &view, const CMatrix &rm, COLORREF color, bool eliminate = true);
	void move(const CVector &d);
	CGraphicObject *copy();
	bool calcSelected(const CPoint &pt, const CRect &viewRect, const CUserView &view, bool eliminate);
	CVertex getCenter();
	void read(CArchive &ar);
	void write(CArchive &ar);

private:
	CSurfaceModel *createSurfaceModel(const CUserView &view);
	double intension;
	CVertex position;
	bool castShadow;
	COLORREF color;

};

#endif // !defined(LIGHT_H__INCLUDED_)
