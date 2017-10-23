
// ExcelToXmlMFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ExcelToXmlMFC.h"
#include "ExcelToXmlMFCDlg.h"
#include "afxdialogex.h"
#include "ExcelConverToXml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExcelToXmlMFCDlg 对话框



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

	//临时变量，用以记录返回的树节点
	HTREEITEM tree_Temp;

	//判断输入目录最后是否存在'\'，不存在则补充
	if (str_Dir.Right(1) != "\\")
		str_Dir += "\\";
	str_Dir += "*.*";
	BOOL res = FileFind.FindFile(str_Dir);
	while (res)
	{
		tree_Temp = tree_Root;			// 控件根节点
		res = FileFind.FindNextFile();
		if (FileFind.IsDirectory() && !FileFind.IsDots())//目录是文件夹
		{
			CString strPath = FileFind.GetFilePath(); //得到路径，做为递归调用的开始
			CString strTitle = FileFind.GetFileName();//得到目录名，做为树控的结点
			tree_Temp = m_mytree.InsertItem(strTitle, 0, 0, tree_Root);
			ShowFile(strPath, tree_Temp);	// 递归显示
		}
		else if (!FileFind.IsDirectory() && !FileFind.IsDots())//如果是文件
		{
			CString strPath = FileFind.GetFilePath(); //得到路径，做为递归调用的开始
			CString strTitle = FileFind.GetFileName();//得到文件名，做为树控的结点
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
		if (hParentItem == NULL)//没有父节点  
		{
			ItemPath = m_mytree.GetItemText(hItem);
			ItemPath = ItemPath + TemPath;

			break;
		}
		else//有父节点  
		{
			TemPath = _T("\\") + m_mytree.GetItemText(hItem) + TemPath;
			hItem = m_mytree.GetParentItem(hItem);
		}
	}
	return ItemPath;
}

void CExcelToXmlMFCDlg::GetLogicalDrives(HTREEITEM hParent)
{
	//获取系统分区驱动器字符串信息  
	size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //驱动器总长度  
	char *pDriveStrings = new char[szAllDriveStrings + sizeof(_T(""))];  //建立数组  
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = strlen(pDriveStrings);                        //驱动大小  
	while (szDriveString > 0)
	{
		m_mytree.InsertItem(pDriveStrings, hParent);       //在父节点hParent下添加盘符  
		pDriveStrings += szDriveString + 1;             //pDriveStrings即C:\ D:\ E:\盘  
		szDriveString = strlen(pDriveStrings);
	}
}

void CExcelToXmlMFCDlg::GetDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_mytree.GetChildItem(hParent);   //获取指定位置中的子项  
	while (hChild)
	{
		CString strText = m_mytree.GetItemText(hChild);  //检索列表中项目文字  
		if (strText.Right(1) != "\\")                   //从右边1开始获取从右向左nCount个字符  
			strText += _T("\\");
		strText += "*.*";
		//将当前目录下文件枚举并InsertItem树状显示  
		CFileFind file;                                       //定义本地文件查找  
		BOOL bContinue = file.FindFile(strText);              //查找包含字符串的文件  
		while (bContinue)
		{
			bContinue = file.FindNextFile();                  //查找下一个文件  
			if (file.IsDirectory() && !file.IsDots())          //找到文件为内容且不为点"."  
				m_mytree.InsertItem(file.GetFileName(), hChild); //添加盘符路径下树状文件夹  
		}
		GetDriveDir(hChild);                                  //递归调用  
		hChild = m_mytree.GetNextItem(hChild, TVGN_NEXT);        //获取树形控件TVGN_NEXT下兄弟项  
	}
}

BEGIN_MESSAGE_MAP(CExcelToXmlMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SURE, &CExcelToXmlMFCDlg::OnBnClickedSure)
	ON_BN_CLICKED(IDC_CONVER, &CExcelToXmlMFCDlg::OnBnClickedConver)
END_MESSAGE_MAP()


// CExcelToXmlMFCDlg 消息处理程序

BOOL CExcelToXmlMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// todo lkx
	//初始化树形控件
	HTREEITEM tree_Root;

	//控件中插入根节点
	tree_Root = m_mytree.InsertItem("我的电脑");

	//自定义函数 获取驱动驱动子项
	this->GetLogicalDrives(tree_Root);                      
	GetDriveDir(tree_Root);                         
	ShowFile("", tree_Root);//以E:\\test为根目录进行遍历

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExcelToXmlMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
