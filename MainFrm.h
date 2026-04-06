// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__5EAAC64C_B019_11D3_8E75_0080C74A05BC__INCLUDED_)
#define AFX_MAINFRM_H__5EAAC64C_B019_11D3_8E75_0080C74A05BC__INCLUDED_

#include "avidlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Image Const from ImgList
#define UNKNOWN		0
#define SERVER		1
#define NETCOMP		2
#define BANK		3
#define VISION		4
#define BIN			5
#define COMMAND		6
#define ETC			7
#define LOG			8
#define PARMSV		9
#define REPORTS		10
#define SEQUENCE	11
#define SOUNDS		12
#define SPOOLV		13
#define DATA		14
#define FORMS		15
#define LOGOS		16
#define PARMS		17
#define FILES		18
#define SPOOL		19
#define DATE		20
#define DBF			21
#define I00			22
#define J00			23
// View Const
#define TREE	false
#define LIST	true
// Img List Const
#define VLIST	"03BANK05BIN06COMMAND07ETC08LOG09PARMS10REPORTS11SEQUENCE12SOUNDS13SPOOL"
#define OLIST	"14DATA15FORMS16LOGOS17PARMS18FILES19SPOOL"
#define FLIST	"21DBF22I0023J00"

#define MAXHIST 256

#define IFILE 0x0000
#define IDIR  0x0001

class NodeItm {
 public:
	HTREEITEM node;
	bool on;
};

class CIvExplorerView;
class CLeftView;
class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CIVBar 

class CIVBar : public CReBar
{
public:
	CIVBar();
	CMainFrame *pDad;
	void SetDad(CMainFrame *pap);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVBar)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame 


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void FileMan(LPCSTR szDst, LPCSTR szSrc, bool copy);
	int nSelFiles;
	int *fpos;
	CPoint pt;
	CImageList* dListImg;
	HTREEITEM dto;
	void OnVFOP();
	Cavidlg *avidlg;
	bool DragOp,VFileOp;
	CMenu mnMoC;
	void OnRefresh();
	void OnEditCut();
	void CopyDir(LPCSTR szDir, LPCSTR szDst);
	void KillDir(LPCSTR szDir);
	void CopySel(LPCSTR szSrc, LPCSTR szDst, bool coping);
	void Kill(LPCSTR szScr);
	void Copy(LPCSTR szSrc, LPCSTR szDst);
	char *clipBoard;
	void OnEditPaste();
	void OnEditCopy();
	bool flHist;
	BYTE curNode;
	NodeItm *nodeHist;
	void OnFwd();
	void OnBack();
	void OnUp();
	void ResetCols();
	bool flCols;
	void ShowItems(bool type,bool flRefr);
	void AddItem(HTREEITEM pap, LPCSTR name, bool type, int idx, int img, bool erase);
	void SetImgLst(CWnd *view, DWORD dwStyle, int size, bool Type);
	void Status(LPCTSTR m);
	int DirAdd(HTREEITEM pap, PVOID ivDir, long nItems, LPCSTR szExclude, LPCSTR Img2Items, int defImg, bool tree, bool erase);
	CToolTipCtrl toolTipsTree;
	CDialogBar m_wndDlgBar;
	CStatusBar  m_wndStatusBar;
	void LoadServers();
	virtual ~CMainFrame();
	CIvExplorerView* GetRightPane();
	CLeftView* GetLeftPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;
	CIVBar      m_wndReBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__5EAAC64C_B019_11D3_8E75_0080C74A05BC__INCLUDED_)
