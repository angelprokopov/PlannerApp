#pragma once
#include <afxdialogex.h>
#include "afxcmn.h"
#include "afxdisp.h"
#include "afxwin.h"
#include <afxdtctl.h>
#include <afxbutton.h>
#include "afxdb.h"

class CEditTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditTaskDlg)

public:
	CEditTaskDlg(CWnd* pArent = nullptr);
	virtual ~CEditTaskDlg();

	void SetTaskData(const CString& id,const CString& title, const CString& category, const COleDateTime& dueDate,const CString description);
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_EDIT_TASK_DIALOG};
#endif // AFX_DESIGN_TIME

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

	CString m_TaskId;
	CEdit m_Title;
	CComboBox m_Category;
	CDateTimeCtrl m_dueDate;
	CEdit m_Description;
	CMFCButton m_saveButton;
	CMFCButton m_cancelButton;
	CDatabase m_db;
	CString m_taskTitle;
	CString m_taskCategory;
	COleDateTime m_taskDueDate;
	CString m_taskDescription;

public:

	afx_msg void OnBtnClickedSave();
	afx_msg void OnBtnClickedCancel();
};

