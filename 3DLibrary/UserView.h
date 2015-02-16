// UserView.h: interface for the CUserView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(USERVIEW_H__INCLUDED_)
#define USERVIEW_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vertex.h"

class CScene;

class CUserView  
{
public:
	CUserView();
	CUserView(int type, const CVertex &cop, const CVertex &center, const CVector &vup, double d, double w, double h);
	virtual ~CUserView();

	void move(double dx, double dy);
	void verticalRotate(double theta);
	void horizontalRotate(double theta);
	void zoom(double d);
	void scale(double d);

	void getFaceParameters(double par[5][4]) const;
	void drawHorizontalFace(CDC *pDC, const CRect &viewRect, COLORREF pc, COLORREF sc);

	int getType() const;
	CVertex getCOP() const;
	CVector getVUP() const;
	CVector getVPN() const;
	double getDistance() const;
	double getWidth() const;
	double getHeight() const;
	void setWidth(double w);
	void setHeight(double h);
	CVector getHoriX();
	CVector getHoriY();
	CVertex getVertexOnHF(const CPoint &pt, const CRect &viewRect);
	CVertex getVertexOnFace(const CPoint &pt, const CRect &viewRect, const CVertex &v, const CVector &n); 
	void read(CArchive &ar);
	void write(CArchive &ar);
	void align(const CScene &scene);

	static CUserView createStockView(int viewtype);

private:
	int type;
	CVertex cop;
	CVertex center;
	CVector vup;
	double distance;
	double width;
	double height;
	CVector horix;
	CVector horiy;

};

#endif // !defined(USERVIEW_H__INCLUDED_)
