
// ScribblenewDoc.cpp : CScribblenewDoc 類別的實作
//

#include "stdafx.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Scribblenew.h"
#endif

#include "ScribblenewDoc.h"
#include "PenWidthsDlg.h"
#include <propkey.h>
#include "ScribblenewView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CScribblenewDoc

IMPLEMENT_DYNCREATE(CScribblenewDoc, CDocument)

BEGIN_MESSAGE_MAP(CScribblenewDoc, CDocument)
	ON_COMMAND(ID_PEN_CLEARALL, &CScribblenewDoc::OnPenClearall)
	ON_COMMAND(ID_PEN_THICKLINE, &CScribblenewDoc::OnPenThickline)
	ON_UPDATE_COMMAND_UI(ID_PEN_THICKLINE, &CScribblenewDoc::OnUpdatePenThickline)
	ON_UPDATE_COMMAND_UI(ID_PEN_CLEARALL, &CScribblenewDoc::OnUpdatePenClearall)
	ON_COMMAND(ID_PEN_WIDTHS, &CScribblenewDoc::OnPenWidths)
	ON_COMMAND(ID_PEN_Color, &CScribblenewDoc::OnPenColor)
END_MESSAGE_MAP()


// CScribblenewDoc 建構/解構

CScribblenewDoc::CScribblenewDoc()
{
	// TODO: 在此加入一次建構程式碼

}

CScribblenewDoc::~CScribblenewDoc()
{
}

BOOL CScribblenewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此加入重新初始化程式碼
	// (SDI 文件會重用此文件)
	InitDocument();//
	return TRUE;
}




// CScribblenewDoc 序列化

void CScribblenewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此加入儲存程式碼
		ar << m_sizeDoc;
	}
	else
	{
		// TODO: 在此加入載入程式碼
		ar >> m_sizeDoc;
	}

	m_strokeList.Serialize(ar); //
}

#ifdef SHARED_HANDLERS

// 縮圖的支援
void CScribblenewDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改這段程式碼以繪製文件的資料
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜尋處理常式的支援
void CScribblenewDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 設定來自文件資料的搜尋內容。
	// 內容部分應該以 ";" 隔開

	// 範例:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CScribblenewDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CScribblenewDoc 診斷

#ifdef _DEBUG
void CScribblenewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScribblenewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CScribblenewDoc 命令
BOOL CScribblenewDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	InitDocument(); 
	return TRUE;
}

void CScribblenewDoc::DeleteContents() 
{
	while (!m_strokeList.IsEmpty())
	{
		delete m_strokeList.RemoveHead(); //remove head stroke
	}
	CDocument::DeleteContents();
}

void CScribblenewDoc::InitDocument()  //pen data setting
{   
	m_bThickPen = FALSE;
	m_nPenWidth = 2; //  2 pixel pen width
	m_nThinWidth = 2;   //  thin pen is 2 pixels wide
	m_nThickWidth = 4;  //  thick pen is 4 pixels wide
	m_PenColor = RGB(0, 0, 0);
	ReplacePen();	

	m_sizeDoc = CSize(900,1500);

	//recall m_penCur will cause crash
  //  m_penCur.CreatePen(PS_SOLID, m_nPenWidth, RGB(0,0,0));
}

CStroke* CScribblenewDoc::NewStroke()
{
	//creat new CStroke and add into list tail
	CStroke* pStrokeItem = new CStroke(m_nPenWidth,m_PenColor); 
	
	m_strokeList.AddTail(pStrokeItem);
	SetModifiedFlag();

	return pStrokeItem;
}


///////////////////////////////////////Cstroke

IMPLEMENT_SERIAL(CStroke, CObject, 2)
	CStroke::CStroke()
{
	// This empty constructor should be used by serialization only
}

CStroke::CStroke(UINT nPenWidth, COLORREF PenColor)
{
	m_nPenWidth = nPenWidth;
	m_PenColor =  PenColor; 

	m_rectBounding.SetRectEmpty();
}

void CStroke::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_rectBounding;
		ar << (WORD)m_nPenWidth;  //save
		ar << (COLORREF)m_PenColor;

		m_pointArray.Serialize(ar);
	}
	else
	{    //load
		ar >> m_rectBounding;
		WORD w;
		ar >> w;
		m_nPenWidth = w;

		COLORREF c;
		ar >> c;
		m_PenColor = c;

		m_pointArray.Serialize(ar);
	}
}

BOOL CStroke::DrawStroke(CDC* pDC)
{
	CPen penStroke;
	if (!penStroke.CreatePen(PS_SOLID, m_nPenWidth, m_PenColor))
		return FALSE;
	CPen* pOldPen = pDC->SelectObject(&penStroke);

	//repaint from stroke
	pDC->MoveTo(m_pointArray[0]);
	for (int i=1; i < m_pointArray.GetSize(); i++)
	{
		pDC->LineTo(m_pointArray[i]);
	}

	pDC->SelectObject(pOldPen);
	return TRUE;
}

void CScribblenewDoc::OnPenClearall()
{
	// TODO: Add your command handler code here
	DeleteContents();
	SetModifiedFlag();

	UpdateAllViews(NULL); 
}


void CScribblenewDoc::OnPenThickline()
{
	// TODO: Add your command handler code here
	m_bThickPen = !m_bThickPen;

	//change pen between thick and thin
	ReplacePen();
}

// replace current pen width
void CScribblenewDoc::ReplacePen(){
	m_nPenWidth = m_bThickPen? m_nThickWidth : m_nThinWidth;
	//change current pen to new width pen
	m_penCur.DeleteObject(); 
	m_penCur.CreatePen(PS_SOLID, m_nPenWidth, m_PenColor);
}

void CScribblenewDoc::OnUpdatePenThickline(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bThickPen);
}


void CScribblenewDoc::OnUpdatePenClearall(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_strokeList.IsEmpty());

}


void CScribblenewDoc::OnPenWidths()
{
	// TODO: Add your command handler code here
	CPenWidthsDlg dlg;
	// Initialize dialog data
	dlg.m_nThinWidth = m_nThinWidth;
	dlg.m_nThickWidth = m_nThickWidth;

	//if press IDOK
	if (dlg.DoModal() == IDOK)
	{
		// get dlg setting value
		m_nThinWidth = dlg.m_nThinWidth;
		m_nThickWidth = dlg.m_nThickWidth;

		//Update pen
		ReplacePen();
	}

}

void CStroke::FinishStroke()
{
	
	//when after painting(LButtonUp) calculate the repainting rect.
	if (m_pointArray.GetSize()==0)//no  painting
	{
		m_rectBounding.SetRectEmpty();
		return;
	}
	CPoint pt = m_pointArray[0];
	m_rectBounding = CRect(pt.x, pt.y, pt.x, pt.y);

	for (int i=1; i < m_pointArray.GetSize(); i++)
	{
		
		pt = m_pointArray[i];
		m_rectBounding.left   = min(m_rectBounding.left, pt.x);
		m_rectBounding.right  = max(m_rectBounding.right, pt.x);
		m_rectBounding.top    = max(m_rectBounding.top, pt.y);
		m_rectBounding.bottom = min(m_rectBounding.bottom, pt.y);
	}

	//add pen width
	m_rectBounding.InflateRect(CSize(m_nPenWidth, m_nPenWidth));
	return;
}



void CScribblenewDoc::OnPenColor() //pick color
{
	// TODO: Add your command handler code here
	CColorDialog dlg(m_PenColor);
	if(dlg.DoModal() == IDOK) //allowed user pick on color
	{
		m_PenColor = dlg.GetColor();
		ReplacePen(); 
	}
}
