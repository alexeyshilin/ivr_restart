// restartView.h : interface of the CrestartView class
//


#pragma once


class CrestartView : public CView
{
protected: // create from serialization only
	CrestartView();
	DECLARE_DYNCREATE(CrestartView)

// Attributes
public:
	CrestartDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CrestartView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in restartView.cpp
inline CrestartDoc* CrestartView::GetDocument() const
   { return reinterpret_cast<CrestartDoc*>(m_pDocument); }
#endif

