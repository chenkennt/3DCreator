// MySplitterWnd.h: interface for the CMySplitterWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSPLITTERWND_H__8962B9E0_8F15_48D0_9238_24604E0F0291__INCLUDED_)
#define AFX_MYSPLITTERWND_H__8962B9E0_8F15_48D0_9238_24604E0F0291__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySplitterWnd : public CSplitterWnd  
{
public:
	void SetColumnInfo(int col, int cxIdeal, int cxMin);

	CMySplitterWnd();
	virtual ~CMySplitterWnd();

protected:
	//{{AFX_MSG(CMySplitterWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point){}
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point){}
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_MYSPLITTERWND_H__8962B9E0_8F15_48D0_9238_24604E0F0291__INCLUDED_)
