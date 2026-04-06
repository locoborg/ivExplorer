// DispImg.cpp : implementation file
//

#include "stdafx.h"
#include "ivExplorer.h"
#include "DispImg.h"
#include "ivExplorerDoc.h"

#include <os2.h>
#include <db.h>
#include <ivio.h>

#include <wingdi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDispImg dialog


CDispImg::CDispImg(CWnd* pParent /*=NULL*/)
	: CDialog(CDispImg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDispImg)
	//}}AFX_DATA_INIT
}

CDispImg::SetFName(LPCSTR szFn)
{
	strcpy(szfname,szFn);
}

void CDispImg::Status(CString m,bool flerr)
{
	if(sBar->m_hWnd!=NULL)
		sBar->SetPaneText(0,m);
	if(flerr) m_ImgData.AddString(m);
}

void CDispImg::SetSBar(CStatusBar *sb)
{
	sBar=sb;
}

void CDispImg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDispImg)
	DDX_Control(pDX, IDC_LIST1, m_ImgData);
	DDX_Control(pDX, IDC_IMG, m_img);
	DDX_Control(pDX, IDC_COUNT, m_nImg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDispImg, CDialog)
	//{{AFX_MSG_MAP(CDispImg)
	ON_BN_CLICKED(IDOK5, OnBegin)
	ON_BN_CLICKED(IDOK2, OnBack)
	ON_BN_CLICKED(IDOK4, OnForward)
	ON_BN_CLICKED(IDOK6, OnEnd)
	ON_BN_CLICKED(IDOK3, OnImgBack)
	ON_BN_CLICKED(IDOK, OnExit)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDispImg message handlers

void CDispImg::OnBegin() 
{
	// TODO: Add your control notification handler code here
	cImg=0;
	DbImgSeek(hdbImageBitonal,DB_SEEK_BEGIN,0,NULL);
}

void CDispImg::OnBack() 
{
	// TODO: Add your control notification handler code here
	DbError r=DbImgSeek(hdbImageBitonal,DB_SEEK_BEGIN,--cImg,NULL);
	if(cImg<0) cImg=0;
	char szImg[10];
	sprintf(szImg,"%d",cImg);
	m_nImg.SetWindowText(szImg);
	ReadImg();
}

void CDispImg::OnForward() 
{
	// TODO: Add your control notification handler code here
	DbError r=DbImgSeek(hdbImageBitonal,DB_SEEK_BEGIN,++cImg,NULL);
	if(r>0) cImg--;
	char szImg[10];
	sprintf(szImg,"%d",cImg);
	m_nImg.SetWindowText(szImg);
	ReadImg();
}

void CDispImg::OnEnd() 
{
	// TODO: Add your control notification handler code here
	cImg=0;
	DbImgSeek(hdbImageBitonal,DB_SEEK_END,0,NULL);
	ReadImg();
}

void CDispImg::OnImgBack() 
{
	char sztmp[128];
	int ret=0;

	flBack=!flBack;

	TiffItemDataType   *TiffItemData;

	TiffItemHeader = (TiffItemHeaderType *)ivImgBuff;
	TiffItemData = (TiffItemDataType *)(ivImgBuff + sizeof(TiffItemHeaderType));

	// Erase img data
	int nlines=m_ImgData.GetCount();
	for(int ss=5; ss<nlines; ss++)
		m_ImgData.DeleteString(5);
	// Display img data
	CString strMsg;
	strMsg.Format("Image data:   %s",TiffItemHeader->szDocName);
	m_ImgData.AddString(strMsg);

	strMsg.Format("size :  (%d, %d)",TiffItemData -> ulWidthValue,TiffItemData -> ulHeightValue);
	m_ImgData.AddString(strMsg);	

	DWORD BottomUpFlag =(TiffItemHeader->ulREIReserved1Value == 1) ? 0 : 1;

	Status("Converting image ...",false);


	// TIFF_TAG_IMAGE_WIDTH:
	iw=TiffItemData -> ulWidthValue;
	// TIFF_TAG_IMAGE_LENGTH:
	ih=TiffItemData -> ulHeightValue;

	flJPEG=false;
	if (TiffItemData->usTagCount > 50){
		Status("Image read error: Image data is incorrect",true);
	}else if (TiffItemData->usTagCount > 22){
		TiffJPEGDataType *TiffData = (TiffJPEGDataType *)TiffItemData;
		if(flBack)//ulStripByteCountsValue or ulNextIFD
			TiffData = (TiffJPEGDataType *)(ivImgBuff + TiffData->ulNextIFD);

		// Disp res
		strMsg.Format("DPI :  (%d, %d), (%d, %d), (%d, %d)",TiffData -> ulXResolutionValue,TiffData -> ulYResolutionValue,TiffData -> ulXRes[0],TiffData -> ulYRes[0],TiffData -> ulXRes[1],TiffData -> ulYRes[1]);
		m_ImgData.AddString(strMsg);

		try{
		DecompressJPEG( TiffData -> bData,TiffData -> ulStripByteCountsValue,(PBYTE) fimg,				
		     			ih,iw,iw,
     					(unsigned char *) TiffData -> szQTable,(unsigned char *) TiffData -> szDCTable,(unsigned char *) TiffData -> szACTable,
	    				1,0,0,ih,iw);
		} catch(...) {}
		flJPEG=true;
	}else{
		if(flBack)//ulStripByteCountsValue or ulNextIFD
			TiffItemData = (TiffItemDataType *)(ivImgBuff + TiffItemData->ulNextIFD);
		// Disp res
		strMsg.Format("DPI :  (%d, %d), (%d, %d), (%d, %d)",TiffItemData -> ulXResolutionValue,TiffItemData -> ulYResolutionValue,TiffItemData -> ulXRes[0],TiffItemData -> ulYRes[0],TiffItemData -> ulXRes[1],TiffItemData -> ulYRes[1]);
		m_ImgData.AddString(strMsg);

		if((ret=G4toBmp (iw, ih, TiffItemData -> bData,fimg,0,4,((iw+7)/8)))!=0){
			sprintf(sztmp,"Image convertion error #%d : Image format incorrect (Expected TIFF 6.0)",ret);
			Status(sztmp,true);
		}
	}
		
	Status("Use buttons to change image",false);

	OnPaint();	
}

void CDispImg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if (hdbImageBitonal != (DBHANDLE) NULL)
			DbImgClose (hdbImageBitonal);
	EndDialog(0);
}

BOOL CDispImg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString    strMsg,
			   strFileName;
	DWORD      dwResult1;
			   
	
	FieldDef *FieldIndex=new FieldDef;

	PBYTE               pbReadBuffer   = new BYTE[65536L * 4L];
    TiffFileHeader = (TiffFileHeaderType *) new (TiffFileHeaderType);
    TiffItemHeader = (TiffItemHeaderType *) new BYTE[TIFF_ALLOC];

	cImg=0;
	flBack=false;
	hdbImageBitonal = (DBHANDLE) NULL;
	fimg=new UCHAR[MAX_IMAGE_SIZE];
	ivImgBuff=new char[MAX_IMAGE_SIZE];


	FieldIndex -> usOffset = 0L;

	Status("Opening image file...",false);
    
	if ((dwResult1 = DbAnywhereImgOpen (&hdbImageBitonal,
					            szfname,
						        NULL,
							    1,
					            DB_MODE_READ,
						        (PVOID) TiffFileHeader)) != 0)
		{
		strMsg . Format ("Open Image File Error: %s %d", szfname, dwResult1);
		Status (strMsg,true);

		delete [] TiffItemHeader;
		delete (TiffFileHeader);
		delete [] pbReadBuffer;

		return false;

		}

	strMsg.Format("Name:   %s",TiffFileHeader->szDocName);
	m_ImgData.AddString(strMsg);
	strMsg.Format("Time & Date:   %s",TiffFileHeader->szDateTime);
	m_ImgData.AddString(strMsg);
	strMsg.Format("Description:   %s",TiffFileHeader->szDescription);
	m_ImgData.AddString(strMsg);
	strMsg.Format("Make:   %s",TiffFileHeader->szMake);
	m_ImgData.AddString(strMsg);
	strMsg.Format("Software:   %s",TiffFileHeader->szSoftware);
	m_ImgData.AddString(strMsg);

	DbImgSeek(hdbImageBitonal,DB_SEEK_BEGIN,cImg,NULL);
	ReadImg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDispImg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (hdbImageBitonal != (DBHANDLE) NULL)
			DbImgClose (hdbImageBitonal);	
	CDialog::OnClose();
}

void CDispImg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	
}

void CDispImg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	int iBits=1;
	DWORD PasteImg=NOTSRCCOPY;
	if(flJPEG)// iBits=8;
		PasteImg=SRCCOPY;

	// temporary CDC for check images
	CDC tmpCDC;
	tmpCDC.CreateCompatibleDC(NULL);
	if(tmpCDC.GetSafeHdc() != NULL){
				CBitmap *imgbm=new CBitmap();
				if(!flJPEG)
					imgbm->CreateBitmap(iw,ih,1,iBits,fimg);
				else {
					HDC               hdc;
					COLORBITMAPINFO   cbmpi;
					hdc     = CreateDC("DISPLAY",NULL,NULL,NULL);

					cbmpi.bmInfoHeader.biSize          = sizeof(cbmpi.bmInfoHeader); 
					cbmpi.bmInfoHeader.biWidth         = iw;
					cbmpi.bmInfoHeader.biHeight        = ih;
					cbmpi.bmInfoHeader.biPlanes        = 1; 
					cbmpi.bmInfoHeader.biBitCount      = 8;
					cbmpi.bmInfoHeader.biCompression   = BI_RGB; 
					cbmpi.bmInfoHeader.biSizeImage     = 0; 
					cbmpi.bmInfoHeader.biXPelsPerMeter = 70; 
					cbmpi.bmInfoHeader.biYPelsPerMeter = 70; 
					cbmpi.bmInfoHeader.biClrUsed       = 256; 
					cbmpi.bmInfoHeader.biClrImportant  = 256; 

					for (int i=0; i<256; i++)
					{
						cbmpi.bmColors[i].rgbBlue     = (BYTE) i;
						cbmpi.bmColors[i].rgbGreen    = (BYTE) i;
						cbmpi.bmColors[i].rgbRed      = (BYTE) i;
						cbmpi.bmColors[i].rgbReserved = 0;
					}

					imgbm = CBitmap::FromHandle(CreateDIBitmap(hdc,&cbmpi.bmInfoHeader,CBM_INIT,fimg,(CONST BITMAPINFO *)&cbmpi,DIB_RGB_COLORS));
					DeleteDC(hdc);
				}	
				tmpCDC.SelectObject(imgbm);
				CRect Rect;
				m_img.GetClientRect(&Rect);
				m_img.GetDC() -> StretchBlt(Rect.TopLeft().x,Rect.TopLeft().y,
											Rect.Width(), Rect.Height(),&tmpCDC,
											0, 0, iw, ih, PasteImg);
	}

	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDispImg::OnDestroy() 
{
	if (hdbImageBitonal != (DBHANDLE) NULL)
			DbImgClose (hdbImageBitonal);
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

bool CDispImg::ReadImg()
{
	char sztmp[128];
	int ret=0;

	DWORD      dwResult1;
	TiffItemDataType   *TiffItemData;

	long lNumItems=1;

	Status("Reading image...",false);

	if((dwResult1=DbImgRead (hdbImageBitonal,
				 (PVOID) ivImgBuff,
				 MAX_IMAGE_SIZE,
				 &lNumItems,
				 DB_IMG_BOTH)) != 0)
	{
		Status("Error reading image file",true);
		return false;
	}

    TiffItemHeader = (TiffItemHeaderType *)ivImgBuff;

	// Erase img data
	int nlines=m_ImgData.GetCount();
	for(int ss=5; ss<nlines; ss++)
		m_ImgData.DeleteString(5);
	// Display img data
	CString strMsg;
	strMsg.Format("Image data:   %s",TiffItemHeader->szDocName);
	m_ImgData.AddString(strMsg);
	

	TiffItemData = (TiffItemDataType *)(ivImgBuff + sizeof(TiffItemHeaderType));

	strMsg.Format("size :  (%d, %d)",TiffItemData -> ulWidthValue,TiffItemData -> ulHeightValue);
	m_ImgData.AddString(strMsg);

	Status("Converting image ...",false);

	// TIFF_TAG_IMAGE_WIDTH:
	iw=TiffItemData -> ulWidthValue;
	// TIFF_TAG_IMAGE_LENGTH:
	ih=TiffItemData -> ulHeightValue;

	flJPEG=false;
	if (TiffItemData->usTagCount > 50){
		Status("Image read error: Image data is incorrect",true);
	}else if (TiffItemData->usTagCount > 22){
		TiffJPEGDataType *TiffData = (TiffJPEGDataType *)TiffItemData;
		if(flBack)//ulStripByteCountsValue or ulNextIFD
			TiffData = (TiffJPEGDataType *)(ivImgBuff + TiffData->ulNextIFD);

		// Disp res
		strMsg.Format("DPI :  (%d, %d), (%d, %d), (%d, %d)",TiffData -> ulXResolutionValue,TiffData -> ulYResolutionValue,TiffData -> ulXRes[0],TiffData -> ulYRes[0],TiffData -> ulXRes[1],TiffData -> ulYRes[1]);
		m_ImgData.AddString(strMsg);
		try{
		DecompressJPEG( TiffData -> bData,TiffData -> ulStripByteCountsValue,(PBYTE) fimg,				
		     			ih,iw,iw,
     					(unsigned char *) TiffData -> szQTable,(unsigned char *) TiffData -> szDCTable,(unsigned char *) TiffData -> szACTable,
	    				1,0,0,ih,iw);
		} catch (...){}
		flJPEG=true;
	}else{
		if(flBack)//ulStripByteCountsValue or ulNextIFD
			TiffItemData = (TiffItemDataType *)(ivImgBuff + TiffItemData->ulNextIFD);

		// Disp res
		strMsg.Format("DPI :  (%d, %d), (%d, %d), (%d, %d)",TiffItemData -> ulXResolutionValue,TiffItemData -> ulYResolutionValue,TiffItemData -> ulXRes[0],TiffItemData -> ulYRes[0],TiffItemData -> ulXRes[1],TiffItemData -> ulYRes[1]);
		m_ImgData.AddString(strMsg);

		if((ret=G4toBmp (iw, ih, TiffItemData -> bData,fimg,0,4,((iw+7)/8)))!=0){
			sprintf(sztmp,"Image convertion error #%d : Image format incorrect (Expected TIFF 6.0)",ret);
			Status(sztmp,true);
		}
	}
	
	Status("Use buttons to change image",false);

	OnPaint();
	return true;
}
