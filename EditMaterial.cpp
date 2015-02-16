// EditMaterial.cpp : implementation file
//

#include "stdafx.h"
#include "3DCreator.h"
#include "EditMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditMaterial dialog


CEditMaterial::CEditMaterial(CWnd* pParent /*=NULL*/)
	: CDialog(CEditMaterial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditMaterial)
	m_ka = 0.0;
	m_kd = 0.0;
	m_kr = 0.0;
	m_ks = 0.0;
	m_kt = 0.0;
	m_n = 0;
	m_rr = 0.0;
	m_vbr = 0.0;
	m_vscale = 0.0;
	m_vbump = FALSE;
	m_vtex = FALSE;
	m_vbscale = 0.0;
	//}}AFX_DATA_INIT
}


void CEditMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditMaterial)
	DDX_Control(pDX, IDC_SBSCALE, m_sbscale);
	DDX_Control(pDX, IDC_BSCALE, m_bscale);
	DDX_Control(pDX, IDC_BUMPMAP, m_bumpmap);
	DDX_Control(pDX, IDC_LOADBUMP, m_loadbump);
	DDX_Control(pDX, IDC_TEXTURE, m_texture);
	DDX_Control(pDX, IDC_STATIC_SCALE, m_sscale);
	DDX_Control(pDX, IDC_STATIC_BR, m_sbr);
	DDX_Control(pDX, IDC_SCALE, m_scale);
	DDX_Control(pDX, IDC_MAP, m_map);
	DDX_Control(pDX, IDC_LOADMAP, m_loadmap);
	DDX_Control(pDX, IDC_BUMP, m_bump);
	DDX_Control(pDX, IDC_BR, m_br);
	DDX_Text(pDX, IDC_KA, m_ka);
	DDV_MinMaxDouble(pDX, m_ka, 0., 1.);
	DDX_Text(pDX, IDC_KD, m_kd);
	DDV_MinMaxDouble(pDX, m_kd, 0., 1.);
	DDX_Text(pDX, IDC_KR, m_kr);
	DDV_MinMaxDouble(pDX, m_kr, 0., 1.);
	DDX_Text(pDX, IDC_KS, m_ks);
	DDV_MinMaxDouble(pDX, m_ks, 0., 1.);
	DDX_Text(pDX, IDC_KT, m_kt);
	DDV_MinMaxDouble(pDX, m_kt, 0., 1.);
	DDX_Text(pDX, IDC_N, m_n);
	DDV_MinMaxInt(pDX, m_n, 0, 200);
	DDX_Text(pDX, IDC_RR, m_rr);
	DDV_MinMaxDouble(pDX, m_rr, 0., 1000.);
	DDX_Text(pDX, IDC_BR, m_vbr);
	DDV_MinMaxDouble(pDX, m_vbr, 0., 1000.);
	DDX_Text(pDX, IDC_SCALE, m_vscale);
	DDV_MinMaxDouble(pDX, m_vscale, 0., 10.);
	DDX_Check(pDX, IDC_BUMP, m_vbump);
	DDX_Check(pDX, IDC_TEXTURE, m_vtex);
	DDX_Text(pDX, IDC_BSCALE, m_vbscale);
	DDV_MinMaxDouble(pDX, m_vbscale, 0., 10.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditMaterial, CDialog)
	//{{AFX_MSG_MAP(CEditMaterial)
	ON_BN_CLICKED(IDC_TEXTURE, OnTexture)
	ON_BN_CLICKED(IDC_LOADMAP, OnLoadmap)
	ON_BN_CLICKED(IDC_BUMP, OnBump)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LOADBUMP, OnLoadbump)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditMaterial message handlers

BOOL CEditMaterial::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HBITMAP bmp = m_map.GetBitmap();
	
	if (m_vtex)
	{
		load = true;
		m_map.SetBitmap(bitmap);

		CBitmap *bmp = CBitmap::FromHandle(bitmap);
		BITMAP bmpinfo;
		bmp->GetBitmap(&bmpinfo);
		double w = 160;
		double h = 120;
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
		m_map.SetWindowPos(NULL, 10, 212, (int)nw, (int)nh, 0);
		m_map.RedrawWindow();
	}
	else
	{
		m_loadmap.EnableWindow(FALSE);
		m_scale.EnableWindow(FALSE);
		m_sscale.EnableWindow(FALSE);
		load = false;
	}

	bitmap = bmp;

	bmp = m_bumpmap.GetBitmap();
	if (m_vbump)
	{
		loadbump = true;
		m_bumpmap.SetBitmap(bumpBitmap);

		CBitmap *bmp = CBitmap::FromHandle(bumpBitmap);
		BITMAP bmpinfo;
		bmp->GetBitmap(&bmpinfo);
		double w = 160;
		double h = 120;
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
		m_bumpmap.SetWindowPos(NULL, 177, 212, (int)nw, (int)nh, 0);
		m_bumpmap.RedrawWindow();
	}
	else
	{
		m_loadbump.EnableWindow(FALSE);
		m_br.EnableWindow(FALSE);
		m_sbr.EnableWindow(FALSE);
		m_sbscale.EnableWindow(FALSE);
		m_bscale.EnableWindow(FALSE);
		loadbump = false;
	}

	bumpBitmap = bmp;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditMaterial::OnTexture() 
{
	if (m_texture.GetCheck() && !load && !loadMap()) m_texture.SetCheck(0);

	if (m_texture.GetCheck())
	{
		m_loadmap.EnableWindow(TRUE);
		m_scale.EnableWindow(TRUE);
		m_sscale.EnableWindow(TRUE);
	}
	else
	{
		m_loadmap.EnableWindow(FALSE);
		m_scale.EnableWindow(FALSE);
		m_sscale.EnableWindow(FALSE);
	}
}

void CEditMaterial::OnLoadmap() 
{
	loadMap();
}

bool CEditMaterial::loadMap()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "位图文件(*.bmp)|*.bmp||");
	if (dlg.DoModal() == IDOK)
	{
		HANDLE handle = LoadImage(0, dlg.GetFileName(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (handle == NULL) return false;
		HBITMAP hBitmap = (HBITMAP)handle;
		CBitmap *bmp = CBitmap::FromHandle(hBitmap);
		HBITMAP oldbmp = m_map.GetBitmap();
		if (oldbmp != bitmap) DeleteObject(oldbmp);
		m_map.SetBitmap(hBitmap);

		BITMAP bmpinfo;
		bmp->GetBitmap(&bmpinfo);
		double w = 160;
		double h = 120;
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
		m_map.SetWindowPos(NULL, 10, 212, (int)nw, (int)nh, 0);
		m_map.RedrawWindow();
		load = true;
		return true;
	}
	else return false;
}

void CEditMaterial::OnBump() 
{
	if (m_bump.GetCheck() && !loadbump && !loadBump()) m_bump.SetCheck(0);

	if (m_bump.GetCheck())
	{
		m_loadbump.EnableWindow(TRUE);
		m_br.EnableWindow(TRUE);
		m_sbr.EnableWindow(TRUE);
		m_sbscale.EnableWindow(TRUE);
		m_bscale.EnableWindow(TRUE);
	}
	else
	{
		m_loadbump.EnableWindow(FALSE);
		m_br.EnableWindow(FALSE);
		m_sbr.EnableWindow(FALSE);
		m_sbscale.EnableWindow(FALSE);
		m_bscale.EnableWindow(FALSE);
	}
}

void CEditMaterial::OnDestroy() 
{
	CDialog::OnDestroy();
	
	HBITMAP bmp = m_map.GetBitmap();
	m_map.SetBitmap(bitmap);
	bitmap = bmp;
	bmp = m_bumpmap.GetBitmap();
	m_map.SetBitmap(bumpBitmap);
	bumpBitmap = bmp;
}

bool CEditMaterial::loadBump()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "位图文件(*.bmp)|*.bmp||");
	if (dlg.DoModal() == IDOK)
	{
		HANDLE handle = LoadImage(0, dlg.GetFileName(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (handle == NULL) return false;
		HBITMAP hBitmap = (HBITMAP)handle;
		CBitmap *bmp = CBitmap::FromHandle(hBitmap);
		HBITMAP oldbmp = m_bumpmap.GetBitmap();
		if (oldbmp != bitmap) DeleteObject(oldbmp);
		m_bumpmap.SetBitmap(hBitmap);

		BITMAP bmpinfo;
		bmp->GetBitmap(&bmpinfo);
		double w = 160;
		double h = 120;
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
		m_bumpmap.SetWindowPos(NULL, 177, 212, (int)nw, (int)nh, 0);
		m_bumpmap.RedrawWindow();
		loadbump = true;
		return true;
	}
	else return false;
}

void CEditMaterial::OnLoadbump() 
{
	loadBump();
}
