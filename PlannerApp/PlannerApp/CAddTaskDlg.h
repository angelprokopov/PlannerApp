#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdb.h"
#include <atlimage.h>
#include <gdiplus.h>
using namespace Gdiplus;

class CAddTaskDlg : public CDialogEx {
	
	DECLARE_DYNAMIC(CAddTaskDlg);

public:
	CAddTaskDlg(CWnd* pArrent = nullptr);
	virtual ~CAddTaskDlg();

#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_ADD_TASK_DIALOG};
#endif // AFX_DESIGN_TIME

private:
	ULONG_PTR m_gdiplusToken;
	CImage m_bgImage;
	CBrush m_bgBrush;
	CFont m_labelFont;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDc, CWnd* pWnd, UINT nCtlColor);

	CDatabase m_db;

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

