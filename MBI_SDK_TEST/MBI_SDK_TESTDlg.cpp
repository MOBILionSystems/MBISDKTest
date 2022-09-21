
// MBI_SDK_TESTDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MBI_SDK_TEST.h"
#include "MBI_SDK_TESTDlg.h"
#include "afxdialogex.h"
#include <winuser.h>
#include <format>
#include <string>
#include <wchar.h>
#include "MBIConstants.h"
#include <gdiplus.h>
#include "json/json.h"
#include <iostream>
#include <fstream>
#include "MBICalibration.h"
#include <sstream>
#include <iomanip>

using namespace Gdiplus;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMBISDKTESTDlg dialog

CMBISDKTESTDlg::CMBISDKTESTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MBISDK_TEST_DIALOG, pParent)
	, m_nDisplayFrameIndex(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMBISDKTESTDlg::~CMBISDKTESTDlg()
{
	if (pMyFile != NULL)
	{
		//delete pMyFile;
	}
	
	if (pMyCalib != NULL)
	{
		delete pMyCalib;
	}
}

void CMBISDKTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMBISDKTESTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD_FILE, &CMBISDKTESTDlg::OnBnClickedBtnLoadFile)
	ON_BN_CLICKED(IDC_BTN_NUM_FRAMES, &CMBISDKTESTDlg::OnBnClickedBtnNumFrames)
	ON_BN_CLICKED(IDC_BTN_MAX_SCANS, &CMBISDKTESTDlg::OnBnClickedBtnMaxScans)
	ON_BN_CLICKED(IDC_BTN_CALIBRATION, &CMBISDKTESTDlg::OnBnClickedBtnCalibration)
	ON_BN_CLICKED(IDC_BTN_TOF_ERROR, &CMBISDKTESTDlg::OnBnClickedBtnTofError)
	ON_BN_CLICKED(IDC_BTN_INDEX_MICRO, &CMBISDKTESTDlg::OnBnClickedBtnIndexMicro)
	ON_BN_CLICKED(IDC_BTN_INDEX_MZ, &CMBISDKTESTDlg::OnBnClickedBtnIndexMz)
	ON_BN_CLICKED(IDC_BTN_MICRO_MZ, &CMBISDKTESTDlg::OnBnClickedBtnMicroMz)
	ON_BN_CLICKED(IDC_BTN_MZ_MICRO, &CMBISDKTESTDlg::OnBnClickedBtnMzMicro)
	ON_BN_CLICKED(IDC_BTN_MZ_INDEX, &CMBISDKTESTDlg::OnBnClickedBtnMzIndex)
	ON_BN_CLICKED(IDC_BTN_SLOPE, &CMBISDKTESTDlg::OnBnClickedBtnSlope)
	ON_BN_CLICKED(IDC_BTN_INTERCEPT, &CMBISDKTESTDlg::OnBnClickedBtnIntercept)
	ON_BN_CLICKED(IDC_BTN_MAX_POINTS, &CMBISDKTESTDlg::OnBnClickedBtnMaxPoints)
	ON_BN_CLICKED(IDC_BTN_TIME_1_5, &CMBISDKTESTDlg::OnBnClickedBtnTime15)
	ON_BN_CLICKED(IDOK, &CMBISDKTESTDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMBISDKTESTDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_AUTOMATE, &CMBISDKTESTDlg::OnBnClickedBtnAutomate)
	ON_BN_CLICKED(IDC_BTN_IS_FRAG_DATA, &CMBISDKTESTDlg::OnBnClickedBtnIsFragData)
	ON_BN_CLICKED(IDC_BTN_LOAD_FRAMES, &CMBISDKTESTDlg::OnBnClickedBtnLoadFrames)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_ATTIC, &CMBISDKTESTDlg::OnBnClickedButtonShowAttic)
	ON_BN_CLICKED(IDC_BUTTON_RT_TIC, &CMBISDKTESTDlg::OnBnClickedButtonRtTic)
	ON_BN_CLICKED(IDC_BUTTON_INDEX_COUNT, &CMBISDKTESTDlg::OnBnClickedButtonIndexCount)
	ON_BN_CLICKED(IDC_BUTTON_INDEX_POSITION, &CMBISDKTESTDlg::OnBnClickedButtonIndexPosition)
	ON_BN_CLICKED(IDC_BUTTON_DATA_COUNT, &CMBISDKTESTDlg::OnBnClickedButtonDataCount)
	ON_BN_CLICKED(IDC_BUTTON_DATA_POSITION, &CMBISDKTESTDlg::OnBnClickedButtonDataPosition)
END_MESSAGE_MAP()

BOOL CMBISDKTESTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMBISDKTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMBISDKTESTDlg::OnPaint()
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
HCURSOR CMBISDKTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMBISDKTESTDlg::OnBnClickedBtnLoadFile()
{
	
	char filename[MAX_PATH];
	const char* lpPath = NULL;

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = L"";// MBI Data Files\0 * .mbi";
	ofn.lpstrFile = (LPWSTR)(&filename);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = L"Chose MBI Data File.";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	
	if (GetOpenFileNameA((LPOPENFILENAMEA)(&ofn)))
	{
		lpPath = (const char *)(&filename);
		std::string strFileName(filename);
		pMyFile = new MBI::MBIFile(strFileName.c_str());
		bool bSuccess = pMyFile->Init();
		if (bSuccess)
		{
			std::shared_ptr<std::string> pStrInput = pMyFile->GetFilename();
			ShowOutput(IDC_STATIC_FILENAME, *pStrInput, -1);
			std::string strVersion = "MBISDK Version: " + pMyFile->GetVersion();
			ShowOutput(IDC_STATIC_VERSION, strVersion, -1);
			ShowOutput(IDC_STATIC_2, "", -1);
			ShowOutput(IDC_STATIC_3, "", -1);
			ShowOutput(IDC_STATIC_4, "", -1);
			ShowOutput(IDC_STATIC_5, "", -1);
			ShowOutput(IDC_STATIC_6, "", -1);
			ShowOutput(IDC_STATIC_7, "", -1);
			ShowOutput(IDC_STATIC_8, "", -1);
			ShowOutput(IDC_STATIC_9, "", -1);
			ShowOutput(IDC_STATIC_10, "", -1);
			ShowOutput(IDC_STATIC_11, "", -1);
			ShowOutput(IDC_STATIC_12, "", -1);
			ShowOutput(IDC_STATIC_13, "", -1);
			ShowOutput(IDC_STATIC_14, "", -1);
			ShowOutput(IDC_STATIC_19, "", -1);
			ShowOutput(IDC_STATIC_ATTIC_DATA, "", -1);
			ShowOutput(IDC_STATIC_DATA_COUNT_DATA, "", -1);
			ShowOutput(IDC_STATIC_DATA_POSITION_DATA, "", -1);
			ShowOutput(IDC_STATIC_INDEX_COUNT_DATA, "", -1);
			ShowOutput(IDC_STATIC_INDEX_POSITION_DATA, "", -1);
			ShowOutput(IDC_STATIC_RTTIC_DATA, "", -1);
		}
		else
		{
			ShowOutput(IDC_STATIC_FILENAME, "FILE FAILED TO INIT", -1);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnNumFrames()
{
	if (pMyFile != NULL)
	{
		int retrievedNumFrames = pMyFile->NumFrames();
		std::string strTitle = "";
		ShowOutput(IDC_STATIC_2, strTitle, retrievedNumFrames);
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnMaxScans()
{
	if (pMyFile != NULL)
	{
		int nMaxScansInFrames = pMyFile->GetMaxScansInFrame(1); //default to first frame
		std::string strTitle = "";
		ShowOutput(IDC_STATIC_3, strTitle, nMaxScansInFrames);
	}	
}

void CMBISDKTESTDlg::OnBnClickedBtnCalibration()
{
	if (pMyFile != NULL)
	{
		pMyCalib = new MBI::TofCalibration(pMyFile->GetCalibration());
		std::string strInput = "Loaded";
		int nFailureStatus = pMyCalib->getFailureStatus();
		if (nFailureStatus != 0)
		{
			std::string strStatusText = pMyCalib->getStatusText();
			strInput = "Failed: ";
			strInput += strStatusText;
		}
		ShowOutput(IDC_STATIC_4, strInput, -1);
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnTofError()
{
	if (pMyFile != NULL)
	{
		if (pMyCalib != NULL)
		{
			double dbTofError = pMyCalib->TofError(0.0);
			std::string strTitle = "";
			ShowOutputDB(IDC_STATIC_5, strTitle, dbTofError);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnIndexMicro()
{
	if (pMyFile != NULL)
	{
		if (pMyCalib != NULL)
		{
			double dbIndex = pMyCalib->IndexToMicroseconds(1);
			std::string strTitle = "";
			ShowOutputDB(IDC_STATIC_6, strTitle, dbIndex);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnIndexMz()
{
	if (pMyFile != NULL)
	{
		if (pMyCalib != NULL)
		{
			double dbIndex2 = pMyCalib->IndexToMz(1);
			std::string strTitle = "";
			ShowOutputDB(IDC_STATIC_7, strTitle, dbIndex2);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnMicroMz()
{
	if (pMyFile != NULL)
	{
		if (pMyCalib != NULL)
		{
			size_t nIndex3 = pMyCalib->MicrosecondsToIndex(1.0);
			std::string strTitle = "";
			ShowOutput(IDC_STATIC_8, strTitle, nIndex3);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnMzMicro()
{
	if (pMyFile != NULL)
	{
		if (pMyCalib != NULL)
		{
			double dbIndex4 = pMyCalib->MzToMicroseconds(100.0);
			std::string strTitle = "";
			ShowOutputDB(IDC_STATIC_9, strTitle, dbIndex4);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnMzIndex()
{
	if (pMyFile != NULL)
	{
		if (pMyCalib != NULL)
		{
			size_t nIndex5 = pMyCalib->MzToIndex(1.0);
			std::string strTitle = "";
			ShowOutput(IDC_STATIC_10, strTitle, nIndex5);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnSlope()
{
	if (pMyFile != NULL)
	{
		if (pMyCalib != NULL)
		{
			double dbIndex6 = pMyCalib->Slope();
			std::string strTitle = "";
			ShowOutputDB(IDC_STATIC_11, strTitle, dbIndex6);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnIntercept()
{
	if (pMyFile != NULL)
	{
		if (pMyCalib != NULL)
		{
			double dbIndex7 = pMyCalib->Intercept();
			std::string strTitle = "";
			ShowOutputDB(IDC_STATIC_12, strTitle, dbIndex7);
		}
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnMaxPoints()
{
	if (pMyFile != NULL)
	{
		int nIndex8 = pMyFile->GetMaxPointsInScan();
		std::string strTitle = "";
		ShowOutput(IDC_STATIC_13, strTitle, nIndex8);
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnTime15()
{
	if (pMyFile != NULL)
	{
		double dbTime = 0.0;
		std::string strTitle = "Times: ";
		std::string strLoad = "";

		std::shared_ptr<MBI::FrameMetadata> pFrameMetaData = nullptr;
		
		int nMaxFrames = pMyFile->NumFrames();
		
		for (int nIndex = 1; nIndex <= nMaxFrames; nIndex++)
		{
			pFrameMetaData = pMyFile->GetFrameMetadata(nIndex);
			dbTime = pMyFile->GetFrameStartTime(pFrameMetaData);
			strLoad = std::to_string(dbTime);
			strLoad += " | ";
			if (strTitle.length() + strLoad.length() < 100)
			{
				strTitle += strLoad;
			}
		}
		ShowOutput(IDC_STATIC_14, strTitle, -1);
	}
}

void CMBISDKTESTDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CMBISDKTESTDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CMBISDKTESTDlg::ShowOutput(int nStatic, std::string strTitle, long long int nInput)
{
	wchar_t strBuffer[200];
	if (strTitle.length() > 100)
	{
		strTitle = strTitle.substr(0, 99);
	}

	ZeroMemory(&strBuffer, sizeof(strBuffer));
	if (nInput == -1)
	{
	}
	else
	{
		std::string strNum = std::to_string(nInput);
		strTitle += strNum;
	}
	std::wstring strWideTitle(strTitle.begin(), strTitle.end());
	size_t nLength = strWideTitle.length() * 2;
	CopyMemory(strBuffer, strWideTitle.c_str(), nLength);
	GetDlgItem(nStatic)->SetWindowTextW(strBuffer);
}

void CMBISDKTESTDlg::AppendOutput(int nStatic, std::string strTitle, int nInput)
{
	LPCWSTR lpBuffer = NULL;
	//wchar_t strBuffer[400];
	//ZeroMemory(&strBuffer, sizeof(strBuffer));
	CString csTemp;
	GetDlgItem(nStatic)->GetWindowTextW(csTemp);
	if (csTemp.GetLength() > 0)
	{
		csTemp += "\n"; //add carriage return
	}

	if (nInput == -1)
	{
	}
	else
	{
		std::string strNum = std::to_string(nInput);
		strTitle += strNum;
	}
	std::string strCString = CT2A(csTemp);
	strTitle = strCString + strTitle;
	std::wstring strWideTitle(strTitle.begin(), strTitle.end());
	//int nLength = strWideTitle.length() * 2;
	lpBuffer = (LPCWSTR)(strWideTitle.c_str());
	//CopyMemory(strBuffer, strWideTitle.c_str(), nLength);
	if (lpBuffer != NULL)
	{
		GetDlgItem(nStatic)->SetWindowTextW(lpBuffer);
	}
}

void CMBISDKTESTDlg::ShowOutputDB(int nStatic, std::string strTitle, double dbInput)
{
	wchar_t strBuffer[100];
	ZeroMemory(&strBuffer, sizeof(strBuffer));
	std::ostringstream streamObj;
	streamObj << std::fixed; //stop schenanigans with scientific notions
	streamObj << std::setprecision(30);
	streamObj << dbInput;
	std::string strDouble = streamObj.str();

	strTitle += strDouble;
	std::wstring strWideTitle(strTitle.begin(), strTitle.end());
	size_t nLength = strWideTitle.length() * 2;
	CopyMemory(strBuffer, strWideTitle.c_str(), nLength);
	GetDlgItem(nStatic)->SetWindowTextW(strBuffer);
}

void CMBISDKTESTDlg::OnBnClickedBtnAutomate()
{
	OnBnClickedBtnLoadFile();
	OnBnClickedBtnNumFrames();
	OnBnClickedBtnMaxScans();
	OnBnClickedBtnCalibration();
	OnBnClickedBtnTofError();
	OnBnClickedBtnIndexMicro();
	OnBnClickedBtnIndexMz();
	OnBnClickedBtnMicroMz();
	OnBnClickedBtnMzMicro();
	OnBnClickedBtnMzIndex();
	OnBnClickedBtnSlope();
	OnBnClickedBtnIntercept();
	OnBnClickedBtnMaxPoints();
	OnBnClickedBtnTime15();
	OnBnClickedButtonShowAttic();
	OnBnClickedButtonRtTic();
	OnBnClickedButtonIndexCount();
	OnBnClickedButtonIndexPosition();
	OnBnClickedButtonDataCount();
	OnBnClickedButtonDataPosition();
}

void CMBISDKTESTDlg::OnBnClickedBtnIsFragData()
{
	if (pMyFile != NULL)
	{
		int nMaxFrames = pMyFile->NumFrames();
		if (nFrameIndex > nMaxFrames)
		{
			nFrameIndex = 1; //back to one
		}
		else if (nFrameIndex < 1)
		{
			nFrameIndex = 1;
		}
		std::shared_ptr<MBI::FrameMetadata> pFrameMetaData = pMyFile->GetFrameMetadata(nFrameIndex);

		std::shared_ptr<MBI::Frame> frameTemp1 = pMyFile->GetFrame(nFrameIndex);

		bool bIsFrag = frameTemp1->isFragmentationData();
		std::string strTitle;
		strTitle = "Frame ";
		strTitle += std::to_string(nFrameIndex);
		if (true == bIsFrag)
		{
			strTitle += " is FD.";
		}
		else
		{
			strTitle += " is not FD.";
		}
		ShowOutput(IDC_STATIC_19, strTitle, -1);
		nFrameIndex++;
	}
}

void CMBISDKTESTDlg::OnBnClickedBtnLoadFrames()
{
	if (pMyFile != NULL)
	{
		{
			int retrievedNumFrames = pMyFile->NumFrames();
			std::vector<MBI::Frame> vListOfFrames;
			std::shared_ptr<MBI::FrameMetadata> pFrameMetaData = NULL;

			for (int nIndex = 1; nIndex <= retrievedNumFrames; nIndex++)
			{
				pFrameMetaData = pMyFile->GetFrameMetadata(nIndex);
				MBI::Frame frameTemp(pFrameMetaData, pMyFile);
				vListOfFrames.push_back(frameTemp);
			}

			vListOfFrames.empty();
		}
		std::string strTitle = "Frame Load Test Completed.";
		ShowOutput(IDC_STATIC_20, strTitle, -1);
	}
}

void CMBISDKTESTDlg::OnBnClickedButtonShowAttic()
{
	if (pMyFile != NULL)
	{
		std::string strTitle;
		std::string strLoad;
		nFrameIndex = 1;

		std::shared_ptr<MBI::FrameMetadata> pFrameMetaData = pMyFile->GetFrameMetadata(nFrameIndex);
		std::shared_ptr<MBI::Frame> frameTemp1 = pMyFile->GetFrame(nFrameIndex);

		std::shared_ptr<std::vector<int>> attic_list = frameTemp1->GetATTicList();

		size_t nSize = attic_list->size();
		for (size_t nIndex = 0; nIndex < nSize; nIndex++)
		{
			if (attic_list->at(nIndex) > 0)
			{
				strLoad = std::to_string(attic_list->at(nIndex));
				strLoad += " | ";
				if (strTitle.length() + strLoad.length() < 100)
				{
					strTitle += strLoad;
				}
			}
		}

		ShowOutput(IDC_STATIC_ATTIC_DATA, strTitle, -1);
	}
}

void CMBISDKTESTDlg::OnBnClickedButtonRtTic()
{
	if (pMyFile != NULL)
	{
		std::string strTitle;
		std::string strLoad;
		std::pair<double, long long int> sPair;
		nFrameIndex = 1;

		std::shared_ptr<std::vector<std::pair<double, long long int>>> rttic_list = pMyFile->GetRtTicList();

		size_t nSize = rttic_list->size();
		for (int nIndex = 0; nIndex < nSize; nIndex++)
		{
			sPair = rttic_list->at(nIndex);
			strLoad = std::to_string(sPair.second);
			strLoad += " | ";
			if (strTitle.length() + strLoad.length() < 100)
			{
				strTitle += strLoad;
			}
		}

		ShowOutput(IDC_STATIC_RTTIC_DATA, strTitle, -1);
	}
}

void CMBISDKTESTDlg::OnBnClickedButtonIndexCount()
{
	if (pMyFile != NULL)
	{
		std::string strTitle;
		std::string strLoad;
		nFrameIndex = 1;
		int nLastFoundValue = 0;

		std::shared_ptr<MBI::FrameMetadata> pFrameMetaData = pMyFile->GetFrameMetadata(nFrameIndex);
		std::shared_ptr<MBI::Frame> frameTemp1 = pMyFile->GetFrame(nFrameIndex);

		std::shared_ptr<std::vector<int>> index_count_list = frameTemp1->GetSampleOffsets();

		size_t nSize = index_count_list->size();
		for (size_t nIndex = 0; nIndex < nSize; nIndex++)
		{
			if ((index_count_list->at(nIndex) > 0) && (index_count_list->at(nIndex) > nLastFoundValue))
			{
				strLoad = std::to_string(index_count_list->at(nIndex));
				strLoad += " | ";
				if (strTitle.length() + strLoad.length() < 100)
				{
					strTitle += strLoad;
				}
				nLastFoundValue = index_count_list->at(nIndex);
			}
		}

		ShowOutput(IDC_STATIC_INDEX_COUNT_DATA, strTitle, -1);
	}
}

void CMBISDKTESTDlg::OnBnClickedButtonIndexPosition()
{
	if (pMyFile != NULL)
	{
		std::string strTitle;
		std::string strLoad;
		nFrameIndex = 1;
		int nLastFoundValue = 0;

		std::shared_ptr<MBI::FrameMetadata> pFrameMetaData = pMyFile->GetFrameMetadata(nFrameIndex);
		std::shared_ptr<MBI::Frame> frameTemp1 = pMyFile->GetFrame(nFrameIndex);

		std::shared_ptr<std::vector<int>> index_position_list = frameTemp1->GetGateIndexOffsets();

		size_t nSize = index_position_list->size();
		for (size_t nIndex = 0; nIndex < nSize; nIndex++)
		{
			if ((index_position_list->at(nIndex) > 0) && (index_position_list->at(nIndex) > nLastFoundValue))
			{
				strLoad = std::to_string(nIndex);
				strLoad += " | ";
				if (strTitle.length() + strLoad.length() < 100)
				{
					strTitle += strLoad;
				}
				nLastFoundValue = index_position_list->at(nIndex);
			}
		}

		ShowOutput(IDC_STATIC_INDEX_POSITION_DATA, strTitle, -1);
	}
}

void CMBISDKTESTDlg::OnBnClickedButtonDataCount()
{
	if (pMyFile != NULL)
	{
		std::string strTitle;
		std::string strLoad;
		nFrameIndex = 1;

		std::shared_ptr<MBI::FrameMetadata> pFrameMetaData = pMyFile->GetFrameMetadata(nFrameIndex);
		std::shared_ptr<MBI::Frame> frameTemp1 = pMyFile->GetFrame(nFrameIndex);

		std::shared_ptr<std::vector<int>> data_count_list = frameTemp1->GetSampleIntensities();

		size_t nSize = data_count_list->size();
		for (size_t nIndex = 0; nIndex < nSize; nIndex++)
		{
			if (data_count_list->at(nIndex) > 0)
			{
				strLoad = std::to_string(data_count_list->at(nIndex));
				strLoad += " | ";
				if (strTitle.length() + strLoad.length() < 100)
				{
					strTitle += strLoad;
				}
			}
		}

		ShowOutput(IDC_STATIC_DATA_COUNT_DATA, strTitle, -1);
	}
}

void CMBISDKTESTDlg::OnBnClickedButtonDataPosition()
{
	if (pMyFile != NULL)
	{
		std::string strTitle;
		std::string strLoad;
		nFrameIndex = 1;
		std::pair<int, int> sPair;

		std::shared_ptr<MBI::FrameMetadata> pFrameMetaData = pMyFile->GetFrameMetadata(nFrameIndex);
		std::shared_ptr<MBI::Frame> frameTemp1 = pMyFile->GetFrame(nFrameIndex);

		std::shared_ptr<std::vector<std::pair<int, int>>> data_count_list = frameTemp1->GetGates();

		size_t nSize = data_count_list->size();
		for (size_t nIndex = 0; nIndex < nSize; nIndex++)
		{
			sPair = data_count_list->at(nIndex);
			if (sPair.first > 0)
			{
				strLoad = " [";
				strLoad += std::to_string(sPair.first);
				strLoad += " | ";
				strLoad += std::to_string(sPair.second);
				strLoad += "]";
				if (strTitle.length() + strLoad.length() < 100)
				{
					strTitle += strLoad;
				}
			}
		}

		ShowOutput(IDC_STATIC_DATA_POSITION_DATA, strTitle, -1);
	}
}
