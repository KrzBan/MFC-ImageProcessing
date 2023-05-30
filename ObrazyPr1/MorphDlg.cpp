// MorphDlg.cpp : implementation file
//

#include "pch.h"
#include "ObrazyPr1.h"
#include "MorphDlg.h"
#include "afxdialogex.h"


// MorphDlg dialog

IMPLEMENT_DYNAMIC(MorphDlg, CDialog)

MorphDlg::MorphDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MORPH, pParent)
{

}

MorphDlg::~MorphDlg()
{
}

void MorphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MorphDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &MorphDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MorphDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &MorphDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// MorphDlg message handlers

void MorphDlg::OnBnClickedButton1()
{
	m_Choice = 0;
	CDialog::OnOK();
}


void MorphDlg::OnBnClickedButton2()
{
	m_Choice = 0;
	CDialog::OnOK();
}


void MorphDlg::OnBnClickedButton3()
{
	m_Choice = 0;
	CDialog::OnOK();
}
