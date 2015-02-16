// RenderResult.cpp : implementation file
//

#include "stdafx.h"
#include "3DCreator.h"
#include "RenderResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenderResult dialog


CRenderResult::CRenderResult(CWnd* pParent /*=NULL*/)
	: CDialog(CRenderResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenderResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRenderResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenderResult)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRenderResult, CDialog)
	//{{AFX_MSG_MAP(CRenderResult)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_COMMAND(ID_RENDER_SAVE, OnRenderSave)
	ON_COMMAND(ID_RENDER_CLOSE, OnRenderClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenderResult message handlers

void CRenderResult::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (!rendered)
	{
		view.align(*scene);
		scene->render(&dc, viewRect, view, antialias);

		CDC memDC;
		memDC.CreateCompatibleDC(NULL);
		bmp.CreateCompatibleBitmap(&dc, viewRect.Width() + 1, viewRect.Height() + 1);
		CBitmap *oldbmp = memDC.SelectObject(&bmp);
		memDC.BitBlt(0, 0, viewRect.Width() + 1, viewRect.Height() + 1, &dc, 0, 0, SRCCOPY);
		memDC.SelectObject(oldbmp);
		rendered = true;
	}
	else
	{
		CDC memDC;
		memDC.CreateCompatibleDC(NULL);
		CBitmap *oldbmp = memDC.SelectObject(&bmp);
		dc.BitBlt(0, 0, viewRect.Width() + 1, viewRect.Height() + 1, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldbmp);
		memDC.DeleteDC();
	}
}

BOOL CRenderResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowPos(NULL, 0, 0, viewRect.Width() + 8, viewRect.Height() + 46, 0);
	rendered = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRenderResult::OnDestroy() 
{
	CDialog::OnDestroy();

	bmp.DeleteObject();
}

void CRenderResult::OnRenderSave() 
{
	CFileDialog dlg(FALSE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Î»Í¼ÎÄ¼þ(*.bmp)|*.bmp||");
	if (dlg.DoModal() == IDOK) saveBitmap(bmp, dlg.GetFileName());
}

BOOL CRenderResult::saveBitmap(CBitmap &bmp, const CString &filename)
{
	HBITMAP hBitmap = (HBITMAP)bmp.GetSafeHandle();

	HDC hDC;
	int iBits = 24;
	WORD wBitCount = 24;
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;
	BITMAP Bitmap;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal,hOldPal = NULL;

	bmp.GetBitmap(&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight ;

	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	fh = CreateFile(filename, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)return FALSE;

	bmfHdr.bfType = 0x4D42;
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+ (DWORD)sizeof(BITMAPINFOHEADER)	+ dwPaletteSize;
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,&dwWritten, NULL);

	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;
}
void CRenderResult::OnRenderClose() 
{
	OnOK();
}
