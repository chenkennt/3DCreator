#if !defined(AFX_EDITMATERIAL_H__96CC5C82_8F40_44BC_8891_DA30C75FBCB0__INCLUDED_)
#define AFX_EDITMATERIAL_H__96CC5C82_8F40_44BC_8891_DA30C75FBCB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMaterial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditMaterial dialog

class CEditMaterial : public CDialog
{
// Construction
public:
	bool load;
	bool loadbump;
	HBITMAP bitmap;
	HBITMAP bumpBitmap;
	CEditMaterial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditMaterial)
	enum { IDD = IDD_EDIT_MATERIAL };
	CStatic	m_sbscale;
	CEdit	m_bscale;
	CStatic	m_bumpmap;
	CButton	m_loadbump;
	CButton	m_texture;
	CStatic	m_sscale;
	CStatic	m_sbr;
	CEdit	m_scale;
	CStatic	m_map;
	CButton	m_loadmap;
	CButton	m_bump;
	CEdit	m_br;
	double	m_ka;
	double	m_kd;
	double	m_kr;
	double	m_ks;
	double	m_kt;
	int		m_n;
	double	m_rr;
	double	m_vbr;
	double	m_vscale;
	BOOL	m_vbump;
	BOOL	m_vtex;
	double	m_vbscale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditMaterial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditMaterial)
	virtual BOOL OnInitDialog();
	afx_msg void OnTexture();
	afx_msg void OnLoadmap();
	afx_msg void OnBump();
	afx_msg void OnDestroy();
	afx_msg void OnLoadbump();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool loadMap();
	bool loadBump();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMATERIAL_H__96CC5C82_8F40_44BC_8891_DA30C75FBCB0__INCLUDED_)
