// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "MisaGuitar.h"
#include "ChildView.h"

#include "GUI.h"
#include "LCDFrameBuffer.h"
#include "ucGuiMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MisaGuitar

static COLORREF FrameBuffer[800*600];

extern BYTE unThreadRun;

#ifdef __cplusplus
extern "C"
{
#endif

CChildView* pView = NULL;

void DisplayUpdate()
{
	if(pView)
	{
		pView->UpdateDisplay();
	}
}

#ifdef __cplusplus
}
#endif

UINT ucGUI_Thread(LPVOID pPara)
{
	UINT ret = KitaraMenu();
	//return ucGuiMain(pPara);
	if(pView)
	{
		pView->Invalidate(FALSE);
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
#if 1
	CDC dcMemory;
	BITMAP bmpInfo;
	m_bmp.SetBitmapBits(sizeof(FrameBuffer),FrameBuffer);
	m_bmp.GetBitmap(&bmpInfo);
	dcMemory.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = dcMemory.SelectObject(&m_bmp);
	dc.BitBlt(0,0,bmpInfo.bmWidth, bmpInfo.bmHeight,&dcMemory,0,0,SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);
#endif
	// Do not call CWnd::OnPaint() for painting messages
}


BOOL CChildView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	pView = this;
	ZeroMemory(FrameBuffer,sizeof(FrameBuffer));
	m_bmp.CreateBitmap(800,600,1,32,NULL);
	m_bmp.SetBitmapBits(sizeof(FrameBuffer),FrameBuffer);
	SetLcdFrameBuffer(FrameBuffer);
	unThreadRun = TRUE;
	m_MouseDown = FALSE;
	pGuiThread = NULL;
	KitaraInit();
	pGuiThread = ::AfxBeginThread(ucGUI_Thread,0);
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CChildView::OnDestroy()
{
	unThreadRun = FALSE;
	::WaitForSingleObject(pGuiThread->m_hThread,5000);
	m_bmp.DeleteObject();
	KitaraExit();
}

void CChildView::UpdateDisplay()
{
#if 0
	CDC dcMemory;
	BITMAP bmpInfo;
	m_bmp.SetBitmapBits(sizeof(FrameBuffer),FrameBuffer);
	m_bmp.GetBitmap(&bmpInfo);
	CDC* pDc = GetDC();
	dcMemory.CreateCompatibleDC(pDc);
	CBitmap* pOldBitmap = dcMemory.SelectObject(&m_bmp);
	pDc->BitBlt(0,0,bmpInfo.bmWidth, bmpInfo.bmHeight,&dcMemory,0,0,SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);
	ReleaseDC(pDc);
#else
	if(unThreadRun)
	{
		Invalidate(FALSE);
	}
#endif
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	GUI_PID_STATE pid_state;
	pid_state.Pressed = TRUE;
	pid_state.x = point.x;
	pid_state.y = point.y;
	if(GetLcdFlip())
	{
		pid_state.x = 800-pid_state.x;
		pid_state.y = 600-pid_state.y;
	}
	GUI_TOUCH_StoreStateEx(&pid_state);
	m_MouseDown = TRUE;
	CWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	GUI_PID_STATE pid_state;
	m_MouseDown = FALSE;
	pid_state.Pressed = FALSE;
	pid_state.x = point.x;
	pid_state.y = point.y;
	if(GetLcdFlip())
	{
		pid_state.x = 800-pid_state.x;
		pid_state.y = 600-pid_state.y;
	}
	GUI_TOUCH_StoreStateEx(&pid_state);
	CWnd ::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	GUI_PID_STATE pid_state;
	if(m_MouseDown)
	{
		pid_state.Pressed = TRUE;
		pid_state.x = point.x;
		pid_state.y = point.y;
		if(GetLcdFlip())
		{
			pid_state.x = 800-pid_state.x;
			pid_state.y = 600-pid_state.y;
		}
		GUI_TOUCH_StoreStateEx(&pid_state);
	}
	CWnd ::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return 0;
	//return CWnd ::OnEraseBkgnd(pDC);
}

void CChildView::StartSimulation()
{
	if(unThreadRun)
	{
		return;
	}
	unThreadRun = 1;
	pGuiThread = ::AfxBeginThread(ucGUI_Thread,0);
}

void CChildView::StopSimulation()
{
	unThreadRun = FALSE;
	::WaitForSingleObject(pGuiThread->m_hThread,5000);
}


void CChildView::StartQuickset()
{
}
