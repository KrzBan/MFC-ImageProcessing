#pragma once


// EntropyDlg dialog

class EntropyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EntropyDlg)

public:
	EntropyDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EntropyDlg();

	int GetRange() { return m_Range; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENTROPY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditrange();
	afx_msg void OnBnClickedCancel();

	int m_Range = 9;
};
