// CDotsDoc.cpp : implementation of the CCDotsDoc class
//

#include "stdafx.h"
#include "CDots.h"

#include "CDotsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCDotsDoc

IMPLEMENT_DYNCREATE(CCDotsDoc, CDocument)

BEGIN_MESSAGE_MAP(CCDotsDoc, CDocument)
	//{{AFX_MSG_MAP(CCDotsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDotsDoc construction/destruction

CCDotsDoc::CCDotsDoc()
{
	// TODO: add one-time construction code here

}

CCDotsDoc::~CCDotsDoc()
{
}

BOOL CCDotsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCDotsDoc serialization

void CCDotsDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CCDotsDoc diagnostics

#ifdef _DEBUG
void CCDotsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCDotsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCDotsDoc commands
