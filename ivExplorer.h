// ivExplorer.h : main header file for the IVEXPLORER application
//

#if !defined(AFX_IVEXPLORER_H__5EAAC648_B019_11D3_8E75_0080C74A05BC__INCLUDED_)
#define AFX_IVEXPLORER_H__5EAAC648_B019_11D3_8E75_0080C74A05BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerApp:
// See ivExplorer.cpp for the implementation of this class
//

class CIvExplorerApp : public CWinApp
{
public:
	int nodeTop;
	int nodeBott;

	CIvExplorerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIvExplorerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CIvExplorerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVEXPLORER_H__5EAAC648_B019_11D3_8E75_0080C74A05BC__INCLUDED_)
