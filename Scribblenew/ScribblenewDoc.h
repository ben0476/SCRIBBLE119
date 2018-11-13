
// ScribblenewDoc.h : CScribblenewDoc 類別的介面
//


#pragma once

class CStroke : public CObject
{
public:
	//CStroke( UINT nPenWidth );
	//add pen color item 
	CStroke(UINT nPenWidth, COLORREF PenColor = RGB(0, 0, 0) );
protected:
	CStroke();
	DECLARE_SERIAL(CStroke)
	
	// Attributes
protected:
	UINT  m_nPenWidth;    // pen width 
	CRect m_rectBounding; // smallest rect that surrounds all
	COLORREF m_PenColor;  //pen color
public:
	CArray<CPoint,CPoint>  m_pointArray;   // draw points
	
	// of the points in the stroke
	// measured in MM_LOENGLISH units
	// (0.01 inches, with Y-axis inverted)
public:
	CRect& GetBoundingRect() { return m_rectBounding; }
    
	// Operations
public:
	BOOL DrawStroke(CDC* pDC);
	void FinishStroke();

public:
	virtual void Serialize(CArchive& ar);
};

class CScribblenewDoc : public CDocument
{
protected: // 僅從序列化建立
	CScribblenewDoc();
	DECLARE_DYNCREATE(CScribblenewDoc)
	void ReplacePen();
	
// 屬性
public:
	CTypedPtrList<CObList,CStroke*> m_strokeList; //List 
	CPen* GetCurrentPen() { return &m_penCur; }
	COLORREF GetBackgroundColor() { return m_MBackgroundColor; }
	CSize GetDocSize(){ return m_sizeDoc; }
protected:
	UINT m_nPenWidth; // current pen width
	CPen m_penCur; // current pen style
	BOOL            m_bThickPen;       // TRUE if current pen is thick
	UINT            m_nThinWidth;
	UINT            m_nThickWidth;
	COLORREF        m_PenColor;
	CSize           m_sizeDoc;
	COLORREF        m_MBackgroundColor;

// 作業
public:
	CStroke* NewStroke();//
	
// 覆寫
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);//
	virtual void DeleteContents();//
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 程式碼實作
public:
	virtual ~CScribblenewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void InitDocument();//

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 為搜尋處理常式設定搜尋內容的 Helper 函式
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnPenClearall();
	afx_msg void OnPenThickline();
	afx_msg void OnUpdatePenThickline(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePenClearall(CCmdUI *pCmdUI);
	afx_msg void OnPenWidths();
	;
	afx_msg void OnPenColor();
	//afx_msg void OnIddCanvas();
	afx_msg void OnFileNew();
};

