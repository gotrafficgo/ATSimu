// trafficDoc.h : interface of the CTrafficDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAFFICDOC_H__E907100F_12D2_4E34_94D2_0EBCE8C4CE8E__INCLUDED_)
#define AFX_TRAFFICDOC_H__E907100F_12D2_4E34_94D2_0EBCE8C4CE8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CTrafficDoc : public CDocument
{
protected: // create from serialization only
	CTrafficDoc();
	DECLARE_DYNCREATE(CTrafficDoc)

// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrafficDoc)
	public:
		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrafficDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTrafficDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAFFICDOC_H__E907100F_12D2_4E34_94D2_0EBCE8C4CE8E__INCLUDED_)
