; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=Cavidlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ivExplorer.h"
LastPage=0

ClassCount=8
Class1=CIvExplorerApp
Class2=CIvExplorerDoc
Class3=CIvExplorerView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDD_DIALOG1
Class5=CLeftView
Class6=CAboutDlg
Class7=CDispImg
Resource3=IDR_MAINFRAME
Resource4=IDR_MENU1
Class8=Cavidlg
Resource5=IDD_DIALOGTMP

[CLS:CIvExplorerApp]
Type=0
HeaderFile=ivExplorer.h
ImplementationFile=ivExplorer.cpp
Filter=N

[CLS:CIvExplorerDoc]
Type=0
HeaderFile=ivExplorerDoc.h
ImplementationFile=ivExplorerDoc.cpp
Filter=C

[CLS:CIvExplorerView]
Type=0
HeaderFile=ivExplorerView.h
ImplementationFile=ivExplorerView.cpp
Filter=W
BaseClass=CListView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC



[CLS:CLeftView]
Type=0
HeaderFile=LeftView.h
ImplementationFile=LeftView.cpp
Filter=T
BaseClass=CTreeView
VirtualFilter=VWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ivExplorer.cpp
ImplementationFile=ivExplorer.cpp
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
Command1=ID_FILE_OPEN
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_PREVIEW
Command4=ID_FILE_PRINT_SETUP
Command5=ID_FILE_SEND_MAIL
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_VIEW_FILEOPREPORT
Command14=ID_APP_ABOUT
CommandCount=14

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDR_MAINFRAME]
Type=1
Class=?
ControlCount=2
Control1=IDC_PATH,edit,1350633600
Control2=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_BACK
Command2=ID_FILE_FWD
Command3=ID_FILE_UP
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_VIEW_LARGEICON
Command9=ID_VIEW_SMALLICON
Command10=ID_VIEW_LIST
Command11=ID_VIEW_DETAILS
Command12=ID_APP_ABOUT
Command13=ID_REFRESH
CommandCount=13

[DLG:IDD_DIALOG1]
Type=1
Class=CDispImg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDOK2,button,1342242817
Control3=IDOK3,button,1342242817
Control4=IDOK4,button,1342242817
Control5=IDC_COUNT,static,1342308353
Control6=IDOK5,button,1073807361
Control7=IDOK6,button,1073807361
Control8=IDC_IMG,static,1342181902
Control9=IDC_LIST1,listbox,1352749313

[CLS:CDispImg]
Type=0
HeaderFile=DispImg.h
ImplementationFile=DispImg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDispImg
VirtualFilter=dWC

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_COPY
Command2=ID_MOVE
Command3=ID_CANCEL
CommandCount=3

[DLG:IDD_DIALOGTMP]
Type=1
Class=Cavidlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_AVI,SysAnimate32,1350631431
Control3=IDC_PROGRESS1,msctls_progress32,1350565888
Control4=IDC_LIST1,listbox,1353797889

[CLS:Cavidlg]
Type=0
HeaderFile=avidlg.h
ImplementationFile=avidlg.cpp
BaseClass=CDialog
Filter=D
LastObject=Cavidlg
VirtualFilter=dWC

