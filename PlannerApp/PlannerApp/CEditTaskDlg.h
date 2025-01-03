#pragma once
#include <afxdialogex.h>
#include "afxcmn.h"
#include "afxdisp.h"
#include "afxwin.h"
#include <afxdtctl.h>
#include <afxbutton.h>
#include "afxdb.h"
#include <atlimage.h>
#include <gdiplus.h>
using namespace Gdiplus;

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

private:
	ULONG_PTR m_gdiplusToken;
	CImage m_bgImage;
	CBrush m_bgBrush;
	CFont m_labelFont;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDc, CWnd* pWnd, UINT nCtlColor);

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

