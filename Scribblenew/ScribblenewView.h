
// ScribblenewView.h : CScribblenewView 類別的介面
//

#pragma once


class CScribblenewView : public CScrollView
{
protected: // 僅從序列化建立
	CScribblenewView();
	DECLARE_DYNCREATE(CScribblenewView)

// 屬性
public:
	CScribblenewDoc* GetDocument() const;
	virtual void OnInitialUpdate();
protected:
	CStroke*    m_pStrokeCur;   // stroke in progress
	CPoint      m_ptPrev;       // last mouse point in stroke 
private:
	CDC *m_pdcDisplayMemory;
	CBitmap *m_pBitmap;
// 作業
public:

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CScribblenewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* pHint);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void ResyncScrollSizes();
};

#ifndef _DEBUG  // ScribblenewView.cpp 中的偵錯版本
inline CScribblenewDoc* CScribblenewView::GetDocument() const
   { return reinterpret_cast<CScribblenewDoc*>(m_pDocument); }
#endif

