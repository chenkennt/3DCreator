// ScenePreference.cpp : implementation file
//

#include "stdafx.h"
#include "3DCreator.h"
#include "ScenePreference.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScenePreference dialog


CScenePreference::CScenePreference(CWnd* pParent /*=NULL*/)
	: CDialog(CScenePreference::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScenePreference)
	m_aintension = 0.0;
	m_vbkmap = FALSE;
	m_vscale = 0.0;
	//}}AFX_DATA_INIT
}


void CScenePreference::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScenePreference)
	DDX_Control(pDX, IDC_STATIC_SCALE, m_sscale);
	DDX_Control(pDX, IDC_SCALE, m_scale);
	DDX_Control(pDX, IDC_BACKBITMAP, m_back);
	DDX_Control(pDX, IDC_LOADBACK, m_loadback);
	DDX_Control(pDX, IDC_BACKMAP, m_bkmap);
	DDX_Control(pDX, IDC_BKCOLOR, m_bkcolor);
	DDX_Control(pDX, IDC_ACOLOR, m_acolor);
	DDX_Text(pDX, IDC_AINTENSION, m_aintension);
	DDV_MinMaxDouble(pDX, m_aintension, 0., 1.);
	DDX_Check(pDX, IDC_BACKMAP, m_vbkmap);
	DDX_Text(pDX, IDC_SCALE, m_vscale);
	DDV_MinMaxDouble(pDX, m_vscale, 0., 10.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScenePreference, CDialog)
	//{{AFX_MSG_MAP(CScenePreference)
	ON_BN_CLICKED(IDC_ACOLOR, OnAcolor)
	ON_BN_CLICKED(IDC_BKCOLOR, OnBkcolor)
	ON_BN_CLICKED(IDC_LOADBACK, OnLoadback)
	ON_BN_CLICKED(IDC_BACKMAP, OnBackmap)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenePreference message handlers

void CScenePreference::OnAcolor() 
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_acolor.color = dlg.GetColor();
		m_acolor.RedrawWindow();
	}
}

void CScenePreference::OnBkcolor() 
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_bkcolor.color = dlg.GetColor();
		m_bkcolor.RedrawWindow();
	}
}

BOOL CScenePreference::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HBITMAP bmp = m_back.GetBitmap();
	
	if (m_vbkmap)
	{
		load = true;
		m_back.SetBitmap(bitmap);

		CBitmap *bmp = CBitmap::FromHandle(bitmap);
		BITMAP bmpinfo;
		bmp->GetBitmap(&bmpinfo);
		double w = 200;
		double h = 100;
		double nw = bmpinfo.bmWidth;
		double nh = bmpinfo.bmHeight;
		if (nw > w)
		{
			nh = nh * w / nw;
			nw = w;
		}
		if (nh > h)
		{
			nw = nw * h / nh;
			nh = h;
		}
		m_back.SetWindowPos(NULL, 7, 120, (int)nw, (int)nh, 0);
		m_back.RedrawWindow();
	}
	else
	{
		m_loadback.EnableWindow(FALSE);
		m_scale.EnableWindow(FALSE);
		m_sscale.EnableWindow(FALSE);
		load = false;
	}

	bitmap = bmp;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CScenePreference::loadMap()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Î»Í¼ÎÄ¼þ(*.bmp)|*.bmp||");
	if (dlg.DoModal() == IDOK)
	{
		HANDLE handle = LoadImage(0, dlg.GetFileName(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (handle == NULL) return false;
		HBITMAP hBitmap = (HBITMAP)handle;
		CBitmap *bmp = CBitmap::FromHandle(hBitmap);
		HBITMAP oldbmp = m_back.GetBitmap();
		if (oldbmp != bitmap) DeleteObject(oldbmp);
		m_back.SetBitmap(hBitmap);

		BITMAP bmpinfo;
		bmp->GetBitmap(&bmpinfo);
		double w = 200;
		double h = 100;
		double nw = bmpinfo.bmWidth;
		double nh = bmpinfo.bmHeight;
		if (nw > w)
		{
			nh = nh * w / nw;
			nw = w;
		}
		if (nh > h)
		{
			nw = nw * h / nh;
			nh = h;
		}
		m_back.SetWindowPos(NULL, 7, 120, (int)nw, (int)nh, 0);
		m_back.RedrawWindow();
		load = true;
		return true;
	}
	else return false;
}

void CScenePreference::OnLoadback() 
{
	loadMap();
}

void CScenePreference::OnBackmap() 
{
	if (m_bkmap.GetCheck() && !load && !loadMap()) m_bkmap.SetCheck(0);

	if (m_bkmap.GetCheck())
	{
		m_loadback.EnableWindow(TRUE);
		m_scale.EnableWindow(TRUE);
		m_sscale.EnableWindow(TRUE);
	}
	else
	{
		m_loadback.EnableWindow(FALSE);
		m_scale.EnableWindow(FALSE);
		m_sscale.EnableWindow(FALSE);
	}
}

void CScenePreference::OnDestroy() 
{
	CDialog::OnDestroy();
	
	HBITMAP bmp = m_back.GetBitmap();
	m_back.SetBitmap(bitmap);
	bitmap = bmp;
}
