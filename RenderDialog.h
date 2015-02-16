#if !defined(AFX_RENDERDIALOG_H__534B6E07_933E_426B_A2BD_B372E4605719__INCLUDED_)
#define AFX_RENDERDIALOG_H__534B6E07_933E_426B_A2BD_B372E4605719__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenderDialog dialog

class CRenderDialog : public CDialog
{
// Construction
public:
	CString name[4];
	int active;

	CRenderDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRenderDialog)
	enum { IDD = IDD_RENDER };
	CComboBox	m_view;
	int		m_height;
	int		m_width;
	int		m_antialias;
	int		m_vview;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenderDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenderDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERDIALOG_H__534B6E07_933E_426B_A2BD_B372E4605719__INCLUDED_)
