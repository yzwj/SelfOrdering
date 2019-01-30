// yoda_self_orderingDoc.h : interface of the Cyoda_self_orderingDoc class
//


#pragma once


class Cyoda_self_orderingDoc : public CDocument
{
protected: // create from serialization only
	Cyoda_self_orderingDoc();
	DECLARE_DYNCREATE(Cyoda_self_orderingDoc)

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
	virtual ~Cyoda_self_orderingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


