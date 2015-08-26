
// SearchAlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SearchAlg.h"
#include "SearchAlgDlg.h"
#include "afxdialogex.h"
#include "math.h"
#include "DialogAnime.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EARTHRADIUS (6371) //��λ��km
#define PI (3.1415926535898)
#define MAINAREA_BG (RGB(0, 176, 240))

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSearchAlgDlg �Ի���



CSearchAlgDlg::CSearchAlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchAlgDlg::IDD, pParent)
	, m_LongtitudeStart(0)
	, m_LatitudeStart(0)
	, m_longtitudeEnd(0)
	, m_LatitudeEnd(0)
	, m_findProb(0)
	, m_searchWidth(0)
	, m_xDivNum(0)
	, m_yDivNum(0)
	, m_leakSweep(_T(""))
	, m_doubleSweep(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(1);
	//�û��Զ����ʼ������
	m_axisX = 0;
	m_axisY = 0;
	m_nFPts = 0;
	m_currPt = CPoint(0,0);//��ǰ�����ʼֵ
	m_stepPixel = 5;
	m_periodMS = 5;
	m_routeAlpha = 90;
	m_missionAreaFlag = false;
	m_searchParamFlag = false;
}

//��������
CSearchAlgDlg::~CSearchAlgDlg()
{
	if (NULL != m_fpResDlg)//�����ģ̬�Ի����Ѵ�����������
		delete m_fpResDlg;
}

void CSearchAlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LongtitudeStart, m_LongtitudeStart);
	DDV_MinMaxDouble(pDX, m_LongtitudeStart, 0, 180);
	DDX_Text(pDX, IDC_LatitudeStart, m_LatitudeStart);
	DDV_MinMaxDouble(pDX, m_LatitudeStart, 0, 90);
	DDX_Text(pDX, IDC_LongtitudeEnd, m_longtitudeEnd);
	DDV_MinMaxDouble(pDX, m_longtitudeEnd, 0, 180);
	DDX_Text(pDX, IDC_LatitudeEnd, m_LatitudeEnd);
	DDV_MinMaxDouble(pDX, m_LatitudeEnd, 0, 90);
	DDX_Text(pDX, IDC_FindProb, m_findProb);
	DDV_MinMaxDouble(pDX, m_findProb, 0, 1);
	DDX_Text(pDX, IDC_SearchWidth, m_searchWidth);
	DDX_Control(pDX, IDC_SearchMode, m_searchMode);
	DDX_Text(pDX, IDC_XDivNum, m_xDivNum);
	DDV_MinMaxUInt(pDX, m_xDivNum, 2, 8);
	DDX_Text(pDX, IDC_YDivNum, m_yDivNum);
	DDV_MinMaxUInt(pDX, m_yDivNum, 2, 8);
	DDX_Text(pDX, IDC_LeakSweep, m_leakSweep);
	DDX_Text(pDX, IDC_DoubleSweep, m_doubleSweep);
}

BEGIN_MESSAGE_MAP(CSearchAlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_About, &CSearchAlgDlg::OnAbout)
	ON_BN_CLICKED(IDC_SetMissionArea, &CSearchAlgDlg::OnBnClickedSetmissionarea)
	ON_BN_CLICKED(IDC_SetSearchParam, &CSearchAlgDlg::OnBnClickedSetsearchparam)
	ON_BN_CLICKED(IDC_BeginSearch, &CSearchAlgDlg::OnBnClickedBeginsearch)
	ON_BN_CLICKED(IDC_fPResult, &CSearchAlgDlg::OnBnClickedfpresult)
	ON_WM_TIMER()
	ON_COMMAND(ID_SetAnime, &CSearchAlgDlg::OnSetanime)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CSearchAlgDlg ��Ϣ�������

BOOL CSearchAlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//m_searchMode.SetCurSel(0); //��������ģʽ������Ϊ�����Ԫ��
	
	//�趨�������Ĭ��ֵ
	m_xDivNum = 6;
	m_yDivNum = 6;
	UpdateData(FALSE);
	GetDlgItem(IDC_fPResult)->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSearchAlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSearchAlgDlg::OnPaint()
{
	if (IsIconic())//�жϴ����Ƿ�����С��״̬���������С����ť֮��),
		//������С���˶Ի���֮����ȻIsIconic()��ֵ��TRUE������OnPaint()�������������С�
		//��ΪOnPaint()��Ӧ����WM_PAINT��Ϣ����WM_PAINT��Ϣ����Կͻ����ġ�һ����С���˵Ĵ��ڲ���Ҫ�ػ�ͻ�����
		//һ��ûɶ���õ�����
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		//------------�ҵĻ�ͼ��------------
		CWnd *pWnd = GetDlgItem(IDC_MainArea);
		CDC *pDC = pWnd->GetDC();
		CRect mainAreaRect;
		GetDlgItem(IDC_MainArea)->GetClientRect(mainAreaRect);


		//����˫����ķ�ʽ�ػ�MainArea��
		CDC MemDC; //���ȶ���һ����ʾ�豸����
		CBitmap MemBitmap;//����һ��λͼ����
		//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
		MemDC.CreateCompatibleDC(NULL);
		//-----------��ʱ�����ܻ�ͼ����Ϊû�еط��� ^_^----------------
		//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
		MemBitmap.CreateCompatibleBitmap(pDC, mainAreaRect.Width(), mainAreaRect.Height());
		//��λͼѡ�뵽�ڴ���ʾ�豸��
		//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
		CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
		//���ñ���ɫ��λͼ����ɾ����������õ��ǰ�ɫ��Ϊ����
		//��Ҳ�������Լ�Ӧ���õ���ɫ
		MemDC.FillSolidRect(0, 0, mainAreaRect.Width(), mainAreaRect.Height(), MAINAREA_BG);
		//-------------------------��ͼ---------------------------------

		pWnd->Invalidate();//���ض�������Ϊ��Ч�����ȴ�ˢ��
		//������ˢ�µ�ʱ���ǲ���ˢ�´�����ؼ�����ģ���̬�ı��Ȳ��ֿؼ�����
		pWnd->UpdateWindow();
		CBrush brs;
		brs.CreateSolidBrush(MAINAREA_BG);
		pDC->FillRect(&mainAreaRect, &brs);
		pDC->SetBkMode(TRANSPARENT);//�������屳����ɫΪ͸��

		//-------------------------���廭��-----------------------------
		CPen *pPenRed = new CPen();
		pPenRed->CreatePen(PS_DOT, 5, RGB(255, 0, 0));//��ɫ����
		CPen* pPenBlack = new CPen();
		pPenBlack->CreatePen(PS_SOLID, 3, RGB(0, 0, 0));//��ɫ����
		CPen* pPenGray = new CPen();
		pPenGray->CreatePen(PS_DOT, 1, RGB(128, 128, 128));//��ɫ����
		CPen *pPenBG = new CPen();
		pPenBG->CreatePen(PS_DOT, 1, MAINAREA_BG);//�������򱳾���ɫ����

		//--------��դ������ͼ------------
		if (m_searchMode.GetCurSel() == 0){//դ��������
			//�ɿ�����mainAreaRect.Width()��mainAreaRect.Height()��ΪaxisX��axisY
			int axisX = mainAreaRect.Width();
			int axisY = mainAreaRect.Height();
			int XPadding = 60;//�����������Ͻǵļ��
			int YPadding = 50;
			m_xPadding = XPadding;
			m_yPadding = YPadding;
			int xDivisionNum = m_xDivNum;
			int yDivisionNum = m_yDivNum;
			//ѡ�к�ɫ���ʣ������浱ǰ����ΪpOldPen
			CGdiObject* pOldPen = pDC->SelectObject(pPenBlack);
			pDC->MoveTo(XPadding, YPadding);//�趨ԭ��
			pDC->LineTo(XPadding, axisY - 30);//��ֱ��
			pDC->MoveTo(XPadding, YPadding);//��ԭ��
			pDC->LineTo(axisX - 30, YPadding);//ˮƽ��
			//�������ͷ
			pDC->MoveTo(axisX - 30 - 5, YPadding - 5);
			pDC->LineTo(axisX - 30, YPadding);
			pDC->MoveTo(axisX - 30 - 5, YPadding + 5);
			pDC->LineTo(axisX - 30, YPadding);
			////�������ͷ
			pDC->MoveTo(XPadding - 5, axisY - 30 - 3);
			pDC->LineTo(XPadding, axisY - 30);
			pDC->MoveTo(XPadding + 5, axisY - 30 - 3);
			pDC->LineTo(XPadding, axisY - 30);

			CString str;
			int i = 0;
			//дx��̶�ֵ��λ
			pDC->TextOut(axisX - 50, YPadding + 10, _T("X(km)"));
			//дy��̶�ֵ��λ
			pDC->TextOut(XPadding - 25, axisY - 25, _T("Y(km)"));

			//������
			int nMeshLengthX = (axisX - XPadding - 10) / (xDivisionNum + 1);
			int nMeshLengthY = (axisY - YPadding - 10) / (yDivisionNum + 1);
			pDC->SelectObject(pPenGray);
			for (i = 1; i<xDivisionNum; i++){
				pDC->MoveTo(XPadding + i*nMeshLengthX, YPadding);
				pDC->LineTo(XPadding + i*nMeshLengthX, axisY - 40);
			}
			//���һ��ʵ���ú�ɫ��ʾ
			pDC->SelectObject(pPenRed);
			pDC->MoveTo(XPadding + xDivisionNum*nMeshLengthX, YPadding);
			pDC->LineTo(XPadding + xDivisionNum*nMeshLengthX, axisY - 40);
			pDC->SelectObject(pPenGray);

			nMeshLengthY = (axisY - YPadding - 10) / (yDivisionNum + 1);
			for (i = 1; i<yDivisionNum; i++){
				pDC->MoveTo(XPadding, YPadding + i*nMeshLengthY);
				pDC->LineTo(axisX - 40, YPadding + i*nMeshLengthY);
			}
			//���һ��ʵ��
			pDC->SelectObject(pPenRed);
			pDC->MoveTo(XPadding, YPadding + yDivisionNum*nMeshLengthY);
			pDC->LineTo(axisX - 40, YPadding + yDivisionNum*nMeshLengthY);
			//��������߽�Ľ���
			pDC->SelectObject(pPenBlack);
			str.Format(L"P(%.1f,%.1f)", m_axisX, m_axisY);
			int wallX = XPadding + xDivisionNum*nMeshLengthX;
			int wallY = YPadding + yDivisionNum*nMeshLengthY;
			m_ratioX = (wallX - XPadding) / m_axisX;
			m_ratioY = (wallY - YPadding) / m_axisY;

			pDC->TextOut(XPadding + xDivisionNum*nMeshLengthX - 45, YPadding + yDivisionNum*nMeshLengthY + 10, str);
			pDC->Ellipse(wallX - 5, wallY - 5, wallX + 5, wallY + 5);


			//��������������
			double xRealDiv = m_axisX / xDivisionNum;
			for (i = 0; i < xDivisionNum + 1; i++){
				str.Format(L"%.1f", i*xRealDiv);
				pDC->TextOut(XPadding + i*nMeshLengthX - 10, YPadding - 25, str);
			}
			//��������������
			double yRealDiv = m_axisY / yDivisionNum;
			for (i = 0; i < yDivisionNum + 1; i++){
				str.Format(L"%.1f", i*yRealDiv);
				pDC->TextOut(XPadding - 45, YPadding + i*nMeshLengthY - 8, str);
			}
			//�ָ���ǰ�Ļ���
			pDC->SelectObject(pOldPen);
			ReleaseDC(pDC);
			//----------------------------
		}
		else if (1 == m_searchMode.GetCurSel()){//������������Ҫ�޶�����ľ�γ�Ȳ���Ȳ��ܱ�֤��������Ϊ����
			if (m_longtitudeEnd - m_LongtitudeStart != m_LatitudeEnd - m_LatitudeStart){
				MessageBox(L"���棺��������Ϊ���Σ���γ�Ȳ������ȣ�");
				return;
			}
			int axisX = mainAreaRect.Width();
			int axisY = mainAreaRect.Height();
			int XPadding = 30;//�����������ҵļ��
			int YPadding = 25;//�����������µļ��
			m_xPadding = XPadding;
			m_yPadding = YPadding;
			int xDivisionNum = m_xDivNum;
			int yDivisionNum = m_yDivNum;
			//ѡ�к�ɫ���ʣ������浱ǰ����ΪpOldPen
			CGdiObject* pOldPen = pDC->SelectObject(pPenBlack);
			int centerX = (axisX - 2 * XPadding) / 2 + XPadding;//��������x
			int centerY = (axisY - 2 * YPadding) / 2 + YPadding;//��������y
			pDC->MoveTo(XPadding, centerY);
			pDC->LineTo(axisX - XPadding, centerY);//ˮƽ��
			pDC->MoveTo(centerX, axisY-YPadding);
			pDC->LineTo(centerX, YPadding);//��ֱ��
			//�������ͷ
			pDC->MoveTo(axisX - XPadding - 5, centerY - 5);
			pDC->LineTo(axisX - XPadding, centerY);
			pDC->MoveTo(axisX - XPadding - 5, centerY + 5);
			pDC->LineTo(axisX - XPadding, centerY);
			////�������ͷ
			pDC->MoveTo(centerX - 5, YPadding + 5);
			pDC->LineTo(centerX, YPadding);
			pDC->MoveTo(centerX + 5, YPadding + 5);
			pDC->LineTo(centerX, YPadding);

			CString str;
			int i = 0;
			//дx��̶�ֵ��λ
			pDC->TextOut(axisX - XPadding-20, centerY-28, _T("X(km)"));
			//дy��̶�ֵ��λ
			pDC->TextOut(centerX-10, YPadding - 25, _T("Y(km)"));
			//x��������
			int nMeshLengthX = (axisX - 2*XPadding - 10) / (xDivisionNum + 1);
			int nMeshLengthY = (axisY - 2*YPadding - 10) / (yDivisionNum + 1);
			pDC->SelectObject(pPenGray);
			for (i = 1; i<xDivisionNum/2; i++){//��������������
				pDC->MoveTo(centerX + i*nMeshLengthX, YPadding);
				pDC->LineTo(centerX + i*nMeshLengthX, axisY - YPadding);
			}
			//���һ��ʵ���ú�ɫ��ʾ
			pDC->SelectObject(pPenRed);
			pDC->MoveTo(centerX + xDivisionNum*nMeshLengthX / 2, YPadding);
			pDC->LineTo(centerX + xDivisionNum*nMeshLengthX / 2, axisY - YPadding);
			pDC->SelectObject(pPenGray);
			for (i = 1; i<xDivisionNum / 2; i++){//��������������
				pDC->MoveTo(centerX - i*nMeshLengthX, YPadding);
				pDC->LineTo(centerX - i*nMeshLengthX, axisY - YPadding);
			}
			pDC->SelectObject(pPenRed);
			pDC->MoveTo(centerX - xDivisionNum*nMeshLengthX / 2, YPadding);
			pDC->LineTo(centerX - xDivisionNum*nMeshLengthX / 2, axisY - YPadding);
			//y��������
			pDC->SelectObject(pPenGray);
			for (i = 1; i<yDivisionNum / 2; i++){//��������������
				pDC->MoveTo(XPadding, centerY - i*nMeshLengthY);
				pDC->LineTo(axisX - XPadding, centerY - i*nMeshLengthY);
			}
			pDC->SelectObject(pPenRed);
			pDC->MoveTo(XPadding, centerY - yDivisionNum*nMeshLengthY / 2);
			pDC->LineTo(axisX - XPadding, centerY - yDivisionNum*nMeshLengthY / 2);
			pDC->SelectObject(pPenGray);
			for (i = 1; i<yDivisionNum / 2; i++){//��������������
				pDC->MoveTo(XPadding, centerY + i*nMeshLengthY);
				pDC->LineTo(axisX - XPadding, centerY + i*nMeshLengthY);
			}
			pDC->SelectObject(pPenRed);
			pDC->MoveTo(XPadding, centerY + yDivisionNum*nMeshLengthY / 2);
			pDC->LineTo(axisX - XPadding, centerY + yDivisionNum*nMeshLengthY / 2);

			//��������߽�Ľ���
			int wallX = xDivisionNum*nMeshLengthX;
			int wallY = yDivisionNum*nMeshLengthY;
			m_ratioX = wallX / m_axisX;
			m_ratioY = wallY / m_axisY;
			pDC->SelectObject(pPenBlack);
			//���P1
			str.Format(L"P1(%.1f,%.1f)", -m_axisX / 2, m_axisY / 2);
			pDC->TextOut((axisX-wallX)/2-45, (axisY-wallY)/2-30 , str);
			pDC->Ellipse((axisX - wallX) / 2 - 5, (axisY - wallY) / 2 - 5, (axisX - wallX) / 2 + 5, (axisY - wallY) / 2 + 5);
			//���P2
			str.Format(L"P2(%.1f,%.1f)", m_axisX / 2, m_axisY / 2);
			pDC->TextOut((axisX - wallX) / 2 + wallX - 65, (axisY - wallY) / 2 - 30, str);
			pDC->Ellipse((axisX - wallX) / 2 + wallX - 5, (axisY - wallY) / 2 - 5, (axisX - wallX) / 2 + wallX + 5, (axisY - wallY) / 2 + 5);
			//���P3
			str.Format(L"P3(%.1f,%.1f)", m_axisX / 2, -m_axisY / 2);
			pDC->TextOut((axisX - wallX) / 2 + wallX - 65, (axisY - wallY) / 2 + wallY + 10, str);
			pDC->Ellipse((axisX - wallX) / 2 + wallX - 5, (axisY - wallY) / 2 + wallY - 5, (axisX - wallX) / 2 + wallX + 5, (axisY - wallY) / 2 + wallY + 5);
			//���P4
			str.Format(L"P4(%.1f,%.1f)", -m_axisX / 2, -m_axisY / 2);
			pDC->TextOut((axisX - wallX) / 2 - 45, (axisY - wallY) / 2 + wallY + 10, str);
			pDC->Ellipse((axisX - wallX) / 2 - 5, (axisY - wallY) / 2 + wallY - 5, (axisX - wallX) / 2 + 5, (axisY - wallY) / 2 + wallY + 5);

			//��������������
			double xRealDiv = m_axisX / xDivisionNum;
			for (i = 1; i <= xDivisionNum/2; i++){//���������ұ�����
				str.Format(L"%.1f", i*xRealDiv);
				pDC->TextOut(centerX + i*nMeshLengthX - 15, centerY + 10, str);
			}
			for (i = 1; i <= xDivisionNum/2; i++){//���������������
				str.Format(L"%.1f", -i*xRealDiv);
				pDC->TextOut(centerX - i*(nMeshLengthX+8) - 10, centerY + 10, str);
			}
			//��������������
			double yRealDiv = m_axisY / yDivisionNum;
			for (i = 1; i <= yDivisionNum/2; i++){//���������ϱ�����
				str.Format(L"%.1f", i*yRealDiv);
				pDC->TextOut(centerX+5, centerY - i*nMeshLengthY - 10, str);
			}
			for (i = 1; i <= yDivisionNum / 2; i++){//���������±�����
				str.Format(L"%.1f", -i*yRealDiv);
				pDC->TextOut(centerX +5, centerY + i*nMeshLengthY - 10, str);
			}
			//����ԭ��
			str.Format(L"0.0");
			pDC->TextOut(centerX +5, centerY+5, str);

			//�ָ���ǰ�Ļ���
			pDC->SelectObject(pOldPen);
			ReleaseDC(pDC);
		}

		
		delete pPenBlack;
		delete pPenRed;
		delete pPenGray;
		delete pPenBG;
		//--------------------------------------------------------------

		//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
		pDC->BitBlt(0, 0, mainAreaRect.Width(), mainAreaRect.Height(), &MemDC, 0, 0, SRCCOPY);
		//��ͼ��ɺ������
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
		ReleaseDC(pDC);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSearchAlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//ͨ�����ָ��ʻ�ȡ������ȣ�ע��P��(0,1)֮��
double CSearchAlgDlg::GetSearchWidth(double findProb)
{
	double deltaPow = 375 / (2 * (log(double(5)) - log(double(3))));
	double omega = sqrt(25 - 2 * deltaPow * log(findProb));
	return 2*omega;
}

//��ȡ����·����m_flightPointSet�У�դ��ģʽ
void CSearchAlgDlg::GetFPSetGrid()
{
	m_fPSetReal.clear();//��ʵ���꣬���������ʾ�����溬double���ݣ�
	m_fPSetScreen.clear();//��Ļ���꣨����ֻ��int�����ݣ�
	
	double W = m_searchWidth;

	m_fPSetReal.push_back(MyPoint(0, W/2));//A0�������
	m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));//A0�������

	int i = 1;
	while (true){
		//�ȼ����A(8i-7)��A(8i)����������Ӧ�����popһЩ��ȥ
		m_fPSetReal.push_back(MyPoint(m_axisX - 1 - W / 2, W / 2 + (2 * i - 2)*W));//A(8i-7)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));

		m_fPSetReal.push_back(MyPoint(m_axisX - W / 2, W / 2 + 1 + (2 * i - 2)*W));//A(8i-6)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));

		m_fPSetReal.push_back(MyPoint(m_axisX - W / 2, W / 2 - 1 + (2 * i - 1)*W));//A(8i-5)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));

		m_fPSetReal.push_back(MyPoint(m_axisX - 1 - W / 2, W / 2 + (2 * i - 1)*W));//A(8i-4)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));

		m_fPSetReal.push_back(MyPoint(W / 2+1, W/2 + (2 * i - 1)*W));//A(8i-3)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));

		m_fPSetReal.push_back(MyPoint(W / 2 , W / 2 + 1 + (2 * i-1) *W));//A(8i-2)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));

		m_fPSetReal.push_back(MyPoint(W / 2 ,  W / 2 -1 + 2 * i *W));//A(8i-1)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));

		m_fPSetReal.push_back(MyPoint(W / 2 + 1, W / 2 + 2 * i *W));//A(8i)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));

		if ((2 * i - 1)*W<m_axisY && 2 * i*W>m_axisY){//ת�����㵽A(8i-4)Ϊֹ���˳���Ϊ(0,W/2+(2i-1)W)
			for (int j = 0; j < 4; j++){
				m_fPSetReal.pop_back();
				m_fPSetScreen.pop_back();
			}
			//ѹ���˳��㣬�˳�����Ϊ��������һ��Ԫ��
			m_fPSetReal.push_back(MyPoint(0, W / 2 + (2 * i - 1)*W));
			m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));
			break;

		}
		else if (2*i*W<m_axisY && (2*i+1)*W>m_axisY){//ת�����㵽A(8i)Ϊֹ���˳���Ϊ(x1,W/2+2iW)
			//ѹ���˳��㣬�˳�����Ϊ��������һ��Ԫ��
			m_fPSetReal.push_back(MyPoint(m_axisX, W / 2 + 2 * i *W));
			m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Grid(m_ratioX, m_ratioY, m_xPadding, m_yPadding));
			break;
		}

		i++;
	}


}
//��ȡ����·����m_flightPointSet�У�����ģʽ
void CSearchAlgDlg::GetFPSetSquare()
{
	m_fPSetReal.clear();//��ʵ���꣬���������ʾ�����溬double���ݣ�
	m_fPSetScreen.clear();//��Ļ���꣨����ֻ��int�����ݣ�

	double W = m_searchWidth;
	CRect mainAreaRect;
	GetDlgItem(IDC_MainArea)->GetClientRect(mainAreaRect);
	int centerX = (mainAreaRect.Width() - 2 * m_xPadding) / 2 + m_xPadding;//��������x
	int centerY = (mainAreaRect.Height() - 2 * m_yPadding) / 2 + m_yPadding;//��������y

	m_fPSetReal.push_back(MyPoint(0, 0));//A0�������
	m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));//A0�������

	//�����ֹ��kֵ
	/*int k = -1;
	if (m_axisX / (2 * W) + 0.5 == ceil(m_axisX / (2 * W))){
		k = int(floor(m_axisX / (2 * W)));
	}
	else if (m_axisX / (2 * W) + 0.5>ceil(m_axisX / (2 * W))){
		k = int(ceil(m_axisX / (2 * W)));
	}
	else{
		k = int(floor(m_axisX / (2 * W)));
	}*/

	int k = int(floor(1 + m_axisX / (2 * W)));

	for (int i = 1; i <= k;i++)
	{
		//�ȼ����A(8i-7)��A(8i)����������Ӧ�����popһЩ��ȥ
		m_fPSetReal.push_back(MyPoint(i*W - W/2 - 1 , (i-1)*W));//A(8i-7)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));

		m_fPSetReal.push_back(MyPoint(i*W - W / 2, (i - 1)*W - 1));//A(8i-6)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));

		m_fPSetReal.push_back(MyPoint(i*W - W / 2, 1-i*W ));//A(8i-5)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));

		m_fPSetReal.push_back(MyPoint(i*W - 1 - W / 2, -i*W));//A(8i-4)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));

		m_fPSetReal.push_back(MyPoint(W / 2 - i*W + 1, -i*W));//A(8i-3)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));

		m_fPSetReal.push_back(MyPoint(W / 2 - i*W, 1-i*W));//A(8i-2)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));

		m_fPSetReal.push_back(MyPoint(W / 2 - i*W, i*W - 1));//A(8i-1)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));

		m_fPSetReal.push_back(MyPoint(W / 2 - i*W + 1, i*W));//A(8i)
		m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));
	
	}
	//ת�����㵽A(8i)Ϊֹ���˳���Ϊ(a,iW)
	//ѹ���˳��㣬�˳�����Ϊ��������һ��Ԫ��
	m_fPSetReal.push_back(MyPoint(m_axisX / 2, k *W));
	m_fPSetScreen.push_back(m_fPSetReal.back().CvtToCP_Square(m_ratioX, m_ratioY, m_xPadding, m_yPadding, centerX, centerY));
}

//ͨ����·���ȡ�����ܳ���
double CSearchAlgDlg::GetFLenGrid(const vector<MyPoint> &fPSetReal)
{
	double flightLen = 0.0;
	//�ȼ���1/4Բ���Ļ���
	double arcLen = (1 * 2 * PI) / 4;
	//���ξ���ͻ���
	for (size_t i = 0; i < fPSetReal.size()-1; i++)
	{
		if (i%2 != 0){//A(2i+1)->A(2i+2)���֣���ֱ�Ӽ��ϻ�����������һ��
			flightLen += arcLen;
		}
		else{//�����ۼӼ��ξ���
			flightLen += sqrt(pow(fPSetReal[i + 1].x - fPSetReal[i].x, 2) + pow(fPSetReal[i + 1].y - fPSetReal[i].y, 2));
		}

	}
	return flightLen;
}

void CSearchAlgDlg::OnAbout()
{
	// TODO:  �ڴ���������������
	CDialog *aboutDlg = new CDialog();
	aboutDlg->Create(MAKEINTRESOURCE(IDD_ABOUTBOX));
	aboutDlg->ShowWindow(true);
}


void CSearchAlgDlg::OnBnClickedSetmissionarea()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_fPResult)->EnableWindow(FALSE);

	UpdateData(TRUE);//��EditControl��ֵȡ�����������Value������
	if (m_LongtitudeStart < 0 || m_LatitudeStart < 0){//��γ�������쳣
		MessageBox(L"���棺��ʼ��γ�Ȳ���С��0��");
		m_missionAreaFlag = false;
		return;
	}
	else if (m_longtitudeEnd > 180 || m_LatitudeEnd > 90){//��γ�������쳣
		MessageBox(L"���棺������γ�Ȳ��ܳ�����Χ��");
		m_missionAreaFlag = false;
		return;
	}
	else if (m_longtitudeEnd - m_LongtitudeStart <= 0 || m_LatitudeEnd - m_LatitudeStart<=0){//��ʼ��γ��С�ڽ�����γ��
		MessageBox(L"���棺������γ��С�ڻ������ʼ��γ�ȣ�");
		m_missionAreaFlag = false;
		return;
	}
	else{//���������ÿ�ε������MissionArea�����µ�����̶�
		if (0 == m_searchMode.GetCurSel()){//դ������
			
		}
		else if (1 == m_searchMode.GetCurSel()){//������������Ҫ�޶�����ľ�γ�Ȳ���Ȳ��ܱ�֤��������Ϊ����
			if (m_longtitudeEnd - m_LongtitudeStart != m_LatitudeEnd - m_LatitudeStart){
				MessageBox(L"���棺��������Ϊ���Σ���γ�Ȳ������ȣ�");
				return;
			}
			UpdateData(TRUE);
			if (m_xDivNum % 2 != 0){//�������ż��
				MessageBox(L"���棺����ģʽ�µ�����������Ϊż����");
				return;
			}
		}
		else{
			MessageBox(L"���棺δѡ������ģʽ��");
		}
		//��������ģʽ����Ҫ������Щֵ
		m_axisX = 2 * PI*EARTHRADIUS*(m_longtitudeEnd - m_LongtitudeStart) / 360;
		m_axisY = 2 * PI*EARTHRADIUS*(m_LatitudeEnd - m_LatitudeStart) / 360;
		//����С�������λ
		m_axisX = floor(m_axisX*100.0f + 0.5) / 100.0f;//����С�������λ
		m_axisY = floor(m_axisY*100.0f + 0.5) / 100.0f;
		//�������������ٴ���240km���ܵõ��ϵ͵ĸ����ʺ�©����
		if ((m_longtitudeEnd - m_LongtitudeStart < (240 * 360 / (2 * PI*EARTHRADIUS))) || 
								(m_LatitudeEnd - m_LatitudeStart < (240 * 360 / (2 * PI*EARTHRADIUS)))){
			MessageBox(L"���棺��������߽粻��С��240km��");
			m_missionAreaFlag = false;
			return;
		}
		
		m_missionAreaFlag = true;
		//����������
		Invalidate();//void Invalidate( BOOL bErase = TRUE );����: bErase �������Ƿ�Ҫ��WM_PAINT��Ϣǰ����WM_ERASEBKGND
		//Ҳ���ǣ���Ĭ������£�TRUE�����ȷ��ͱ���ˢ����Ϣ����ˢ��ǰ������FALSEֻˢ��ǰ����Ҳ����ֻ����OnPaint������
		UpdateWindow();
	}
}

void CSearchAlgDlg::OnBnClickedSetsearchparam()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_findProb <= 0 || m_findProb >= 1){//�������ָ��ʵ���0��1��ֻ�ܽӽ�
		MessageBox(L"���棺���ָ����������");
		m_searchParamFlag = false;
		return;
	}
	else{
		m_searchWidth = GetSearchWidth(m_findProb);
		//����С�����һλ��Ч���֣���д���������
		m_searchWidth = floor(m_searchWidth*100.0f + 0.5) / 100.0f;//����С�������λ
		UpdateData(FALSE);
		m_searchParamFlag = true;
	}
}


void CSearchAlgDlg::OnBnClickedBeginsearch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�жϲ����Ƿ�������ȷ
	if (false == m_missionAreaFlag ){
		MessageBox(L"���棺δ�趨����������");
		return;
	}
	if (false == m_searchParamFlag){
		MessageBox(L"���棺δ�趨������������");
		return;
	}

	Invalidate();
	UpdateWindow();
	m_leakSweep = "";
	m_doubleSweep = "";
	UpdateData(FALSE);
	
	//������ʱ������ʼ��ʾ��������
	SetTimer(ID_TIMER1, m_periodMS, NULL);
	GetDlgItem(IDC_BeginSearch)->EnableWindow(FALSE);
	GetDlgItem(IDC_fPResult)->EnableWindow(FALSE);
	GetDlgItem(IDC_SetMissionArea)->EnableWindow(FALSE);
	GetDlgItem(IDC_SetSearchParam)->EnableWindow(FALSE);


	//�鿴ѡ�е�����ģʽ������ʼ����
	if (0 == m_searchMode.GetCurSel()){//դ������
		//MessageBox(L"դ��");
		GetFPSetGrid();//��ȡդ��ת����
		//����©���ʺ͸����ʣ�ע��ԭ��ʽ��c�ǵ㼯�޳����һ���˳�����Լ���ʼ��ĺ������
		double W = m_searchWidth;
		double leakS = (1 - PI / 4)*pow((1 + W / 2), 2)*(m_fPSetScreen.size() - 2) / 2;
		double leakSweep = leakS / (m_axisX*m_axisY) * 100;
		double doubleSweep = (W*GetFLenGrid(m_fPSetReal) - m_axisX*(m_fPSetReal.back().y + W / 2) + leakS) / (m_axisX*m_axisY) * 100;

		m_leakSweep.Format(L"%.2f %%", leakSweep);
		m_doubleSweep.Format(L"%.2f %%", doubleSweep);
		
	}
	else if (1 == m_searchMode.GetCurSel()){//��������
		//MessageBox(L"����");
		GetFPSetSquare();//��ȡ����ת����

		double W = m_searchWidth;
		//���㺽�н���ʱѭ���ڵĸ���k
		/*int k = -1;
		if (m_axisX / (2 * W) + 0.5 == ceil(m_axisX / (2 * W))){
			k = int(floor(m_axisX / (2 * W)));
		}
		else if (m_axisX / (2 * W) + 0.5>ceil(m_axisX / (2 * W))){
			k = int(ceil(m_axisX / (2 * W)));
		}
		else{
			k = int(floor(m_axisX / (2 * W)));
		}*/
		int k = int(floor(0.5 + m_axisX / (2 * W)));

		//����©���ʺ͸����ʣ�ע��ԭ��ʽ��c�ǵ㼯�޳����һ���˳�����Լ���ʼ��ĺ������
		double leakS = (1 - PI / 4)*pow((1 + W / 2), 2)*(m_fPSetScreen.size() - 2) / 2;
		double leakSweep = leakS/ (m_axisX*m_axisY) * 100;

		double doubleSweep = (W*GetFLenGrid(m_fPSetReal) - (2*k*W+W)*(2*k*W) + leakS) / (m_axisX*m_axisY) * 100;

		m_leakSweep.Format(L"%.2f %%", leakSweep);
		m_doubleSweep.Format(L"%.2f %%", doubleSweep);
	}
	else{
		MessageBox(L"���棺δѡ������ģʽ��");
	}
	//if (m_fPSetScreen.size() == 0)//�����û���趨ģʽ����ֱ���˳�
	//	return;

}


void CSearchAlgDlg::OnBnClickedfpresult()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (m_fPSetScreen.size() == 0)//�����û���趨ģʽ����ֱ���˳�
		return;
	//��ʾ����Ի���
	m_fpResDlg = new CfpResultDlg;
	m_fpResDlg->Create(IDD_FPResult, this);
	m_fpResDlg->ShowWindow(SW_SHOW);
	//�ڸöԻ���ı༭���м�����صĺ�����Ϣ
	CString str;
	for (size_t i = 0; i < m_fPSetReal.size(); i++){
		if (i == 0){//�������ʼ��
			str.Format(L"��ʼ�� : (%.1f,%.1f) \r\n", m_fPSetReal[i].x, m_fPSetReal[i].y);
			m_fpResDlg->m_fPResult.SetSel(m_fpResDlg->m_fPResult.GetWindowTextLengthW(), m_fpResDlg->m_fPResult.GetWindowTextLengthW());//ѡ�����һ��λ��
			m_fpResDlg->m_fPResult.ReplaceSel(str);//�����ı��滻���һ��λ��
			continue;
		}
		if (i == m_fPSetReal.size() - 1){//���㵽������������
			str.Format(L"������ : (%.1f,%.1f)", m_fPSetReal[i].x, m_fPSetReal[i].y);
			m_fpResDlg->m_fPResult.SetSel(m_fpResDlg->m_fPResult.GetWindowTextLengthW(), m_fpResDlg->m_fPResult.GetWindowTextLengthW());//ѡ�����һ��λ��
			m_fpResDlg->m_fPResult.ReplaceSel(str);//�����ı��滻���һ��λ��
			break;
		}
		str.Format(L"P%d : (%.1f,%.1f) \r\n", i, m_fPSetReal[i].x, m_fPSetReal[i].y);
		m_fpResDlg->m_fPResult.SetSel(m_fpResDlg->m_fPResult.GetWindowTextLengthW(), m_fpResDlg->m_fPResult.GetWindowTextLengthW());//ѡ�����һ��λ��
		m_fpResDlg->m_fPResult.ReplaceSel(str);//�����ı��滻���һ��λ��
	}

	//����������Ļ��
	CWnd *pWnd = GetDlgItem(IDC_MainArea);
	CDC *pDC = pWnd->GetDC();
	CPen* pPenBlue = new CPen();
	pPenBlue->CreatePen(PS_SOLID, 3, RGB(0, 0, 255));//��ɫ����
	CGdiObject* pOldPen = pDC->SelectObject(pPenBlue);
	for (size_t i = 0; i < m_fPSetScreen.size(); i++){
		pDC->Ellipse(m_fPSetScreen[i].x - 1, m_fPSetScreen[i].y - 1, m_fPSetScreen[i].x + 1, m_fPSetScreen[i].y + 1);
	}
	//��������������
	CPen *pPenGreen = new CPen();
	pPenGreen->CreatePen(PS_SOLID, 2, RGB(80, 126, 50));
	pDC->SelectObject(pPenGreen);
	for (size_t i = 0; i < m_fPSetScreen.size()-1; i++){
		pDC->MoveTo(m_fPSetScreen[i].x, m_fPSetScreen[i].y);
		pDC->LineTo(m_fPSetScreen[i+1].x, m_fPSetScreen[i+1].y);
	}

	pDC->SelectObject(pOldPen);
	delete pPenBlue;
	ReleaseDC(pDC);
}


void CSearchAlgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == ID_TIMER1){
		
		//����
		CWnd *pWnd = GetDlgItem(IDC_MainArea);
		CDC *pDC = pWnd->GetDC();	

		if (m_currPt == CPoint(0, 0))//��ǰ��Ϊԭ��
			m_currPt = m_fPSetScreen[0];	

		if (m_nFPts == m_fPSetScreen.size() - 1 ){//�����ֹ�����ͽ�ֹ�����趨��ʾ���������ť����
			GetDlgItem(IDC_fPResult)->EnableWindow(TRUE);
			GetDlgItem(IDC_BeginSearch)->EnableWindow(TRUE);
			GetDlgItem(IDC_SetMissionArea)->EnableWindow(TRUE);
			GetDlgItem(IDC_SetSearchParam)->EnableWindow(TRUE);
			KillTimer(ID_TIMER1);
			m_nFPts = 0;
			m_currPt = CPoint(0, 0);//���÷�����ز���
			UpdateData(FALSE);//��©���ʺ͸����ʽ����ʾ
			return;
		}

		//����GDI+��Graphics��ע���ȡhdc�ķ���
		Gdiplus::Graphics myGraphics(pDC->m_hDC);
		//ʹ��GDI+��������һ��͸���ȵı�ˢ��һ��͸���ȵĻ���
		//SolidBrush myBrush(Gdiplus::Color(m_routeAlpha, 237, 125, 49));//��͸����ǳɫ��������ˢ
		//Gdiplus::Pen myPen(Gdiplus::Color(m_routeAlpha, 237, 125, 49), 3);//��͸��ǳɫ����
		SolidBrush myBrush(Gdiplus::Color(m_routeAlpha, 255, 255, 255));//��͸����ǳɫ��������ˢ
		Gdiplus::Pen myPen(Gdiplus::Color(m_routeAlpha, 255, 255, 255), 3);//��͸��ǳɫ����

		pDC->MoveTo(m_currPt.x, m_currPt.y);

		//���������뾶ͶӰ����Ļ�ϵ����
		int screenRX = int(ceil(m_searchWidth/2  * m_ratioX));
		int screenRY = int(ceil(m_searchWidth/2  * m_ratioY));

		//---------------������һ������·���λ��-----------------
		if (m_fPSetScreen[m_nFPts].x < m_fPSetScreen[m_nFPts+1].x){//���Ҳ���
			if (m_currPt.x + m_stepPixel <= m_fPSetScreen[m_nFPts + 1].x)//û�г�����Χ�����Ҳ���
				m_currPt.x = m_currPt.x + m_stepPixel;
			else{//������ǰ�߷�Χ
				m_nFPts++;//��ת����һ���ߣ�����ֵΪ���߶˵�
				m_currPt.x = m_fPSetScreen[m_nFPts].x;
				m_currPt.y = m_fPSetScreen[m_nFPts].y;
				
				myGraphics.DrawEllipse(&myPen, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);
				myGraphics.FillEllipse(&myBrush, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);
				
				return;
			}
		}
		else if (m_fPSetScreen[m_nFPts].x > m_fPSetScreen[m_nFPts+1].x){//���󲽽� 
			if (m_currPt.x - m_stepPixel >= m_fPSetScreen[m_nFPts + 1].x)//û�г�����Χ�����󲽽�
				m_currPt.x = m_currPt.x - m_stepPixel;
			else{//������ǰ�߷�Χ
				m_nFPts++;//��ת����һ���ߣ�����ֵΪ���߶˵�
				m_currPt.x = m_fPSetScreen[m_nFPts].x;
				m_currPt.y = m_fPSetScreen[m_nFPts].y;

				myGraphics.DrawEllipse(&myPen, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);
				myGraphics.FillEllipse(&myBrush, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);
				return;
			}
		}

		if (m_fPSetScreen[m_nFPts].y < m_fPSetScreen[m_nFPts+1].y){//���²���
			if (m_currPt.y + m_stepPixel <= m_fPSetScreen[m_nFPts + 1].y)//û�г�����Χ�����²���
				m_currPt.y = m_currPt.y + m_stepPixel;
			else{//������ǰ�߷�Χ
				m_nFPts++;//��ת����һ���ߣ�����ֵΪ���߶˵�
				m_currPt.x = m_fPSetScreen[m_nFPts].x;
				m_currPt.y = m_fPSetScreen[m_nFPts].y;

				myGraphics.DrawEllipse(&myPen, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);
				myGraphics.FillEllipse(&myBrush, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);

				return;
			}
		}
		else if (m_fPSetScreen[m_nFPts].y > m_fPSetScreen[m_nFPts+1].y){//���ϲ��� 
			if (m_currPt.y - m_stepPixel >= m_fPSetScreen[m_nFPts + 1].y)//û�г�����Χ�����ϲ���
				m_currPt.y = m_currPt.y - m_stepPixel;
			else{//������ǰ�߷�Χ
				m_nFPts++;//��ת����һ���ߣ�����ֵΪ���߶˵�
				m_currPt.x = m_fPSetScreen[m_nFPts].x;
				m_currPt.y = m_fPSetScreen[m_nFPts].y;

				myGraphics.DrawEllipse(&myPen, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);
				myGraphics.FillEllipse(&myBrush, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);

				return;
			}
		}

		if (m_fPSetScreen[m_nFPts].x == m_fPSetScreen[m_nFPts + 1].x && m_fPSetScreen[m_nFPts].y == m_fPSetScreen[m_nFPts + 1].y){
			m_nFPts++;//������һ��
		}
		
		myGraphics.DrawEllipse(&myPen, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);
		myGraphics.FillEllipse(&myBrush, m_currPt.x - screenRX, m_currPt.y - screenRY, 2 * screenRX, 2 * screenRY);
		
	
		ReleaseDC(pDC);
	}
	else{
		//������������δԤ���Timer
	}
	CDialogEx::OnTimer(nIDEvent);
}

//�趨�˶���Ч������
void CSearchAlgDlg::OnSetanime()
{
	// TODO:  �ڴ���������������
	CDialogAnime DlgAnime;
	DlgAnime.m_periodMS = m_periodMS;
	DlgAnime.m_pixelStep = m_stepPixel;
	DlgAnime.m_routeAlpha = m_routeAlpha;

	if (IDOK == DlgAnime.DoModal()){//������ok��
		m_stepPixel = DlgAnime.m_pixelStep;
		m_periodMS = DlgAnime.m_periodMS;
		m_routeAlpha = DlgAnime.m_routeAlpha;
	}

}


void CSearchAlgDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rect_ctr;
	//��ȡPicture�ؼ������Ļ���Ͻǵ����꣬/�洢��rect_ctr��
	GetDlgItem(IDC_MainArea)->GetWindowRect(&rect_ctr);
	CDC *pDC = GetDlgItem(IDC_MainArea)->GetDC();

	ScreenToClient(rect_ctr); //��ȡPicture�ؼ���ԶԻ���ͻ������Ͻǵ�����
	point.x -= rect_ctr.left; //point��ȡ���������ԶԻ���ͻ������Ͻǵ����꣬��ȥrect_ctr.left��
	point.y -= rect_ctr.top; //rect_ctr.top�󣬼�Ϊ������Picture�ؼ����Ͻǵ�����
	
	CString str;
	//������λ��Picture�ؼ�֮�⣬�ڶԻ����ϲ���ʾ����
	if (point.x < 0 || point.y<0 || point.x + rect_ctr.left>rect_ctr.right || point.y + rect_ctr.top>rect_ctr.bottom || m_ratioX==0 || m_ratioY == 0){
		;
	}
	else{
		double xReal, yReal;
		if (m_searchMode.GetCurSel() == 0){//դ��ģʽ
			xReal = (point.x - m_xPadding) / m_ratioX;
			yReal = (point.y - m_yPadding) / m_ratioY;
		}
		else{//����ģʽ
			CRect mainAreaRect;
			GetDlgItem(IDC_MainArea)->GetClientRect(mainAreaRect);
			int centerX = int((mainAreaRect.Width() - 2 * m_xPadding) / 2 + m_xPadding);//��������x
			int centerY = int((mainAreaRect.Height() - 2 * m_yPadding) / 2 + m_yPadding);//��������y
			xReal = (point.x - centerX) / m_ratioX;
			yReal = (point.y - centerY) / m_ratioY;
		}
		//���Ͻ�λ����ʾx�����y����
		str.Format(L"X: %5.1f  Y: %5.1f  ", xReal, yReal);
		pDC->SetBkColor(MAINAREA_BG);
		pDC->SetTextColor(RGB(255,255,255));
		pDC->TextOutW(0, 0, str);
	}
	

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CSearchAlgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (GetDlgItem(IDC_MainArea)->GetSafeHwnd() == pMsg->hwnd && pMsg->message == WM_MOUSEMOVE)
	{
		OnMouseMove(0, pMsg->pt);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



