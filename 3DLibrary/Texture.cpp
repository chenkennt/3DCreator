// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTexture::CTexture()
{
	bits = NULL;
	bumpmap = NULL;
	scale = 1;
	br = 1;
}

CTexture::CTexture(const CTexture &t)
{
	texture = t.texture;
	br = t.br;
	bump = t.bump;
	height = t.height;
	width = t.width;
	scale = t.scale;
	bheight = t.bheight;
	bwidth = t.bwidth;
	bscale = t.bscale;
	if (!t.texture || t.bits == NULL) bits = NULL;
	else
	{
		bits = new COLORREF[width * height];
		memcpy(bits, t.bits, width * height * sizeof(COLORREF));
	}
	if (!t.bump || t.bumpmap == NULL) bumpmap = NULL;
	else
	{
		bumpmap = new COLORREF[bwidth * bheight];
		memcpy(bumpmap, t.bumpmap, bwidth * bheight * sizeof(COLORREF));
	}
}

CTexture::~CTexture()
{
	if (bits != NULL) delete[] bits;
	if (bumpmap != NULL) delete[] bumpmap;
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CTexture::loadImage(HBITMAP hBitmap)
{
	CBitmap *bmp = CBitmap::FromHandle(hBitmap);
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	CBitmap *oldbmp = dc.SelectObject(bmp);
	BITMAP bmpinfo;
	bmp->GetBitmap(&bmpinfo);
	width = bmpinfo.bmWidth;
	height = bmpinfo.bmHeight;

	if (bits != NULL) delete[] bits;
	bits = new COLORREF[width * height];

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			bits[i * height + j] = dc.GetPixel(i, height - 1 - j);

	dc.SelectObject(oldbmp);
	dc.DeleteDC();
	bmp->DeleteObject();
}

COLORREF CTexture::getPixel(double x, double y)
{
	if (bits == NULL) return RGB(0, 0, 0);
	int ix = (int)(x / scale);
	int iy = (int)(y / scale);
	ix = ix % width;
	iy = iy % height;
	if (ix < 0) ix += width;
	if (iy < 0) iy += height;
	return bits[ix * height + iy];
}

void CTexture::setScale(double s)
{
	scale = s;
}

double CTexture::getDepth(COLORREF c)
{
	return sqrt(GetRValue(c) * GetRValue(c) + GetGValue(c) * GetGValue(c) + GetBValue(c) * GetBValue(c)) / (255 * sqrt(3));
}

void CTexture::getBumpPixel(double x, double y, double bp[2])
{
	if (bumpmap == NULL || !bump)
	{
		bp[0] = bp[1] = 0;
		return;
	}

	int ix = (int)(x / bscale);
	int iy = (int)(y / bscale);
	ix = ix % bwidth;
	iy = iy % bheight;
	if (ix < 0) ix += bwidth;
	if (iy < 0) iy += bheight;
	double d = getDepth(bumpmap[ix * bheight + iy]);
	double dx = getDepth(bumpmap[(ix + 1) % bwidth * bheight + iy]);
	double dy = getDepth(bumpmap[ix * bheight + (iy + 1) % bheight]);
	bp[0] = br * (dx - d);
	bp[1] = br * (dy - d);
}

void CTexture::setBump(bool b)
{
	bump = b;
}

bool CTexture::getBump()
{
	return bump;
}

void CTexture::setBumpRate(double b)
{
	br = b;
}

HBITMAP CTexture::createBitmap(HDC hdc)
{
	HDC hDC = CreateCompatibleDC(NULL);
	HBITMAP bmp = CreateCompatibleBitmap(hdc, width, height);
	HGDIOBJ oldbmp = SelectObject(hDC, bmp);

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			SetPixel(hDC, i, height - 1 - j, bits[i * height + j]);

	SelectObject(hDC, oldbmp);
	DeleteDC(hDC);

	return bmp;
}

double CTexture::getScale()
{
	return scale;
}

double CTexture::getBumpRate()
{
	return br;
}

void CTexture::read(CArchive &ar)
{
	if (bits)
	{
		delete bits;
		bits = NULL;
	}
	if (bumpmap)
	{
		delete bumpmap;
		bumpmap = NULL;
	}

	ar.Read(&texture, sizeof(texture));
	if (texture)
	{
		ar.Read(&width, sizeof(width));
		ar.Read(&height, sizeof(height));
		bits = new COLORREF[width * height];
		ar.Read(bits, sizeof(COLORREF) * width * height);
		ar.Read(&scale, sizeof(scale));
	}
	ar.Read(&bump, sizeof(bump));
	if (bump)
	{
		ar.Read(&bwidth, sizeof(bwidth));
		ar.Read(&bheight, sizeof(bheight));
		bumpmap = new COLORREF[bwidth * bheight];
		ar.Read(bumpmap, sizeof(COLORREF) * bwidth * bheight);
		ar.Read(&bscale, sizeof(bscale));
		ar.Read(&br, sizeof(br));
	}
}

void CTexture::write(CArchive &ar)
{
	ar.Write(&texture, sizeof(texture));
	if (texture)
	{
		ar.Write(&width, sizeof(width));
		ar.Write(&height, sizeof(height));
		ar.Write(bits, sizeof(COLORREF) * width * height);
		ar.Write(&scale, sizeof(scale));
	}
	ar.Write(&bump, sizeof(bump));
	if (bump)
	{
		ar.Write(&bwidth, sizeof(bwidth));
		ar.Write(&bheight, sizeof(bheight));
		ar.Write(bumpmap, sizeof(COLORREF) * bwidth * bheight);
		ar.Write(&bscale, sizeof(bscale));
		ar.Write(&br, sizeof(br));
	}
}

bool CTexture::loadBumpMapping(HBITMAP hBitmap)
{
	CBitmap *bmp = CBitmap::FromHandle(hBitmap);
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	CBitmap *oldbmp = dc.SelectObject(bmp);
	BITMAP bmpinfo;
	bmp->GetBitmap(&bmpinfo);
	bwidth = bmpinfo.bmWidth;
	bheight = bmpinfo.bmHeight;

	if (bumpmap != NULL) delete[] bumpmap;
	bumpmap = new COLORREF[bwidth * bheight];

	for (int i = 0; i < bwidth; i++)
		for (int j = 0; j < bheight; j++)
			bumpmap[i * bheight + j] = dc.GetPixel(i, bheight - 1 - j);

	dc.SelectObject(oldbmp);
	dc.DeleteDC();
	bmp->DeleteObject();

	return true;
}

bool CTexture::getTexture()
{
	return texture;
}

void CTexture::setTexture(bool t)
{
	texture = t;
}

HBITMAP CTexture::createBumpBitmap(HDC hdc)
{
	HDC hDC = CreateCompatibleDC(NULL);
	HBITMAP bmp = CreateCompatibleBitmap(hdc, bwidth, bheight);
	HGDIOBJ oldbmp = SelectObject(hDC, bmp);

	for (int i = 0; i < bwidth; i++)
		for (int j = 0; j < bheight; j++)
			SetPixel(hDC, i, bheight - 1 - j, bumpmap[i * bheight + j]);

	SelectObject(hDC, oldbmp);
	DeleteDC(hDC);

	return bmp;
}

double CTexture::getBumpScale()
{
	return bscale;
}

void CTexture::setBumpScale(double s)
{
	bscale = s;
}