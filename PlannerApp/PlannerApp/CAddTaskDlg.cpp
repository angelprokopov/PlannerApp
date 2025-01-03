#include "pch.h"
#include "PlannerAppDlg.h"
#include "CAddTaskDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "afxdb.h"

IMPLEMENT_DYNAMIC(CAddTaskDlg, CDialogEx)

CAddTaskDlg::CAddTaskDlg(CWnd* pArrent) : CDialogEx(IDD_ADD_TASK_DIALOG, pArrent) {}

CAddTaskDlg::~CAddTaskDlg() {

}

void CAddTaskDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TASK_TITLE, m_TaskTitle);
	DDX_Control(pDX, IDC_TASK_CATEGORY, m_TaskCategory);
	DDX_Control(pDX, IDC_TASK_DUEDATE, m_TaskDueDate);
	DDX_Control(pDX, IDC_TASK_DESCRIPTION, m_TaskDescription);
	DDX_Control(pDX, IDOK, m_SaveButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

BEGIN_MESSAGE_MAP(CAddTaskDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddTaskDlg::OnBtnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CAddTaskDlg::OnBtnClickedCancel)
END_MESSAGE_MAP()

BOOL CAddTaskDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	
	try
	{
		m_db.OpenEx(_T("DSN=PlannerDSN"), CDatabase::noOdbcDialog);
	}
	catch (CDBException* ex)
	{
		AfxMessageBox(ex->m_strError);
		ex->Delete();
	}


	m_TaskCategory.AddString(_T("Personal"));
	m_TaskCategory.AddString(_T("Work"));
	m_TaskCategory.AddString(_T("Other"));
	m_TaskCategory.SetCurSel(0);

	m_SaveButton.SetFaceColor(RGB(0, 120, 215));
	m_CancelButton.SetFaceColor(RGB(255, 69, 0));
	return TRUE;
}

void CAddTaskDlg::OnBtnClickedSave() {
	CString title, category, description;
	COleDateTime dueDate;

	m_TaskTitle.GetWindowText(title);
	m_TaskCategory.GetWindowText(category);
	m_TaskDueDate.GetTime(dueDate);
	m_TaskDescription.GetWindowText(description);

	if (title.IsEmpty()) {
		AfxMessageBox(_T("Title cannot be empty"));
		return;
	}
	
	try
	{
		CString query;
		query.Format(_T("insert into Tasks (Title, Category, DueDate, Description) values ('%s','%s','%s','%s');"),
			title,category,dueDate.Format(_T("%d-%m-%Y %H:%M:%S")), description);

		m_db.ExecuteSQL(query);

		CDialogEx* pParent = (CDialogEx*)GetParent();
		if (pParent != nullptr) {
			pParent->SendMessage(WM_USER_REFRESH_TASKS, 0, 0);
		}

		AfxMessageBox(_T("Task saved successfully"));
		EndDialog(IDOK);
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		e->Delete();
	}
}

void CAddTaskDlg::OnBtnClickedCancel() {
	EndDialog(IDCANCEL);
}

void CAddTaskDlg::OnDestroy() {
	if (m_db.IsOpen()) {
		m_db.Close();
	}

	CDialogEx::OnDestroy();
}