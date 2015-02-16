#if !defined(AFX_RENDERRESULT_H__AD306C55_7C2F_431F_9161_8F2CE13C39F7__INCLUDED_)
#define AFX_RENDERRESULT_H__AD306C55_7C2F_431F_9161_8F2CE13C39F7__INCLUDED_

#include "3DLibrary\Scene.h"
#include "3DLibrary\UserView.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenderResult dialog

class CRenderResult : public CDialog
{
// Construction
public:
	CScene *scene;
	CRect viewRect;
	CUserView view;
	int antialias;
	bool rendered;
	CBitmap bmp;

	CRenderResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRenderResult)
	enum { IDD = IDD_RENDER_RESULT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenderResult)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRenderSave();
	afx_msg void OnRenderClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL saveBitmap(CBitmap &bmp, const CString &filename);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERRESULT_H__AD306C55_7C2F_431F_9161_8F2CE13C39F7__INCLUDED_)
