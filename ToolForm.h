#if !defined(AFX_TOOLFORM_H__8F31C2B3_0AFA_4076_9E30_B4DD65A8624A__INCLUDED_)
#define AFX_TOOLFORM_H__8F31C2B3_0AFA_4076_9E30_B4DD65A8624A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorButton.h"
#include "3DCreatorDoc.h"
#include "MyButton.h"

// ToolForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CToolForm : public CFormView
{
protected:
	CToolForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CToolForm)

// Form Data
public:
	//{{AFX_DATA(CToolForm)
	enum { IDD = IDD_TOOLFORM };
	CButton	m_apply;
	CTabCtrl	m_tab;
	CMyButton	m_selectbool;
	CMyButton	m_intersect;
	CMyButton	m_difference;
	CMyButton	m_union;
	CMyButton	m_newsphere;
	CMyButton	m_newlight;
	CMyButton	m_newcylinder;
	CMyButton	m_newcone;
	CMyButton	m_newcube;
	CColorButton	m_color;
	CStatic	m_sproperty3;
	CStatic	m_sproperty2;
	CStatic	m_sproperty1;
	CStatic	m_sposz;
	CStatic	m_sposy;
	CStatic	m_sposx;
	CStatic	m_sname;
	CStatic	m_scolor;
	CComboBox	m_selectcreate;
	CEdit	m_property3;
	CEdit	m_property2;
	CEdit	m_property1;
	CEdit	m_posz;
	CEdit	m_posy;
	CEdit	m_posx;
	CButton	m_property;
	CButton	m_position;
	CEdit	m_name;
	CButton	m_material;
	CComboBox	m_castshadow;
	CButton	m_basic;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CMy3DCreatorDoc *GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void refreshButtons();
	void updateApply();

protected:
	virtual ~CToolForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CToolForm)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeSelectCreate();
	afx_msg void OnNewCube();
	afx_msg void OnNewCone();
	afx_msg void OnNewCylinder();
	afx_msg void OnNewSphere();
	afx_msg void OnIntersect();
	afx_msg void OnDifference();
	afx_msg void OnUnion();
	afx_msg void OnSelectBoolObject();
	afx_msg void OnNewLight();
	afx_msg void OnColor();
	afx_msg void OnChangeName();
	afx_msg void OnChangePosX();
	afx_msg void OnChangePosY();
	afx_msg void OnChangePosZ();
	afx_msg void OnChangeProperty1();
	afx_msg void OnChangeProperty2();
	afx_msg void OnChangeProperty3();
	afx_msg void OnSelchangeCastshadow();
	afx_msg void OnApply();
	afx_msg void OnMaterial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLFORM_H__8F31C2B3_0AFA_4076_9E30_B4DD65A8624A__INCLUDED_)
