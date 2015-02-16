// Material.cpp: implementation of the CMaterial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Material.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaterial::CMaterial()
{
	color = RGB(255, 255, 255);
	kd = 0.8;
	ka = 0.5;
	ks = 0.5;
	kr = 0;
	kt = 0;
	rr = 1.5;
	n = 50;
	texture = NULL;
}

CMaterial::CMaterial(COLORREF c)
{
	color = c;
	kd = 0.8;
	ka = 0.5;
	ks = 0.5;
	kr = 0;
	kt = 0;
	rr = 1.5;
	n = 50;
	texture = NULL;
}

CMaterial::~CMaterial()
{
	if (texture != NULL) delete texture;
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

CMaterial &CMaterial::operator=(const CMaterial &m)
{
	color = m.color;
	ka = m.ka;
	kd = m.kd;
	kr = m.kr;
	ks = m.ks;
	kt = m.kt;
	n = m.n;
	rr = m.rr;
	if (m.texture == NULL) texture = NULL;
	else texture = new CTexture(*m.texture);

	return *this;
}

void CMaterial::read(CArchive &ar)
{
	ar.Read(&color, sizeof(color));
	ar.Read(&ka, sizeof(ka));
	ar.Read(&kd, sizeof(kd));
	ar.Read(&kr, sizeof(kr));
	ar.Read(&ks, sizeof(ks));
	ar.Read(&kt, sizeof(kt));
	ar.Read(&n, sizeof(n));
	ar.Read(&rr, sizeof(rr));
	bool tex;
	ar.Read(&tex, sizeof(tex));
	if (tex)
	{
		texture = new CTexture;
		texture->read(ar);
	}
}

void CMaterial::write(CArchive &ar)
{
	ar.Write(&color, sizeof(color));
	ar.Write(&ka, sizeof(ka));
	ar.Write(&kd, sizeof(kd));
	ar.Write(&kr, sizeof(kr));
	ar.Write(&ks, sizeof(ks));
	ar.Write(&kt, sizeof(kt));
	ar.Write(&n, sizeof(n));
	ar.Write(&rr, sizeof(rr));
	bool tex = (texture != NULL);
	ar.Write(&tex, sizeof(tex));
	if (tex) texture->write(ar);
}