// EntropyDlg.cpp : implementation file
//

#include "pch.h"
#include "ObrazyPr1.h"
#include "EntropyDlg.h"
#include "afxdialogex.h"


// EntropyDlg dialog

IMPLEMENT_DYNAMIC(EntropyDlg, CDialogEx)

EntropyDlg::EntropyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENTROPY, pParent)
{

}

EntropyDlg::~EntropyDlg()
{
}

void EntropyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	CString num;
	DDX_Text(pDX, IDC_EDITRANGE, num);
	if (num == L"") m_Range = 4;
	else m_Range = _wtoi(num);
}


BEGIN_MESSAGE_MAP(EntropyDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &EntropyDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDITRANGE, &EntropyDlg::OnEnChangeEditrange)
	ON_BN_CLICKED(IDCANCEL, &EntropyDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// EntropyDlg message handlers


void EntropyDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void EntropyDlg::OnEnChangeEditrange()
{
	// TODO:  Add your control notification handler code here
}


void EntropyDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
