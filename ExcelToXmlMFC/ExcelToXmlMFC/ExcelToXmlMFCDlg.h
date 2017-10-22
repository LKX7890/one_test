
// ExcelToXmlMFCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CExcelToXmlMFCDlg �Ի���
class CExcelToXmlMFCDlg : public CDialogEx
{
// ����
public:
	CExcelToXmlMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EXCELTOXMLMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
// todo lkx
	void ShowFile(CString str_Dir, HTREEITEM tree_Root);
	CString GetSelectItemPath(HTREEITEM hItem);
	void GetLogicalDrives(HTREEITEM hParent);
	void GetDriveDir(HTREEITEM hParent);
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
