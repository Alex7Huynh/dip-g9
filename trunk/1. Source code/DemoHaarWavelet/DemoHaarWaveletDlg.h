
// DemoHaarWaveletDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDemoHaarWaveletDlg dialog
class CDemoHaarWaveletDlg : public CDialogEx
{
// Construction
public:
	CDemoHaarWaveletDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEMOHAARWAVELET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnForward();
	afx_msg void OnBnClickedBnOpenFile();
	CEdit m_imagePath;
	CEdit m_iterations;
	afx_msg void OnBnClickedBnDenoising();
	afx_msg void OnBnClickedBnAbout();
	afx_msg void OnBnClickedBtnauto();
	CProgressCtrl m_progressBar;
};
