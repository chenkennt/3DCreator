// 3DCreatorView.h : interface of the CMy3DCreatorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DCREATORVIEW_H__7FE41CA7_83DF_42EA_AD73_1189065A4831__INCLUDED_)
#define AFX_3DCREATORVIEW_H__7FE41CA7_83DF_42EA_AD73_1189065A4831__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DLibrary\UserView.h"
#include "3DCreatorDoc.h"

#define DRAW_ALLVIEW 0
#define DRAW_ACTIVEVIEW 1
#define DRAW_BORDER 2
#define VIEW_GAP 2
#define BKCOLOR RGB(144, 144, 144)
#define GRIDCOLOR RGB(128, 128, 128)
#define AXISCOLOR RGB(64, 64, 64)
#define SELECTEDCOLOR RGB(255, 255, 255)
#define TEXTCOLOR RGB(224, 224, 224)
#define GAPCOLOR RGB(0, 0, 0)
#define ACTIVECOLOR RGB(255, 255, 128)

const COLORREF Colors[32] = {
	RGB(255,128,128),
	RGB(255,255,128),
	RGB(128,255,128),
	RGB(  0,255,128),
	RGB(128,255,255),
	RGB(  0,128,255),
	RGB(255,128,192),
	RGB(255,128,255),
	RGB(255,  0,  0),
	RGB(255,255,  0),
	RGB(128,255,  0),
	RGB(  0,255, 64),
	RGB(  0,255,255),
	RGB(  0,128,192),
	RGB(128,128,192),
	RGB(255,  0,255),
	RGB(128, 64, 64),
	RGB(255,128, 64),
	RGB(  0,255,  0),
	RGB(  0,128,128),
	RGB(  0, 64,128),
	RGB(128,128,255),
	RGB(128,  0, 64),
	RGB(255,  0,128),
	RGB(128,  0,  0),
	RGB(255,128,  0),
	RGB(  0,128,  0),
	RGB(  0,128, 64),
	RGB(  0,  0,255),
	RGB(  0,  0,160),
	RGB(128,  0,128),
	RGB(128,  0,255)};

class CMy3DCreatorView : public CView
{
protected: // create from serialization only
	CMy3DCreatorView();
	DECLARE_DYNCREATE(CMy3DCreatorView)

// Attributes
public:
	CMy3DCreatorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy3DCreatorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	CUserView view[4];
	CString viewName[4];
	int activeView;
	bool zoom;
	bool eliminate[4];
	bool smooth[4];
	
	void redrawScreen(CDC *pDC, int drawmethod);

	virtual ~CMy3DCreatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMy3DCreatorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEliminate();
	afx_msg void OnZoom();
	afx_msg void OnTop();
	afx_msg void OnBack();
	afx_msg void OnBottom();
	afx_msg void OnLeft();
	afx_msg void OnFront();
	afx_msg void OnRight();
	afx_msg void OnPerspective();
	afx_msg void OnSmooth();
	afx_msg void OnWireframe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CPoint oldpos;
	CVertex oldver;

};

#ifndef _DEBUG  // debug version in 3DCreatorView.cpp
inline CMy3DCreatorDoc* CMy3DCreatorView::GetDocument()
   { return (CMy3DCreatorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DCREATORVIEW_H__7FE41CA7_83DF_42EA_AD73_1189065A4831__INCLUDED_)
