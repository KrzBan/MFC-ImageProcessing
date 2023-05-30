#pragma once

#include <array>
// AffineDlg dialog

class AffineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AffineDlg)

	std::array<double, 4> m_Transform{};
	bool m_ValidData;

public:
	AffineDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AffineDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AFFINE };
#endif

	std::array<double, 4> GetTransform() { return m_Transform; }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
