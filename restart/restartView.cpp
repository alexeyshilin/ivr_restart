// restartView.cpp : implementation of the CrestartView class
//

#include "stdafx.h"
#include "restart.h"

#include "restartDoc.h"
#include "restartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CrestartView

IMPLEMENT_DYNCREATE(CrestartView, CView)

BEGIN_MESSAGE_MAP(CrestartView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CrestartView construction/destruction

CrestartView::CrestartView()
{
	// TODO: add construction code here

}

CrestartView::~CrestartView()
{
}

BOOL CrestartView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CrestartView drawing

void CrestartView::OnDraw(CDC* /*pDC*/)
{
	CrestartDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CrestartView printing

BOOL CrestartView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CrestartView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CrestartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CrestartView diagnostics

#ifdef _DEBUG
void CrestartView::AssertValid() const
{
	CView::AssertValid();
}

void CrestartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CrestartDoc* CrestartView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrestartDoc)));
	return (CrestartDoc*)m_pDocument;
}
#endif //_DEBUG


// CrestartView message handlers
