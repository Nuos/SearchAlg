#pragma once
#include "afxwin.h"


// CfpResultDlg �Ի���

class CfpResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CfpResultDlg)

public:
	CfpResultDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CfpResultDlg();

// �Ի�������
	enum { IDD = IDD_FPResult };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ������ʾ
	CEdit m_fPResult;
};
