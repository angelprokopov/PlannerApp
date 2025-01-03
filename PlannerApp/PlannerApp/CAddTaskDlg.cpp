#include "pch.h"
#include "PlannerAppDlg.h"
#include "CAddTaskDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "afxdb.h"
#include <gdiplus.h>
using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CAddTaskDlg, CDialogEx)

CAddTaskDlg::CAddTaskDlg(CWnd* pArrent) : CDialogEx(IDD_ADD_TASK_DIALOG, pArrent) {}

CAddTaskDlg::~CAddTaskDlg() {
	GdiplusShutdown(m_gdiplusToken);
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
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CAddTaskDlg::OnBtnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CAddTaskDlg::OnBtnClickedCancel)
END_MESSAGE_MAP()

BOOL CAddTaskDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	
	m_labelFont.CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));

	GetDlgItem(IDC_STATIC_TITLE)->SetFont(&m_labelFont);
	GetDlgItem(IDC_STATIC_CATEGORY)->SetFont(&m_labelFont);
	GetDlgItem(IDC_STATIC_DUEDATE)->SetFont(&m_labelFont);
	GetDlgItem(IDC_STATIC_DESC)->SetFont(&m_labelFont);

	GdiplusStartupInput gdiplusStartInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartInput, nullptr);

	m_bgBrush.CreateSolidBrush(RGB(255, 255, 255));

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

HBRUSH CAddTaskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
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

void CAddTaskDlg::OnPaint() {
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	Gdiplus::Graphics graphis(dc.m_hDC);
	Gdiplus::Image image(L"res\\edit_task.jpg");

	/*int imgWidth = image.GetWidth();
	int imgHeight = image.GetHeight();

	float xScale = static_cast<float>(rect.Width()) / imgWidth;
	float yScale = static_cast<float>(rect.Height()) / imgHeight;
	float scale = min(xScale, yScale);

	int scaledWidth = static_cast<int>(imgWidth * scale);
	int scaledHeight = static_cast<int>(imgHeight * scale);
	
	int xOffset = (rect.Width() - scaledWidth) / 2;
	int yOffset = (rect.Height() - scaledHeight) / 2;*/

	graphis.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	graphis.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
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