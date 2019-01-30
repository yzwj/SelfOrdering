// yoda_self_orderingDoc.cpp : implementation of the Cyoda_self_orderingDoc class
//

#include "stdafx.h"
#include "yoda_self_ordering.h"

#include "yoda_self_orderingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cyoda_self_orderingDoc

IMPLEMENT_DYNCREATE(Cyoda_self_orderingDoc, CDocument)

BEGIN_MESSAGE_MAP(Cyoda_self_orderingDoc, CDocument)
END_MESSAGE_MAP()


// Cyoda_self_orderingDoc construction/destruction

Cyoda_self_orderingDoc::Cyoda_self_orderingDoc()
{
	// TODO: add one-time construction code here

}

Cyoda_self_orderingDoc::~Cyoda_self_orderingDoc()
{
}

BOOL Cyoda_self_orderingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// Cyoda_self_orderingDoc serialization

void Cyoda_self_orderingDoc::Serialize(CArchive& ar)
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


// Cyoda_self_orderingDoc diagnostics

#ifdef _DEBUG
void Cyoda_self_orderingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cyoda_self_orderingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cyoda_self_orderingDoc commands
