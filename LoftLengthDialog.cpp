// LoftLengthDialog.cpp : implementation file
//

#include "stdafx.h"
#include "3DCreator.h"
#include "LoftLengthDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoftLengthDialog dialog


CLoftLengthDialog::CLoftLengthDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoftLengthDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoftLengthDialog)
	m_length = 0.0;
	//}}AFX_DATA_INIT
}


void CLoftLengthDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoftLengthDialog)
	DDX_Text(pDX, IDC_LENGTH, m_length);
	DDV_MinMaxDouble(pDX, m_length, 0., 10000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoftLengthDialog, CDialog)
	//{{AFX_MSG_MAP(CLoftLengthDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoftLengthDialog message handlers
