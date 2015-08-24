// DialogAnime.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SearchAlg.h"
#include "DialogAnime.h"
#include "afxdialogex.h"


// CDialogAnime �Ի���

IMPLEMENT_DYNAMIC(CDialogAnime, CDialogEx)

CDialogAnime::CDialogAnime(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogAnime::IDD, pParent)
	, m_periodMS(1)
	, m_pixelStep(2)
	, m_routeAlpha(90)
{

}

CDialogAnime::~CDialogAnime()
{
}

void CDialogAnime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Period, m_periodMS);
	DDV_MinMaxInt(pDX, m_periodMS, 0, 10000);
	DDX_Text(pDX, IDC_PixelStep, m_pixelStep);
	DDV_MinMaxInt(pDX, m_pixelStep, 0, 1000);
	DDX_Text(pDX, IDC_RouteAlpha, m_routeAlpha);
	DDV_MinMaxInt(pDX, m_routeAlpha, 0, 255);
}


BEGIN_MESSAGE_MAP(CDialogAnime, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogAnime::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogAnime ��Ϣ�������


void CDialogAnime::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);//���ý����ϵĶ����ŵ����������
	CDialogEx::OnOK();
}
