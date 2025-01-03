#include "pch.h"
#include "CEditTaskDlg.h"
#include "resource.h"
#include "PlannerAppDlg.h"



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
	m_Category.AddString(_T("Other"));
	m_Category.AddString(_T("Personal"));
	m_Category.AddString(_T("Work"));
	m_Category.SelectString(-1, m_taskCategory);
	m_dueDate.SetTime(m_taskDueDate);
	m_Description.SetWindowText(m_taskDescription);

	return TRUE;
}

void CEditTaskDlg::SetTaskData(const CString& id,const CString& title, const CString& category, const COleDateTime& dueDate, const CString description) {
	m_TaskId = id;
	m_taskTitle = title;
	m_taskCategory = category;
	m_taskDueDate = dueDate;
	m_taskDescription = description;
}

void CEditTaskDlg::OnBtnClickedSave() {
	GetDlgItemText(IDC_EDIT_TASK, m_taskTitle);
	GetDlgItemText(IDC_EDIT_CATEGORY, m_taskCategory);
	GetDlgItemText(IDC_EDIT_DESCRIPTION, m_taskDescription);

	CDateTimeCtrl* pDateTimeCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_EDIT_DUEDATE);
	if (pDateTimeCtrl) {
		pDateTimeCtrl->GetTime(m_taskDueDate);
	}
	try
	{
		if (!m_db.IsOpen()) {
			CPlannerAppDlg* pParent = dynamic_cast<CPlannerAppDlg*>(GetParent());
			if (pParent) {
				pParent->ConnectToDatabase();
			}
		}

		CString query;
		query.Format(_T("update Tasks set Title = '%s', Category = '%s', DueDate = '%s', Description = '%s' where TaskId = '%s'"), m_taskTitle, m_taskCategory, m_taskDueDate.Format(_T("%d-%m-%Y %H:%M:%S")), m_taskDescription, m_TaskId);
		m_db.ExecuteSQL(query);

		EndDialog(IDOK);
	}
	catch (CDBException* ex)
	{
		AfxMessageBox(ex->m_strError);
		ex->Delete();
	}
}

void CEditTaskDlg::OnBtnClickedCancel() {
	EndDialog(IDCANCEL);
}