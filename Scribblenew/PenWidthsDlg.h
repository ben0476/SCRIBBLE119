#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPenWidthsDlg dialog

class CPenWidthsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPenWidthsDlg)

public:
	CPenWidthsDlg(CWnd* pParent = NULL);   // standard constructor
	CPenWidthsDlg(int nTextThinWidth, int nTextThickWidth, CWnd* pParent = NULL);
	virtual ~CPenWidthsDlg();
	void    UdatePenWidthValue();
	//virtual BOOL    PreTranslateMessage(MSG* pMsg);
// Dialog Data
	enum { IDD = IDD_PEN_WIDTHS };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDefault();
	int m_nThinWidth ;
	int m_nThickWidth;
	afx_msg void OnEnChangeThinPenEidth();
//	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SlThinWidth;
//	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeThickPenWidth();
	CSliderCtrl m_SlThickWidth;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CEdit THINEDC;
	CEdit THICKEDC;
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEnKillfocusThinPenEidth();
//	afx_msg void OnEnHscrollThickPenWidth();
	afx_msg void OnEnKillfocusThickPenWidth();
	
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
