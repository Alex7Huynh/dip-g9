
// DemoHaarWaveletDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DemoHaarWavelet.h"
#include "DemoHaarWaveletDlg.h"
#include "afxdialogex.h"
#include <afx.h>
#include "HaarWavelet.h"
#include "afxcmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDemoHaarWaveletDlg* sHaarWaveletDlg = NULL;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDemoHaarWaveletDlg dialog




CDemoHaarWaveletDlg::CDemoHaarWaveletDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemoHaarWaveletDlg::IDD, pParent)
{
	sHaarWaveletDlg = this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoHaarWaveletDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EB_IMAGE_PATH, m_imagePath);
	DDX_Control(pDX, IDC_EB_Iteration, m_iterations);
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
}

BEGIN_MESSAGE_MAP(CDemoHaarWaveletDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BN_FORWARD, &CDemoHaarWaveletDlg::OnBnClickedBnForward)
	ON_BN_CLICKED(IDC_BN_OPEN_FILE, &CDemoHaarWaveletDlg::OnBnClickedBnOpenFile)
	ON_BN_CLICKED(IDC_BN_DENOISING, &CDemoHaarWaveletDlg::OnBnClickedBnDenoising)
	ON_BN_CLICKED(IDC_BN_ABOUT, &CDemoHaarWaveletDlg::OnBnClickedBnAbout)
	ON_BN_CLICKED(IDC_BTNAUTO, &CDemoHaarWaveletDlg::OnBnClickedBtnauto)
END_MESSAGE_MAP()


// CDemoHaarWaveletDlg message handlers

BOOL CDemoHaarWaveletDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_iterations.SetWindowTextW(L"1");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemoHaarWaveletDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemoHaarWaveletDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoHaarWaveletDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDemoHaarWaveletDlg::OnBnClickedBnForward()
{
	CString filePath = L"";
	m_imagePath.GetWindowTextW(filePath);

	CString iteration = L"";
	m_iterations.GetWindowTextW(iteration);

	CHaarWavelet a;
	a.RunHaarTransform(filePath, _wtoi(iteration.GetString()));
}


void CDemoHaarWaveletDlg::OnBnClickedBnOpenFile()
{
	// Show Open File Dialog
	TCHAR szFilters[]= _T("All Files (*.*)|*.*||");
	// Create an Open dialog
	CFileDialog fileDlg (TRUE, _T("All"), _T("*.*"),
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		// Get path of the selected file.
		CString m_strPathname = fileDlg.GetPathName();
		m_imagePath.SetWindowText(m_strPathname);
	}
}


void CDemoHaarWaveletDlg::OnBnClickedBnDenoising()
{
	// TODO: Add your control notification handler code here
	CString filePath = L"";
	m_imagePath.GetWindowTextW(filePath);

	CHaarWavelet a;
	a.Denoise(filePath);
}


void CDemoHaarWaveletDlg::OnBnClickedBnAbout()
{
	wchar_t tmp[512] = L"Group 9 - Image Processing & Computer Vision\n\tUniversity of Science - 2014";
	MessageBox(tmp, L"About us", 0);
}


static CStringArray staticFilePaths;
static CString staticOutputFolder;

UINT ThreadAutoTransformMode(void* inData)
{
	CHaarWavelet haarWavelet;
	haarWavelet.RunHaarTransformAutoMode(staticFilePaths, staticOutputFolder);
	MessageBox(NULL, L"This progress is finished. Please check the result.csv file in output folder.", L"Information", MB_OK);
	sHaarWaveletDlg->m_progressBar.SetMarquee(FALSE, 1);
	sHaarWaveletDlg->m_progressBar.SetPos(0);

	// Enable dialog controls
	CWnd* Pfield = sHaarWaveletDlg->GetDlgItem( IDC_BN_FORWARD );
	Pfield->EnableWindow(TRUE); 
	Pfield = sHaarWaveletDlg->GetDlgItem( IDC_BN_OPEN_FILE );
	Pfield->EnableWindow(TRUE);
	Pfield = sHaarWaveletDlg->GetDlgItem( IDC_BN_DENOISING );
	Pfield->EnableWindow(TRUE); 
	Pfield = sHaarWaveletDlg->GetDlgItem( IDC_BTNAUTO );
	Pfield->EnableWindow(TRUE); 
	return 0;
}

void CDemoHaarWaveletDlg::OnBnClickedBtnauto()
{
	// TODO: Add your control notification handler code here
	// Show Open File Dialog
	TCHAR szFilters[]= _T("txt Files (*.txt)|*.txt||");
	// Create an Open dialog
	CFileDialog fileDlg (TRUE, _T("txt Files"), _T("*.txt"),
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		// Get path of the selected file.
		CString filePath = fileDlg.GetPathName();

		staticFilePaths.RemoveAll();

		// Parse input file
		TRY {
			CStdioFile file(filePath, CFile::modeRead); 

			CString strLine = L"";
			while (file.ReadString(strLine)) {
				staticFilePaths.Add(strLine);
			}

			file.Close();

			// Browser output folder
			CFolderPickerDialog folderPicker;
			if (folderPicker.DoModal() == IDOK) {
				MessageBox(L"Press OK to start.", L"Information", MB_OK);

				staticOutputFolder = folderPicker.GetFolderPath();

				m_progressBar.SetRange(0, 1);
				m_progressBar.SetForegroundWindow();
				m_progressBar.SetMarquee(TRUE, 1);

				// Disable dialog item
				CWnd* Pfield = GetDlgItem( IDC_BN_FORWARD );
				Pfield->EnableWindow(FALSE); 
				Pfield = GetDlgItem( IDC_BN_OPEN_FILE );
				Pfield->EnableWindow(FALSE);
				Pfield = GetDlgItem( IDC_BN_DENOISING );
				Pfield->EnableWindow(FALSE); 
				Pfield = GetDlgItem( IDC_BTNAUTO );
				Pfield->EnableWindow(FALSE); 

				AfxBeginThread(ThreadAutoTransformMode, NULL);
				
			}
		}
		CATCH_ALL (ex) {
			ex->ReportError();
		}
		END_CATCH_ALL
	}
}
