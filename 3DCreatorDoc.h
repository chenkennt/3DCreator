// 3DCreatorDoc.h : interface of the CMy3DCreatorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DCREATORDOC_H__8F9167FB_4454_47E4_855B_D0CB928BB443__INCLUDED_)
#define AFX_3DCREATORDOC_H__8F9167FB_4454_47E4_855B_D0CB928BB443__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DLibrary\Scene.h"

#define STATUS_SELECTING 0
#define STATUS_NEW_CUBE 1
#define STATUS_NEW_SPHERE 2
#define STATUS_NEW_CYLINDER 3
#define STATUS_NEW_CONE 4
#define STATUS_UNION 5
#define STATUS_INTERSECT 6
#define STATUS_DIFFERENCE 7
#define STATUS_NEW_LIGHT 8
#define STATUS_MOVE 9
#define STATUS_ROTATE 10
#define STATUS_SCALE 11
#define STATUS_MOVEVIEW 12
#define STATUS_ROTATEVIEW 13
#define STATUS_SCALEVIEW 14
#define STATUS_ZOOMVIEW 15

class CMy3DCreatorDoc : public CDocument
{
protected: // create from serialization only
	CMy3DCreatorDoc();
	DECLARE_DYNCREATE(CMy3DCreatorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy3DCreatorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	CScene scene;
	int status;
	int createstep;
	CGraphicObject *selection;
	CGraphicObject *clipboard;
	CCSGModel *temp;
	CVertex ov;
	int currname;
	int currlight;

	virtual ~CMy3DCreatorDoc();

	void switchStatus(int status);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMy3DCreatorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DCREATORDOC_H__8F9167FB_4454_47E4_855B_D0CB928BB443__INCLUDED_)
