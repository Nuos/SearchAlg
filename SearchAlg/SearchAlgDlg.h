
// SearchAlgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>
#include "MyPoint.h"
#include "CfpResultDlg.h"
#include "gdiplus.h"//����GDI+ͷ�ļ�

using namespace Gdiplus;//ʹ��GDI+�����ռ�
using namespace std;
#pragma comment(lib, "gdiplus.lib")

#define ID_TIMER1 1//���ö�ʱ��ID

// CSearchAlgDlg �Ի���
class CSearchAlgDlg : public CDialogEx
{
// ����
public:
	CSearchAlgDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSearchAlgDlg();//���Լ���ӵ�����������ԭ����û��

// �Ի�������
	enum { IDD = IDD_SEARCHALG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout();
	afx_msg void OnBnClickedSetmissionarea();
	afx_msg void OnBnClickedSetsearchparam();
	afx_msg void OnBnClickedBeginsearch();
private:
	//������ʼ��
	double m_LongtitudeStart;
	//��γ��ʼ��
	double m_LatitudeStart;
	//����������
	double m_longtitudeEnd;
	//��γ������
	double m_LatitudeEnd;
private:
	//��������Ĵ�С����λkm
	double m_axisX;
	double m_axisY;
private:
	//��ʵ��������Ļӳ��ı�ȳ�
	double m_ratioX;
	double m_ratioY;
	int m_xPadding;
	int m_yPadding;

private:
	// ���ָ���
	double m_findProb;
	// �������
	double m_searchWidth;
	double m_flightLength;//�����ܳ���
private:
	double GetSearchWidth(double findProb);//ͨ�����ָ��ʻ�ȡ�������
	CComboBox m_searchMode;
	CfpResultDlg *m_fpResDlg;
private:
	vector<CPoint> m_fPSetScreen;//���㼯�ϣ������Ļ����ϵ��
	vector<MyPoint> m_fPSetReal;//���㼯�ϣ������ʵ����ϵ��
	void GetFPSetGrid();//դ������
	void GetFPSetSquare();//��������
	double GetFLenGrid(const vector<MyPoint> &fPSetReal);//��ȡդ�����������ܳ���

	// X���������
	UINT m_xDivNum;
	// Y���������
	UINT m_yDivNum;
	// ©����
	CString m_leakSweep;
	// ������
	CString m_doubleSweep;
	int m_nFPts;//��¼��ǰ������һ����
	CPoint m_currPt;//��¼�ɻ����ڵ�����ֵ
	int m_stepPixel;//ÿ��һ����ʱ��������������ص���
	int m_periodMS;//������������(ms)
	int m_routeAlpha;//����͸���ȣ�0-255
	COLORREF m_areaColor;//������ɫ
	
	bool m_missionAreaFlag;//�Ƿ�ɹ�������������
	bool m_searchParamFlag;//�Ƿ�ɹ��趨��������

public:
	afx_msg void OnBnClickedfpresult();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSetanime();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
