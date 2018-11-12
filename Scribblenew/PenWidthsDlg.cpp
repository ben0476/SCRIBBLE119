// PenWidthsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Scribblenew.h"
#include "PenWidthsDlg.h"
#include "afxdialogex.h"


// CPenWidthsDlg dialog

IMPLEMENT_DYNAMIC(CPenWidthsDlg, CDialog)

CPenWidthsDlg::CPenWidthsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPenWidthsDlg::IDD, pParent)
	, m_nThinWidth(0)
	, m_nThickWidth(0)
{

}



CPenWidthsDlg::~CPenWidthsDlg()
{
}

void CPenWidthsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_THIN_PEN_EIDTH, m_nThinWidth);
	DDV_MinMaxInt(pDX, m_nThinWidth, 1, 20);
	DDX_Text(pDX, IDC_THICK_PEN_WIDTH, m_nThickWidth);
	DDV_MinMaxInt(pDX, m_nThickWidth, 1, 20);
	DDX_Control(pDX, IDC_THIN_SLIDER, m_SlThinWidth);
	DDX_Control(pDX, IDC_THICK_SLIDER, m_SlThickWidth);
	DDX_Control(pDX, IDC_THIN_PEN_EIDTH, THINEDC);
	DDX_Control(pDX, IDC_THICK_PEN_WIDTH, THICKEDC);
}


BEGIN_MESSAGE_MAP(CPenWidthsDlg, CDialog)
	ON_BN_CLICKED(IDC_DEFAULT, &CPenWidthsDlg::OnBnClickedDefault)
	ON_EN_CHANGE(IDC_THIN_PEN_EIDTH, &CPenWidthsDlg::OnEnChangeThinPenEidth)

	ON_EN_CHANGE(IDC_THICK_PEN_WIDTH, &CPenWidthsDlg::OnEnChangeThickPenWidth)
	ON_WM_HSCROLL()
//	ON_WM_KILLFOCUS()
ON_EN_KILLFOCUS(IDC_THIN_PEN_EIDTH, &CPenWidthsDlg::OnEnKillfocusThinPenEidth)
//ON_EN_HSCROLL(IDC_THICK_PEN_WIDTH, &CPenWidthsDlg::OnEnHscrollThickPenWidth)
ON_EN_KILLFOCUS(IDC_THICK_PEN_WIDTH, &CPenWidthsDlg::OnEnKillfocusThickPenWidth)
END_MESSAGE_MAP()


// CPenWidthsDlg message handlers


void CPenWidthsDlg::OnBnClickedDefault()
{
	// TODO: Add your control notification handler code here
	m_nThinWidth = 2;
	m_nThickWidth = 4;
	UdatePenWidthValue();
}

BOOL CPenWidthsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();// **********remember to reset position all the time 
	m_SlThinWidth.SetRange(1,20,TRUE); //slider range 1-20
	//m_SlThinWidth.SetPos(m_nThinWidth); //get default value from InitDocument() and set pos 
	m_SlThickWidth.SetRange(1,20,TRUE);
   //m_SlThickWidth.SetPos(m_nThickWidth);
	
	UdatePenWidthValue();

	return TRUE; 
}

void CPenWidthsDlg::OnEnChangeThinPenEidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString strGetNumThin;
	CEdit *editThin = (CEdit *)GetDlgItem(IDC_THIN_PEN_EIDTH);
	editThin->GetWindowText(strGetNumThin);//get text from EIDTH
	m_nThinWidth = _ttoi(strGetNumThin);//CString to int
	if(m_nThinWidth > m_nThickWidth)
	{
		m_nThinWidth = m_nThickWidth;
	}
	else
	{
		m_nThinWidth = m_nThinWidth;
	}
    m_SlThinWidth.SetPos(m_nThinWidth); //set slider position
	//UpdateData(FALSE);
	
}

void CPenWidthsDlg::OnEnChangeThickPenWidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	
	CString strGetNumThick;
	CEdit *editThick = (CEdit *)GetDlgItem(IDC_THICK_PEN_WIDTH);
	editThick->GetWindowText(strGetNumThick);//get text from EIDTH
	m_nThickWidth = _ttoi(strGetNumThick);//CString to int
	if(m_nThickWidth < m_nThinWidth)//thick can't smaller than thin
	{  
		m_nThickWidth = m_nThinWidth;   
	}   
	else 
	{
		m_nThickWidth = m_nThickWidth;
	}
	m_SlThickWidth.SetPos(m_nThickWidth);  //set slider position 
	//UpdateData(FALSE);
	
}


void CPenWidthsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	// TODO: Add your message handler code here and/or call default
	
	if(pScrollBar == (CScrollBar *) &m_SlThinWidth){
		m_nThinWidth = m_SlThinWidth.GetPos();
		if(m_nThinWidth > m_nThickWidth) 
			m_nThinWidth = m_nThickWidth;
		
	}
	else if(pScrollBar == (CScrollBar *) &m_SlThickWidth){
		m_nThickWidth = m_SlThickWidth.GetPos(); //get slider position to value
		if(m_nThickWidth < m_nThinWidth)  //thick can't smaller than thin
			m_nThickWidth = m_nThinWidth;
		
	}
	UdatePenWidthValue();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPenWidthsDlg::UdatePenWidthValue()
{

	m_SlThinWidth.SetPos(m_nThinWidth); //set slider position
	SetDlgItemInt(IDC_THIN_PEN_EIDTH, m_nThinWidth);

	m_SlThickWidth.SetPos(m_nThickWidth);
	SetDlgItemInt(IDC_THICK_PEN_WIDTH, m_nThickWidth);
}

void CPenWidthsDlg::OnEnKillfocusThinPenEidth()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	UdatePenWidthValue();
}

void CPenWidthsDlg::OnEnKillfocusThickPenWidth()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	UdatePenWidthValue();
}



