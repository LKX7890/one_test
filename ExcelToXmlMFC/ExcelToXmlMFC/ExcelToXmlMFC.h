
// ExcelToXmlMFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CExcelToXmlMFCApp: 
// �йش����ʵ�֣������ ExcelToXmlMFC.cpp
//

class CExcelToXmlMFCApp : public CWinApp
{
public:
	CExcelToXmlMFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CExcelToXmlMFCApp theApp;