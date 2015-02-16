#if !defined(AFX_LOFTDIALOG_H__36D233CD_1E6C_43DF_97DB_348B1285A0DE__INCLUDED_)
#define AFX_LOFTDIALOG_H__36D233CD_1E6C_43DF_97DB_348B1285A0DE__INCLUDED_

#include "MyButton.h"
#include "3DLIBRARY\UserView.h"	// Added by ClassView
#include "3DLibrary\Shape.h"
#include "3DLibrary\CSGModel.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LoftDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoftDialog dialog

class CLoftDialog : public CDialog
{
// Construction
public:
	CCSGModel *loft;
	CLoftDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoftDialog)
	enum { IDD = IDD_LOFT };
	CMyButton	m_status;
	CMyButton	m_posy;
	CMyButton	m_posx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoftDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoftDialog)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLoftMoveview();
	afx_msg void OnLoftZoomview();
	afx_msg void OnLoftDrawshape();
	afx_msg void OnLoftEdit();
	afx_msg void OnLoftMove();
	afx_msg void OnLoftRotate();
	afx_msg void OnLoftScale();
	afx_msg void OnCircleloft();
	afx_msg void OnLineloft();
	afx_msg void OnExitloft();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CUserView view;
	CRect viewRect;
	CShape shape;
	int status;
	int createstep;
	CVertex v1;
	CVertex v2;
	CVertex v3;
	CVertex v4;
	CVertex v5;
	CPoint oldpos;
	ShapeElement *select;
	ShapeLink *slink;
	bool close;
	bool scenter;

	void redrawScreen(CDC *pDC);
	void switchStatus(int s);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOFTDIALOG_H__36D233CD_1E6C_43DF_97DB_348B1285A0DE__INCLUDED_)
