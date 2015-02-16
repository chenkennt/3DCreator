// MySplitterWnd.cpp: implementation of the CMySplitterWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3DCreator.h"
#include "MySplitterWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CMySplitterWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySplitterWnd::CMySplitterWnd()
{

}

CMySplitterWnd::~CMySplitterWnd()
{

}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

void CMySplitterWnd::SetColumnInfo(int col, int cxIdeal, int cxMin)
{
	ASSERT_VALID(this);
	ASSERT(col >= 0 && col < m_nMaxCols);
	ASSERT(cxIdeal >= 0);
	ASSERT(cxMin >= 0);

	if (!m_pColInfo) return;

	m_pColInfo[col].nIdealSize = cxIdeal;
	m_pColInfo[col].nMinSize = cxMin;
	RecalcLayout();
}