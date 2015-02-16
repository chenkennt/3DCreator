// RenderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "3DCreator.h"
#include "RenderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderDialog dialog


CRenderDialog::CRenderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRenderDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenderDialog)
	m_height = 0;
	m_width = 0;
	m_antialias = -1;
	m_vview = -1;
	//}}AFX_DATA_INIT
}


void CRenderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenderDialog)
	DDX_Control(pDX, IDC_VIEW, m_view);
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDV_MinMaxInt(pDX, m_height, 0, 10000);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDV_MinMaxInt(pDX, m_width, 0, 10000);
	DDX_CBIndex(pDX, IDC_ANTIALIAS, m_antialias);
	DDX_CBIndex(pDX, IDC_VIEW, m_vview);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRenderDialog, CDialog)
	//{{AFX_MSG_MAP(CRenderDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderDialog message handlers

BOOL CRenderDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_view.AddString(name[0]);
	m_view.AddString(name[1]);
	m_view.AddString(name[2]);
	m_view.AddString(name[3]);
	m_view.SetCurSel(active);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
