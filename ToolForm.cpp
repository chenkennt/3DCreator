// ToolForm.cpp : implementation file
//

#include "stdafx.h"
#include "3DCreator.h"
#include "ToolForm.h"
#include "MainFrm.h"
#include "EditMaterial.h"
#include "3DCreatorView.h"
#include "3DLibrary\Constants.h"
#include "3DLibrary\Cube.h"
#include "3DLibrary\Sphere.h"
#include "3DLibrary\Cylinder.h"
#include "3DLibrary\Cone.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolForm

IMPLEMENT_DYNCREATE(CToolForm, CFormView)

CToolForm::CToolForm()
	: CFormView(CToolForm::IDD)
{
	//{{AFX_DATA_INIT(CToolForm)
	//}}AFX_DATA_INIT
}

CToolForm::~CToolForm()
{
}

void CToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolForm)
	DDX_Control(pDX, IDC_APPLY, m_apply);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Control(pDX, IDC_SELECT_BOOL_OBJECT, m_selectbool);
	DDX_Control(pDX, IDC_INTERSECT, m_intersect);
	DDX_Control(pDX, IDC_DIFFERENCE, m_difference);
	DDX_Control(pDX, IDC_UNION, m_union);
	DDX_Control(pDX, IDC_NEW_SPHERE, m_newsphere);
	DDX_Control(pDX, IDC_NEW_LIGHT, m_newlight);
	DDX_Control(pDX, IDC_NEW_CYLINDER, m_newcylinder);
	DDX_Control(pDX, IDC_NEW_CONE, m_newcone);
	DDX_Control(pDX, IDC_NEW_CUBE, m_newcube);
	DDX_Control(pDX, IDC_COLOR, m_color);
	DDX_Control(pDX, IDC_STATIC_PROPERTY3, m_sproperty3);
	DDX_Control(pDX, IDC_STATIC_PROPERTY2, m_sproperty2);
	DDX_Control(pDX, IDC_STATIC_PROPERTY1, m_sproperty1);
	DDX_Control(pDX, IDC_STATIC_POS_Z, m_sposz);
	DDX_Control(pDX, IDC_STATIC_POS_Y, m_sposy);
	DDX_Control(pDX, IDC_STATIC_POS_X, m_sposx);
	DDX_Control(pDX, IDC_STATIC_NAME, m_sname);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_scolor);
	DDX_Control(pDX, IDC_SELECT_CREATE, m_selectcreate);
	DDX_Control(pDX, IDC_PROPERTY3, m_property3);
	DDX_Control(pDX, IDC_PROPERTY2, m_property2);
	DDX_Control(pDX, IDC_PROPERTY1, m_property1);
	DDX_Control(pDX, IDC_POS_Z, m_posz);
	DDX_Control(pDX, IDC_POS_Y, m_posy);
	DDX_Control(pDX, IDC_POS_X, m_posx);
	DDX_Control(pDX, IDC_PROPERTY, m_property);
	DDX_Control(pDX, IDC_POSITION, m_position);
	DDX_Control(pDX, IDC_NAME, m_name);
	DDX_Control(pDX, IDC_MATERIAL, m_material);
	DDX_Control(pDX, IDC_CASTSHADOW, m_castshadow);
	DDX_Control(pDX, IDC_BASIC, m_basic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolForm, CFormView)
	//{{AFX_MSG_MAP(CToolForm)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_CBN_SELCHANGE(IDC_SELECT_CREATE, OnSelchangeSelectCreate)
	ON_BN_CLICKED(IDC_NEW_CUBE, OnNewCube)
	ON_BN_CLICKED(IDC_NEW_CONE, OnNewCone)
	ON_BN_CLICKED(IDC_NEW_CYLINDER, OnNewCylinder)
	ON_BN_CLICKED(IDC_NEW_SPHERE, OnNewSphere)
	ON_BN_CLICKED(IDC_INTERSECT, OnIntersect)
	ON_BN_CLICKED(IDC_DIFFERENCE, OnDifference)
	ON_BN_CLICKED(IDC_UNION, OnUnion)
	ON_BN_CLICKED(IDC_SELECT_BOOL_OBJECT, OnSelectBoolObject)
	ON_BN_CLICKED(IDC_NEW_LIGHT, OnNewLight)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_POS_X, OnChangePosX)
	ON_EN_CHANGE(IDC_POS_Y, OnChangePosY)
	ON_EN_CHANGE(IDC_POS_Z, OnChangePosZ)
	ON_EN_CHANGE(IDC_PROPERTY1, OnChangeProperty1)
	ON_EN_CHANGE(IDC_PROPERTY2, OnChangeProperty2)
	ON_EN_CHANGE(IDC_PROPERTY3, OnChangeProperty3)
	ON_CBN_SELCHANGE(IDC_CASTSHADOW, OnSelchangeCastshadow)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_MATERIAL, OnMaterial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolForm diagnostics

#ifdef _DEBUG
void CToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolForm message handlers

void CToolForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if (m_tab.GetItemCount() < 2)
	{
		m_tab.InsertItem(0, "生成");
		m_tab.InsertItem(1, "修改");
	}
	m_selectcreate.SetCurSel(0);

	refreshButtons();
}

void CToolForm::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	GetDocument()->switchStatus(STATUS_SELECTING);
	refreshButtons();

	*pResult = 0;
}

void CToolForm::refreshButtons()
{
	int s = m_tab.GetCurFocus();
	int c = m_selectcreate.GetCurSel();

	if (s == 0) m_selectcreate.ShowWindow(SW_SHOW);
	else m_selectcreate.ShowWindow(SW_HIDE);

	if (s == 0 && c == 0)
	{
		m_newcube.ShowWindow(SW_SHOW);
		m_newsphere.ShowWindow(SW_SHOW);
		m_newcylinder.ShowWindow(SW_SHOW);
		m_newcone.ShowWindow(SW_SHOW);
	}
	else
	{
		m_newcube.ShowWindow(SW_HIDE);
		m_newsphere.ShowWindow(SW_HIDE);
		m_newcylinder.ShowWindow(SW_HIDE);
		m_newcone.ShowWindow(SW_HIDE);
	}

	if (s == 0 && c == 1)
	{
		m_intersect.ShowWindow(SW_SHOW);
		m_union.ShowWindow(SW_SHOW);
		m_difference.ShowWindow(SW_SHOW);
		m_selectbool.ShowWindow(SW_SHOW);
	}
	else
	{
		m_intersect.ShowWindow(SW_HIDE);
		m_union.ShowWindow(SW_HIDE);
		m_difference.ShowWindow(SW_HIDE);
		m_selectbool.ShowWindow(SW_HIDE);
	}

	if (s == 0 && c == 2) m_newlight.ShowWindow(SW_SHOW);
	else m_newlight.ShowWindow(SW_HIDE);

	if (s == 1 && GetDocument()->selection != NULL)
	{
		m_basic.ShowWindow(SW_SHOW);
		m_color.ShowWindow(SW_SHOW);
		m_name.ShowWindow(SW_SHOW);
		m_scolor.ShowWindow(SW_SHOW);
		m_sname.ShowWindow(SW_SHOW);
		m_position.ShowWindow(SW_SHOW);
		m_posx.ShowWindow(SW_SHOW);
		m_posy.ShowWindow(SW_SHOW);
		m_posz.ShowWindow(SW_SHOW);
		m_sposx.ShowWindow(SW_SHOW);
		m_sposy.ShowWindow(SW_SHOW);
		m_sposz.ShowWindow(SW_SHOW);
		m_property.ShowWindow(SW_SHOW);
		m_property1.ShowWindow(SW_SHOW);
		m_property2.ShowWindow(SW_SHOW);
		m_property3.ShowWindow(SW_SHOW);
		m_sproperty1.ShowWindow(SW_SHOW);
		m_sproperty2.ShowWindow(SW_SHOW);
		m_sproperty3.ShowWindow(SW_SHOW);
		m_castshadow.ShowWindow(SW_SHOW);
		m_material.ShowWindow(SW_SHOW);
		m_apply.ShowWindow(SW_SHOW);

		switch (GetDocument()->selection->getType())
		{
		case TYPE_CUBE:
			{
				m_castshadow.ShowWindow(SW_HIDE);
				m_apply.EnableWindow(FALSE);

				CGeometry *g = (CGeometry*)GetDocument()->selection;
				const CCube *cube = (CCube*)g->getCSGModel();
				m_color.color = g->material.color;
				m_name.SetWindowText(g->name);
				CString s;
				s.Format("%f", cube->getCenter().x);
				m_posx.SetWindowText(s);
				s.Format("%f", cube->getCenter().y);
				m_posy.SetWindowText(s);
				s.Format("%f", cube->getCenter().z);
				m_posz.SetWindowText(s);
				m_sproperty1.SetWindowText("长");
				m_sproperty2.SetWindowText("宽");
				m_sproperty3.SetWindowText("高");
				s.Format("%f", cube->getLength());
				m_property1.SetWindowText(s);
				s.Format("%f", cube->getWidth());
				m_property2.SetWindowText(s);
				s.Format("%f", cube->getHeight());
				m_property3.SetWindowText(s);
				break;
			}
		case TYPE_SPHERE:
			{
				m_castshadow.ShowWindow(SW_HIDE);
				m_sproperty2.ShowWindow(SW_HIDE);
				m_sproperty3.ShowWindow(SW_HIDE);
				m_property2.ShowWindow(SW_HIDE);
				m_property3.ShowWindow(SW_HIDE);
				m_apply.EnableWindow(FALSE);

				CGeometry *g = (CGeometry*)GetDocument()->selection;
				const CSphere *sphere = (CSphere*)g->getCSGModel();
				m_color.color = g->material.color;
				m_name.SetWindowText(g->name);
				CString s;
				s.Format("%f", sphere->getCenter().x);
				m_posx.SetWindowText(s);
				s.Format("%f", sphere->getCenter().y);
				m_posy.SetWindowText(s);
				s.Format("%f", sphere->getCenter().z);
				m_posz.SetWindowText(s);
				m_sproperty1.SetWindowText("半径");
				s.Format("%f", sphere->getRadius());
				m_property1.SetWindowText(s);
				break;
			}
		case TYPE_CYLINDER:
			{
				m_castshadow.ShowWindow(SW_HIDE);
				m_sproperty3.ShowWindow(SW_HIDE);
				m_property3.ShowWindow(SW_HIDE);
				m_apply.EnableWindow(FALSE);

				CGeometry *g = (CGeometry*)GetDocument()->selection;
				const CCylinder *cylinder = (CCylinder*)g->getCSGModel();
				m_color.color = g->material.color;
				m_name.SetWindowText(g->name);
				CString s;
				s.Format("%f", cylinder->getCenter().x);
				m_posx.SetWindowText(s);
				s.Format("%f", cylinder->getCenter().y);
				m_posy.SetWindowText(s);
				s.Format("%f", cylinder->getCenter().z);
				m_posz.SetWindowText(s);
				m_sproperty1.SetWindowText("底面半径");
				m_sproperty2.SetWindowText("高");
				s.Format("%f", cylinder->getPRadius());
				m_property1.SetWindowText(s);
				s.Format("%f", cylinder->getHeight());
				m_property2.SetWindowText(s);
				break;
			}
		case TYPE_CONE:
			{
				m_castshadow.ShowWindow(SW_HIDE);
				m_apply.EnableWindow(FALSE);

				CGeometry *g = (CGeometry*)GetDocument()->selection;
				const CCone *cone = (CCone*)g->getCSGModel();
				m_color.color = g->material.color;
				m_name.SetWindowText(g->name);
				CString s;
				s.Format("%f", cone->getBCenter().x);
				m_posx.SetWindowText(s);
				s.Format("%f", cone->getBCenter().y);
				m_posy.SetWindowText(s);
				s.Format("%f", cone->getBCenter().z);
				m_posz.SetWindowText(s);
				m_sproperty1.SetWindowText("下底半径");
				m_sproperty2.SetWindowText("上底半径");
				m_sproperty3.SetWindowText("高");
				s.Format("%f", cone->getBRadius());
				m_property1.SetWindowText(s);
				s.Format("%f", cone->getTRadius());
				m_property2.SetWindowText(s);
				s.Format("%f", cone->getHeight());
				m_property3.SetWindowText(s);
				break;
			}
		case TYPE_BOOLEAN:
		case TYPE_CIRCLELOFT:
		case TYPE_LINELOFT:
			{
				m_castshadow.ShowWindow(SW_HIDE);
				m_sproperty1.ShowWindow(SW_HIDE);
				m_sproperty2.ShowWindow(SW_HIDE);
				m_sproperty3.ShowWindow(SW_HIDE);
				m_property1.ShowWindow(SW_HIDE);
				m_property2.ShowWindow(SW_HIDE);
				m_property3.ShowWindow(SW_HIDE);
				m_apply.EnableWindow(FALSE);

				CGeometry *g = (CGeometry*)GetDocument()->selection;
				m_color.color = g->material.color;
				m_name.SetWindowText(g->name);
				CString s;
				s.Format("%f", g->getCSGModel()->getCenter().x);
				m_posx.SetWindowText(s);
				s.Format("%f", g->getCSGModel()->getCenter().y);
				m_posy.SetWindowText(s);
				s.Format("%f", g->getCSGModel()->getCenter().z);
				m_posz.SetWindowText(s);
				break;
			}
		case TYPE_LIGHT:
			{
				m_sproperty3.ShowWindow(SW_HIDE);
				m_property2.ShowWindow(SW_HIDE);
				m_property3.ShowWindow(SW_HIDE);
				m_material.ShowWindow(SW_HIDE);
				m_apply.EnableWindow(FALSE);

				CLight *l = (CLight*)GetDocument()->selection;
				m_color.color = l->getColor();
				m_name.SetWindowText(l->name);
				CString s;
				s.Format("%f", l->getPosition().x);
				m_posx.SetWindowText(s);
				s.Format("%f", l->getPosition().y);
				m_posy.SetWindowText(s);
				s.Format("%f", l->getPosition().z);
				m_posz.SetWindowText(s);
				m_sproperty1.SetWindowText("光强");
				m_sproperty2.SetWindowText("产生阴影");
				s.Format("%f", l->getIntension());
				m_property1.SetWindowText(s);
				m_castshadow.SetCurSel(1 - l->getCastShadow());
				break;
			}
		}
	}
	else
	{
		m_basic.ShowWindow(SW_HIDE);
		m_color.ShowWindow(SW_HIDE);
		m_name.ShowWindow(SW_HIDE);
		m_scolor.ShowWindow(SW_HIDE);
		m_sname.ShowWindow(SW_HIDE);
		m_position.ShowWindow(SW_HIDE);
		m_posx.ShowWindow(SW_HIDE);
		m_posy.ShowWindow(SW_HIDE);
		m_posz.ShowWindow(SW_HIDE);
		m_sposx.ShowWindow(SW_HIDE);
		m_sposy.ShowWindow(SW_HIDE);
		m_sposz.ShowWindow(SW_HIDE);
		m_property.ShowWindow(SW_HIDE);
		m_property1.ShowWindow(SW_HIDE);
		m_property2.ShowWindow(SW_HIDE);
		m_property3.ShowWindow(SW_HIDE);
		m_sproperty1.ShowWindow(SW_HIDE);
		m_sproperty2.ShowWindow(SW_HIDE);
		m_sproperty3.ShowWindow(SW_HIDE);
		m_castshadow.ShowWindow(SW_HIDE);
		m_material.ShowWindow(SW_HIDE);
		m_apply.ShowWindow(SW_HIDE);
	}
	int status = GetDocument()->status;
	m_newcube.pushed = (status == STATUS_NEW_CUBE);
	m_newsphere.pushed = (status == STATUS_NEW_SPHERE);
	m_newcylinder.pushed = (status == STATUS_NEW_CYLINDER);
	m_newcone.pushed = (status == STATUS_NEW_CONE);
	m_intersect.pushed = (status == STATUS_INTERSECT);
	m_union.pushed = (status == STATUS_UNION);
	m_difference.pushed = (status == STATUS_DIFFERENCE);
	m_selectbool.pushed = (GetDocument()->createstep == 1);
	m_newlight.pushed = (status == STATUS_NEW_LIGHT);

	updateApply();
	RedrawWindow();
}

void CToolForm::OnSelchangeSelectCreate() 
{
	GetDocument()->switchStatus(STATUS_SELECTING);
	refreshButtons();
}

CMy3DCreatorDoc *CToolForm::GetDocument()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if (pFrame) return (CMy3DCreatorDoc*)pFrame->GetActiveDocument();
	return NULL;
}

void CToolForm::OnNewCube() 
{
	GetDocument()->switchStatus(STATUS_NEW_CUBE);
	refreshButtons();
}

void CToolForm::OnNewCone() 
{
	GetDocument()->switchStatus(STATUS_NEW_CONE);
	refreshButtons();
}

void CToolForm::OnNewCylinder() 
{
	GetDocument()->switchStatus(STATUS_NEW_CYLINDER);
	refreshButtons();
}

void CToolForm::OnNewSphere() 
{
	GetDocument()->switchStatus(STATUS_NEW_SPHERE);
	refreshButtons();
}

void CToolForm::OnIntersect() 
{
	GetDocument()->switchStatus(STATUS_INTERSECT);
	refreshButtons();
}

void CToolForm::OnDifference() 
{
	GetDocument()->switchStatus(STATUS_DIFFERENCE);
	refreshButtons();
}

void CToolForm::OnUnion() 
{
	GetDocument()->switchStatus(STATUS_UNION);
	refreshButtons();
}

void CToolForm::OnSelectBoolObject() 
{
	if (GetDocument()->status != STATUS_INTERSECT && GetDocument()->status != STATUS_UNION && GetDocument()->status != STATUS_DIFFERENCE) return;
	if (!GetDocument()->selection || GetDocument()->selection->getType() == TYPE_LIGHT) return;
	GetDocument()->createstep = 1 - GetDocument()->createstep;
	refreshButtons();
}

void CToolForm::OnNewLight() 
{
	GetDocument()->switchStatus(STATUS_NEW_LIGHT);
	refreshButtons();
}

void CToolForm::OnColor() 
{
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_color.color = dlg.GetColor();
		m_color.RedrawWindow();
		updateApply();
	}
}

void CToolForm::OnChangeName() 
{
	updateApply();
}

void CToolForm::updateApply()
{
	if (!GetDocument()->selection) return;

	bool m = false;

	CString s;
	m_name.GetWindowText(s);
	if (s != GetDocument()->selection->name) m = true;

	if (GetDocument()->selection->getType() == TYPE_LIGHT)
	{
		if (((CLight*)GetDocument()->selection)->getColor() != m_color.color) m = true;
	}
	else if (((CGeometry*)GetDocument()->selection)->material.color != m_color.color) m = true;

	CVertex c;
	if (GetDocument()->selection->getType() == TYPE_CONE)
	{
		const CCone *cone = (CCone*)((CGeometry*)GetDocument()->selection)->getCSGModel();
		c = cone->getBCenter();
	}
	else c = GetDocument()->selection->getCenter();

	m_posx.GetWindowText(s);
	if (fabs(atof(s) - c.x) > ZERO) m = true;
	m_posy.GetWindowText(s);
	if (fabs(atof(s) - c.y) > ZERO) m = true;
	m_posz.GetWindowText(s);
	if (fabs(atof(s) - c.z) > ZERO) m = true;

	switch (GetDocument()->selection->getType())
	{
	case TYPE_CUBE:
		{
			const CCube *cube = (CCube*)((CGeometry*)GetDocument()->selection)->getCSGModel();
			m_property1.GetWindowText(s);
			if (fabs(atof(s) - cube->getLength()) > ZERO) m = true;
			m_property2.GetWindowText(s);
			if (fabs(atof(s) - cube->getWidth()) > ZERO) m = true;
			m_property3.GetWindowText(s);
			if (fabs(atof(s) - cube->getHeight()) > ZERO) m = true;
			break;
		}
	case TYPE_SPHERE:
		{
			const CSphere *sphere = (CSphere*)((CGeometry*)GetDocument()->selection)->getCSGModel();
			m_property1.GetWindowText(s);
			if (fabs(atof(s) - sphere->getRadius()) > ZERO) m = true;
			break;
		}
	case TYPE_CYLINDER:
		{
			const CCylinder *cylinder = (CCylinder*)((CGeometry*)GetDocument()->selection)->getCSGModel();
			m_property1.GetWindowText(s);
			if (fabs(atof(s) - cylinder->getPRadius()) > ZERO) m = true;
			m_property2.GetWindowText(s);
			if (fabs(atof(s) - cylinder->getHeight()) > ZERO) m = true;
			break;
		}
	case TYPE_CONE:
		{
			const CCone *cone = (CCone*)((CGeometry*)GetDocument()->selection)->getCSGModel();
			m_property1.GetWindowText(s);
			if (fabs(atof(s) - cone->getBRadius()) > ZERO) m = true;
			m_property2.GetWindowText(s);
			if (fabs(atof(s) - cone->getTRadius()) > ZERO) m = true;
			m_property3.GetWindowText(s);
			if (fabs(atof(s) - cone->getHeight()) > ZERO) m = true;
			break;
		}
	case TYPE_LIGHT:
		{
			CLight *light = (CLight*)GetDocument()->selection;
			m_property1.GetWindowText(s);
			if (fabs(atof(s) - light->getIntension()) > ZERO) m = true;
			bool cs = (m_castshadow.GetCurSel() == 0);
			if (cs != light->getCastShadow()) m = true;
			break;
		}
	}

	m_apply.EnableWindow(m);
}

void CToolForm::OnChangePosX() 
{
	updateApply();
}

void CToolForm::OnChangePosY() 
{
	updateApply();
}

void CToolForm::OnChangePosZ() 
{
	updateApply();
}

void CToolForm::OnChangeProperty1() 
{
	updateApply();
}

void CToolForm::OnChangeProperty2() 
{
	updateApply();
}

void CToolForm::OnChangeProperty3() 
{
	updateApply();
}

void CToolForm::OnSelchangeCastshadow() 
{
	updateApply();
}

void CToolForm::OnApply() 
{
	CString s;
	m_name.GetWindowText(s);
	GetDocument()->selection->name = s;

	if (GetDocument()->selection->getType() == TYPE_LIGHT) ((CLight*)GetDocument()->selection)->setColor(m_color.color);
	else ((CGeometry*)GetDocument()->selection)->material.color = m_color.color;

	m_posx.GetWindowText(s);
	double x = atof(s);
	m_posy.GetWindowText(s);
	double y = atof(s);
	m_posz.GetWindowText(s);
	double z = atof(s);
	CVertex c(x, y, z);

	if (GetDocument()->selection->getType() == TYPE_LIGHT) ((CLight*)GetDocument()->selection)->setPosition(c);
	else if (GetDocument()->selection->getType() == TYPE_CONE)
	{
		CVertex oc = ((CCone*)((CGeometry*)GetDocument()->selection)->getCSGModel())->getBCenter();
		((CGeometry*)GetDocument()->selection)->move(c - oc);
	}
	else
	{
		CVertex oc = ((CGeometry*)GetDocument()->selection)->getCSGModel()->getCenter();
		((CGeometry*)GetDocument()->selection)->move(c - oc);
	}

	switch (GetDocument()->selection->getType())
	{
	case TYPE_CUBE:
		{
			CCube *cube = (CCube*)((CGeometry*)GetDocument()->selection)->getCSGModel();
			m_property1.GetWindowText(s);
			double l = fabs(atof(s));
			m_property2.GetWindowText(s);
			double w = fabs(atof(s));
			m_property3.GetWindowText(s);
			double h = fabs(atof(s));
			cube->setLength(l);
			cube->setWidth(w);
			cube->setHeight(h);
			((CGeometry*)GetDocument()->selection)->recreateSurfaceModel();
			break;
		}
	case TYPE_SPHERE:
		{
			CSphere *sphere = (CSphere*)((CGeometry*)GetDocument()->selection)->getCSGModel();
			m_property1.GetWindowText(s);
			double r = fabs(atof(s));
			sphere->setRadius(r);
			((CGeometry*)GetDocument()->selection)->recreateSurfaceModel();
			break;
		}
	case TYPE_CYLINDER:
		{
			CCylinder *cylinder = (CCylinder*)((CGeometry*)GetDocument()->selection)->getCSGModel();
			m_property1.GetWindowText(s);
			double pr = fabs(atof(s));
			m_property2.GetWindowText(s);
			double h = fabs(atof(s));
			cylinder->setPRadius(pr);
			cylinder->setHeight(h);
			((CGeometry*)GetDocument()->selection)->recreateSurfaceModel();
			break;
		}
	case TYPE_CONE:
		{
			CCone *cone = (CCone*)((CGeometry*)GetDocument()->selection)->getCSGModel();
			m_property1.GetWindowText(s);
			double br = fabs(atof(s));
			m_property2.GetWindowText(s);
			double tr = fabs(atof(s));
			m_property3.GetWindowText(s);
			double h = fabs(atof(s));
			cone->setBRadius(br);
			cone->setTRadius(tr);
			cone->setHeight(h);
			((CGeometry*)GetDocument()->selection)->recreateSurfaceModel();
			break;
		}
	case TYPE_LIGHT:
		{
			CLight *light = (CLight*)GetDocument()->selection;
			m_property1.GetWindowText(s);
			double i = atof(s);
			if (i < 0) i = 0;
			if (i > 1) i = 1;
			light->setIntension(i);
			bool cs = (m_castshadow.GetCurSel() == 0);
			light->setCastShadow(cs);
			break;
		}
	}

	GetDocument()->SetModifiedFlag();
	m_apply.EnableWindow(FALSE);
	GetDocument()->UpdateAllViews(NULL);
}

void CToolForm::OnMaterial() 
{
	if (!GetDocument()->selection || GetDocument()->selection->getType() == TYPE_LIGHT) return;

	CEditMaterial dlg;
	CGeometry *g = (CGeometry*)GetDocument()->selection;

	dlg.m_ka = g->material.ka;
	dlg.m_kd = g->material.kd;
	dlg.m_ks = g->material.ks;
	dlg.m_kr = g->material.kr;
	dlg.m_kt = g->material.kt;
	dlg.m_n = g->material.n;
	dlg.m_rr = g->material.rr;
	dlg.m_vtex = (g->material.texture != NULL && g->material.texture->getTexture());
	dlg.m_vbump = (g->material.texture != NULL && g->material.texture->getBump());

	CDC *pDC = GetDC();

	if (g->material.texture && g->material.texture->getTexture())
	{
		dlg.bitmap = g->material.texture->createBitmap(pDC->GetSafeHdc());
		dlg.m_vscale = g->material.texture->getScale();
	}
	else dlg.m_vscale = 1;

	if (g->material.texture && g->material.texture->getBump())
	{
		dlg.bumpBitmap = g->material.texture->createBumpBitmap(pDC->GetSafeHdc());
		dlg.m_vbr = g->material.texture->getBumpRate();
		dlg.m_vbscale = g->material.texture->getBumpScale();
	}
	else dlg.m_vbscale = dlg.m_vbr = 1;

	ReleaseDC(pDC);

	if (dlg.DoModal() == IDOK)
	{
		g->material.ka = dlg.m_ka;
		g->material.kd = dlg.m_kd;
		g->material.ks = dlg.m_ks;
		g->material.kr = dlg.m_kr;
		g->material.kt = dlg.m_kt;
		g->material.n = dlg.m_n;
		g->material.rr = dlg.m_rr;
		if (dlg.m_vtex || dlg.m_vbump)
		{
			if (!g->material.texture) g->material.texture = new CTexture;
			if (dlg.m_vtex)
			{
				g->material.texture->setTexture(true);
				g->material.texture->loadImage(dlg.bitmap);
				g->material.texture->setScale(dlg.m_vscale);
			}
			else g->material.texture->setTexture(false);
			if (dlg.m_vbump)
			{
				g->material.texture->setBump(true);
				g->material.texture->loadBumpMapping(dlg.bumpBitmap);
				g->material.texture->setBumpRate(dlg.m_vbr);
				g->material.texture->setBumpScale(dlg.m_vbscale);
			}
			else g->material.texture->setBump(false);
		}
		else if (g->material.texture)
		{
			delete g->material.texture;
			g->material.texture = NULL;
		}

		GetDocument()->SetModifiedFlag();
	}
	if (dlg.load) DeleteObject(dlg.bitmap);
	if (dlg.loadbump) DeleteObject(dlg.bumpBitmap);
}