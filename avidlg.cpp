// avidlg.cpp : implementation file
//

#include "stdafx.h"
#include "ivExplorer.h"

#include "MainFrm.h"
#include "avidlg.h"
#include "LeftView.h"

#include <ivio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT FileThread(LPVOID pParam){
	Cavidlg *pap=(Cavidlg *)pParam;
	if(pap->szDst!=NULL) pap->CopySel();
	else pap->Kill(NULL);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Cavidlg dialog


Cavidlg::Cavidlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cavidlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Cavidlg)
	//}}AFX_DATA_INIT
}

Cavidlg::Cavidlg(UINT IDMOV)
	: CDialog(Cavidlg::IDD, NULL)
{
	flModal=false;
	nIDMov=IDMOV;
}
// new DoModal constructor
Cavidlg::Cavidlg(LPCSTR szD, LPCSTR szS, bool copying, LPVOID pap)
	: CDialog(Cavidlg::IDD, NULL)
{
	int sizeD=0, sizeS=0;
	pDad=(CMainFrame *)pap;
	flModal=true;
	if(szD!=NULL){
		nIDMov=IDR_AVIFCOPY;
		sizeD=strlen(szD)+1;
		szDst=new char[sizeD];
		memcpy(szDst,szD,sizeD);
	} else{
		nIDMov=IDR_AVIFDEL;
		szDst=NULL;
	}
	sizeS=strlen(szS)+1;
	szSrc=new char[sizeS];
	memcpy(szSrc,szS,sizeS);
	flMove=!copying;
}


void Cavidlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cavidlg)
	DDX_Control(pDX, IDC_LIST1, m_Out);
	DDX_Control(pDX, IDOK, m_Butt);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_AVI, m_avi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Cavidlg, CDialog)
	//{{AFX_MSG_MAP(Cavidlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cavidlg message handlers
void Cavidlg::SetShowMode(LPCSTR sFrom, LPCSTR sTo)
{
	m_Out.SetHorizontalExtent(500);
	CString output;
	output+=sFrom;
	output+=" -> ";
	output+=sTo;
	m_Out.SetCurSel(m_Out.AddString(output));
}

void Cavidlg::Output(LPCSTR m)
{
	m_Out.SetCurSel(m_Out.AddString(m));
	m_Out.ShowWindow(SW_SHOW);
}

void Cavidlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	int n=m_Out.GetCount();
	for(int i=0; i<n; i++)
		m_Out.DeleteString(i);
	CDialog::OnClose();
}

BOOL Cavidlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Out.ShowWindow(SW_SHOW);
	m_Butt.ShowWindow(SW_SHOW);
	m_progress.ShowWindow(SW_SHOW);
	m_avi.Open(nIDMov);
	m_avi.Play(0,-1,-1);
	m_avi.ShowWindow(SW_SHOW);
	// Self copy dlg
	if(flModal)
		AfxBeginThread(FileThread,this);
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Cavidlg::Stop()
{
	Output("Done.");
	m_progress.SetPos(100);
	m_avi.Stop();
}

bool Cavidlg::Copy(LPCSTR szDst, LPCSTR szSrc)
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
		Output(sMsg);
		AfxMessageBox("Error opening file");
        return false;
	}

	if ((error = IVOpen(szDst,
	               (unsigned long *)&dstHd,&ulRes,0,
	                OS2FILE_NORMAL,
					OS2OPEN_ACTION_CREATE_IF_NEW | OS2OPEN_ACTION_FAIL_IF_EXISTS,
					OS2OPEN_SHARE_DENYNONE       | OS2OPEN_ACCESS_WRITEONLY,0L)) != 0)
	{
		if(error==80){
			sMsg.Format("The file selected:\n  %s\nAlready exist replace?",szDst);
			if(AfxMessageBox(sMsg,MB_YESNO)==IDNO) return false; // do not replace
			//Replace file
			if ((error = IVOpen(szDst,
	               (unsigned long *)&dstHd,&ulRes,0,
	                OS2FILE_NORMAL,
					OS2OPEN_ACTION_CREATE_IF_NEW | OS2OPEN_ACTION_REPLACE_IF_EXISTS,
					OS2OPEN_SHARE_DENYNONE       | OS2OPEN_ACCESS_WRITEONLY,0L)) != 0)
			{
				sMsg.Format("Fatal Error replacing file %s Code = %d",szDst,error);
				Output(sMsg);
				AfxMessageBox("Error opening file");
				IVClose(srcHd);
				return false;
			}
		} else {
			sMsg.Format("Fatal Error creating file %s Code = %d",szDst,error);
			Output(sMsg);
			AfxMessageBox("Error opening file");
			IVClose(srcHd);
			return false;
		}
	}


	// Set file name in DLG
	SetShowMode(szSrc,szDst);

	char *Buff=new char[0x10000]; // 1 Meg Buff

	while((error=IVRead(srcHd, Buff, 0x10000, &ulRes))==0&&!flDone){
		ulDat=ulRes;
		if(ulRes<0x10000) flDone=true;
		if(ulDat>0)
			if((error=IVWrite(dstHd, Buff, ulDat, &ulRes))!=0){
				sMsg.Format("Error writing to file:\n %s\n Error Code: %d",szDst,error);
				AfxMessageBox(sMsg);
				flDone=true;
			}
		if(ulDat>ulRes) {
			sMsg.Format("Error data maybe lost in file:\n %s\n Error Code: %d Continue with opeartion?",szDst,error);
			if(AfxMessageBox(sMsg,MB_YESNO)==IDNO) {
				flDone=true;
				error=-5000;
			}

		}
	}
	
	IVClose(srcHd);
	IVClose(dstHd);

	if(error!=0){
		sMsg.Format("Error reading from file:\n %s\n Error Code: %d",szSrc,error);
		AfxMessageBox(sMsg);
		return false;
	}
	
	return true;
}

void Cavidlg::Kill(LPCSTR szSr)
{
	CString sMsg,sSel;
	char szfname[128];
	bool flDone=false;
	int p=-1,lp=0,error=0;

	if(szSr!=NULL)
		sSel=szSr;
	else sSel=szSrc;

	SetWindowText("Deleting files ...");

	int sl=sSel.GetLength();
	while(!flDone){
		Output("Deleting files:");
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
		m_progress.SetPos((int)(100*p/sl));

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
				SetShowMode(szfname,"Limbo");
		}
	}

//	pDad->OnRefresh();
	// AVI Dialog kill
	m_Butt.SetWindowText("Close");
	Stop();
	if(!pDad->VFileOp&&szDst==NULL)
		EndDialog(0);
}

void Cavidlg::CopySel()
{
	CString sfn,sSel=szSrc;
	char szfname[128];
	bool flDone=false, flCopyOk=false;
	int p=-1,lp=0,fnp;

	if(!flMove) SetWindowText("Copying files ...");
	else SetWindowText("Moving files ...");

	int sl=sSel.GetLength();
	while(!flDone){
		Output("Copying files:");
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
		m_progress.SetPos((int)(100*p/sl));
		// if it is a dir get file inside
		if(szfname[strlen(szfname)-1]=='+'){
			szfname[strlen(szfname)-1]=0;
			flCopyOk=CopyDir(szDst,szfname);
			if(flMove&&flCopyOk) {
				Output("Deleting files:");
				KillDir(szfname);
			}
		} else {
			// get the file name pointer "last \"
			sfn=szfname;
			fnp=sfn.ReverseFind('\\');
			if(fnp>0){
				flCopyOk=Copy(szDst+sfn.Right(p-lp-fnp-1),szfname);
				if(flMove&&flCopyOk) Kill(szfname);
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
	m_Butt.SetWindowText("Close");
	Stop();
	if(!pDad->VFileOp)
		EndDialog(0);

	Output("Done.");
//	pDad->OnRefresh();
//	if(pDad->dto!=NULL) pDad->GetLeftPane()->GetTreeCtrl().Select(pDad->dto,TVGN_CARET);
//	pDad->dto=NULL;
//	pDad->OnRefresh();
}

void Cavidlg::KillDir(LPCSTR szDir)
{
	IVDIRSTRUCT *ivDir;
	long nFiles=0;
	int error;
	CString sMsg,sfn, sDir=szDir;

	sDir+="\\";
	IVFindFiles(sDir+"*.*",&ivDir, &nFiles, OS2FILE_DIRECTORY);

	for(int i=0; i<nFiles; i++){
		sfn=ivDir[i].szFullFileName;
		if(sfn=="."||sfn=="..") error=0;// pDad->Status("File operation in progress ...");
		else if(ivDir[i].ulAttrFile&OS2FILE_DIRECTORY)
			KillDir(sDir+sfn);
		else {
			if((error=IVDelete(sDir+sfn))!=0){
				// IVDel error
				sMsg.Format("Error deleting file:\n %s\n Error Code: %d Continue with opeartion?",sDir+sfn,error);
				if(AfxMessageBox(sMsg,MB_YESNO)==IDNO) break;
			}else {
				SetShowMode(sDir+sfn,"Limbo");
				Sleep(100);
			}

		}
		// show %
		m_progress.SetPos((int)(100*i/nFiles));
	}
	IVDeleteDir(szDir);

//	pDad->OnRefresh();
}

bool Cavidlg::CopyDir(LPCSTR szDst, LPCSTR szDir)
{
	IVDIRSTRUCT *ivDir;
	long nFiles=0,p;
	CString sfn, sDir=szDir, sDst=szDst;
	// check for recurrency error
	p=sDir.GetLength();
	if(p<=sDst.GetLength())
		if(sDst.Left(p)==sDir){
			AfxMessageBox("Can not copy parent folder to child folder!");
			return false;
		}


	// Make sure dir in dst exist
	p=sDir.ReverseFind('\\');
	if(p<0){
		// Source not a file
		sfn.Format("Error: %s\n Not a Directory.",sDir);
		AfxMessageBox(sfn);
		return false;
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

	// get the copy resul
	bool flCopyOk=true;
	for(int i=0; i<nFiles; i++){
		sfn=ivDir[i].szFullFileName;
		if(sfn=="."||sfn=="..") sfn=".";// pDad->Status("File operation in progress ...");
		else if(ivDir[i].ulAttrFile&OS2FILE_DIRECTORY)
			flCopyOk&=CopyDir(sDst,sDir+sfn);
		else {
			flCopyOk&=Copy(sDst+sfn, sDir+sfn);
		}
		// show %
		m_progress.SetPos((int)(100*i/nFiles));
	}
	return flCopyOk;
}

