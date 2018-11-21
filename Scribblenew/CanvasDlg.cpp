// CanvasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Scribblenew.h"
#include "CanvasDlg.h"
#include "afxdialogex.h"


// CanvasDlg dialog

IMPLEMENT_DYNAMIC(CanvasDlg, CDialog)

CanvasDlg::CanvasDlg(CWnd* pParent  /*=NULL*/,const CSize &CanvasSize, const COLORREF BackGColor) // standard constructor
: CDialog(CanvasDlg::IDD, pParent)
, m_FilePath(_T(""))
{
	m_CanvasWidthV = CanvasSize.cx;
	m_CanvasHeightV = CanvasSize.cy;
	m_BackgroundColor = BackGColor;
}

CanvasDlg::~CanvasDlg()
{
}

void CanvasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Canvas_Width, m_CanvasWidthV);
	DDX_Text(pDX, IDC_Canvas_Height, m_CanvasHeightV);
	DDX_Text(pDX, IDC_SHOW_PATH, m_FilePath);
}


BEGIN_MESSAGE_MAP(CanvasDlg, CDialog)
//	ON_COMMAND(ID_FILE_NEW, &CanvasDlg::OnFileNew)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Background_Color, &CanvasDlg::OnClickedBackgroundColor)
	
	
	ON_BN_CLICKED(IDC_CLEAR, &CanvasDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_BROWSE, &CanvasDlg::OnBnClickedBrowse)
//	ON_EN_KILLFOCUS(IDC_SHOW_PATH, &CanvasDlg::OnEnKillfocusShowPath)
ON_EN_CHANGE(IDC_SHOW_PATH, &CanvasDlg::OnEnChangeShowPath)
END_MESSAGE_MAP()


// CanvasDlg message handlers


//void CanvasDlg::OnFileNew()
//{
//	// TODO: Add your command handler code here
//}


HBRUSH CanvasDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	

	if(pWnd->GetDlgCtrlID() == IDC_Color_Pick)
	{
		pDC->SetBkColor(m_BackgroundColor);
		HBRUSH Brush = CreateSolidBrush(m_BackgroundColor);
		return Brush;
	}
	
	return hbr;
}


void CanvasDlg::OnClickedBackgroundColor()
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg(m_BackgroundColor); 
	if(dlg.DoModal() == IDOK)
	{
		m_BackgroundColor = dlg.GetColor();

		//Updates the specified rectangle or region in the given window's client area.
		RedrawWindow();
	}
}

void CanvasDlg::OnBnClickedClear()
{
	// TODO: Add your control notification handler code here
	//clear the path text
	SetDlgItemText(IDC_SHOW_PATH, _T(""));

	/*GetDlgItem(IDC_Canvas_Width)->EnableWindow(TRUE);
	GetDlgItem(IDC_Canvas_Height)->EnableWindow(TRUE);
	GetDlgItem(IDC_Background_Color)->EnableWindow(TRUE);*/
}


void CanvasDlg::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog fd(
					TRUE, 
					NULL, 
					NULL, 
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ENABLESIZING,
					_T("BMP Files (*.bmp)|*.bmp;)"),
					NULL);

	if(fd.DoModal() == IDOK)  
	{
		//get file path
		 m_FilePath =  fd.GetPathName();
		//show file path 
		SetDlgItemText(IDC_SHOW_PATH, m_FilePath);
		GetImageSize();
	}
	else  
	{

	}

}

void CanvasDlg::OnEnChangeShowPath()
{
	CString Gettext;
	GetDlgItemText(IDC_SHOW_PATH, Gettext);
	if (Gettext.IsEmpty())
	{
		GetDlgItem(IDC_Canvas_Width)->EnableWindow(TRUE);
		GetDlgItem(IDC_Canvas_Height)->EnableWindow(TRUE);
		GetDlgItem(IDC_Background_Color)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_Canvas_Width)->EnableWindow(FALSE);
		GetDlgItem(IDC_Canvas_Height)->EnableWindow(FALSE);
		GetDlgItem(IDC_Background_Color)->EnableWindow(FALSE);
	}
}

void CanvasDlg::GetImageSize()
{
	if(m_FilePath.IsEmpty() )
	{
		return ;
	}

	CImage image;
	LPCSTR FilePath = (LPCSTR)(LPCTSTR)m_FilePath;
	image.Load ((LPCTSTR) FilePath);

	//CClientDC dc(this);	
	//CSize IMGSize(image.GetWidth(), image.GetHeight());
	SetDlgItemInt(IDC_Canvas_Width, image.GetWidth() );
	SetDlgItemInt(IDC_Canvas_Height,image.GetHeight() );

}