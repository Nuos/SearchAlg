#pragma once


// CDialogAnime �Ի���

class CDialogAnime : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAnime)

public:
	CDialogAnime(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogAnime();

// �Ի�������
	enum { IDD = IDD_SetAnime };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��������
	int m_periodMS;
	// ���Ʋ���
	int m_pixelStep;
	afx_msg void OnBnClickedOk();
	// ·��͸���ȣ�0-255����ȫ͸-��͸
	int m_routeAlpha;
};
