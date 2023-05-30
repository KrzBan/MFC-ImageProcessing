#pragma once


// MorphDlg dialog

class MorphDlg : public CDialog
{
	DECLARE_DYNAMIC(MorphDlg)

public:
	MorphDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MorphDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MORPH };
#endif
private:
	int m_Choice = 0;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	int GetChoice() {
		return m_Choice;
	}
};
