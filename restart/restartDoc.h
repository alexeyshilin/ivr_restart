// restartDoc.h : interface of the CrestartDoc class
//


#pragma once

class CrestartDoc : public CDocument
{
protected: // create from serialization only
	CrestartDoc();
	DECLARE_DYNCREATE(CrestartDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CrestartDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


