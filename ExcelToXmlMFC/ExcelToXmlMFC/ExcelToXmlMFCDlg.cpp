
// ExcelToXmlMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExcelToXmlMFC.h"
#include "ExcelToXmlMFCDlg.h"
#include "afxdialogex.h"
#include "ExcelConverToXml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExcelToXmlMFCDlg �Ի���



CExcelToXmlMFCDlg::CExcelToXmlMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExcelToXmlMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExcelToXmlMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILETREE, m_mytree);
	DDX_Control(pDX, IDC_SURE, m_sure);
	DDX_Control(pDX, IDC_SHOWPATH, m_showpath);
}

void CExcelToXmlMFCDlg::ShowFile(CString str_Dir, HTREEITEM tree_Root)
{
	CFileFind FileFind;

	//��ʱ���������Լ�¼���ص����ڵ�
	HTREEITEM tree_Temp;

	//�ж�����Ŀ¼����Ƿ����'\'���������򲹳�
	if (str_Dir.Right(1) != "\\")
		str_Dir += "\\";
	str_Dir += "*.*";
	BOOL res = FileFind.FindFile(str_Dir);
	while (res)
	{
		tree_Temp = tree_Root;			// �ؼ����ڵ�
		res = FileFind.FindNextFile();
		if (FileFind.IsDirectory() && !FileFind.IsDots())//Ŀ¼���ļ���
		{
			CString strPath = FileFind.GetFilePath(); //�õ�·������Ϊ�ݹ���õĿ�ʼ
			CString strTitle = FileFind.GetFileName();//�õ�Ŀ¼������Ϊ���صĽ��
			tree_Temp = m_mytree.InsertItem(strTitle, 0, 0, tree_Root);
			ShowFile(strPath, tree_Temp);	// �ݹ���ʾ
		}
		else if (!FileFind.IsDirectory() && !FileFind.IsDots())//������ļ�
		{
			CString strPath = FileFind.GetFilePath(); //�õ�·������Ϊ�ݹ���õĿ�ʼ
			CString strTitle = FileFind.GetFileName();//�õ��ļ�������Ϊ���صĽ��
			m_mytree.InsertItem(strTitle, 0, 0, tree_Temp);
		}
	}
	FileFind.Close();
}

CString CExcelToXmlMFCDlg::GetSelectItemPath(HTREEITEM hItem)
{
	CString ItemPath;
	ItemPath.Empty();
	CString TemPath(_T(""));

	while (TRUE)
	{
		HTREEITEM hParentItem = m_mytree.GetParentItem(hItem);
		if (hParentItem == NULL)//û�и��ڵ�  
		{
			ItemPath = m_mytree.GetItemText(hItem);
			ItemPath = ItemPath + TemPath;

			break;
		}
		else//�и��ڵ�  
		{
			TemPath = _T("\\") + m_mytree.GetItemText(hItem) + TemPath;
			hItem = m_mytree.GetParentItem(hItem);
		}
	}
	return ItemPath;
}

void CExcelToXmlMFCDlg::GetLogicalDrives(HTREEITEM hParent)
{
	//��ȡϵͳ�����������ַ�����Ϣ  
	size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //�������ܳ���  
	char *pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];  //��������  
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = strlen(pDriveStrings);                        //������С  
	while (szDriveString > 0)
	{
		m_mytree.InsertItem(pDriveStrings, hParent);       //�ڸ��ڵ�hParent������̷�  
		pDriveStrings += szDriveString + 1;             //pDriveStrings��C:\ D:\ E:\��  
		szDriveString = strlen(pDriveStrings);
	}
}

void CExcelToXmlMFCDlg::GetDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_mytree.GetChildItem(hParent);   //��ȡָ��λ���е�����  
	while (hChild)
	{
		CString strText = m_mytree.GetItemText(hChild);  //�����б�����Ŀ����  
		if (strText.Right(1) != "\\")                   //���ұ�1��ʼ��ȡ��������nCount���ַ�  
			strText += _T("\\");
		strText += "*.*";
		//����ǰĿ¼���ļ�ö�ٲ�InsertItem��״��ʾ  
		CFileFind file;                                       //���屾���ļ�����  
		BOOL bContinue = file.FindFile(strText);              //���Ұ����ַ������ļ�  
		while (bContinue)
		{
			bContinue = file.FindNextFile();                  //������һ���ļ�  
			if (file.IsDirectory() && !file.IsDots())          //�ҵ��ļ�Ϊ�����Ҳ�Ϊ��"."  
				m_mytree.InsertItem(file.GetFileName(), hChild); //����̷�·������״�ļ���  
		}
		GetDriveDir(hChild);                                  //�ݹ����  
		hChild = m_mytree.GetNextItem(hChild, TVGN_NEXT);        //��ȡ���οؼ�TVGN_NEXT���ֵ���  
	}
}

BEGIN_MESSAGE_MAP(CExcelToXmlMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SURE, &CExcelToXmlMFCDlg::OnBnClickedSure)
	ON_BN_CLICKED(IDC_CONVER, &CExcelToXmlMFCDlg::OnBnClickedConver)
END_MESSAGE_MAP()


// CExcelToXmlMFCDlg ��Ϣ�������

BOOL CExcelToXmlMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// todo lkx
	//��ʼ�����οؼ�
	HTREEITEM tree_Root;

	//�ؼ��в�����ڵ�
	tree_Root = m_mytree.InsertItem("�ҵĵ���");

	//�Զ��庯�� ��ȡ������������
	this->GetLogicalDrives(tree_Root);                      
	GetDriveDir(tree_Root);                         
	ShowFile("", tree_Root);//��E:\\testΪ��Ŀ¼���б���

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExcelToXmlMFCDlg::OnPaint()
{
	if (IsIconic())
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
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CExcelToXmlMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CExcelToXmlMFCDlg::OnBnClickedSure()
{
	// todo lkx
	HTREEITEM select_item = m_mytree.GetSelectedItem();
	m_excelpath = GetSelectItemPath(select_item);
}


void CExcelToXmlMFCDlg::OnBnClickedConver()
{
	// todo lkx
	char *xmlpath = NULL;
	ExcelConverToXml exceltoxml;
	exceltoxml.Start(m_excelpath, xmlpath);
	//m_xmlpath = xmlpath;
	//m_showpath.GetWindowText(m_xmlpath);
	//UpdateData(false);
}
