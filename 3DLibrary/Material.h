// Material.h: interface for the CMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MATERIAL_H__INCLUDED_)
#define MATERIAL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Texture.h"

class CMaterial  
{
public:
	COLORREF color;
	double kd;
	double ka;
	double ks;
	double kr;
	double kt;
	double rr;
	int n;
	CTexture *texture;

	CMaterial();
	CMaterial(COLORREF c);
	virtual ~CMaterial();

	CMaterial &operator=(const CMaterial &m);
	void read(CArchive &ar);
	void write(CArchive &ar);

};

#endif // !defined(MATERIAL_H__INCLUDED_)
