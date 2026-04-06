// ivExplorerDoc.h : interface of the CIvExplorerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IVEXPLORERDOC_H__5EAAC64E_B019_11D3_8E75_0080C74A05BC__INCLUDED_)
#define AFX_IVEXPLORERDOC_H__5EAAC64E_B019_11D3_8E75_0080C74A05BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIvExplorerDoc : public CDocument
{
protected: // create from serialization only
	CIvExplorerDoc();
	DECLARE_DYNCREATE(CIvExplorerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIvExplorerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument( LPCTSTR lpszPathName );
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Status(LPCTSTR m);
	void SetSBar(CStatusBar *sb);
	CStatusBar *sBar;
	CWnd *DispBuff;
	bool OpenDBF(LPCSTR szfname);
	bool OpenFile(LPCSTR fname, CWnd *wnd);
	virtual ~CIvExplorerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CIvExplorerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVEXPLORERDOC_H__5EAAC64E_B019_11D3_8E75_0080C74A05BC__INCLUDED_)
