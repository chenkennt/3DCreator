# Microsoft Developer Studio Project File - Name="3DCreator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=3DCreator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "3DCreator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "3DCreator.mak" CFG="3DCreator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "3DCreator - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "3DCreator - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "3DCreator - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "3DCreator - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "3DCreator - Win32 Release"
# Name "3DCreator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3DCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\3DCreator.rc
# End Source File
# Begin Source File

SOURCE=.\3DCreatorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\3DCreatorView.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\BoolObject.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\CircleLoftObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Cone.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\CSGModel.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Cube.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Cylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\DifferenceObject.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Edge.cpp
# End Source File
# Begin Source File

SOURCE=.\EditMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\GraphicObject.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\IntersectObject.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Light.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\LineLoftObject.cpp
# End Source File
# Begin Source File

SOURCE=.\LoftDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LoftLengthDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\MyButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderResult.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\ScenePreference.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Shape.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Surface.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\SurfaceModel.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolForm.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\UnionObject.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\UserView.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Vector.cpp
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Vertex.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3DCreator.h
# End Source File
# Begin Source File

SOURCE=.\3DCreatorDoc.h
# End Source File
# Begin Source File

SOURCE=.\3DCreatorView.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\BoolObject.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\CircleLoftObject.h
# End Source File
# Begin Source File

SOURCE=.\ColorButton.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Cone.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Constants.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\CSGModel.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Cube.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Cylinder.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\DifferenceObject.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Edge.h
# End Source File
# Begin Source File

SOURCE=.\EditMaterial.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Geometry.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\GraphicObject.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\IntersectObject.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Light.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\LineLoftObject.h
# End Source File
# Begin Source File

SOURCE=.\LoftDialog.h
# End Source File
# Begin Source File

SOURCE=.\LoftLengthDialog.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Material.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\MyButton.h
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\RenderDialog.h
# End Source File
# Begin Source File

SOURCE=.\RenderResult.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Scene.h
# End Source File
# Begin Source File

SOURCE=.\ScenePreference.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Shape.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Sphere.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Surface.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\SurfaceModel.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Texture.h
# End Source File
# Begin Source File

SOURCE=.\ToolForm.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\UnionObject.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\UserView.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Vector.h
# End Source File
# Begin Source File

SOURCE=.\3DLibrary\Vertex.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\3DCreator.ico
# End Source File
# Begin Source File

SOURCE=.\res\3DCreator.rc2
# End Source File
# Begin Source File

SOURCE=.\res\3DCreatorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\3DCreator.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
