
// PlannerAppDlg.h : header file
//

#include <vector>
#include "Task.h"
#include "afxbutton.h"
#include "CAddTaskDlg.h"
#include <shellapi.h>
#include "afxdb.h"
#include <gdiplus.h>
using namespace Gdiplus;

#pragma once

#define TIMER_CHECK_NOTIFICATIONS 1
#define WM_USER_REFRESH_TASKS (WM_USER+1)

// CPlannerAppDlg dialog
class CPlannerAppDlg : public CDialogEx
{
// Construction
public:
	CPlannerAppDlg(CWnd* pParent = nullptr);	// standard constructor
	afx_msg BOOL ConnectToDatabase();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLANNERAPP_DIALOG };
#endif

private:
	ULONG_PTR m_gdiplusToken;

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

	CImage m_bgImage;

	CDatabase m_db;

	CMFCListCtrl m_TaskListCtrl;
	

	void LoadTasksFromDatabase();

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRefreshTask(WPARAM wParam, LPARAM lParam);
	afx_msg void ShowNotification(const CString& message);
	afx_msg void OnBtnClickedAddTask();
	afx_msg void OnBtnClickedEditTask();
	afx_msg void OnBtnClickedDeleteTask();
	DECLARE_MESSAGE_MAP()
};
