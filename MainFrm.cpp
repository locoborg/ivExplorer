// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ivExplorer.h"


#include "MainFrm.h"
#include "ivExplorerDoc.h"
#include "ivExplorerView.h"
#include "LeftView.h"
#include "avidlg.h"

#include <fstream.h>

#include <ivio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCSTR pDst, pSrc;
bool flCopy;
UINT FileManThread(LPVOID pParam){
	Cavidlg *FMDlg = new Cavidlg(pDst, pSrc, flCopy, pParam);
	FMDlg->DoModal();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CIVBar

CIVBar::CIVBar() : CReBar()
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CIVBar, CReBar)
	//{{AFX_MSG_MAP(CAboutDlg)
		ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CIVBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(pDad->DragOp){
		pDad->dListImg->DragLeave(this);
		pDad->dListImg->EndDrag();
		pDad->GetRightPane()->GetListCtrl().SetExtendedStyle(0);
		pDad->GetLeftPane()->GetTreeCtrl().ModifyStyle(TVS_TRACKSELECT,0);

		pDad->DragOp=false;
	}
	CReBar::OnMouseMove(nFlags, point);
}

void CIVBar::SetDad(CMainFrame *pap)
{
	pDad=pap;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_NCMOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_BACK, OnBack)
	ON_COMMAND(ID_FILE_FWD, OnFwd)
	ON_COMMAND(ID_FILE_UP, OnUp)
	ON_COMMAND(ID_EDIT_COPY,OnEditCopy)
    ON_COMMAND(ID_EDIT_PASTE,OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT,OnEditCut)
	ON_COMMAND(ID_REFRESH,OnRefresh)
	ON_COMMAND(ID_VIEW_FILEOPREPORT,OnVFOP)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create(this,RBS_FIXEDORDER|RBS_BANDBORDERS) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY );
	// Set dad to bar
	m_wndReBar.SetDad(this);

	// History settings
	nodeHist=new NodeItm[MAXHIST];
	curNode=0;
	for(int n=0; n<MAXHIST; n++) nodeHist[n].on=false;
	flHist=false;
	DragOp=false;
	// clipboard for file operationg
	clipBoard=NULL;
	fpos=NULL;
	// File Op. DLG Inits
	VFileOp=false;
	avidlg=NULL;
	dto=NULL;
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(200, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CIvExplorerView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	// Set style
	((CLeftView *)m_wndSplitter.GetPane(0,0))->GetTreeCtrl().ModifyStyle(0, TVS_HASLINES);//|TVS_SHOWSELALWAYS);
	// Set Image List for tree
	SetImgLst(&((CLeftView *)m_wndSplitter.GetPane(0,0))->GetTreeCtrl(), TVSIL_NORMAL, 20, TREE);
	// Set Image List for view LVS_ICON LVSIL_NORMAL LVS_SMALLICON LVS_REPORT
	SetImgLst(&GetRightPane()->GetListCtrl(),LVS_REPORT,16,LIST);
	GetRightPane()->ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	GetRightPane()->GetListCtrl().SetHoverTime(1);
	// Get System.cfg server data
	LoadServers();
	// Pass pointer to man window
	GetLeftPane()->SetDad(this);
	GetRightPane()->SetDad(this);
	// Add colums
	flCols=true;
	ResetCols();
	// Drag op flase(Not draging);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

CIvExplorerView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CIvExplorerView* pView = DYNAMIC_DOWNCAST(CIvExplorerView, pWnd);
	return pView;
}

CLeftView* CMainFrame::GetLeftPane()
{
	return (CLeftView*)m_wndSplitter.GetPane(0,0);
}


void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CIvExplorerView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	int iSize=16;
	CIvExplorerView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CIvExplorerView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			iSize=32;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1){
			// Get Image List for views
			SetImgLst(&pView->GetListCtrl(), dwStyle,iSize,LIST);

			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
		}
	}
}

void CMainFrame::LoadServers()
{
	CString data,svrData;
	LONG nItems=0;
	char szTmp[256];
	int si=0,ci=0,pi=0,ii=0,comm=0;

	Status("Loading system configuration...");
	fstream *fb=new fstream("\\VISION\\ETC\\SYSTEM.CFG", ios::in, filebuf::openprot);

	while(!fb->eof()){
		fb->getline(szTmp,256);
		data=szTmp;
		si=data.Find("=");
		ci=data.Find(";");
		comm=data.Find(":"); // ID Print and Comm serves
		if(si<0) continue;
		if(ci>0 && ci<si) continue;
		if(ci<0) ci=data.GetLength();
		if(comm>0&&comm<ci) comm=1; // Mark Print and Comm Severs
		else comm=0;
		svrData=data.Left(si);
		pi=svrData.Find(".");
		// get the type of icon for line
		if(svrData.Left(pi)=="SERVER"/*&&!comm*/) ii=SERVER;
		else if(svrData.Left(pi)=="BRANCH"&&!comm) ii=NETCOMP;
		else if(svrData.Left(pi)=="SORTER"&&!comm) ii=NETCOMP;
		else ii=UNKNOWN;
		// parse the rest of line
//		svrData+=" ("+data.Mid(si+1,ci-si-1);
//		svrData.TrimRight(); svrData+=")";
		((CLeftView *)m_wndSplitter.GetPane(0,0))->GetTreeCtrl().InsertItem(svrData,ii,ii);
	}
	fb->close();
	delete fb;
	Status("Done loading configuration.");
}

int CMainFrame::DirAdd(HTREEITEM pap, PVOID ivDir, long nItems, LPCSTR szExclude, LPCSTR Img2Items, int defImg, bool tree, bool erase)
{
	CString sExc=szExclude, sImg2Items=Img2Items, sTmp;
	int img=defImg,iidx=0,it=0,nImgDef=0;
	char szTmp[64];

	// Make sure cols are rigth
	ResetCols();
	// Get new items to display
	if(erase&&tree==LIST) GetRightPane()->GetListCtrl().DeleteAllItems();
	for(int c=0; c<nItems; c++){
		sImg2Items=Img2Items;
		sTmp=((IVDIRSTRUCT *)ivDir)[c].szFullFileName;
		sTmp.MakeUpper();
		if(tree==TREE&&!(((IVDIRSTRUCT *)ivDir)[c].ulAttrFile&OS2FILE_DIRECTORY)) continue;

		if(sExc.Find(sTmp)<0){
			if(((IVDIRSTRUCT *)ivDir)[c].ulAttrFile&OS2FILE_DIRECTORY) {
				iidx=sImg2Items.Find(sTmp);
				nImgDef=1;
			} else {
				sImg2Items=FLIST;
				iidx=sImg2Items.Find(sTmp.Right(3));
				nImgDef=2;
			}
			if(iidx<2) {
				if(defImg!=0) img=defImg;
				else switch (nImgDef){
					case 0:
						img=defImg;
						break;
					case 1:
						img=25;
						break;
					case 2:
						img=24;
						break;
					default:
						img=defImg;
				}
			} else img=atoi(sImg2Items.Mid(iidx-2,2));
//			sTmp.Format("%s (%s)",((IVDIRSTRUCT *)ivDir)[c].szFullFileName,((IVDIRSTRUCT *)ivDir)[c].szNodeName);
			if(tree==TREE)
				((CLeftView *)m_wndSplitter.GetPane(0,0))->GetTreeCtrl().InsertItem(((IVDIRSTRUCT *)ivDir)[c].szFullFileName,img,img,pap);
			else {
				GetRightPane()->GetListCtrl().InsertItem(it,((IVDIRSTRUCT *)ivDir)[c].szFullFileName,img);
				GetRightPane()->GetListCtrl().SetItem(it,1,LVIF_TEXT,((IVDIRSTRUCT *)ivDir)[c].szNodeName,img,0,0,NULL);
				sprintf(szTmp,"%01d.%d Kb",((IVDIRSTRUCT *)ivDir)[c].ulFileSize/1024,((IVDIRSTRUCT *)ivDir)[c].ulFileSize%1024);
				GetRightPane()->GetListCtrl().SetItem(it,2,LVIF_TEXT,szTmp,img,0,0,NULL);
				sprintf(szTmp,"%02d/%02d/%04d",
					((IVDIRSTRUCT *)ivDir)[c].dateCreation.month,
					((IVDIRSTRUCT *)ivDir)[c].dateCreation.day,
					((IVDIRSTRUCT *)ivDir)[c].dateCreation.year+1980);
				GetRightPane()->GetListCtrl().SetItem(it,3,LVIF_TEXT,szTmp,img,0,0,NULL);
				sprintf(szTmp,"%02d:%02d:%02d",
					((IVDIRSTRUCT *)ivDir)[c].timeCreation.hours,
					((IVDIRSTRUCT *)ivDir)[c].timeCreation.minutes,
				((IVDIRSTRUCT *)ivDir)[c].timeCreation.twosecs);
				GetRightPane()->GetListCtrl().SetItem(it,4,LVIF_TEXT,szTmp,img,0,0,NULL);
				sprintf(szTmp,"%d",img);
				GetRightPane()->GetListCtrl().SetItem(it,5,LVIF_TEXT,szTmp,img,0,0,NULL);
				if(((IVDIRSTRUCT *)ivDir)[c].ulAttrFile&OS2FILE_DIRECTORY) GetRightPane()->GetListCtrl().SetItemData(it,IDIR);
				else GetRightPane()->GetListCtrl().SetItemData(it,IFILE);
			}
			it++;
		}
	}
	return it;
}

void CMainFrame::Status(LPCTSTR m)
{
	if(m_wndStatusBar.m_hWnd!=NULL)
		m_wndStatusBar.SetPaneText(0,m);
}

void CMainFrame::SetImgLst(CWnd *view, DWORD dwStyle, int size, bool Type)
{
	CImageList *oldImgs, *imglst=new CImageList();
	imglst->Create(size, size, ILC_COLOR32|ILC_MASK, 2, 2);
	imglst->Add(AfxGetApp()->LoadIcon(IDI_INFO));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_SEVER));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_BRANCH));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_BANK));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_VISION));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_BIN));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_COMMAND));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_ETC));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_LOG));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_PARMSV));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_REPORTS));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_SEQUENCE));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_SOUNDS));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_SPOOLV));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_DATA));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_FORMS));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_LOGOS));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_PARMS));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_FILES));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_SPOOL));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_DATE));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_DBF));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_I00));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_J00));
	imglst->Add(AfxGetApp()->LoadIcon(IDR_IVEXPLTYPE));
	imglst->Add(AfxGetApp()->LoadIcon(IDI_FOLDER));
	if(Type==LIST) oldImgs=((CListCtrl *)view)->SetImageList(imglst,dwStyle );
	else oldImgs=((CTreeCtrl *)view)->SetImageList(imglst,dwStyle );

	if(oldImgs!=NULL) delete oldImgs;
}

void CMainFrame::AddItem(HTREEITEM pap, LPCSTR name, bool type, int idx, int img, bool erase)
{
	if(erase&&type==LIST) GetRightPane()->GetListCtrl().DeleteAllItems();
	if(type==LIST) GetRightPane()->GetListCtrl().InsertItem(idx,name,img);
	else ((CLeftView *)m_wndSplitter.GetPane(0,0))->GetTreeCtrl().InsertItem(name,img,img,pap);
}

void CMainFrame::ShowItems(bool type, bool flRefr)
{
	CString sPath,sImgs=OLIST,sEx="..";
	int nlevel=0,nImg=0, tmp=0, tImg=0;
	long nFiles=0;
	IVDIRSTRUCT *ivDir;
	// Get path
	sPath.Empty();
	HTREEITEM node=GetLeftPane()->GetTreeCtrl().GetSelectedItem();
	HTREEITEM nodeyo=node;
	// Set in history
	if(type==LIST&&!flHist){
		nodeHist[curNode].node=node;
		nodeHist[curNode].on=true;
		curNode++;
	}
	if(flHist) flHist=false;

	// Do not read again
	if(type==TREE){
		if(!flRefr&&GetLeftPane()->GetTreeCtrl().GetItemState(nodeyo,0)&TVIS_EXPANDEDONCE)
			return;
		// Erase all childs in the current branch.
		if(flRefr)
			GetLeftPane()->GetTreeCtrl().Expand(nodeyo,TVE_COLLAPSERESET|TVE_COLLAPSE);

		nlevel=GetRightPane()->GetListCtrl().GetItemCount();
		int im=-1;
		for(int i=0; i<nlevel; i++){
			im=GetRightPane()->GetListCtrl().GetNextItem(im,LVNI_ALL);
			nImg=atoi(GetRightPane()->GetListCtrl().GetItemText(im,5));
			if(GetRightPane()->GetListCtrl().GetItemData(im)==IDIR)
				GetLeftPane()->GetTreeCtrl().InsertItem(GetRightPane()->GetListCtrl().GetItemText(im,0),nImg,nImg,nodeyo);
		}
		if(flRefr)
			GetLeftPane()->GetTreeCtrl().Expand(nodeyo,TVE_EXPAND);
		return;
	}
		

	GetLeftPane()->GetTreeCtrl().GetItemImage(nodeyo,nImg,nlevel);
	nlevel=0;
	while(node!=NULL){
		sPath="\\"+GetLeftPane()->GetTreeCtrl().GetItemText(node)+sPath;
		GetLeftPane()->GetTreeCtrl().GetItemImage(node,tImg,tmp);
		if(tImg==BANK) sPath="\\VISION\\BANK"+sPath;
		node=GetLeftPane()->GetTreeCtrl().GetParentItem(node);
		nlevel++;
	}
	((CEdit *)m_wndDlgBar.GetDlgItem(IDC_PATH))->SetWindowText(sPath);
	// Get dir
	if(type==LIST)
		Status("Reading Path...");
	int it=0;
	if(nlevel==1&&nImg>UNKNOWN){
		// Get Vision dir
		IVFindFiles(sPath+"\\VISION",&ivDir, &nFiles, OS2FILE_DIRECTORY);
		it+=DirAdd(nodeyo,ivDir,nFiles,sEx,"",VISION,type,true);
		// Get Banks
		sPath+="\\VISION\\BANK\\*.*";
		IVFindFiles(sPath,&ivDir, &nFiles, OS2FILE_DIRECTORY);
		it+=DirAdd(nodeyo,ivDir,nFiles,sEx,"",BANK,type,false);
	} else if(nlevel>1){
		int im=UNKNOWN;
		if(nImg==DATA) im=DATE;
		if(nImg==VISION){
			sImgs=VLIST;
			sEx+="BANK";
		}
		sPath+="\\*.*";
		IVFindFiles(sPath,&ivDir, &nFiles, OS2FILE_DIRECTORY);
		it+=DirAdd(nodeyo,ivDir,nFiles,sEx,sImgs,im,type,true);
	}
	// Using sPath do display status
	if(type==LIST){
		sPath.Format("%d Items found",it);
		Status(sPath);
	}
}

void CMainFrame::ResetCols()
{
	if(!flCols) return;
	flCols=false;
	GetRightPane()->flShowingFile=false;
	// Get curnent num cols
	int nc=GetRightPane()->GetListCtrl().GetHeaderCtrl()->GetItemCount();
	// Del cols
	for(int j=nc; j>=0; j--)
		if(!GetRightPane()->GetListCtrl().DeleteColumn(j))
			GetRightPane()->GetListCtrl().DeleteColumn(j);
	// Setting title
	GetRightPane()->GetDocument()->SetTitle("Untitle");
	// inserting cols
	GetRightPane()->GetListCtrl().InsertColumn(0,"Item Name",LVCFMT_LEFT,150,0);
	GetRightPane()->GetListCtrl().InsertColumn(1,"Network Node",LVCFMT_LEFT,100,1);
	GetRightPane()->GetListCtrl().InsertColumn(2,"Item Size",LVCFMT_RIGHT,80,2);
	GetRightPane()->GetListCtrl().InsertColumn(3,"Creation Date (m/d/y)",LVCFMT_CENTER,100,3);
	GetRightPane()->GetListCtrl().InsertColumn(4,"Creation Time (h:m:s)",LVCFMT_CENTER,100,4);
	GetRightPane()->GetListCtrl().InsertColumn(5,"Image",LVCFMT_CENTER,0,5);
	GetRightPane()->ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}


void CMainFrame::OnBack()
{
	if(nodeHist[(BYTE)(curNode-2)].on){
		flHist=true;
		GetLeftPane()->GetTreeCtrl().Select(nodeHist[(BYTE)(curNode-2)].node,TVGN_CARET);
		curNode--;
	} else Status("No more history, reach back");
}

void CMainFrame::OnUp()
{
	HTREEITEM node=GetLeftPane()->GetTreeCtrl().GetSelectedItem();
	node=GetLeftPane()->GetTreeCtrl().GetParentItem(node);	
	GetLeftPane()->GetTreeCtrl().Select(node,TVGN_CARET);
}

void CMainFrame::OnFwd()
{
	if(nodeHist[curNode].on){
		flHist=true;
		GetLeftPane()->GetTreeCtrl().Select(nodeHist[curNode].node,TVGN_CARET);
		curNode++;
	} else Status("No more history, reach front");
}

void CMainFrame::OnEditCopy()
{
	if(clipBoard!=NULL) delete clipBoard;

	int row, col;
	m_wndSplitter.GetActivePane(&row,&col);
	if(col==0){
		clipBoard=new char[256];
		*clipBoard=0;
		// get Path
		CString sDir;
		((CEdit *)m_wndDlgBar.GetDlgItem(IDC_PATH))->GetWindowText(sDir);
		sprintf(clipBoard,"%s",sDir+"+");
	} else {
		clipBoard=new char[1024];
		*clipBoard=0;
		// get Path
		CString sDir;
		((CEdit *)m_wndDlgBar.GetDlgItem(IDC_PATH))->GetWindowText(sDir);
		// get file names
		POSITION pos =GetRightPane()->GetListCtrl().GetFirstSelectedItemPosition();
		int nItem;
		while (pos){
			nItem = GetRightPane()->GetListCtrl().GetNextSelectedItem(pos);
			if(IFILE==GetRightPane()->GetListCtrl().GetItemData(nItem))
				strcat(clipBoard,sDir+"\\"+GetRightPane()->GetListCtrl().GetItemText(nItem,0)+"|");
			else if(IDIR==GetRightPane()->GetListCtrl().GetItemData(nItem))
				strcat(clipBoard,sDir+"\\"+GetRightPane()->GetListCtrl().GetItemText(nItem,0)+"+|");
		}
	}
}

void CMainFrame::OnEditPaste()
{
	if(clipBoard==NULL){
		AfxMessageBox("Nothing to Paste!");
		Status("Nothing to Paste!");
		return;
	}
	int row, col;
	CString sDir;

	m_wndSplitter.GetActivePane(&row,&col);
	if(col==0){
		// get Paste path
		((CEdit *)m_wndDlgBar.GetDlgItem(IDC_PATH))->GetWindowText(sDir);
	} else {
		// get Paste Path
		((CEdit *)m_wndDlgBar.GetDlgItem(IDC_PATH))->GetWindowText(sDir);
		// get file names
		POSITION pos =GetRightPane()->GetListCtrl().GetFirstSelectedItemPosition();
		int nItem = GetRightPane()->GetListCtrl().GetNextSelectedItem(pos);
		if(IDIR!=GetRightPane()->GetListCtrl().GetItemData(nItem)){
			AfxMessageBox("Can only copy into folders!");
			Status("Can only copy into folders!");
			return;
		}
		sDir+="\\"+GetRightPane()->GetListCtrl().GetItemText(nItem,0);
	}	
	sDir+="\\";
	CopySel(sDir,clipBoard,true);
}

void CMainFrame::Copy(LPCSTR szDst, LPCSTR szSrc)
{
	IVHANDLE srcHd, dstHd;
	LONG error;
	ULONG ulRes,ulDat;
	CString sMsg;
	bool flDone=false;

	if ((error = IVOpen(szSrc,
	               (unsigned long *)&srcHd,&ulRes,0,
	                OS2FILE_NORMAL,
					OS2OPEN_ACTION_FAIL_IF_NEW | OS2OPEN_ACTION_OPEN_IF_EXISTS ,
					OS2OPEN_SHARE_DENYNONE       | OS2OPEN_ACCESS_READONLY,0L)) != 0)
	{
		sMsg.Format("Fatal Error opening file %s Code = %d",szSrc,error);
		Status(sMsg);
		AfxMessageBox("Error opening file");
        return;
	}

	if ((error = IVOpen(szDst,
	               (unsigned long *)&dstHd,&ulRes,0,
	                OS2FILE_NORMAL,
					OS2OPEN_ACTION_CREATE_IF_NEW | OS2OPEN_ACTION_FAIL_IF_EXISTS,
					OS2OPEN_SHARE_DENYNONE       | OS2OPEN_ACCESS_WRITEONLY,0L)) != 0)
	{
		if(error==80){
			sMsg.Format("The file selected:\n  %s\nAlready exist replace?",szDst);
			if(AfxMessageBox(sMsg,MB_YESNO)==IDNO) return; // do not replace
			//Replace file
			if ((error = IVOpen(szDst,
	               (unsigned long *)&dstHd,&ulRes,0,
	                OS2FILE_NORMAL,
					OS2OPEN_ACTION_CREATE_IF_NEW | OS2OPEN_ACTION_REPLACE_IF_EXISTS,
					OS2OPEN_SHARE_DENYNONE       | OS2OPEN_ACCESS_WRITEONLY,0L)) != 0)
			{
				sMsg.Format("Fatal Error replacing file %s Code = %d",szDst,error);
				Status(sMsg);
				AfxMessageBox("Error opening file");
				IVClose(srcHd);
				return;
			}
		} else {
			sMsg.Format("Fatal Error creating file %s Code = %d",szDst,error);
			Status(sMsg);
			AfxMessageBox("Error opening file");
			IVClose(srcHd);
			return;
		}
	}


	// Set file name in DLG
	if(avidlg!=NULL)
		avidlg->SetShowMode(szSrc,szDst);

	char *Buff=new char[0x10000]; // 1 Meg Buff

	while((error=IVRead(srcHd, Buff, 0x10000, &ulRes))==0&&!flDone){
		ulDat=ulRes;
		if(ulRes<0x10000) flDone=true;
		if(ulDat>0)
			if((error=IVWrite(dstHd, Buff, ulDat, &ulRes))!=0){
				sMsg.Format("Error writing to file:\n %s\n Error Code: %d",szDst,error);
				AfxMessageBox(sMsg);
				IVClose(srcHd);
				IVClose(dstHd);
				return;
			}
		if(ulDat>ulRes) {
			sMsg.Format("Error data maybe lost in file:\n %s\n Error Code: %d Continue with opeartion?",szDst,error);
			if(AfxMessageBox(sMsg,MB_YESNO)==IDNO) flDone=true;

		}
	}
	if(error!=0){
		sMsg.Format("Error reading from file:\n %s\n Error Code: %d",szSrc,error);
		AfxMessageBox(sMsg);
	}

	
	IVClose(srcHd);
	IVClose(dstHd);

}

void CMainFrame::Kill(LPCSTR szSrc)
{
	CString sMsg,sSel=szSrc;
	char szfname[128];
	bool flDone=false;
	int p=-1,lp=0,error=0;

	// AVI Dialog create
	if(avidlg==NULL){
		avidlg=new Cavidlg(IDR_AVIFDEL);
		avidlg->Create(IDD_DIALOGTMP);
		avidlg->SetWindowText("Deleting files ...");
		avidlg->ShowWindow(SW_SHOW);
	}

	int sl=sSel.GetLength();
	while(!flDone){
		avidlg->Output("Deleting files:");
		lp=p+1;
		p=sSel.Find("|",lp);
		if(p>0){
			sprintf(szfname,"%s",sSel.Mid(lp,p-lp));
		} else {
			p=sl;
			sprintf(szfname,"%s",sSel.Right(p-lp));
			flDone=true;
		}
		// show %
		if(avidlg!=NULL) avidlg->m_progress.SetPos((int)(100*p/sl));

		// if no file next
		if(strlen(szfname)<1) continue;
		// if it is a dir get file inside
		if(szfname[strlen(szfname)-1]=='+'){
			szfname[strlen(szfname)-1]=0;
			KillDir(szfname);
		} else {
			if((error=IVDelete(szfname))!=0){
				// IVDel error
				sMsg.Format("Error deleting file:\n %s\n Error Code: %d Continue with opeartion?",szfname,error);
				if(AfxMessageBox(sMsg,MB_YESNO)==IDNO) flDone=true;
			} else 
				avidlg->SetShowMode(szfname,"Limbo");
		}
	}


	OnRefresh();
	// AVI Dialog kill
	if(avidlg!=NULL){
		avidlg->m_avi.Stop();
		avidlg->m_Butt.SetWindowText("Close");
		avidlg->Stop();
		if(!VFileOp){
			avidlg->EndDialog(0);
			avidlg=NULL;
		}
	}

}

void CMainFrame::CopySel(LPCSTR szDst, LPCSTR szSrc, bool coping)
{
	CString sfn,sSel=szSrc;
	char szfname[128];
	bool flDone=false;
	int p=-1,lp=0,fnp;

	// AVI Dialog create
	if(avidlg==NULL){
		avidlg=new Cavidlg(IDR_AVIFCOPY);
		avidlg->Create(IDD_DIALOGTMP);
		avidlg->ShowWindow(SW_SHOW);
	}
	if(coping) avidlg->SetWindowText("Copying files ...");
	else avidlg->SetWindowText("Moving files ...");

	int sl=sSel.GetLength();
	while(!flDone){
		avidlg->Output("Copying files:");
		lp=p+1;
		p=sSel.Find("|",lp);
		if(p>0){
			sprintf(szfname,"%s",sSel.Mid(lp,p-lp));
		} else {
			p=sl;
			sprintf(szfname,"%s",sSel.Right(p-lp));
			flDone=true;
		}
		// show %
		avidlg->m_progress.SetPos((int)(100*p/sl));
		// if it is a dir get file inside
		if(szfname[strlen(szfname)-1]=='+'){
			szfname[strlen(szfname)-1]=0;
			CopyDir(szDst,szfname);
			if(!coping) {
				avidlg->Output("Deleting files:");
				KillDir(szfname);
			}
		} else {
			// get the file name pointer "last \"
			sfn=szfname;
			fnp=sfn.ReverseFind('\\');
			if(fnp>0){
				Copy(szDst+sfn.Right(p-lp-fnp-1),szfname);
				if(!coping) Kill(szfname);
			} else {
				// Copy error
				if(strlen(szfname)>0){
					sfn.Format("Error: %s\n Not a file.",szfname);
					AfxMessageBox(sfn);
				} //else no file selected
			}
		}
	}

	// AVI Dialog kill
	if(avidlg!=NULL){
		avidlg->m_avi.Stop();
		avidlg->m_Butt.SetWindowText("Close");
		avidlg->Stop();
		if(!VFileOp){
			avidlg->EndDialog(0);
			avidlg=NULL;
		}
	}

	Status("Done.");
	OnRefresh();
	if(dto!=NULL) GetLeftPane()->GetTreeCtrl().Select(dto,TVGN_CARET);
	dto=NULL;
	OnRefresh();
}

void CMainFrame::KillDir(LPCSTR szDir)
{
	IVDIRSTRUCT *ivDir;
	long nFiles=0;
	int error;
	CString sMsg,sfn, sDir=szDir;

	sDir+="\\";
	IVFindFiles(sDir+"*.*",&ivDir, &nFiles, OS2FILE_DIRECTORY);

	for(int i=0; i<nFiles; i++){
		sfn=ivDir[i].szFullFileName;
		if(sfn=="."||sfn=="..") Status("File operation in progress ...");
		else if(ivDir[i].ulAttrFile&OS2FILE_DIRECTORY)
			KillDir(sDir+sfn);
		else {
			if((error=IVDelete(sDir+sfn))!=0){
				// IVDel error
				sMsg.Format("Error deleting file:\n %s\n Error Code: %d Continue with opeartion?",sDir+sfn,error);
				if(AfxMessageBox(sMsg,MB_YESNO)==IDNO) break;
			}else {
				avidlg->SetShowMode(sDir+sfn,"Limbo");
				Sleep(100);
			}

		}
		// show %
		avidlg->m_progress.SetPos((int)(100*i/nFiles));
	}
	IVDeleteDir(szDir);

	OnRefresh();
}

void CMainFrame::CopyDir(LPCSTR szDst, LPCSTR szDir)
{
	IVDIRSTRUCT *ivDir;
	long nFiles=0,p;
	CString sfn, sDir=szDir, sDst=szDst;
	// check for recurrency error
	p=sDir.GetLength();
	if(p<=sDst.GetLength())
		if(sDst.Left(p)==sDir){
			AfxMessageBox("Can not copy parent folder to child folder!");
			return;
		}


	// Make sure dir in dst exist
	p=sDir.ReverseFind('\\');
	if(p<0){
		// Source not a file
		sfn.Format("Error: %s\n Not a Directory.",sDir);
		AfxMessageBox(sfn);
		return;
	}
	p=sDir.GetLength()-p;
	sfn=szDst;
	sfn+=sDir.Right(p-1);
//	IVFindFiles(sfn,&ivDir, &nFiles, OS2FILE_DIRECTORY);
//	if(nFiles<1) 
		p=IVCreateDir(sfn,0L);
		sDst=sfn+"\\";
	// Copy files
	sDir+="\\";
	IVFindFiles(sDir+"*.*",&ivDir, &nFiles, OS2FILE_DIRECTORY);

	for(int i=0; i<nFiles; i++){
		sfn=ivDir[i].szFullFileName;
		if(sfn=="."||sfn=="..") Status("File operation in progress ...");
		else if(ivDir[i].ulAttrFile&OS2FILE_DIRECTORY)
			CopyDir(sDst,sDir+sfn);
		else {
			Copy(sDst+sfn, sDir+sfn);
		}
		// show %
		avidlg->m_progress.SetPos((int)(100*i/nFiles));
	}
}

void CMainFrame::OnEditCut()
{
	if(clipBoard!=NULL) {
//		if(AfxMessageBox("ClipBoard will be deleted?",MB_YESNO)==IDNO) return;
		delete clipBoard;
	}
	

	int row, col;
	m_wndSplitter.GetActivePane(&row,&col);
	if(col==0){
		clipBoard=new char[256];
		*clipBoard=0;
		// get Path
		CString sDir;
		((CEdit *)m_wndDlgBar.GetDlgItem(IDC_PATH))->GetWindowText(sDir);
		sprintf(clipBoard,"%s",sDir+"+");
		OnUp();
	} else {
		clipBoard=new char[1024];
		*clipBoard=0;
		// get Path
		CString sDir;
		((CEdit *)m_wndDlgBar.GetDlgItem(IDC_PATH))->GetWindowText(sDir);
		// get file names
		POSITION pos =GetRightPane()->GetListCtrl().GetFirstSelectedItemPosition();
		int nItem;
		while (pos){
			nItem = GetRightPane()->GetListCtrl().GetNextSelectedItem(pos);
			if(IFILE==GetRightPane()->GetListCtrl().GetItemData(nItem))
				strcat(clipBoard,sDir+"\\"+GetRightPane()->GetListCtrl().GetItemText(nItem,0)+"|");
			else if(IDIR==GetRightPane()->GetListCtrl().GetItemData(nItem))
				strcat(clipBoard,sDir+"\\"+GetRightPane()->GetListCtrl().GetItemText(nItem,0)+"+|");
		}
		if(strlen(clipBoard)>0)
			clipBoard[strlen(clipBoard)-1]=0;
	}
//	Kill(clipBoard);
	FileMan(NULL,clipBoard,true);
//	delete clipBoard;
//	clipBoard=NULL;
}

void CMainFrame::OnRefresh()
{
	ShowItems(LIST,true);
	ShowItems(TREE,true);
}

void CMainFrame::OnVFOP()
{
	VFileOp=!VFileOp;
	//GetMenu()->GetSubMenu(2)->
	if(VFileOp) GetMenu()->GetSubMenu(2)->CheckMenuItem(ID_VIEW_FILEOPREPORT,MF_CHECKED);
	else GetMenu()->GetSubMenu(2)->CheckMenuItem(ID_VIEW_FILEOPREPORT,MF_UNCHECKED);
}


void CMainFrame::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(DragOp){
		dListImg->DragLeave(this);
		dListImg->EndDrag();
		DragOp=false;
		GetRightPane()->GetListCtrl().SetExtendedStyle(0);
		GetLeftPane()->GetTreeCtrl().ModifyStyle(TVS_TRACKSELECT,0);
	}
//	CMainFrame::OnNcMouseMove(nHitTest, point);
}

void CMainFrame::FileMan(LPCSTR szDst, LPCSTR szSrc, bool copy)
{
	pDst=szDst;
	pSrc=szSrc;
	flCopy=copy;
	AfxBeginThread(FileManThread,this);
}
