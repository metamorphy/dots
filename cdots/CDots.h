// CDots.h : main header file for the CDOTS application
//

#if !defined(AFX_CDOTS_H__5C25EB93_DC7D_4B73_A530_4066FC8456BD__INCLUDED_)
#define AFX_CDOTS_H__5C25EB93_DC7D_4B73_A530_4066FC8456BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCDotsApp:
// See CDots.cpp for the implementation of this class
//

class CCDotsApp : public CWinApp
{
public:
	CCDotsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDotsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCDotsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDOTS_H__5C25EB93_DC7D_4B73_A530_4066FC8456BD__INCLUDED_)
