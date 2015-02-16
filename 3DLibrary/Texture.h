// Texture.h: interface for the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(TEXTURE_H__INCLUDED_)
#define TEXTURE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTexture  
{
public:
	CTexture();
	CTexture(const CTexture &t);
	virtual ~CTexture();

	void loadImage(HBITMAP hBitmap);
	COLORREF getPixel(double x, double y);
	void getBumpPixel(double x, double y, double bp[2]);
	void setScale(double s);
	void setTexture(bool t);
	void setBump(bool b);
	void setBumpRate(double b);
	void setBumpScale(double s);
	bool getBump();
	bool getTexture();
	bool loadBumpMapping(HBITMAP hBitmap);
	HBITMAP createBitmap(HDC hdc);
	HBITMAP createBumpBitmap(HDC hdc);
	double getScale();
	double getBumpRate();
	double getBumpScale();
	void read(CArchive &ar);
	void write(CArchive &ar);

private:
	COLORREF *bits;
	COLORREF *bumpmap;
	int height;
	int width;
	int bwidth;
	int bheight;
	double scale;
	bool bump;
	double br;
	bool texture;
	double bscale;

	double getDepth(COLORREF c);

};

#endif // !defined(TEXTURE_H__INCLUDED_)
