#if !defined(AFX_LOFTLENGTHDIALOG_H__C524F466_2591_47B8_8BF0_09EAB80B03BC__INCLUDED_)
#define AFX_LOFTLENGTHDIALOG_H__C524F466_2591_47B8_8BF0_09EAB80B03BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoftLengthDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoftLengthDialog dialog

class CLoftLengthDialog : public CDialog
{
// Construction
public:
	CLoftLengthDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoftLengthDialog)
	enum { IDD = IDD_LOFTLENGTH };
	double	m_length;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoftLengthDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoftLengthDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOFTLENGTHDIALOG_H__C524F466_2591_47B8_8BF0_09EAB80B03BC__INCLUDED_)
