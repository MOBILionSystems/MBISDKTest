
// MBI_SDK_TESTDlg.h : header file
//

#pragma once
#include <string>
#include "MBIFile.h"
#include "MBICalibration.h"

typedef HRESULT(CALLBACK* LPFNDLLFUNCConstChar)(const char* path);
typedef HRESULT(CALLBACK* LPFNDLLFUNCPtrString)(std::string * path);

// CMBISDKTESTDlg dialog
class CMBISDKTESTDlg : public CDialogEx
{
// Construction
public:
	CMBISDKTESTDlg(CWnd* pParent = nullptr);	// standard constructor
	~CMBISDKTESTDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MBISDK_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	HINSTANCE hDLL; //handle to DLL
	LPFNDLLFUNCConstChar lpfnDllFuncMBIFileConstructor; //function pointer
	LPFNDLLFUNCPtrString lpfnDllFuncMBITestString; //function pointer
	LPFNDLLFUNCPtrString lpfnDllFuncMBIFileConstructorString;
	HRESULT hrReturnVal;

	enum
	{
		ENUM_FRAME_CONSTRUCTOR = 7,
		ENUM_MBIFILE_CONSTRUCTOR_STRING = 30,
		ENUM_MBIFILE_CONSTRUCTOR_CHAR = 31
	};

	MBI::MBIFile* pMyFile;
	MBI::TofCalibration* pMyCalib;
	int nFrameIndex;

	void ShowOutput(int nStatic, std::string strTitle, long long nInput);
	void AppendOutput(int nStatic, std::string strTitle, int nInput);
	void ShowOutputDB(int nStatic, std::string strTitle, double dbInput);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_nDisplayFrameIndex;
	int m_nStartScanNum;
	int m_nEndScanNum;
	afx_msg void OnBnClickedBtnLoadFile();
	afx_msg void OnBnClickedBtnNumFrames();
	afx_msg void OnBnClickedBtnMaxScans();
	afx_msg void OnBnClickedBtnCalibration();
	afx_msg void OnBnClickedBtnTofError();
	afx_msg void OnBnClickedBtnIndexMicro();
	afx_msg void OnBnClickedBtnIndexMz();
	afx_msg void OnBnClickedBtnMicroMz();
	afx_msg void OnBnClickedBtnMzMicro();
	afx_msg void OnBnClickedBtnMzIndex();
	afx_msg void OnBnClickedBtnSlope();
	afx_msg void OnBnClickedBtnIntercept();
	afx_msg void OnBnClickedBtnMaxPoints();
	afx_msg void OnBnClickedBtnTime15();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnAutomate();
	afx_msg void OnBnClickedBtnScanMzIndexSparse();
	afx_msg void OnBnClickedBtnScanMzIndexDense();
	afx_msg void OnBnClickedBtnScanTofIndexSparse();
	afx_msg void OnBnClickedBtnScanTofIndexDense();
	afx_msg void OnBnClickedBtnIsFragData();
	afx_msg void OnBnClickedBtnLoadFrames();
	afx_msg void OnBnClickedButtonShowAttic();
	afx_msg void OnBnClickedButtonRtTic();
	afx_msg void OnBnClickedButtonIndexCount();
	afx_msg void OnBnClickedButtonIndexPosition();
	afx_msg void OnBnClickedButtonDataCount();
	afx_msg void OnBnClickedButtonDataPosition();
};
