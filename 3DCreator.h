// 3DCreator.h : main header file for the 3DCREATOR application
//

#if !defined(AFX_3DCREATOR_H__3043D03A_7919_4E1F_8931_9386EE3A8B86__INCLUDED_)
#define AFX_3DCREATOR_H__3043D03A_7919_4E1F_8931_9386EE3A8B86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorApp:
// See 3DCreator.cpp for the implementation of this class
//

class CMy3DCreatorApp : public CWinApp
{
public:
	CMy3DCreatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy3DCreatorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMy3DCreatorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DCREATOR_H__3043D03A_7919_4E1F_8931_9386EE3A8B86__INCLUDED_)
