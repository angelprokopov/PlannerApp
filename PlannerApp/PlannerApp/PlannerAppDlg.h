
// PlannerAppDlg.h : header file
//

#include <vector>
#include "Task.h"
#include "afxbutton.h"
#include "CAddTaskDlg.h"
#include <shellapi.h>

#pragma once

#define TIMER_CHECK_NOTIFICATIONS 1

// CPlannerAppDlg dialog
class CPlannerAppDlg : public CDialogEx
{
// Construction
public:
	CPlannerAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLANNERAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	std::vector<Task> taskList;
	NOTIFYICONDATA m_NotifyIconData;

	CMFCButton m_addButton;
	CMFCButton m_editButton;
	CMFCButton m_deleteButton;

	CImage m_AddImage;
	CImage m_EditImage;
	CImage m_DeleteImage;

	CMFCListCtrl m_TaskListCtrl;
	
	void UpdateTaskList();

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void ConnectToDatabase();
	afx_msg void ShowNotification(const CString& message);
	afx_msg void OnBtnClickedAddTask();
	afx_msg void OnBtnClickedEditTask();
	afx_msg void OnBtnClickedDeleteTask();
	DECLARE_MESSAGE_MAP()
};
