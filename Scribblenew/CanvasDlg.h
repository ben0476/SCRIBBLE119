#pragma once


// CanvasDlg dialog

class CanvasDlg : public CDialog
{
	DECLARE_DYNAMIC(CanvasDlg)

public:
	CanvasDlg(CWnd* pParent = NULL,const CSize &CanvasSize = CSize(800, 600), const COLORREF BackGColor = RGB(255, 255, 255));   // standard constructor
	virtual ~CanvasDlg();

// Dialog Data
	enum { IDD = IDD_CANVAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetImageSize();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnFileNew();
	int m_CanvasWidthV;
	int m_CanvasHeightV;
	COLORREF m_BackgroundColor;
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedBackgroundColor();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedBrowse();
//	afx_msg void OnEnKillfocusShowPath();
	afx_msg void OnEnChangeShowPath();
	CString m_FilePath;
};
