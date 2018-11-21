
// ScribblenewView.cpp : CScribblenewView 類別的實作
//

#include "stdafx.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Scribblenew.h"
#endif

#include "ScribblenewDoc.h"
#include "ScribblenewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScribblenewView

IMPLEMENT_DYNCREATE(CScribblenewView, CScrollView)

BEGIN_MESSAGE_MAP(CScribblenewView, CScrollView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_WM_HSCROLL()
//	ON_WM_VSCROLL()
//ON_WM_ERASEBKGND()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CScribblenewView 建構/解構

CScribblenewView::CScribblenewView()
{
	// TODO: 在此加入建構程式碼
	
}

CScribblenewView::~CScribblenewView()
{

}

BOOL CScribblenewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CView::PreCreateWindow(cs);
}

// CScribblenewView 描繪

void CScribblenewView::OnDraw(CDC* pDC)
{
	CScribblenewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此加入原生資料的描繪程式碼
	//
	CBitmap MemBitmap;
	CRect rectClip;
	CRect rectClipf;
	pDC->GetClipBox(&rectClip);//get seeing area
	GetClientRect(&rectClipf);
	CSize CanvasSize = pDoc->GetDocSize();  //get canvas size we set
	CRect CanvasRect = rectClip;
	CanvasRect.SetRect(0, 0, CanvasSize.cx, 0- CanvasSize.cy); //set canvas rect
	//double buffer
	MemDC.CreateCompatibleDC(pDC);
	bmpCanvas.CreateCompatibleBitmap(pDC, rectClip.right, rectClip.bottom);
	CBitmap *pOldBi = MemDC.SelectObject(&bmpCanvas);
    CImage image;
	CString getpath = GetDocument()->GetFilePath();
	if(GetDocument()->IfUseImage())
	{
		LPCSTR FilePath = (LPCSTR)(LPCTSTR)getpath;
		image.Load ((LPCTSTR)FilePath);
		
		CBitmap *pOldBi = MemDC.SelectObject(&bmpCanvas);
		//filled default color to all panel
		MemDC.FillSolidRect(0, 0, rectClip.right, rectClip.bottom, RGB(255, 255, 255));
		image.Draw(MemDC, 0, 0); 
	}
	else
	{
		
		CBitmap *pOldBi = MemDC.SelectObject(&bmpCanvas);
		//filled default color to all panel
		MemDC.FillSolidRect(0, 0, rectClip.right, rectClip.bottom, RGB(255, 255, 255));
		//filled color we pick for canvas
		MemDC.FillSolidRect(0, 0, CanvasRect.right, 0 - CanvasRect.bottom, pDoc->GetBackgroundColor());
	}


	//CBitmap *pOldBi = MemDC.SelectObject(pBitmap);
	////painting area limit
    MemDC.SelectClipRgn(&Canvas);
//filled color we pick for canvas
 //   MemDC.FillSolidRect(0, 0, CanvasRect.right, 0 - CanvasRect.bottom, pDoc->GetBackgroundColor());
	
	/*pDC->LPtoDP(&rectClip);
	rectClip.InflateRect(1, -1); */

	CTypedPtrList<CObList,CStroke*>& strokeList = pDoc->m_strokeList;
	POSITION pos = strokeList.GetHeadPosition();
	while (pos != NULL)
	{
		//BOOL ll =FALSE;
		CStroke* pStroke = strokeList.GetNext(pos);
		CRect rectStroke,RrectStroke;
		rectStroke.NormalizeRect();
		rectStroke = pStroke->GetBoundingRect();
		pDC->LPtoDP(&rectStroke);
		rectStroke.InflateRect(1, 1); 

/*			if (!RrectStroke.IntersectRect(&rectStroke, &rectClip))
			continue;*/
			pStroke->DrawStroke(&MemDC);
		
	}

	pDC->BitBlt(0,0,rectClip.right,rectClip.bottom,&MemDC,0,0,SRCCOPY);
	pDC->SelectObject(pOldBi); 
	

	bmpCanvas.DeleteObject();
	MemDC.DeleteDC();
                           
}


// CScribblenewView 列印

BOOL CScribblenewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void CScribblenewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void CScribblenewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}


// CScribblenewView 診斷

#ifdef _DEBUG
void CScribblenewView::AssertValid() const
{
	CView::AssertValid();
}

void CScribblenewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScribblenewDoc* CScribblenewView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScribblenewDoc)));
	return (CScribblenewDoc*)m_pDocument;
}
#endif //_DEBUG


// CScribblenewView 訊息處理常式


void CScribblenewView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// CScrollView changes the viewport origin and mapping mode,so we need to covert point coordinate
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	//mouse right click star painting and creat new stroke
	m_pStrokeCur = GetDocument()->NewStroke();
	// Add first point to the new stroke
	m_pStrokeCur->m_pointArray.Add(point);

	SetCapture();       // Capture the mouse until button up.
	m_ptPrev = point;   //be the first point of line
	
	CView::OnLButtonDown(nFlags, point);
	return;
}


void CScribblenewView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (GetCapture() != this)
		return;

	CScribblenewDoc* pDoc = GetDocument();

	CClientDC dc(this);

	 // set up mapping mode and viewport origin
	OnPrepareDC(&dc); 
	dc.DPtoLP(&point);
    dc.SelectClipRgn(&Canvas);

   
	CPen* pOldPen = dc.SelectObject(pDoc->GetCurrentPen());
	dc.MoveTo(m_ptPrev);
	dc.LineTo(point);
	dc.SelectObject(pOldPen);
	 m_pStrokeCur->m_pointArray.Add(point);
	
	//finish painting event,start calculate rect
	m_pStrokeCur->FinishStroke();
	//update others views to change 
	pDoc->UpdateAllViews(this, 0L, m_pStrokeCur);

	ReleaseCapture(); 
	
	CView::OnLButtonUp(nFlags, point);
	return;
}


void CScribblenewView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (GetCapture() != this)
	return; // If this window (view) didn't capture the mouse,
	// then the user isn't drawing in this window.

	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	dc.SelectClipRgn(&Canvas);

	CPen* pOldPen = dc.SelectObject(GetDocument()->GetCurrentPen());
	dc.MoveTo(m_ptPrev);
	dc.LineTo(point);
	dc.SelectObject(pOldPen);
	m_pStrokeCur->m_pointArray.Add(point);
	m_ptPrev = point;
	
	//Invalidate();
	CView::OnMouseMove(nFlags, point);
	return;
}

void CScribblenewView::OnUpdate(CView* /* pSender */, LPARAM /* lHint */, 
							 CObject* pHint) 
{
	// The document has informed this view that some data has changed.

	if (pHint != NULL)
	{
		if (pHint->IsKindOf(RUNTIME_CLASS(CStroke)))
		{
			// The hint is that a stroke as been changed.
			
			CStroke* pStroke = (CStroke*)pHint;
			CClientDC dc(this);
			OnPrepareDC(&dc);
			CRect rectInvalid = pStroke->GetBoundingRect();
			dc.LPtoDP(&rectInvalid);
			InvalidateRect(&rectInvalid);
			return;
		}
	}
	
	Invalidate();
	return;
}

void CScribblenewView::OnInitialUpdate() 
{
	
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
	Canvas.CreateRectRgn(0, 0,GetDocument()->GetDocSize().cx,GetDocument()->GetDocSize().cy);


	CScrollView::OnInitialUpdate();
}


//void CScribblenewView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
//}


//void CScribblenewView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO: Add your message handler code here and/or call default
//	CClientDC dc(this);
//	
//	
//	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
//}



//BOOL CScribblenewView::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	return TRUE;
//}


BOOL CScribblenewView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE; 

}


