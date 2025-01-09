#include "pch.h"
#include "CEditTaskDlg.h"
#include "resource.h"
#include "PlannerAppDlg.h"


IMPLEMENT_DYNAMIC(CEditTaskDlg, CDialogEx)

CEditTaskDlg::CEditTaskDlg(CWnd* pArent) : CDialogEx(IDD_EDIT_TASK_DIALOG, pArent){}

CEditTaskDlg::~CEditTaskDlg() {
	GdiplusShutdown(m_gdiplusToken);
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
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CEditTaskDlg::OnBtnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CEditTaskDlg::OnBtnClickedCancel)
END_MESSAGE_MAP()

HBRUSH CEditTaskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CEditTaskDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	CDateTimeCtrl* pDateTimeCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_EDIT_DUEDATE);

	if (pDateTimeCtrl != nullptr) {
		pDateTimeCtrl->ModifyStyle(0, DTS_SHOWNONE);
		pDateTimeCtrl->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	}
	else {
		AfxMessageBox(_T("DateTime picker control not found"));
	}
	
	m_labelFont.CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));

	GetDlgItem(IDC_TITLE_LABEL)->SetFont(&m_labelFont);
	GetDlgItem(IDC_CATEGORY_LABEL)->SetFont(&m_labelFont);
	GetDlgItem(IDC_DUEDATE_LABEL)->SetFont(&m_labelFont);
	GetDlgItem(IDC_DESCRIPTION_LABEL)->SetFont(&m_labelFont);


	GdiplusStartupInput gdiplusStartInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartInput, nullptr);



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
	CString updatedTitle, updatedCategory, updatedDescription;
	COleDateTime updatedDueDate;

	GetDlgItemText(IDC_EDIT_TITLE, updatedTitle);
	GetDlgItemText(IDC_EDIT_CATEGORY, updatedCategory);
	GetDlgItemText(IDC_EDIT_DESCRIPTION, updatedDescription);

	CDateTimeCtrl* pDateCtrl = (CDateTimeCtrl*)GetDlgItem(IDC_EDIT_DUEDATE);
	
	pDateCtrl->GetTime(updatedDueDate);

	if (updatedTitle.IsEmpty())
	{
		AfxMessageBox(_T("Title cannot be empty"));
		return;
	}

	CString dueDateString;
	if (updatedDueDate.GetStatus() == COleDateTime::valid) {
		dueDateString = updatedDueDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	else {
		dueDateString = _T("NULL");
	}

	try
	{
		if (!m_db.IsOpen()) {
			m_db.OpenEx(_T("DSN=PlannerDSN"), CDatabase::noOdbcDialog);
		}

		m_db.BeginTrans();

		CString updateQuery;
		updateQuery.Format(_T("update Tasks set Title = '%s', Category = '%s', DueDate = '%s', Description = '%s' WHERE TaskId = %s"), updatedTitle, updatedCategory, dueDateString, updatedDescription, m_TaskId);

		m_db.ExecuteSQL(updateQuery);
		m_db.CommitTrans();

		AfxMessageBox(_T("Task updated successfully"));
		CDialogEx::OnOK();
	}
	catch (CDBException* e)
	{
		m_db.Rollback();
		AfxMessageBox(e->m_strError);
		e->Delete();
	}
}

void CEditTaskDlg::OnPaint() {
	CPaintDC dc(this);
	
	CRect rect;
	GetClientRect(&rect);

	Gdiplus::Graphics graphis(dc.m_hDC);
	Gdiplus::Image image(L"res\\add_task.jpg");

	/*int imgWidth = m_bgImage.GetWidth();
	int imgHeight = m_bgImage.GetHeight();

	float xScale = static_cast<float>(rect.Width()) / imgWidth;
	float yScale = static_cast<float>(rect.Height()) / imgHeight;
	float scale = min(xScale, yScale);

	int newWidth = static_cast<int>(imgWidth * scale);
	int newHeight = static_cast<int>(imgHeight * scale);

	int xOffset = (rect.Width() - newWidth) / 2;
	int yOffset = (rect.Height() - newHeight) / 2;*/


	graphis.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	graphis.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
	
}

void CEditTaskDlg::OnBtnClickedCancel() {
	EndDialog(IDCANCEL);
}