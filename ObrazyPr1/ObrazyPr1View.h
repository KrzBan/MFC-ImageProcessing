
// ObrazyPr1View.h : interface of the CObrazyPr1View class
//

#pragma once

#include <vector>
#include "ImageMov.h"

class CObrazyPr1View : public CView
{
protected: // create from serialization only
	CObrazyPr1View() noexcept;
	DECLARE_DYNCREATE(CObrazyPr1View)

// Attributes
public:
	CObrazyPr1Doc* GetDocument() const;
private:
	CRect* m_pClientRect;

	std::vector<CImageMov> m_imageHistory;
	int m_currentImage = -1;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CObrazyPr1View();

	void InsertImage(CImageMov&& image);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnToolsTograyscale();
	afx_msg void OnUpdateToolsTograyscale(CCmdUI* pCmdUI);
	afx_msg void OnToolsAffine();
	afx_msg void OnUpdateToolsAffine(CCmdUI* pCmdUI);
	afx_msg void OnToolsEntropyfilter();
	afx_msg void OnUpdateToolsEntropyfilter(CCmdUI* pCmdUI);
	afx_msg void OnToolsErode();
	afx_msg void OnUpdateToolsErode(CCmdUI* pCmdUI);
	afx_msg void OnToolsDilate();
	afx_msg void OnUpdateToolsDilate(CCmdUI* pCmdUI);
	afx_msg void OnToolsMorph();
	afx_msg void OnUpdateToolsMorph(CCmdUI* pCmdUI);
	afx_msg void OnToolsClearborder();
	afx_msg void OnUpdateToolsClearborder(CCmdUI* pCmdUI);
	afx_msg void OnToolsEntropyfilter2();
	afx_msg void OnUpdateToolsEntropyfilter2(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debug version in ObrazyPr1View.cpp
inline CObrazyPr1Doc* CObrazyPr1View::GetDocument() const
   { return reinterpret_cast<CObrazyPr1Doc*>(m_pDocument); }
#endif

