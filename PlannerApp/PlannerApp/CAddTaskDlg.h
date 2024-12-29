#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CAddTaskDlg : public CDialogEx {
	
	DECLARE_DYNAMIC(CAddTaskDlg);

public:
	CAddTaskDlg(CWnd* pArrent = nullptr);
	virtual ~CAddTaskDlg();

#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_ADD_TASK_DIALOG};
#endif // AFX_DESIGN_TIME
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	CEdit m_TaskTitle;
	CComboBox m_TaskCategory;
	CDateTimeCtrl m_TaskDueDate;
	CEdit m_TaskDescription;
	CMFCButton m_SaveButton;
	CMFCButton m_CancelButton;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBtnClickedSave();
	afx_msg void OnBtnClickedCancel();
};

