#include "pch.h"
#include "CAddTaskDlg.h"
#include "afxdialogex.h"
#include "resource.h"

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
	
	m_TaskCategory.AddString(_T("Personal"));
	m_TaskCategory.AddString(_T("Work"));
	m_TaskCategory.AddString(_T("Other"));
	m_TaskCategory.SetCurSel(0);

	m_SaveButton.SetFaceColor(RGB(0, 120, 215));
	m_CancelButton.SetFaceColor(RGB(255, 69, 0));
	return TRUE;
}

void CAddTaskDlg::OnBtnClickedSave() {

}

void CAddTaskDlg::OnBtnClickedCancel() {
	EndDialog(IDCANCEL);
}