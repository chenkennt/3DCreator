#if !defined(AFX_SCENEPREFERENCE_H__EF712D84_C30B_4EF2_BF80_9DAFFD320A79__INCLUDED_)
#define AFX_SCENEPREFERENCE_H__EF712D84_C30B_4EF2_BF80_9DAFFD320A79__INCLUDED_

#include "ColorButton.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScenePreference.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScenePreference dialog

class CScenePreference : public CDialog
{
// Construction
public:
	bool load;
	HBITMAP bitmap;

	CScenePreference(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScenePreference)
	enum { IDD = IDD_SCENE_PREFERENCE };
	CStatic	m_sscale;
	CEdit	m_scale;
	CStatic	m_back;
	CButton	m_loadback;
	CButton	m_bkmap;
	CColorButton	m_bkcolor;
	CColorButton	m_acolor;
	double	m_aintension;
	BOOL	m_vbkmap;
	double	m_vscale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenePreference)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScenePreference)
	afx_msg void OnAcolor();
	afx_msg void OnBkcolor();
	virtual BOOL OnInitDialog();
	afx_msg void OnLoadback();
	afx_msg void OnBackmap();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool loadMap();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEPREFERENCE_H__EF712D84_C30B_4EF2_BF80_9DAFFD320A79__INCLUDED_)
