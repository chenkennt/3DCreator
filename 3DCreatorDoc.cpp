// 3DCreatorDoc.cpp : implementation of the CMy3DCreatorDoc class
//

#include "stdafx.h"
#include "3DCreator.h"

#include "3DCreatorDoc.h"
#include "MainFrm.h"
#include "3DCreatorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorDoc

IMPLEMENT_DYNCREATE(CMy3DCreatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy3DCreatorDoc, CDocument)
	//{{AFX_MSG_MAP(CMy3DCreatorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorDoc construction/destruction

CMy3DCreatorDoc::CMy3DCreatorDoc()
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	srand(time.wMilliseconds);
	temp = NULL;
	clipboard = NULL;
}

CMy3DCreatorDoc::~CMy3DCreatorDoc()
{
	if (temp != NULL) delete temp;
	if (clipboard != NULL) delete clipboard;
}

BOOL CMy3DCreatorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	scene.clear();
	status = STATUS_SELECTING;
	selection = NULL;
	if (temp != NULL) delete temp;
	temp = NULL;
	currname = 1;
	currlight = 1;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorDoc serialization

void CMy3DCreatorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		scene.write(ar);
		POSITION pos = GetFirstViewPosition();
		CMy3DCreatorView *view = (CMy3DCreatorView*)GetNextView(pos);
		for (int i = 0; i < 4; i++)
		{
			ar<<view->viewName[i];
			view->view[i].write(ar);
			ar.Write(&view->smooth[i], sizeof(bool));
		}
	}
	else
	{
		switchStatus(STATUS_SELECTING);
		selection = NULL;
		if (temp != NULL) delete temp;
		temp = NULL;
		currname = 1;
		currlight = 1;

		scene.read(ar);
		POSITION pos = GetFirstViewPosition();
		CMy3DCreatorView *view = (CMy3DCreatorView*)GetNextView(pos);
		for (int i = 0; i < 4; i++)
		{
			ar>>view->viewName[i];
			view->view[i].read(ar);
			ar.Read(&view->smooth[i], sizeof(bool));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorDoc diagnostics

#ifdef _DEBUG
void CMy3DCreatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy3DCreatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMy3DCreatorDoc commands

void CMy3DCreatorDoc::switchStatus(int status)
{
	bool bi = false;
	if (this->status == STATUS_INTERSECT || this->status == STATUS_UNION || this->status == STATUS_DIFFERENCE) bi = true;

	if (status != this->status) this->status = status;
	else this->status = STATUS_SELECTING;

	if (temp != NULL)
	{
		delete temp;
		temp = NULL;
		UpdateAllViews(NULL);
	}

	if ((this->status != STATUS_INTERSECT && this->status != STATUS_UNION && this->status != STATUS_DIFFERENCE) || !bi) createstep = 0;
}
