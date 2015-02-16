; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLoftDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "3DCreator.h"
LastPage=0

ClassCount=14
Class1=CMy3DCreatorApp
Class2=CMy3DCreatorDoc
Class3=CMy3DCreatorView
Class4=CMainFrame

ResourceCount=13
Resource1=IDD_SCENE_PREFERENCE
Resource2=IDD_EDIT_MATERIAL
Class5=CAboutDlg
Class6=CToolForm
Class7=CColorButton
Class8=CMyButton
Resource3=IDD_LOFT
Resource4=IDD_ABOUTBOX
Resource5=IDD_TOOLFORM
Class9=CLoftDialog
Resource6=IDR_POPUP_MENU
Resource7=IDD_LOFTLENGTH
Class10=CEditMaterial
Resource8=IDD_RENDER
Class11=CLoftLengthDialog
Resource9=IDD_RENDER_RESULT
Class12=CScenePreference
Resource10=IDR_MAINFRAME
Class13=CRenderDialog
Resource11=IDR_RENDER
Class14=CRenderResult
Resource12=IDR_LOFT_MENU
Resource13=IDR_MAINFRAME (English (U.S.))

[CLS:CMy3DCreatorApp]
Type=0
HeaderFile=3DCreator.h
ImplementationFile=3DCreator.cpp
Filter=N

[CLS:CMy3DCreatorDoc]
Type=0
HeaderFile=3DCreatorDoc.h
ImplementationFile=3DCreatorDoc.cpp
Filter=N
LastObject=CMy3DCreatorDoc

[CLS:CMy3DCreatorView]
Type=0
HeaderFile=3DCreatorView.h
ImplementationFile=3DCreatorView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_WIREFRAME


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=3DCreator.cpp
ImplementationFile=3DCreator.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_CUT
Command8=ID_EDIT_COPY
Command9=ID_EDIT_PASTE
Command10=ID_EDIT_DELETE
Command11=ID_EDIT_MOVE
Command12=ID_EDIT_ROTATE
Command13=ID_EDIT_SCALE
Command14=ID_EDIT_LOFT
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_VIEW_MOVE
Command18=ID_VIEW_ROTATE
Command19=ID_VIEW_ZOOM
Command20=ID_VIEW_SCALE
Command21=ID_SCENE_PREFERENCE
Command22=ID_SCENE_RENDER
Command23=ID_APP_ABOUT
CommandCount=23

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[DLG:IDD_TOOLFORM]
Type=1
Class=CToolForm
ControlCount=33
Control1=IDC_NEW_CUBE,button,1342242827
Control2=IDC_NEW_CONE,button,1342242827
Control3=IDC_NEW_SPHERE,button,1342242827
Control4=IDC_NEW_CYLINDER,button,1342242827
Control5=IDC_INTERSECT,button,1342242827
Control6=IDC_DIFFERENCE,button,1342242827
Control7=IDC_UNION,button,1342242827
Control8=IDC_SELECT_CREATE,combobox,1344339971
Control9=IDC_SELECT_BOOL_OBJECT,button,1342242827
Control10=IDC_POSITION,button,1342177287
Control11=IDC_STATIC_POS_X,static,1342308352
Control12=IDC_POS_X,edit,1350631552
Control13=IDC_STATIC_POS_Y,static,1342308352
Control14=IDC_POS_Y,edit,1350631552
Control15=IDC_STATIC_POS_Z,static,1342308352
Control16=IDC_POS_Z,edit,1350631552
Control17=IDC_NEW_LIGHT,button,1342242827
Control18=IDC_PROPERTY,button,1342177287
Control19=IDC_PROPERTY1,edit,1350631552
Control20=IDC_PROPERTY2,edit,1350631552
Control21=IDC_PROPERTY3,edit,1350631552
Control22=IDC_STATIC_PROPERTY1,static,1342308352
Control23=IDC_STATIC_PROPERTY2,static,1342308352
Control24=IDC_STATIC_PROPERTY3,static,1342308352
Control25=IDC_BASIC,button,1342177287
Control26=IDC_STATIC_NAME,static,1342308352
Control27=IDC_STATIC_COLOR,static,1342308352
Control28=IDC_NAME,edit,1350631552
Control29=IDC_COLOR,button,1342242827
Control30=IDC_CASTSHADOW,combobox,1344339971
Control31=IDC_MATERIAL,button,1342242816
Control32=IDC_APPLY,button,1342242816
Control33=IDC_TAB,SysTabControl32,1342177280

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_DELETE
Command9=ID_EDIT_CUT
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_EDIT_CUT
Command15=ID_EDIT_UNDO
CommandCount=15

[CLS:CToolForm]
Type=0
HeaderFile=ToolForm.h
ImplementationFile=ToolForm.cpp
BaseClass=CFormView
Filter=D
LastObject=CToolForm
VirtualFilter=VWC

[CLS:CColorButton]
Type=0
HeaderFile=ColorButton.h
ImplementationFile=ColorButton.cpp
BaseClass=CButton
Filter=W
LastObject=CColorButton
VirtualFilter=BWC

[CLS:CMyButton]
Type=0
HeaderFile=MyButton.h
ImplementationFile=MyButton.cpp
BaseClass=CButton
Filter=W
VirtualFilter=BWC
LastObject=CMyButton

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_DELETE
Command8=ID_EDIT_LOFT
Command9=ID_SELECTING
Command10=ID_VIEW_MOVE
Command11=ID_VIEW_ROTATE
Command12=ID_VIEW_ZOOM
Command13=ID_VIEW_SCALE
Command14=ID_EDIT_MOVE
Command15=ID_EDIT_ROTATE
Command16=ID_EDIT_SCALE
Command17=ID_SCENE_RENDER
Command18=ID_APP_ABOUT
CommandCount=18

[MNU:IDR_POPUP_MENU]
Type=1
Class=CMy3DCreatorView
Command1=ID_WIREFRAME
Command2=ID_SMOOTH
Command3=ID_ZOOM
Command4=ID_ELIMINATE
Command5=ID_FRONT
Command6=ID_BACK
Command7=ID_TOP
Command8=ID_BOTTOM
Command9=ID_LEFT
Command10=ID_RIGHT
Command11=ID_PERSPECTIVE
CommandCount=11

[DLG:IDD_LOFT]
Type=1
Class=CLoftDialog
ControlCount=3
Control1=IDC_POSX,button,1342242827
Control2=IDC_POSY,button,1342242827
Control3=IDC_STATUS,button,1342242827

[CLS:CLoftDialog]
Type=0
HeaderFile=LoftDialog.h
ImplementationFile=LoftDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_LOFT_DRAWSHAPE
VirtualFilter=dWC

[MNU:IDR_LOFT_MENU]
Type=1
Class=?
Command1=ID_CIRCLELOFT
Command2=ID_LINELOFT
Command3=ID_EXITLOFT
Command4=ID_LOFT_DRAWSHAPE
Command5=ID_LOFT_EDIT
Command6=ID_LOFT_MOVE
Command7=ID_LOFT_ROTATE
Command8=ID_LOFT_SCALE
Command9=ID_LOFT_MOVEVIEW
Command10=ID_LOFT_ZOOMVIEW
CommandCount=10

[DLG:IDD_EDIT_MATERIAL]
Type=1
Class=CEditMaterial
ControlCount=28
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_KA,edit,1350631552
Control11=IDC_KD,edit,1350631552
Control12=IDC_KS,edit,1350631552
Control13=IDC_N,edit,1350631552
Control14=IDC_KR,edit,1350631552
Control15=IDC_KT,edit,1350631552
Control16=IDC_RR,edit,1350631552
Control17=IDC_TEXTURE,button,1342242819
Control18=IDC_MAP,static,1342177294
Control19=IDC_STATIC_SCALE,static,1342308352
Control20=IDC_SCALE,edit,1350631552
Control21=IDC_BUMP,button,1342242819
Control22=IDC_STATIC_BR,static,1342308352
Control23=IDC_BR,edit,1350631552
Control24=IDC_LOADMAP,button,1342242816
Control25=IDC_LOADBUMP,button,1342242816
Control26=IDC_BUMPMAP,static,1342177294
Control27=IDC_SBSCALE,static,1342308352
Control28=IDC_BSCALE,edit,1350631552

[CLS:CEditMaterial]
Type=0
HeaderFile=EditMaterial.h
ImplementationFile=EditMaterial.cpp
BaseClass=CDialog
Filter=D
LastObject=CEditMaterial
VirtualFilter=dWC

[DLG:IDD_LOFTLENGTH]
Type=1
Class=CLoftLengthDialog
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LENGTH,edit,1350631552

[CLS:CLoftLengthDialog]
Type=0
HeaderFile=LoftLengthDialog.h
ImplementationFile=LoftLengthDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CLoftLengthDialog

[DLG:IDD_SCENE_PREFERENCE]
Type=1
Class=CScenePreference
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_AINTENSION,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_ACOLOR,button,1342242827
Control7=IDC_BACKMAP,button,1342242819
Control8=IDC_LOADBACK,button,1342242816
Control9=IDC_BKCOLOR,button,1342242827
Control10=IDC_STATIC,static,1342308352
Control11=IDC_BACKBITMAP,static,1342177294
Control12=IDC_STATIC_SCALE,static,1342308352
Control13=IDC_SCALE,edit,1350631552

[CLS:CScenePreference]
Type=0
HeaderFile=ScenePreference.h
ImplementationFile=ScenePreference.cpp
BaseClass=CDialog
Filter=D
LastObject=CScenePreference
VirtualFilter=dWC

[DLG:IDD_RENDER]
Type=1
Class=CRenderDialog
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_WIDTH,edit,1350631552
Control6=IDC_HEIGHT,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_VIEW,combobox,1344339971
Control9=IDC_STATIC,static,1342308352
Control10=IDC_ANTIALIAS,combobox,1344339971

[CLS:CRenderDialog]
Type=0
HeaderFile=RenderDialog.h
ImplementationFile=RenderDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CRenderDialog
VirtualFilter=dWC

[DLG:IDD_RENDER_RESULT]
Type=1
Class=CRenderResult
ControlCount=0

[CLS:CRenderResult]
Type=0
HeaderFile=RenderResult.h
ImplementationFile=RenderResult.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_RENDER_CLOSE
VirtualFilter=dWC

[MNU:IDR_RENDER]
Type=1
Class=?
Command1=ID_RENDER_SAVE
Command2=ID_RENDER_CLOSE
CommandCount=2

