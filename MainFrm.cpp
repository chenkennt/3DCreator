// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "3DCreator.h"

#include "MainFrm.h"
#include "3DCreatorView.h"
#include "ToolForm.h"
#include "LoftDialog.h"
#include "ScenePreference.h"
#include "RenderDialog.h"
#include "RenderResult.h"
#include "3DLibrary\Constants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SELECTING, OnSelecting)
	ON_UPDATE_COMMAND_UI(ID_SELECTING, OnUpdateSelecting)
	ON_COMMAND(ID_VIEW_MOVE, OnViewMove)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE, OnUpdateViewMove)
	ON_COMMAND(ID_VIEW_ROTATE, OnViewRotate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE, OnUpdateViewRotate)
	ON_COMMAND(ID_VIEW_SCALE, OnViewScale)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCALE, OnUpdateViewScale)
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, OnUpdateViewZoom)
	ON_COMMAND(ID_EDIT_ROTATE, OnEditRotate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ROTATE, OnUpdateEditRotate)
	ON_COMMAND(ID_EDIT_SCALE, OnEditScale)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SCALE, OnUpdateEditScale)
	ON_COMMAND(ID_EDIT_MOVE, OnEditMove)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVE, OnUpdateEditMove)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_LOFT, OnEditLoft)
	ON_COMMAND(ID_SCENE_RENDER, OnSceneRender)
	ON_COMMAND(ID_SCENE_PREFERENCE, OnScenePreference)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_POS_X,
	ID_POS_Y,
	ID_POS_Z,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_wndSplitter.CreateStatic(this, 1, 2)) return FALSE;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMy3DCreatorView), CSize(300, 300), pContext)) return FALSE;
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolForm), CSize(300, 300), pContext)) return FALSE;

	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);
	int w = rc.Width() - 188;
	if (w < 0) w = 0;
	m_wndSplitter.SetColumnInfo(0, w, 0);
}

void CMainFrame::OnSelecting() 
{
	bool r = false;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if ((pDoc->status == STATUS_NEW_CUBE || pDoc->status == STATUS_NEW_SPHERE || pDoc->status == STATUS_NEW_CONE || pDoc->status == STATUS_NEW_CYLINDER) && pDoc->createstep != 0) r = true;
		pDoc->switchStatus(STATUS_SELECTING);
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		if (r)
		{
			CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
			CDC *pDC = view->GetDC();
			view->redrawScreen(pDC, DRAW_ALLVIEW);
			ReleaseDC(pDC);
		}
	}
}

void CMainFrame::OnUpdateSelecting(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->SetCheck(pDoc->status == STATUS_SELECTING);
}

void CMainFrame::OnViewMove() 
{
	bool r = false;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if ((pDoc->status == STATUS_NEW_CUBE || pDoc->status == STATUS_NEW_SPHERE || pDoc->status == STATUS_NEW_CONE || pDoc->status == STATUS_NEW_CYLINDER) && pDoc->createstep != 0) r = true;
		pDoc->switchStatus(STATUS_MOVEVIEW);
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		if (r)
		{
			CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
			CDC *pDC = view->GetDC();
			view->redrawScreen(pDC, DRAW_ALLVIEW);
			ReleaseDC(pDC);
		}
	}
}

void CMainFrame::OnUpdateViewMove(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->SetCheck(pDoc->status == STATUS_MOVEVIEW);
}

void CMainFrame::OnViewRotate() 
{
	bool r = false;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if ((pDoc->status == STATUS_NEW_CUBE || pDoc->status == STATUS_NEW_SPHERE || pDoc->status == STATUS_NEW_CONE || pDoc->status == STATUS_NEW_CYLINDER) && pDoc->createstep != 0) r = true;
		pDoc->switchStatus(STATUS_ROTATEVIEW);
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		if (r)
		{
			CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
			CDC *pDC = view->GetDC();
			view->redrawScreen(pDC, DRAW_ALLVIEW);
			ReleaseDC(pDC);
		}
	}
}

void CMainFrame::OnUpdateViewRotate(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->SetCheck(pDoc->status == STATUS_ROTATEVIEW);
}

void CMainFrame::OnViewScale() 
{
	bool r = false;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if ((pDoc->status == STATUS_NEW_CUBE || pDoc->status == STATUS_NEW_SPHERE || pDoc->status == STATUS_NEW_CONE || pDoc->status == STATUS_NEW_CYLINDER) && pDoc->createstep != 0) r = true;
		pDoc->switchStatus(STATUS_SCALEVIEW);
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		if (r)
		{
			CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
			CDC *pDC = view->GetDC();
			view->redrawScreen(pDC, DRAW_ALLVIEW);
			ReleaseDC(pDC);
		}
	}
}

void CMainFrame::OnUpdateViewScale(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->SetCheck(pDoc->status == STATUS_SCALEVIEW);
}

void CMainFrame::OnViewZoom() 
{
	bool r = false;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if ((pDoc->status == STATUS_NEW_CUBE || pDoc->status == STATUS_NEW_SPHERE || pDoc->status == STATUS_NEW_CONE || pDoc->status == STATUS_NEW_CYLINDER) && pDoc->createstep != 0) r = true;
		pDoc->switchStatus(STATUS_ZOOMVIEW);
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		if (r)
		{
			CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
			CDC *pDC = view->GetDC();
			view->redrawScreen(pDC, DRAW_ALLVIEW);
			ReleaseDC(pDC);
		}
	}
}

void CMainFrame::OnUpdateViewZoom(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->SetCheck(pDoc->status == STATUS_ZOOMVIEW);
}

void CMainFrame::OnEditRotate() 
{
	bool r = false;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if ((pDoc->status == STATUS_NEW_CUBE || pDoc->status == STATUS_NEW_SPHERE || pDoc->status == STATUS_NEW_CONE || pDoc->status == STATUS_NEW_CYLINDER) && pDoc->createstep != 0) r = true;
		pDoc->switchStatus(STATUS_ROTATE);
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		if (r)
		{
			CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
			CDC *pDC = view->GetDC();
			view->redrawScreen(pDC, DRAW_ALLVIEW);
			ReleaseDC(pDC);
		}
	}
}

void CMainFrame::OnUpdateEditRotate(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->SetCheck(pDoc->status == STATUS_ROTATE);
}

void CMainFrame::OnEditScale() 
{
	bool r = false;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if ((pDoc->status == STATUS_NEW_CUBE || pDoc->status == STATUS_NEW_SPHERE || pDoc->status == STATUS_NEW_CONE || pDoc->status == STATUS_NEW_CYLINDER) && pDoc->createstep != 0) r = true;
		pDoc->switchStatus(STATUS_SCALE);
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		if (r)
		{
			CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
			CDC *pDC = view->GetDC();
			view->redrawScreen(pDC, DRAW_ALLVIEW);
			ReleaseDC(pDC);
		}
	}
}

void CMainFrame::OnUpdateEditScale(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->SetCheck(pDoc->status == STATUS_SCALE);
}

void CMainFrame::OnEditMove() 
{
	bool r = false;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if ((pDoc->status == STATUS_NEW_CUBE || pDoc->status == STATUS_NEW_SPHERE || pDoc->status == STATUS_NEW_CONE || pDoc->status == STATUS_NEW_CYLINDER) && pDoc->createstep != 0) r = true;
		pDoc->switchStatus(STATUS_MOVE);
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		if (r)
		{
			CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
			CDC *pDC = view->GetDC();
			view->redrawScreen(pDC, DRAW_ALLVIEW);
			ReleaseDC(pDC);
		}
	}
}

void CMainFrame::OnUpdateEditMove(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->SetCheck(pDoc->status == STATUS_MOVE);
}

void CMainFrame::OnEditCopy() 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if (!pDoc->selection) return;

		if (pDoc->clipboard) delete pDoc->clipboard;
		pDoc->clipboard = pDoc->selection->copy();
	}
}

void CMainFrame::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->Enable(pDoc->selection != NULL);
}

void CMainFrame::OnEditCut() 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if (!pDoc->selection) return;

		if (pDoc->clipboard) delete pDoc->clipboard;
		pDoc->scene.remove(pDoc->selection, false);
		pDoc->clipboard = pDoc->selection;
		pDoc->selection = NULL;
		pDoc->SetModifiedFlag();

		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
		CDC *pDC = view->GetDC();
		view->redrawScreen(pDC, DRAW_ALLVIEW);
		ReleaseDC(pDC);
	}
}

void CMainFrame::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->Enable(pDoc->selection != NULL);
}

void CMainFrame::OnEditDelete() 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if (!pDoc->selection) return;

		pDoc->scene.remove(pDoc->selection);
		pDoc->selection = NULL;
		pDoc->SetModifiedFlag();

		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
		CDC *pDC = view->GetDC();
		view->redrawScreen(pDC, DRAW_ALLVIEW);
		ReleaseDC(pDC);
	}
}

void CMainFrame::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->Enable(pDoc->selection != NULL);
}

void CMainFrame::OnEditPaste() 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc)
	{
		if (!pDoc->clipboard) return;

		pDoc->clipboard->move(CVector(10, 10, 10));
		if (pDoc->clipboard->getType() == TYPE_LIGHT)
		{
			CLight *l = (CLight*)pDoc->clipboard->copy();
			CString name;
			name.Format("Light%d", pDoc->currlight);
			pDoc->currlight++;
			pDoc->scene.add(l, name);
			pDoc->selection = l;
		}
		else
		{
			CGeometry *g = (CGeometry*)pDoc->clipboard->copy();
			CString name;
			name.Format("Object%d", pDoc->currname);
			pDoc->currname++;
			pDoc->scene.add(g, name);
			pDoc->selection = g;
		}

		pDoc->SetModifiedFlag();

		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
		CDC *pDC = view->GetDC();
		view->redrawScreen(pDC, DRAW_ALLVIEW);
		ReleaseDC(pDC);
	}
}

void CMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	if (pDoc) pCmdUI->Enable(pDoc->clipboard != NULL);
}

void CMainFrame::OnEditLoft() 
{
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	pDoc->switchStatus(STATUS_SELECTING);

	CLoftDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		CGeometry *g = new CGeometry(dlg.loft);
		g->material.color = Colors[abs(rand()) % 32];
		CString name;
		name.Format("Object%d", pDoc->currname);
		pDoc->currname++;
		pDoc->scene.add(g, name);
		pDoc->selection = g;
		pDoc->SetModifiedFlag();
		CToolForm *form = (CToolForm*)m_wndSplitter.GetPane(0, 1);
		form->refreshButtons();
		CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);
		CDC *pDC = view->GetDC();
		view->redrawScreen(pDC, DRAW_ALLVIEW);
		ReleaseDC(pDC);
	}
}

void CMainFrame::OnSceneRender() 
{
	CRenderDialog dlg;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	CMy3DCreatorView *view = (CMy3DCreatorView*)m_wndSplitter.GetPane(0, 0);

	CRect crc;
	view->GetClientRect(crc);
	int w = (crc.Width() - 3 * VIEW_GAP) / 2;
	int h = (crc.Height() - 3 * VIEW_GAP) / 2;
	if (!view->zoom)
	{
		dlg.m_width = w;
		dlg.m_height = h;
	}
	else
	{
		dlg.m_width = w * 2;
		dlg.m_height = h * 2;
	}
	dlg.m_antialias = 0;
	dlg.name[0] = view->viewName[0];
	dlg.name[1] = view->viewName[1];
	dlg.name[2] = view->viewName[2];
	dlg.name[3] = view->viewName[3];
	dlg.active = view->activeView;

	if (dlg.DoModal() == IDOK)
	{
		CRenderResult result;
		result.scene = &pDoc->scene;
		result.view = view->view[dlg.m_vview];
		result.view.setWidth(result.view.getHeight() * dlg.m_width / dlg.m_height);
		result.viewRect = CRect(0, 0, dlg.m_width, dlg.m_height);
		result.antialias = dlg.m_antialias;

		result.DoModal();
	}
}
void CMainFrame::OnScenePreference() 
{
	CScenePreference dlg;
	CMy3DCreatorDoc *pDoc = (CMy3DCreatorDoc*)GetActiveDocument();
	dlg.m_acolor.color = pDoc->scene.ambientLight.getColor();
	dlg.m_aintension = pDoc->scene.ambientLight.getIntension();
	dlg.m_bkcolor.color = pDoc->scene.background.color;
	dlg.m_vbkmap = (pDoc->scene.background.texture != NULL);
	if (pDoc->scene.background.texture)
	{
		CDC *pDC = GetDC();
		dlg.bitmap = pDoc->scene.background.texture->createBitmap(pDC->GetSafeHdc());
		ReleaseDC(pDC);

		dlg.m_vscale = pDoc->scene.background.texture->getScale();
	}
	else dlg.m_vscale = 1;

	if (dlg.DoModal() == IDOK)
	{
		pDoc->scene.ambientLight.setIntension(dlg.m_aintension);
		pDoc->scene.ambientLight.setColor(dlg.m_acolor.color);
		pDoc->scene.background.color = dlg.m_bkcolor.color;
		if (dlg.m_vbkmap)
		{
			if (!pDoc->scene.background.texture) pDoc->scene.background.texture = new CTexture;
			pDoc->scene.background.texture->loadImage(dlg.bitmap);
			pDoc->scene.background.texture->setScale(dlg.m_vscale);
		}
		else if (pDoc->scene.background.texture)
		{
			delete pDoc->scene.background.texture;
			pDoc->scene.background.texture = NULL;
		}

		pDoc->SetModifiedFlag();
	}
	if (dlg.load) DeleteObject(dlg.bitmap);
}