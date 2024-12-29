#include "pch.h"
#include "CEditTaskDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CEditTaskDlg, CDialogEx)

CEditTaskDlg::CEditTaskDlg(CWnd* pArent) : CDialogEx(IDD_EDIT_TASK_DIALOG, pArent){}

CEditTaskDlg::~CEditTaskDlg() {

}

void CEditTaskDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_TITLE, m_Title);
	DDX_Control(pDX, IDC_EDIT_CATEGORY, m_Category);
	DDX_Control(pDX, IDC_EDIT_DUEDATE, m_dueDate);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_Description);
	DDX_Control(pDX, IDOK, m_saveButton);
	DDX_Control(pDX, IDCANCEL, m_cancelButton);
}

BEGIN_MESSAGE_MAP(CEditTaskDlg,CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditTaskDlg::OnBtnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CEditTaskDlg::OnBtnClickedCancel)
END_MESSAGE_MAP()

BOOL CEditTaskDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	m_Title.SetWindowText(m_taskTitle);

	return TRUE;
}

void CEditTaskDlg::SetTaskData(const CString& title, const CString& category, const COleDateTime& dueDate, const CString description) {
	m_taskTitle = title;
	m_taskCategory = category;
	m_taskDueDate = dueDate;
	m_taskDescription = description;
}

void CEditTaskDlg::GetTaskData(CString& title, CString& category, CString& description) const {
	title = m_taskTitle;
	category = m_taskCategory;
	description = m_taskDescription;
}

void CEditTaskDlg::OnBtnClickedSave() {

}

void CEditTaskDlg::OnBtnClickedCancel() {
	EndDialog(IDCANCEL);
}