// CDotsView.cpp : implementation of the CCDotsView class
//

#include "stdafx.h"
#include "CDots.h"

#include "CDotsDoc.h"
#include "CDotsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



typedef struct Dot {
  short row;  /* row number of dot, 0..boardSize-1 */
  short col;  /* column number of dot, 0..boardSize-1 */
} Dot;
typedef struct DotLine {
  Dot dot1;  /* first dot of a line */
  Dot dot2;  /* second dot of a line */
    /* legal lines are formed by dots in the same row, in adjacent columns,
       or in the same column in adjacent rows */
} DotLine; 
void DrawDots();
void OpponentMove(const DotLine opponentLine);
void InitDots(short boardSize, BOOL playFirst, CWnd* dotWindow);
short PlayDots(DotLine yourLines[]);
void GetClick(int h, int v);
	extern CDC *gpDC; //#######


/////////////////////////////////////////////////////////////////////////////
// CCDotsView

IMPLEMENT_DYNCREATE(CCDotsView, CView)

BEGIN_MESSAGE_MAP(CCDotsView, CView)
	//{{AFX_MSG_MAP(CCDotsView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDotsView construction/destruction

CCDotsView::CCDotsView()
{
	// TODO: add construction code here

}

CCDotsView::~CCDotsView()
{
}

BOOL CCDotsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCDotsView drawing

void CCDotsView::OnDraw(CDC* pDC)
{
	CCDotsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	
	gpDC = pDC;
	static BOOL first = true;
	if (first) {
		first = false;
		InitDots(10, true, NULL);

		DrawDots();
	/*
		DotLine dotline;
		dotline.dot1.col = 3;
		dotline.dot1.row = 1;
		dotline.dot2.col = 3;
		dotline.dot2.row = 2;
		OpponentMove(dotline);

		DotLine dots[100];
		short x = PlayDots(dots);
		x = PlayDots(dots);
		x = PlayDots(dots);
		x = PlayDots(dots);
		x = PlayDots(dots);
		x = PlayDots(dots);
		x = PlayDots(dots);*/
	} else
		DrawDots();

}

/////////////////////////////////////////////////////////////////////////////
// CCDotsView printing

BOOL CCDotsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCDotsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCDotsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCDotsView diagnostics

#ifdef _DEBUG
void CCDotsView::AssertValid() const
{
	CView::AssertValid();
}

void CCDotsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCDotsDoc* CCDotsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCDotsDoc)));
	return (CCDotsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCDotsView message handlers

void CCDotsView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC p(this);
	gpDC = &p;
	GetClick(point.x, point.y);

	CView::OnLButtonDown(nFlags, point);
}

