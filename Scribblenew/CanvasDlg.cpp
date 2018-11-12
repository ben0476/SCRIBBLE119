// CanvasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Scribblenew.h"
#include "CanvasDlg.h"
#include "afxdialogex.h"


// CanvasDlg dialog

IMPLEMENT_DYNAMIC(CanvasDlg, CDialogEx)

CanvasDlg::CanvasDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CanvasDlg::IDD, pParent)
{

	m_CanvasWidthV = 800;
	m_CanvasHeightV = 600;
	m_BackgroundColor = RGB(255, 255, 255);
}

CanvasDlg::~CanvasDlg()
{
}

void CanvasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Canvas_Width, m_CanvasWidthV);
	DDX_Text(pDX, IDC_Canvas_Height, m_CanvasHeightV);
}


BEGIN_MESSAGE_MAP(CanvasDlg, CDialogEx)
//	ON_COMMAND(ID_FILE_NEW, &CanvasDlg::OnFileNew)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Background_Color, &CanvasDlg::OnClickedBackgroundColor)
END_MESSAGE_MAP()


// CanvasDlg message handlers


//void CanvasDlg::OnFileNew()
//{
//	// TODO: Add your command handler code here
//}


HBRUSH CanvasDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	

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
