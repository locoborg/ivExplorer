// ivExplorerView.h : interface of the CIvExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IVEXPLORERVIEW_H__5EAAC650_B019_11D3_8E75_0080C74A05BC__INCLUDED_)
#define AFX_IVEXPLORERVIEW_H__5EAAC650_B019_11D3_8E75_0080C74A05BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CIvExplorerView : public CListView
{
protected: // create from serialization only
	CIvExplorerView();
	DECLARE_DYNCREATE(CIvExplorerView)

// Attributes
public:
	bool flDragL, flDragR;
	CMainFrame *pDad;
	CIvExplorerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIvExplorerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CPoint pt;
	bool flMeDraging;
	bool flShowingFile;
	void SetDad(CMainFrame *pap);
	virtual ~CIvExplorerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult);
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CIvExplorerView)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ivExplorerView.cpp
inline CIvExplorerDoc* CIvExplorerView::GetDocument()
   { return (CIvExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVEXPLORERVIEW_H__5EAAC650_B019_11D3_8E75_0080C74A05BC__INCLUDED_)
