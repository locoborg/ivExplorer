# Microsoft Developer Studio Project File - Name="ivExplorer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ivExplorer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ivExplorer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ivExplorer.mak" CFG="ivExplorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ivExplorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ivExplorer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ivExplorer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ivio.lib search.lib misc.lib control.lib mgmt.lib image.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ivExplorer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ivio.lib search.lib misc.lib control.lib mgmt.lib image.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ivExplorer - Win32 Release"
# Name "ivExplorer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\avidlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DispImg.cpp
# End Source File
# Begin Source File

SOURCE=.\ivExplorer.cpp
# End Source File
# Begin Source File

SOURCE=.\ivExplorer.rc
# End Source File
# Begin Source File

SOURCE=.\ivExplorerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ivExplorerView.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\avidlg.h
# End Source File
# Begin Source File

SOURCE=.\DispImg.h
# End Source File
# Begin Source File

SOURCE=.\ivExplorer.h
# End Source File
# Begin Source File

SOURCE=.\ivExplorerDoc.h
# End Source File
# Begin Source File

SOURCE=.\ivExplorerView.h
# End Source File
# Begin Source File

SOURCE=.\LeftView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bank.ico
# End Source File
# Begin Source File

SOURCE=.\res\bin.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Branch.ico
# End Source File
# Begin Source File

SOURCE=.\res\Checkboo.ico
# End Source File
# Begin Source File

SOURCE=.\res\command.ICO
# End Source File
# Begin Source File

SOURCE=.\res\data.ico
# End Source File
# Begin Source File

SOURCE=.\res\etc.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Explorer.ico
# End Source File
# Begin Source File

SOURCE=.\res\Eye.ico
# End Source File
# Begin Source File

SOURCE=.\res\Files.ico
# End Source File
# Begin Source File

SOURCE=.\res\folder.ico
# End Source File
# Begin Source File

SOURCE=.\res\Forms.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Info.ico
# End Source File
# Begin Source File

SOURCE=.\res\ivExplorer.ico
# End Source File
# Begin Source File

SOURCE=.\res\ivExplorer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ivExplorerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\log.ICO
# End Source File
# Begin Source File

SOURCE=.\res\logos.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Mail.ico
# End Source File
# Begin Source File

SOURCE=.\res\Mails.ico
# End Source File
# Begin Source File

SOURCE=.\res\mfiles.ico
# End Source File
# Begin Source File

SOURCE=.\res\parms.ico
# End Source File
# Begin Source File

SOURCE=.\res\parmsf.ICO
# End Source File
# Begin Source File

SOURCE=.\res\reports.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Sequence.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Server.ico
# End Source File
# Begin Source File

SOURCE=.\res\sounds.ICO
# End Source File
# Begin Source File

SOURCE=.\res\spool.ico
# End Source File
# Begin Source File

SOURCE=.\res\spoolf.ICO
# End Source File
# Begin Source File

SOURCE=.\res\System.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\Filecopy.avi
# End Source File
# Begin Source File

SOURCE=.\res\Filedel.avi
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
