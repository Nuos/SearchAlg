// CfpResultDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SearchAlg.h"
#include "CfpResultDlg.h"
#include "afxdialogex.h"


// CfpResultDlg �Ի���

IMPLEMENT_DYNAMIC(CfpResultDlg, CDialogEx)

CfpResultDlg::CfpResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CfpResultDlg::IDD, pParent)
{

}

CfpResultDlg::~CfpResultDlg()
{
}

void CfpResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_fPResult, m_fPResult);
}


BEGIN_MESSAGE_MAP(CfpResultDlg, CDialogEx)
END_MESSAGE_MAP()


// CfpResultDlg ��Ϣ�������
