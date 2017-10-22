
// ExcelToXmlMFCDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CExcelToXmlMFCDlg 对话框
class CExcelToXmlMFCDlg : public CDialogEx
{
// 构造
public:
	CExcelToXmlMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EXCELTOXMLMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
// todo lkx
	void ShowFile(CString str_Dir, HTREEITEM tree_Root);
	CString GetSelectItemPath(HTREEITEM hItem);
	void GetLogicalDrives(HTREEITEM hParent);
	void GetDriveDir(HTREEITEM hParent);
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_mytree;
	CButton m_sure;
	CString m_excelpath;
	CString m_xmlpath;
	afx_msg void OnBnClickedSure();
	afx_msg void OnBnClickedConver();
private:
	CEdit m_showpath;
};
