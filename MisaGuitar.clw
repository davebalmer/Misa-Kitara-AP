; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChildView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MisaGuitar.h"
LastPage=0

ClassCount=4
Class1=CMisaGuitarApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=2
Resource1=IDD_ABOUTBOX
Class2=CChildView
Resource2=IDR_MAINFRAME

[CLS:CMisaGuitarApp]
Type=0
HeaderFile=MisaGuitar.h
ImplementationFile=MisaGuitar.cpp
Filter=N

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=C
BaseClass=CWnd 
VirtualFilter=WC
LastObject=ID_APP_PAUSE

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_APP_PAUSE




[CLS:CAboutDlg]
Type=0
HeaderFile=MisaGuitar.cpp
ImplementationFile=MisaGuitar.cpp
Filter=D

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
Command1=ID_APP_EXIT
Command2=ID_VIEW_TOOLBAR
Command3=ID_VIEW_STATUS_BAR
Command4=ID_APP_ABOUT
CommandCount=4

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_START
Command2=ID_APP_STOP
Command3=ID_APP_PAUSE
Command4=ID_APP_ABOUT
CommandCount=4

