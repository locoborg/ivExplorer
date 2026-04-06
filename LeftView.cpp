// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "ivExplorer.h"

#include "MainFrm.h"
#include "ivExplorerDoc.h"
#include "ivExplorerView.h"
#include "LeftView.h"

#include <fstream.h>

#include <ivio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginrdrag)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
	flMeDraging=true;
	flDragL=false;
	flDragR=false;
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CIvExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
	AfxMessageBox("Not supported yet!");
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CIvExplorerDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIvExplorerDoc)));
	return (CIvExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(pDad->flCols)
		pDad->ShowItems(LIST,false);
	*pResult = 0;
}

// Get parent pointer

void CLeftView::SetDad(CMainFrame *pap){
	pDad=pap;
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM node=GetTreeCtrl().GetSelectedItem();
	GetTreeCtrl().Select(node,TVGN_DROPHILITE);
	pDad->ShowItems(LIST,false);
	*pResult = 0;
}

void CLeftView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CLeftView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	pDad->ShowItems(TREE,false);
	*pResult = 0;
}


void CLeftView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM node;
	if(flMeDraging){
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		node=GetTreeCtrl().HitTest(pt);
		if(!node) return;
		pDad->dto=node;
		pDad->dListImg=GetTreeCtrl().CreateDragImage(pDad->dto);
	} else {
		pDad->dListImg->DragLeave(this);
		pDad->dListImg->EndDrag();
	}
	CPoint ptmp;
	ptmp.x=10; ptmp.y=10;
	pDad->dListImg->BeginDrag(0,ptmp);
	pDad->dListImg->DragEnter(this,pt);
	pDad->DragOp=true;
	flDragL=true;
	pDad->GetRightPane()->GetListCtrl().SetExtendedStyle(LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	GetTreeCtrl().ModifyStyle(0,TVS_TRACKSELECT);
	// Get selection
	if(flMeDraging){
		GetTreeCtrl().Select(node,TVGN_CARET);
		pDad->OnEditCopy();
	}
	flMeDraging=true;
	*pResult = 0;
}


void CLeftView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pDad->DragOp){
		pDad->DragOp=false;
		flDragL=false;
		//Get path to copy to
		CString sPath;
		int nImg=0, tmp=0;
		HTREEITEM node=GetTreeCtrl().HitTest(point);
		if(!node) return;
		GetTreeCtrl().Select(node,TVGN_CARET);
		while(node!=NULL){
			sPath="\\"+GetTreeCtrl().GetItemText(node)+sPath;
			GetTreeCtrl().GetItemImage(node,nImg,tmp);
			if(nImg==BANK) sPath="\\VISION\\BANK"+sPath;
			node=GetTreeCtrl().GetParentItem(node);
		}
		sPath+="\\";
		pDad->CopySel(sPath,pDad->clipBoard,true);
		// Let go of list
		pDad->dListImg->DragLeave(this);
		pDad->dListImg->EndDrag();
//		delete pDad->dListImg;
		pDad->GetRightPane()->GetListCtrl().SetExtendedStyle(0);
		GetTreeCtrl().ModifyStyle(TVS_TRACKSELECT,0);

	}

	CTreeView::OnLButtonUp(nFlags, point);
}

void CLeftView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pDad->DragOp){
		pDad->DragOp=false;
		flDragR=false;
		//Get path to copy to
		CString sPath;
		int nImg=0, tmp=0;
		HTREEITEM node=GetTreeCtrl().HitTest(point);
		if(!node) return;
		GetTreeCtrl().Select(node,TVGN_CARET);
		while(node!=NULL){
			sPath="\\"+GetTreeCtrl().GetItemText(node)+sPath;
			GetTreeCtrl().GetItemImage(node,nImg,tmp);
			if(nImg==BANK) sPath="\\VISION\\BANK"+sPath;
			node=GetTreeCtrl().GetParentItem(node);
		}
		sPath+="\\";
		pDad->CopySel(sPath,pDad->clipBoard,false);
		// Let go of list
		pDad->dListImg->DragLeave(this);
		pDad->dListImg->EndDrag();
//		delete pDad->dListImg;
		pDad->GetRightPane()->GetListCtrl().SetExtendedStyle(0);
		GetTreeCtrl().ModifyStyle(TVS_TRACKSELECT,0);
	}

	CTreeView::OnRButtonUp(nFlags, point);
}

void CLeftView::OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM node;
	if(flMeDraging){
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		node=GetTreeCtrl().HitTest(pt);
		if(!node) return;
		pDad->dto=node;
		pDad->dListImg=GetTreeCtrl().CreateDragImage(pDad->dto);
	} else {
		pDad->dListImg->DragLeave(this);
		pDad->dListImg->EndDrag();
	}
	CPoint ptmp;
	ptmp.x=10; ptmp.y=10;
	pDad->dListImg->BeginDrag(0,ptmp);
	pDad->dListImg->DragEnter(this,pt);
	pDad->DragOp=true;
	flDragR=true;
	pDad->GetRightPane()->GetListCtrl().SetExtendedStyle(LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	GetTreeCtrl().ModifyStyle(0,TVS_TRACKSELECT);
	// Get selection
	if(flMeDraging){
		GetTreeCtrl().Select(node,TVGN_CARET);
		pDad->OnEditCopy();
	}
	flMeDraging=true;
	*pResult = 0;
}

void CLeftView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pt=point;
	if(pDad->DragOp){
		LRESULT Result;
		if(pDad->GetRightPane()->flDragL&&!flDragL){
			flMeDraging=false;
			OnBegindrag(NULL, &Result);
			pDad->GetRightPane()->flDragL=false;
		}
		if(pDad->GetRightPane()->flDragR&&!flDragR){
			flMeDraging=false;
			OnBeginrdrag(NULL, &Result);
			pDad->GetRightPane()->flDragR=false;
		}
		pDad->dListImg->DragMove(point);
	} else {
		flDragL=false;
		flDragR=false;
	}
	CTreeView::OnMouseMove(nFlags, point);
}

void CLeftView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pt=point;
	CTreeView::OnLButtonDown(nFlags, point);
}

void CLeftView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
 	pt=point;
 	CTreeView::OnRButtonDown(nFlags, point);
}

void CLeftView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CTreeView::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}
