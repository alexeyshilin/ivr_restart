// restartDoc.cpp : implementation of the CrestartDoc class
//

#include "stdafx.h"
#include "restart.h"

#include "restartDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CrestartDoc

IMPLEMENT_DYNCREATE(CrestartDoc, CDocument)

BEGIN_MESSAGE_MAP(CrestartDoc, CDocument)
END_MESSAGE_MAP()


// CrestartDoc construction/destruction

CrestartDoc::CrestartDoc()
{
	// TODO: add one-time construction code here

}

CrestartDoc::~CrestartDoc()
{
}

BOOL CrestartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CrestartDoc serialization

void CrestartDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CrestartDoc diagnostics

#ifdef _DEBUG
void CrestartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CrestartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CrestartDoc commands
