// AffineDlg.cpp : implementation file
//

#include "pch.h"
#include "ObrazyPr1.h"
#include "AffineDlg.h"
#include "afxdialogex.h"

#include <string>

// AffineDlg dialog

IMPLEMENT_DYNAMIC(AffineDlg, CDialogEx)

AffineDlg::AffineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AFFINE, pParent)
{
}

AffineDlg::~AffineDlg()
{
}

const std::array<int, 4> IDs = {
	IDC_TA, IDC_TB, IDC_TC, IDC_TD
};

void AffineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	auto i = 0;
	m_ValidData = false;
	for (auto & val : m_Transform) {
		CString num;
		DDX_Text(pDX, IDs[i++], num);
		if (num == L"") {
			val = 0;
			continue;
		}
		size_t idx = 0;
		val = std::stod((LPCTSTR)num, &idx);
		if (num[idx] != '\0') {
			return;
		}
	}

	m_ValidData = true;
}


BEGIN_MESSAGE_MAP(AffineDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &AffineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &AffineDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// AffineDlg message handlers


void AffineDlg::OnBnClickedOk()
{
	AffineDlg::UpdateData();
	if (m_Transform[0] * m_Transform[3] == m_Transform[1] * m_Transform[2]) {
		MessageBoxW(L"Matrix inverse does not exist.", L"Invalid data", MB_OK);
		return;
	}
	if (m_ValidData) {
		CDialogEx::OnOK();
	}
	else {
		MessageBoxW(L"Data inserted is invalid. Proper value type: double.", L"Invalid data", MB_OK);
	}
}


void AffineDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
