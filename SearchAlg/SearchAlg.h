
// SearchAlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "gdiplus.h"//����GDI+ͷ�ļ�

using namespace Gdiplus;//ʹ��GDI+�����ռ�
#pragma comment(lib, "gdiplus.lib")

// CSearchAlgApp: 
// �йش����ʵ�֣������ SearchAlg.cpp
//

class CSearchAlgApp : public CWinApp
{
public:
	CSearchAlgApp();//���캯��

// ��д
public:
	virtual BOOL InitInstance();
	ULONG_PTR m_gdiplusToken;//����һ��GDI+����
	GdiplusStartupInput m_gdiplusStartupInput;

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CSearchAlgApp theApp;