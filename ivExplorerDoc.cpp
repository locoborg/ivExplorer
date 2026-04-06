// ivExplorerDoc.cpp : implementation of the CIvExplorerDoc class
//

#include "stdafx.h"
#include "ivExplorer.h"
#include "DispImg.h"
#include "ivExplorerDoc.h"

#include <os2.h>
#include <db.h>
#include <ivio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerDoc

IMPLEMENT_DYNCREATE(CIvExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CIvExplorerDoc, CDocument)
	//{{AFX_MSG_MAP(CIvExplorerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIvExplorerDoc construction/destruction

CIvExplorerDoc::CIvExplorerDoc()
{
	// TODO: add one-time construction code here

}

CIvExplorerDoc::~CIvExplorerDoc()
{
}

BOOL CIvExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CIvExplorerDoc serialization

void CIvExplorerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here

	}
}

BOOL CIvExplorerDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
//	AfxMessageBox(lpszPathName);
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CIvExplorerDoc diagnostics

#ifdef _DEBUG
void CIvExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIvExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CIvExplorerDoc commands

bool CIvExplorerDoc::OpenFile(LPCSTR fname, CWnd *wnd)
{
	bool fRet=true;
	CString sfName=fname;
	sfName.MakeUpper();

	DispBuff=wnd;

	if(sfName.Right(4)==".DBF"){
		OpenDBF(sfName);
	} else if(sfName.Right(4)==".I00"){
		CDispImg dImg;
		dImg.SetSBar(sBar);
		dImg.SetFName(sfName);
		dImg.DoModal();
	} else if(sfName.Right(4)==".J00"){
		CDispImg dImg;
		dImg.SetSBar(sBar);
		dImg.SetFName(sfName);
		dImg.DoModal();
	} else {
		// Will implement more file types in future
		AfxMessageBox("File type "+sfName.Right(4)+" not supported yet!");
		fRet=false;
	}
	return fRet;
}

bool CIvExplorerDoc::OpenDBF(LPCSTR szfname)
{
CString       Message;
//OS2HFILE      PODFile;
//ULONG         ulAction;
LONG          error = 0;
CHAR          szBuffer[128],szFieldBuff[64];
DatabaseHdr  *dbHdr;
DBDescRec    *dbDesc;
RecordDefHdr *recDefHdr;
DBHANDLE      hdb;
//FieldDef     *fDef;
//CHAR          szBase[32];
LONG          ulTNumRec = 0,TotalItemsToTranslate=0;
long		  TotalItemsTranslated=0,RecSize, fdefsize=0;
CHAR         *Records;
//CHAR         *Record;
LONG          ItemsRead;

    //////////////////////////////////////
    // Find how many Item to read       //
    //////////////////////////////////////
//sNumRequiredFields = sizeof (fieldInfo) / sizeof (fieldInfo[0]);

    if ((error = DbOpenDatabase (&hdb, szfname,DB_MODE_NOINDEX | DB_MODE_READ, 0, NULL, 0)) != DB_NO_ERROR)
        {
        sprintf(szBuffer, "Fatal Error %ld opening file %s.",error, szfname);
		Status(szBuffer);
		return false;
        }

    DbGetDatabaseInfo (hdb,NULL,&dbHdr,&recDefHdr,NULL);
	RecSize=recDefHdr->ulMaxRecordLength;
    TotalItemsToTranslate += dbHdr->ulNumRecords;
    DbCloseDatabase(hdb);

    //////////////////////////////////////
    // Allocate memory to read the data //
    //////////////////////////////////////

if ((Records = (char *)malloc (TotalItemsToTranslate*RecSize)) == NULL)
    {
	sprintf(szBuffer,"Fatal Error allocating %d bytes for run files selected",TotalItemsToTranslate* RecSize);
	Status(szBuffer);
	return false;
    }
	

	//////////////////////////////////////
    // read the data  and put ino mem   //
    //////////////////////////////////////
//    FileExtractName(szfname,NULL,NULL,szBase,NULL,NULL);

    ///////////////////////
    // Open the database //
    ///////////////////////
	sprintf(szBuffer,"Opening run file %s",szfname);
	Status(szBuffer);


	if ((error = DbOpenDatabase (&hdb, szfname, DB_MODE_READ, 0, NULL, 0)) != DB_NO_ERROR)
    {
		sprintf(szBuffer,"Cannot open file.  Code = %d",error);
		Status(szBuffer);
		free(Records);
		return false;
    }

	DbGetDatabaseInfo(hdb,&dbDesc,&dbHdr,&recDefHdr,NULL);

	if (dbHdr->ulNumRecords == 0)
	{
		sprintf(szBuffer,"File has Zero Records");
		Status(szBuffer);
		free(Records);
		return false;
	}


    ///////////////////
    // Read the data //
    ///////////////////

	sprintf(szBuffer,"Reading Runfile ");
	Status(szBuffer);
	DbSeek(hdb,DB_SEEK_BEGIN,0,NULL);

	ItemsRead = dbHdr->ulNumRecords;

	if ((error = DbRead (hdb, Records, dbHdr->ulNumRecords * recDefHdr->ulMaxRecordLength,&ItemsRead)) != DB_NO_ERROR)
    {
		sprintf(szBuffer,"Error reading file Code = %d",error);
		Status(szBuffer);
		free(Records);
		return false;
    }
	// Set Buff Init
	((CListCtrl*)DispBuff)->DeleteAllItems();
	((CListCtrl*)DispBuff)->ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	for(int j=5; j>=0; j--)
		if(!((CListCtrl*)DispBuff)->DeleteColumn(j)){
//			AfxMessageBox("Colum not del");
			((CListCtrl*)DispBuff)->DeleteColumn(j);
		}

	// Buffer CString for disp
	CString sBuff;

	//Draw by Colums
	int x=0,y;
	for (j=0; j < recDefHdr->sFieldCount; j++){
		sBuff="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		y=recDefHdr->fldDef[j].usFieldDisplayLength;
		x=((CListCtrl*)DispBuff)->GetStringWidth(sBuff.Left(y+8));
		sBuff=recDefHdr->fldDef[j].szFieldAbbr;
		((CListCtrl*)DispBuff)->InsertColumn(j,sBuff,LVCFMT_LEFT,x,j);
	}

	for(int i=0; i<ItemsRead; i++){
		for (j=0; j < recDefHdr->sFieldCount; j++){
			DbFieldToString(&recDefHdr->fldDef[j],Records+(i*RecSize), szFieldBuff, recDefHdr->fldDef[j].usFieldDisplayLength,DB_FMT_STRIP_SPHY | DB_FMT_RIGHTJUST,'0');
			sBuff=szFieldBuff;
			if(j==0) ((CListCtrl*)DispBuff)->InsertItem(i,sBuff,99);
			else ((CListCtrl*)DispBuff)->SetItem(i,j,LVIF_TEXT,sBuff,99,0,0,NULL);
		}
	}
	
	sprintf(szBuffer,"%d Records found, with %d fields",ItemsRead,j);
	Status(szBuffer);
	// Close DBF
	DbCloseDatabase(hdb);

	free(Records);
	return true;
}

void CIvExplorerDoc::SetSBar(CStatusBar *sb)
{
	sBar=sb;
}

void CIvExplorerDoc::Status(LPCTSTR m)
{
	if(sBar->m_hWnd!=NULL)
		sBar->SetPaneText(0,m);
}


