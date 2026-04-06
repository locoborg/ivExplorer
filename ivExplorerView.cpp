// ivExplorerView.cpp : implementation of the CIvExplorerView class
//

#include "stdafx.h"
#include "ivExplorer.h"

#include "MainFrm.h"
#include "ivExplorerDoc.h"
#include "ivExplorerView.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerView

IMPLEMENT_DYNCREATE(CIvExplorerView, CListView)

BEGIN_MESSAGE_MAP(CIvExplorerView, CListView)
	//{{AFX_MSG_MAP(CIvExplorerView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(LVN_BEGINRDRAG, OnBeginrdrag)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerView construction/destruction

CIvExplorerView::CIvExplorerView()
{
	// TODO: add construction code here
	flMeDraging=true;
	flDragL=false;
	flDragR=false;

}

CIvExplorerView::~CIvExplorerView()
{
}

BOOL CIvExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerView drawing

void CIvExplorerView::OnDraw(CDC* pDC)
{
	CIvExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// take this out
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
	AfxMessageBox("Todabia No!");
}

void CIvExplorerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerView printing

BOOL CIvExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIvExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIvExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerView diagnostics

#ifdef _DEBUG
void CIvExplorerView::AssertValid() const
{
	CListView::AssertValid();
}

void CIvExplorerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CIvExplorerDoc* CIvExplorerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIvExplorerDoc)));
	return (CIvExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerView message handlers
void CIvExplorerView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

void CIvExplorerView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(flShowingFile) return;
	// find selected item in list
	POSITION pos =GetListCtrl().GetFirstSelectedItemPosition();
    int nItem = GetListCtrl().GetNextSelectedItem(pos);
//	int i=GetListCtrl().GetItemText(nItem,0).Find(" ");
	CString itemName=GetListCtrl().GetItemText(nItem,0);//.Left(i);
	// Add new items to tree branch
	pDad->ShowItems(TREE,false);
	HTREEITEM node=pDad->GetLeftPane()->GetTreeCtrl().GetSelectedItem();
	// expand tree brach
	pDad->GetLeftPane()->GetTreeCtrl().Expand(node,TVE_EXPAND);
	// search for my node and select it
	node=pDad->GetLeftPane()->GetTreeCtrl().GetChildItem(node);
	while(node!=NULL&&pDad->GetLeftPane()->GetTreeCtrl().GetItemText(node)!=itemName)
		node=pDad->GetLeftPane()->GetTreeCtrl().GetNextSiblingItem(node);
	// if found select tree branch
	if(node!=NULL){
		pDad->GetLeftPane()->GetTreeCtrl().Select(node,TVGN_CARET);
	} else{ // if not a Dir but a file open it
		flShowingFile=true;
		pDad->Status("Opening File "+itemName);
		GetDocument()->SetTitle(itemName);
		// Get path
		itemName="\\"+itemName;
		//Show path
		((CEdit *)pDad->m_wndDlgBar.GetDlgItem(IDC_PATH))->SetWindowText(itemName);
		int tImg=0, tmp=0;
		node=pDad->GetLeftPane()->GetTreeCtrl().GetSelectedItem();
		while(node!=NULL){
			itemName="\\"+pDad->GetLeftPane()->GetTreeCtrl().GetItemText(node)+itemName;
			pDad->GetLeftPane()->GetTreeCtrl().GetItemImage(node,tImg,tmp);
			if(tImg==BANK) itemName="\\VISION\\BANK"+itemName;
			node=pDad->GetLeftPane()->GetTreeCtrl().GetParentItem(node);
		}
		pDad->flCols=true;
		GetDocument()->SetSBar(&pDad->m_wndStatusBar);
		itemName.MakeUpper();
		if(itemName.Right(4)==".I00"||itemName.Right(4)==".J00") flShowingFile=false;
		if(!GetDocument()->OpenFile(itemName,&GetListCtrl())) flShowingFile=false;
	}
	*pResult = 0;
}

void CIvExplorerView::SetDad(CMainFrame *pap)
{
	pDad=pap;
}

void CIvExplorerView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(flShowingFile) return;
	if(flMeDraging){
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		pDad->dto=NULL;
		POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
		if(pos) {
			// Get the drag image
			int nItem = GetListCtrl().GetNextSelectedItem(pos);
			pDad->dListImg=GetListCtrl().CreateDragImage(nItem,&pt);
			if(GetListCtrl().GetSelectedCount()>1){ // if more then 1 file change the images
				pDad->dListImg->Replace(0,AfxGetApp()->LoadIcon(IDI_MFILES));
			//	pDad->dListImg->
			}
			pDad->dListImg->SetImageCount(1);
		}
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
	// Enable Hot tracking for panels
	GetListCtrl().SetExtendedStyle(LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	pDad->GetLeftPane()->GetTreeCtrl().ModifyStyle(0,TVS_TRACKSELECT);
	// Get selection
	if(flMeDraging){
		pDad->OnEditCopy();
	}
	flMeDraging=true;
	*pResult = 0;
}

void CIvExplorerView::OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(flShowingFile) return;
	if(flMeDraging){
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		pDad->dto=NULL;
		POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
		if(pos) {
			// Get the drag image
			int nItem = GetListCtrl().GetNextSelectedItem(pos);
			pDad->dListImg=GetListCtrl().CreateDragImage(nItem,&pt);
			if(GetListCtrl().GetSelectedCount()>1){ // if more then 1 file change the images
				pDad->dListImg->Replace(0,AfxGetApp()->LoadIcon(IDI_MFILES));
			//	pDad->dListImg->
			}
			pDad->dListImg->SetImageCount(1);
		}
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
	// Enable Hot tracking for panels
	GetListCtrl().SetExtendedStyle(LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	pDad->GetLeftPane()->GetTreeCtrl().ModifyStyle(0,TVS_TRACKSELECT);
	// Get selection
	if(flMeDraging){
		pDad->OnEditCopy();
	}
	flMeDraging=true;
	*pResult = 0;
}

void CIvExplorerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pDad->DragOp){
		pDad->DragOp=false;
		flDragL=false;
		pDad->dListImg->EndDrag();
		delete pDad->dListImg;
		// Disable Hot tracking for panels
		GetListCtrl().SetExtendedStyle(0);
		pDad->GetLeftPane()->GetTreeCtrl().ModifyStyle(TVS_TRACKSELECT,0);
		// Copy selection
		CString sfname;
		int nItem=GetListCtrl().GetHotItem();
		((CEdit *)pDad->m_wndDlgBar.GetDlgItem(IDC_PATH))->GetWindowText(sfname);
		if(nItem>-1){
			if(GetListCtrl().GetItemData(nItem)==IDIR)
				sfname+="\\"+GetListCtrl().GetItemText(nItem,0);
			sfname+="\\";
			pDad->CopySel(sfname,pDad->clipBoard,true);
		}
		// Let go of list
		pDad->dListImg->DragLeave(this);
		pDad->dListImg->EndDrag();
//		delete pDad->dListImg;
	}

	CListView::OnLButtonUp(nFlags, point);
}

void CIvExplorerView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pDad->DragOp){
		pDad->DragOp=false;
		flDragR=false;
		pDad->dListImg->EndDrag();
		delete pDad->dListImg;
		// Disable Hot tracking for panels
		GetListCtrl().SetExtendedStyle(0);
		pDad->GetLeftPane()->GetTreeCtrl().ModifyStyle(TVS_TRACKSELECT,0);
		// Move selection
		CString sfname;
		int nItem=GetListCtrl().GetHotItem();
		((CEdit *)pDad->m_wndDlgBar.GetDlgItem(IDC_PATH))->GetWindowText(sfname);
		if(nItem>-1){
			if(GetListCtrl().GetItemData(nItem)==IDIR)
				sfname+="\\"+GetListCtrl().GetItemText(nItem,0);
			sfname+="\\";
			pDad->CopySel(sfname,pDad->clipBoard,false);
		}
		// Let go of list
		pDad->dListImg->DragLeave(this);
		pDad->dListImg->EndDrag();
//		delete pDad->dListImg;
		
	}

	CListView::OnRButtonUp(nFlags, point);
}

void CIvExplorerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pt=point;
	if(pDad->DragOp){
		LRESULT Result;
		if(pDad->GetLeftPane()->flDragL&&!flDragL){
			flMeDraging=false;
			OnBegindrag(NULL, &Result);
			pDad->GetLeftPane()->flDragL=false;
		}
		if(pDad->GetLeftPane()->flDragR&&!flDragR){
			flMeDraging=false;
			OnBeginrdrag(NULL, &Result);
			pDad->GetLeftPane()->flDragR=false;
		}
		
		pDad->dListImg->DragMove(point);
	} else {
		flDragL=false;
		flDragR=false;
	}
	CListView::OnMouseMove(nFlags, point);
}

void CIvExplorerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pt=point;
	CListView::OnLButtonDown(nFlags, point);
}

void CIvExplorerView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pt=point;
	CListView::OnRButtonDown(nFlags, point);
}

