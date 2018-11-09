
// ScribblenewView.cpp : CScribblenewView ���O����@
//

#include "stdafx.h"
// SHARED_HANDLERS �i�H�w�q�b��@�w���B�Y�ϩM�j�M�z�����B�z�`����
// ATL �M�פ��A�ä��\�P�ӱM�צ@�Τ��{���X�C
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
	// �зǦC�L�R�O
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CScribblenewView �غc/�Ѻc

CScribblenewView::CScribblenewView()
{
	// TODO: �b���[�J�غc�{���X

}

CScribblenewView::~CScribblenewView()
{
}

BOOL CScribblenewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �b���g�ѭק� CREATESTRUCT cs 
	// �F��ק�������O�μ˦����ت�

	return CView::PreCreateWindow(cs);
}

// CScribblenewView �yø

void CScribblenewView::OnDraw(CDC* pDC)
{
	CScribblenewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: �b���[�J��͸�ƪ��yø�{���X
	//

/*	CTypedPtrList<CObList,CStroke*>& strokeList = pDoc->m_strokeList;
	POSITION pos = strokeList.GetHeadPosition();
	while (pos != NULL)
	{
		CStroke* pStroke = strokeList.GetNext(pos);
		
		pStroke->DrawStroke(pDC);
	}*/
	CRect rectClip;
	rectClip.NormalizeRect();
	//get seeing area
	pDC->GetClipBox(&rectClip);
	pDC->LPtoDP(&rectClip);
	rectClip.InflateRect(1, 1); // avoid rounding to nothing
	 
	CTypedPtrList<CObList,CStroke*>& strokeList = pDoc->m_strokeList;
	POSITION pos = strokeList.GetHeadPosition();
	while (pos != NULL)
	{
		BOOL ll =FALSE;
		CStroke* pStroke = strokeList.GetNext(pos);
		CRect rectStroke,RrectStroke;
		rectStroke.NormalizeRect();
		rectStroke = pStroke->GetBoundingRect();
		pDC->LPtoDP(&rectStroke);
		rectStroke.InflateRect(1, 1); 

/*			if (!RrectStroke.IntersectRect(&rectStroke, &rectClip))
			continue;*/
	

		pStroke->DrawStroke(pDC);
		
	}
	
}


// CScribblenewView �C�L

BOOL CScribblenewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �w�]���ǳƦC�L�{���X
	return DoPreparePrinting(pInfo);
}

void CScribblenewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �[�J�C�L�e�B�~����l�]�w
}

void CScribblenewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �[�J�C�L�᪺�M���{���X
}


// CScribblenewView �E�_

#ifdef _DEBUG
void CScribblenewView::AssertValid() const
{
	CView::AssertValid();
}

void CScribblenewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScribblenewDoc* CScribblenewView::GetDocument() const // ���O�D��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScribblenewDoc)));
	return (CScribblenewDoc*)m_pDocument;
}
#endif //_DEBUG


// CScribblenewView �T���B�z�`��


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

	m_pStrokeCur->m_pointArray.Add(point);

	// Draw a line from the previous detected point in the mouse
	// drag to the current point.
	CPen* pOldPen = dc.SelectObject(GetDocument()->GetCurrentPen());

	dc.MoveTo(m_ptPrev);
	dc.LineTo(point);

	dc.SelectObject(pOldPen);
	m_ptPrev = point;
	
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
	
	CScrollView::OnInitialUpdate();
}


void CScribblenewView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CScribblenewView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	
	
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

