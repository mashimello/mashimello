// ImpressDoc.cpp : implementation of the CImpressDoc class
//


#include "stdafx.h"
#include "Impress.h"
#include "ImpressDoc.h"

#include "FourierTransformDialog.h"
#include "PropDialog.h"
#include "RotateAngleDialog.h"

#include "MainFrm.h"
#include "filter.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CImpressDoc

IMPLEMENT_DYNCREATE(CImpressDoc, CDocument)

BEGIN_MESSAGE_MAP(CImpressDoc, CDocument)
	//{{AFX_MSG_MAP(CImpressDoc)
	ON_COMMAND(ID_IMAGE_IMAGESIZE, OnImageImagesize)
	ON_COMMAND(ID_IMAGE_TRANSFORM_FLIPVERTICAL, OnImageTransformFlipvertical)
	ON_COMMAND(ID_IMAGE_TRANSFORM_FLIPHORIZONTAL, OnImageTransformFliphorizontal)
	ON_COMMAND(ID_IMAGE_TRANSFORM_ROTATE_ROTATE90CCW, OnImageTransformRotateRotate90ccw)
	ON_COMMAND(ID_IMAGE_TRANSFORM_ROTATE_ROTATE90CW, OnImageTransformRotateRotate90cw)
	ON_COMMAND(ID_IMAGE_TRANSFORM_ROTATE_ROTATENUMERIC, OnImageTransformRotateRotatenumeric)
	ON_COMMAND(ID_IMAGE__GRAYSCALE, OnImageGrayscale)
	ON_COMMAND(ID_IMAGE__MONOCHROME, OnImageMonochrome)
	ON_COMMAND(ID_IMAGE_TRANSFORM_OFFSET, OnImageTransformOffset)
	ON_COMMAND(ID_IMAGE_MODE_INVERT, OnImageModeInvert)
	ON_COMMAND(ID_IMAGE_CHANGEIMAGESIZE_CHANGEUSINGFOURIER, OnImageChangeimagesizeChangeusingfourier)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_IMAGE_QUANTIZE, OnImageQuantize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImpressDoc construction/destruction

CImpressDoc::CImpressDoc(UINT X,UINT Y):pic(X,Y,24)
{

}
CImpressDoc::CImpressDoc()
{	

}


CImpressDoc::~CImpressDoc()
{
}


BOOL CImpressDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;


	CPropDialog dlg(NULL,"Set Image Width to","Set Image Height to",256,256);
	
	if(dlg.DoModal()==IDOK)
	{
	
		pic=CImage(dlg.m_a,dlg.m_b,24);		
		pic.Fill(rgb((unsigned char)0,(unsigned char)0,(unsigned char)0));
		POSITION pos = GetFirstViewPosition();
		CView* pView = GetNextView(pos);		
		pView->UpdateWindow();		
		return TRUE;
		
	}
	else
	{
		return FALSE;		
	}		
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImpressDoc serialization

void CImpressDoc::Serialize(CArchive& ar)
{
	/*
	CString FileName=ar.GetFile()->GetFileName();

	
	if (ar.IsStoring())
	{
		pic.SaveBMPArchive(ar);	
		SetModifiedFlag(FALSE);
	}
	else
	{	
		pic.ReadBMPArchive(ar);
		SetModifiedFlag(FALSE);
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////
// CImpressDoc diagnostics

#ifdef _DEBUG
void CImpressDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImpressDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImpressDoc commands






void CImpressDoc::OnImageImagesize() 
{

	CPropDialog dlg(NULL,"Change Width to","Change Height to",pic.Width(),pic.Height());
	if(dlg.DoModal()==IDOK)
	{
		pic.ChangeSize(dlg.m_a,dlg.m_b);
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);		
	}
	
	
}

CImpressDoc& CImpressDoc::operator =(CImpressDoc &CD)
{
	if(this==&CD) return *this;
	pic=CD.pic;
	return *this;
}

void CImpressDoc::OnImageTransformFlipvertical() 
{
	Hold();
	pic.MirrorX();	
	SetModifiedFlag();
	UpdateAllViews(NULL);
	// TODO: Add your command handler code here
	
}

void CImpressDoc::OnImageTransformFliphorizontal() 
{
	Hold();
	pic.MirrorY();	
	SetModifiedFlag();
	UpdateAllViews(NULL);
	// TODO: Add your command handler code here
	
}

void CImpressDoc::OnImageTransformRotateRotate90ccw() 
{
	Hold();
	pic.Rotate90CW();	
	SetModifiedFlag();
	UpdateAllViews(NULL);
	// TODO: Add your command handler code here
	
}

void CImpressDoc::OnImageTransformRotateRotate90cw() 
{
	Hold();
	pic.Rotate90CCW();	
	SetModifiedFlag();
	UpdateAllViews(NULL);
	// TODO: Add your command handler code here
	
}

void CImpressDoc::OnImageTransformRotateRotatenumeric() 
{
	CRotateAngleDialog dlg;
	if (dlg.DoModal()==IDOK)
	{
		Hold();
		pic.Rotate(dlg.m_angle);
		SetModifiedFlag();	
		UpdateAllViews(NULL);
	}
	
}



void CImpressDoc::OnImageGrayscale() 
{
	Hold();
	pic.ConvertToBW();
	SetModifiedFlag();
	UpdateAllViews(NULL);
	
}

void CImpressDoc::OnImageMonochrome() 
{
	Hold();
	pic.ConvertToMonochrome();
	SetModifiedFlag();
	UpdateAllViews(NULL);
	
}

void CImpressDoc::OnImageTransformOffset() 
{

	CPropDialog dlg(NULL,"Offset X by","Offset Y by",0,0);
	if(dlg.DoModal()==IDOK)
	{
		Hold();
		pic.Offset(dlg.m_a,dlg.m_b);		
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);		
	}
	

	
}





void CImpressDoc::OnImageModeInvert() 
{
	Hold();
	pic.Invert();
	SetModifiedFlag();
	UpdateAllViews(NULL);
	TRACE("Inverted");

	
}

void CImpressDoc::OnImageChangeimagesizeChangeusingfourier() 
{
	CFourierTransformDialog dlg;

	dlg.m_Width=pic.Width();
	dlg.m_Height=pic.Height();
	dlg.m_KernelType=1;
	dlg.m_Smooth=0.95;
	dlg.m_StepOvershoot=12;
	dlg.m_Preset=0;

	if(dlg.DoModal()==IDOK)
	{
		Hold();
		SetModifiedFlag(TRUE);
		pic.ChangeSizeUsingFourier(dlg.m_Width,dlg.m_Height,dlg.m_KernelType,dlg.m_Smooth,dlg.m_StepOvershoot);				
		UpdateAllViews(NULL);		
	}
	

	
}





void CImpressDoc::Hold()
{
	Holded=*(pic.Image);
}

void CImpressDoc::Fetch()
{
	*(pic.Image)=Holded;
	SetModifiedFlag();
	UpdateAllViews(NULL);

}


void CImpressDoc::OnEditUndo() 
{	
	Fetch();

}

BOOL CImpressDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if(!pic.ReadBMPFile(lpszPathName))
		return FALSE;	

	return TRUE;
}



BOOL CImpressDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{	
	BOOL result=CDocument::OnSaveDocument(lpszPathName);

	if(!pic.SaveBMPFile(lpszPathName))
		return FALSE;

	return result;
}

CRect CImpressDoc::GetImageRect()
{
	return CRect(0,0,pic.Width(),pic.Height());
}

void CImpressDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsModified());
}












void CImpressDoc::OnImageQuantize() 
{
	unsigned char *QData = new unsigned char[pic.Width()*pic.Height()];
	unsigned char *PData = new unsigned char[256*3];
	pic.Image->Quantize(16, QData, PData);
	delete QData;
	delete PData;
}
