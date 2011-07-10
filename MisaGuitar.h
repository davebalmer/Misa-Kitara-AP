// MisaGuitar.h : main header file for the MISAGUITAR application
//

#if !defined(AFX_MISAGUITAR_H__3E47222E_E826_4B58_B8D4_6EFDB6B1B277__INCLUDED_)
#define AFX_MISAGUITAR_H__3E47222E_E826_4B58_B8D4_6EFDB6B1B277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMisaGuitarApp:
// See MisaGuitar.cpp for the implementation of this class
//

class CMisaGuitarApp : public CWinApp
{
public:
	CMisaGuitarApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMisaGuitarApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CMisaGuitarApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISAGUITAR_H__3E47222E_E826_4B58_B8D4_6EFDB6B1B277__INCLUDED_)
