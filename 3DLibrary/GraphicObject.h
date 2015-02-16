// GraphicObject.h: interface for the CGraphicObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GRAPHICOBJECT_H__INCLUDED_)
#define GRAPHICOBJECT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserView.h"
#include "Matrix.h"

class CGraphicObject  
{
public:
	CString name;

	CGraphicObject();
	virtual ~CGraphicObject();

	virtual int getType() = 0;
	virtual void draw(CDC *pDC, const CRect &viewRect, const CUserView &view, const CMatrix &rm, COLORREF color, bool eliminate = true) = 0;
	virtual void move(const CVector &d) = 0;
	virtual CGraphicObject *copy() = 0;
	virtual CVertex getCenter() = 0;
	virtual void read(CArchive &ar) = 0;
	virtual void write(CArchive &ar) = 0;

};

#endif // !defined(GRAPHICOBJECT_H__INCLUDED_)
