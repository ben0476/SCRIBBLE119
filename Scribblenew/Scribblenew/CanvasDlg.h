#pragma once


// CanvasDlg dialog

class CanvasDlg : public CDialog
{
	DECLARE_DYNAMIC(CanvasDlg)

public:
	CanvasDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CanvasDlg();

// Dialog Data
	enum { IDD = IDD_CANVAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileNew();
	int m_CanvasWidthV;
	int m_CanvasHeightV;
	COLORREF m_BackgroundColor;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedBackgroundColor();
	
	
};
