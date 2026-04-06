#if !defined(AFX_AVIDLG_H__1C093553_C1F5_11D3_8E92_0080C74A05BC__INCLUDED_)
#define AFX_AVIDLG_H__1C093553_C1F5_11D3_8E92_0080C74A05BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// avidlg.h : header file
//
class CMainFrame;
/////////////////////////////////////////////////////////////////////////////
// Cavidlg dialog

class Cavidlg : public CDialog
{
// Construction
public:
	CMainFrame *pDad;
	char * szSrc;
	char * szDst;
	bool flModal,flMove;
	void Stop();
	UINT nIDMov;
	void Output(LPCSTR m);
	Cavidlg(CWnd* pParent = NULL);   // standard constructor
	Cavidlg(UINT IDMOV);   // standard constructor
	Cavidlg(LPCSTR szD, LPCSTR szS, bool copying, LPVOID pap);
	void SetShowMode(LPCSTR sFrom, LPCSTR sTo);
	void CopySel();
	void Kill(LPCSTR szSrc);
// Dialog Data
	//{{AFX_DATA(Cavidlg)
	enum { IDD = IDD_DIALOGTMP };
	CListBox	m_Out;
	CButton	m_Butt;
	CProgressCtrl	m_progress;
	CAnimateCtrl	m_avi;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cavidlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool CopyDir(LPCSTR szDir, LPCSTR szDst);
	void KillDir(LPCSTR szDir);
	bool Copy(LPCSTR szSrc, LPCSTR szDst);
	// Generated message map functions
	//{{AFX_MSG(Cavidlg)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVIDLG_H__1C093553_C1F5_11D3_8E92_0080C74A05BC__INCLUDED_)
