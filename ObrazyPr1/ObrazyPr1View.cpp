
// ObrazyPr1View.cpp : implementation of the CObrazyPr1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ObrazyPr1.h"
#endif

#include "ObrazyPr1Doc.h"
#include "ObrazyPr1View.h"

#include "EntropyDlg.h"
#include "AffineDlg.h"
#include "MorphDlg.h"

#include "ImageProcessing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CObrazyPr1View

IMPLEMENT_DYNCREATE(CObrazyPr1View, CView)

BEGIN_MESSAGE_MAP(CObrazyPr1View, CView)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, &CObrazyPr1View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &CObrazyPr1View::OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CObrazyPr1View::OnUpdateFileSaveAs)
	ON_COMMAND(ID_EDIT_UNDO, &CObrazyPr1View::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CObrazyPr1View::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CObrazyPr1View::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CObrazyPr1View::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CObrazyPr1View::OnUpdateFileOpen)
	ON_COMMAND(ID_TOOLS_TOGRAYSCALE, &CObrazyPr1View::OnToolsTograyscale)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TOGRAYSCALE, &CObrazyPr1View::OnUpdateToolsTograyscale)
	ON_COMMAND(ID_TOOLS_AFFINE, &CObrazyPr1View::OnToolsAffine)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_AFFINE, &CObrazyPr1View::OnUpdateToolsAffine)
	ON_COMMAND(ID_TOOLS_ENTROPYFILTER, &CObrazyPr1View::OnToolsEntropyfilter)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ENTROPYFILTER, &CObrazyPr1View::OnUpdateToolsEntropyfilter)
	ON_COMMAND(ID_TOOLS_ERODE, &CObrazyPr1View::OnToolsErode)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ERODE, &CObrazyPr1View::OnUpdateToolsErode)
	ON_COMMAND(ID_TOOLS_DILATE, &CObrazyPr1View::OnToolsDilate)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_DILATE, &CObrazyPr1View::OnUpdateToolsDilate)
	ON_COMMAND(ID_TOOLS_MORPH, &CObrazyPr1View::OnToolsMorph)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MORPH, &CObrazyPr1View::OnUpdateToolsMorph)
	ON_COMMAND(ID_TOOLS_CLEARBORDER, &CObrazyPr1View::OnToolsClearborder)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CLEARBORDER, &CObrazyPr1View::OnUpdateToolsClearborder)
	ON_COMMAND(ID_TOOLS_ENTROPYFILTER2, &CObrazyPr1View::OnToolsEntropyfilter2)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ENTROPYFILTER2, &CObrazyPr1View::OnUpdateToolsEntropyfilter2)
END_MESSAGE_MAP()

// CObrazyPr1View construction/destruction

CObrazyPr1View::CObrazyPr1View() noexcept
{
	// TODO: add construction code here
	m_pClientRect = new CRect(0, 0, 0, 0);
}

CObrazyPr1View::~CObrazyPr1View()
{
	delete m_pClientRect;
}

BOOL CObrazyPr1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CObrazyPr1View drawing

void CObrazyPr1View::OnDraw(CDC* pDC)
{
	CObrazyPr1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	
	if (m_imageHistory.size()) {
		
		CImage& currentImage = m_imageHistory[m_currentImage];
		double imageRatio = currentImage.GetWidth() / static_cast<double>(currentImage.GetHeight());
		double clientRatio = m_pClientRect->Width() / static_cast<double>(m_pClientRect->Height());

		int xOffset = 0;
		int yOffset = 0;
		if (clientRatio > imageRatio) {
			int targetWidth = m_pClientRect->Height() * imageRatio;
			xOffset = (m_pClientRect->Width() - targetWidth) / 2;
		}
		else if (clientRatio < imageRatio) {
			int targetHeight = m_pClientRect->Width() / imageRatio;
			yOffset = (m_pClientRect->Height() - targetHeight) / 2;
		}

		if(m_pClientRect->Width() - 2 * xOffset > 0 || m_pClientRect->Height() - 2 * yOffset > 0)
			currentImage.Draw(*pDC, xOffset, yOffset, m_pClientRect->Width() - 2 * xOffset, m_pClientRect->Height() - 2 * yOffset);
	}
	
}

void CObrazyPr1View::InsertImage(CImageMov&& image) {
	++m_currentImage;
	if (m_currentImage < m_imageHistory.size())
		m_imageHistory.erase(m_imageHistory.begin() + m_currentImage, m_imageHistory.end());

	m_imageHistory.push_back(std::move(image));

	Invalidate();
}

// CObrazyPr1View diagnostics

#ifdef _DEBUG
void CObrazyPr1View::AssertValid() const
{
	CView::AssertValid();
}

void CObrazyPr1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CObrazyPr1Doc* CObrazyPr1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CObrazyPr1Doc)));
	return (CObrazyPr1Doc*)m_pDocument;
}
#endif //_DEBUG




// CObrazyPr1View message handlers


void CObrazyPr1View::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo){
	GetClientRect(m_pClientRect);

	CView::OnPrepareDC(pDC, pInfo);
}


void CObrazyPr1View::OnSize(UINT nType, int cx, int cy){
	CView::OnSize(nType, cx, cy);

	GetClientRect(m_pClientRect);
}


void CObrazyPr1View::OnFileOpen(){
	const TCHAR szFilter[] = _T("Image Files (*.bmp; *.jpg; *.jpeg; *.png)|*.bmp; *.jpg; *.jpeg; *.png| All Files (*.*)|*.*||");
	CFileDialog dlg(true, nullptr, nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();

		CImageMov local;
		local.Load(sFilePath);

		InsertImage(std::move(local));
	}
}

void CObrazyPr1View::OnUpdateFileOpen(CCmdUI* pCmdUI){
	pCmdUI->Enable(true);
}


void CObrazyPr1View::OnFileSaveAs(){
	CFileDialog dlg(false, L"png", L"result");
	if (dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();

		m_imageHistory[m_currentImage].Save(sFilePath);
	}
}


void CObrazyPr1View::OnUpdateFileSaveAs(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}


void CObrazyPr1View::OnEditUndo(){
	--m_currentImage;

	Invalidate();
}


void CObrazyPr1View::OnUpdateEditUndo(CCmdUI* pCmdUI){
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_currentImage > 0);
}


void CObrazyPr1View::OnEditRedo(){
	++m_currentImage;

	Invalidate();
}


void CObrazyPr1View::OnUpdateEditRedo(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_currentImage < (m_imageHistory.size()-1 ));
}




void CObrazyPr1View::OnToolsTograyscale(){
	CImageMov grayscale = IP::Process<IP::ToGrayscale>(m_imageHistory.at(m_currentImage));

	InsertImage(std::move(grayscale));
}


void CObrazyPr1View::OnUpdateToolsTograyscale(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}


void CObrazyPr1View::OnToolsAffine(){
	AffineDlg affineDlg;

	if (affineDlg.DoModal() == IDOK) {
		
		auto transform = affineDlg.GetTransform();

		CImageMov affine = IP::Process<IP::Affine>(m_imageHistory.at(m_currentImage), transform);

		InsertImage(std::move(affine));
	}
}


void CObrazyPr1View::OnUpdateToolsAffine(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}


void CObrazyPr1View::OnToolsEntropyfilter(){
	EntropyDlg entropyDlg;

	if (entropyDlg.DoModal() == IDOK) {
		int range = entropyDlg.GetRange();

		CImageMov entropy = IP::Process<IP::EntropyFilt>(m_imageHistory.at(m_currentImage), range);

		InsertImage(std::move(entropy));
	}
}


void CObrazyPr1View::OnUpdateToolsEntropyfilter(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}

void CObrazyPr1View::OnToolsEntropyfilter2(){
	EntropyDlg entropyDlg;

	if (entropyDlg.DoModal() == IDOK) {
		int range = entropyDlg.GetRange();

		CImageMov entropy = IP::Process<IP::EntropyFilt2>(m_imageHistory.at(m_currentImage), range);

		InsertImage(std::move(entropy));
	}
}


void CObrazyPr1View::OnUpdateToolsEntropyfilter2(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}


void CObrazyPr1View::OnToolsErode(){
	CImageMov eroded = IP::Process<IP::Erode>(m_imageHistory.at(m_currentImage));

	InsertImage(std::move(eroded));
}


void CObrazyPr1View::OnUpdateToolsErode(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}


void CObrazyPr1View::OnToolsDilate(){
	CImageMov dilated = IP::Process<IP::Dilate>(m_imageHistory.at(m_currentImage));

	InsertImage(std::move(dilated));
}


void CObrazyPr1View::OnUpdateToolsDilate(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}


void CObrazyPr1View::OnToolsMorph(){
	MorphDlg morphDlg;

	if (morphDlg.DoModal() == IDOK) {
		int choice = morphDlg.GetChoice();

		assert(choice >= 0 && choice <= 3);
		CImageMov morphed = IP::Process<IP::MorphGrad>(m_imageHistory.at(m_currentImage), choice);

		InsertImage(std::move(morphed));
	}
}


void CObrazyPr1View::OnUpdateToolsMorph(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}


void CObrazyPr1View::OnToolsClearborder(){
	CImageMov clear = IP::Process<IP::ClearBorder>(m_imageHistory.at(m_currentImage));

	InsertImage(std::move(clear));
}


void CObrazyPr1View::OnUpdateToolsClearborder(CCmdUI* pCmdUI){
	pCmdUI->Enable(m_imageHistory.size());
}


