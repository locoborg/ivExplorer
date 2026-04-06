#if !defined(AFX_DISPIMG_H__912BA566_B49C_11D3_8E7E_0080C74A05BC__INCLUDED_)
#define AFX_DISPIMG_H__912BA566_B49C_11D3_8E7E_0080C74A05BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DispImg.h : header file
//

#include <image.h>
/////////////////////////////////////////////////////////////////////////////
// CDispImg dialog

#define TIFF_ALLOC		524288L
#define MAX_IMAGE_SIZE	2000000
typedef ULONG       DBHANDLE;

typedef struct {
   BITMAPINFOHEADER bmInfoHeader; 
   RGBQUAD          bmColors[256]; 
} COLORBITMAPINFO; 


class CDispImg : public CDialog
{
// Construction
public:
	bool ReadImg();
	void SetSBar(CStatusBar *sb);
	CStatusBar *sBar;
	void Status(CString m,bool flerr);
	CDispImg(CWnd* pParent = NULL);   // standard constructor
	SetFName(LPCSTR szFn);
	char szfname[128];
	int cImg;
// Dialog Data
	//{{AFX_DATA(CDispImg)
	enum { IDD = IDD_DIALOG1 };
	CListBox	m_ImgData;
	CStatic	m_img;
	CStatic	m_nImg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDispImg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	char *ivImgBuff;//[MAX_IMAGE_SIZE];
	UCHAR *fimg;//[MAX_IMAGE_SIZE];
	int iw, ih;
	bool flBack,flJPEG;
	DBHANDLE hdbImageBitonal;
	TiffFileHeaderType *TiffFileHeader;
	TiffItemHeaderType *TiffItemHeader;
	// Generated message map functions
	//{{AFX_MSG(CDispImg)
	afx_msg void OnBegin();
	afx_msg void OnBack();
	afx_msg void OnForward();
	afx_msg void OnEnd();
	afx_msg void OnImgBack();
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPIMG_H__912BA566_B49C_11D3_8E7E_0080C74A05BC__INCLUDED_)
