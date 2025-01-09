
// PlannerAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PlannerApp.h"
#include "PlannerAppDlg.h"
#include "afxdialogex.h"
#include "afxdb.h"
#include "afxvisualmanagerwindows.h"
#include "CEditTaskDlg.h"
#include <gdiplus.h>
using namespace Gdiplus;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	// CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPlannerAppDlg dialog

CPlannerAppDlg::~CPlannerAppDlg() {
	GdiplusShutdown(m_gdiplusToken);
}

CPlannerAppDlg::CPlannerAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLANNERAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_PLANNER_ICON);
}

void CPlannerAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_TASK, m_addButton);
	DDX_Control(pDX, IDC_EDIT_TASK, m_editButton);
	DDX_Control(pDX, IDC_DELETE_TASK, m_deleteButton);
	DDX_Control(pDX, IDC_TASK_LIST, m_TaskListCtrl);
}

BEGIN_MESSAGE_MAP(CPlannerAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ADD_TASK, &CPlannerAppDlg::OnBtnClickedAddTask)
	ON_BN_CLICKED(IDC_EDIT_TASK, &CPlannerAppDlg::OnBtnClickedEditTask)
	ON_BN_CLICKED(IDC_DELETE_TASK, &CPlannerAppDlg::OnBtnClickedDeleteTask)
END_MESSAGE_MAP()


// CPlannerAppDlg message handlers

BOOL CPlannerAppDlg::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CPlannerAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GdiplusStartupInput gdiplusStartInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartInput, nullptr);

	CRect rect;
	m_TaskListCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	/*m_addButton.GetWindowRect(&rect);
	rect.right = rect.left + 150;
	rect.bottom = rect.top + 70;
	m_addButton.MoveWindow(&rect);

	m_editButton.GetWindowRect(&rect);
	rect.right = rect.left + 150;
	rect.bottom = rect.top + 70;
	m_editButton.MoveWindow(&rect);

	m_deleteButton.GetWindowRect(&rect);
	rect.right = rect.left + 150;
	rect.bottom = rect.top + 70;
	m_deleteButton.MoveWindow(&rect);*/

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())	
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_TaskListCtrl.InsertColumn(0, _T("TaskId"), LVCFMT_LEFT, 60);
	m_TaskListCtrl.InsertColumn(1, _T("Title"), LVCFMT_LEFT, 150);
	m_TaskListCtrl.InsertColumn(2, _T("Category"), LVCFMT_LEFT, 100);
	m_TaskListCtrl.InsertColumn(3, _T("DueDate"), LVCFMT_LEFT, 120);
	m_TaskListCtrl.InsertColumn(4, _T("Description"), LVCFMT_LEFT, 200);

	m_TaskListCtrl.SetExtendedStyle(m_TaskListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	try
	{
		ConnectToDatabase();
		LoadTasksFromDatabase();
	}
	catch (const std::exception&)
	{
		
	}

	
	try
	{
		// Load images
		m_AddImage.Load(_T("res\\img\\add.png"));
		m_EditImage.Load(_T("res\\img\\edit.png"));
		m_DeleteImage.Load(_T("res\\img\\delete.png"));

		int buttonWidth = 160;
		int buttonHeight = 70;
		int buttonSpacing = 14;

		int xPos = rect.right + 10;
		int yPos = rect.top;

		m_addButton.m_bTransparent = TRUE;
		m_editButton.m_bTransparent = TRUE;
		m_deleteButton.m_bTransparent = TRUE;

		m_addButton.SetWindowPos(NULL, xPos, yPos, buttonWidth, buttonHeight, SWP_NOZORDER);
		m_addButton.SetImage(m_AddImage);	
		m_addButton.SetFaceColor(RGB(255, 255, 255),TRUE);
		m_addButton.SetTextColor(RGB(179, 89, 0));
		m_addButton.SetWindowText(_T("Add new task"));

		yPos += buttonHeight + buttonSpacing;

		m_editButton.SetWindowPos(NULL, xPos, yPos, buttonWidth, buttonHeight, SWP_NOZORDER);
		m_editButton.SetImage(m_EditImage);
		m_editButton.SetFaceColor(RGB(255, 255, 255),TRUE);
		m_editButton.SetTextColor(RGB(179, 89, 0));
		m_editButton.SetWindowText(_T("Edit task"));

		yPos += buttonHeight + buttonSpacing;

		m_deleteButton.SetWindowPos(NULL, xPos, yPos, buttonWidth, buttonHeight, SWP_NOZORDER);
		m_deleteButton.SetImage(m_DeleteImage);
		m_deleteButton.SetFaceColor(RGB(255, 255, 255),TRUE);
		m_deleteButton.SetTextColor(RGB(179, 89, 0));
		m_deleteButton.SetWindowText(_T("Delete task"));
	}
	catch (CFileException* ex)
	{
		AfxMessageBox(_T("Failed to load image from file"));
		ex->Delete();
		return FALSE;
	}

	memset(&m_notifyIconData, 0, sizeof(m_notifyIconData));
	m_notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	m_notifyIconData.hWnd = this->GetSafeHwnd();
	m_notifyIconData.uID = 1;
	m_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_notifyIconData.uCallbackMessage = WM_APP + 1;
	m_notifyIconData.hIcon = AfxGetApp()->LoadIconW(IDR_PLANNER_ICON);
	wcscpy_s(m_notifyIconData.szTip, L"Planner App");

	Shell_NotifyIcon(NIM_ADD, &m_notifyIconData);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	SetTimer(TIMER_CHECK_NOTIFICATIONS, 60000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CPlannerAppDlg::OnRefreshTask(WPARAM wParam, LPARAM lParam) {
	LoadTasksFromDatabase();
	return 0;
}


HBRUSH CPlannerAppDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CPlannerAppDlg::OnEraseBkgnd(CDC* pDC) {
	CDialogEx::OnEraseBkgnd(pDC);
	
	CRect rect;
	GetClientRect(&rect);


	return TRUE;
}

void CPlannerAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CPlannerAppDlg::ConnectToDatabase() {
	try
	{
		if (!m_db.IsOpen()) {
			m_db.OpenEx(_T("DSN=PlannerDSN"), CDatabase::noOdbcDialog);
		}
	}
	catch (CDBException* ex)
	{
		AfxMessageBox(ex->m_strError);
		ex->Delete();
		throw;
	}
}

void CPlannerAppDlg::OnBtnClickedAddTask() {
	CAddTaskDlg addTaskDlg(this);
	if (addTaskDlg.DoModal() == IDOK) {
		LoadTasksFromDatabase();
	}
	else {
		AfxMessageBox(_T("Add task canceled"));
	}
}

void CPlannerAppDlg::OnBtnClickedEditTask() {
	int selectedRow = m_TaskListCtrl.GetSelectionMark();
	if (selectedRow == -1) {
		AfxMessageBox(_T("Please select a task for edit"));
		return;
	}

	CString selectedTaskId = m_TaskListCtrl.GetItemText(selectedRow, 0);

	CString title, category, description;
	COleDateTime dueDate;

	try
	{
		if (!m_db.IsOpen()) {
			ConnectToDatabase();
		}

		CRecordset recordSet(&m_db);
		CString query;
		query.Format(_T("select TaskId, Title, Category, DueDate, Description from Tasks where TaskId = '%s'"), selectedTaskId);
		recordSet.Open(CRecordset::forwardOnly, query);

		if (!recordSet.IsEOF()) {
			recordSet.GetFieldValue((short)0, selectedTaskId);
			recordSet.GetFieldValue((short)1, title);
			recordSet.GetFieldValue((short)2, category);
			
			CString dueDateString;
			recordSet.GetFieldValue((short)3, dueDateString);
			recordSet.GetFieldValue((short)4, description);
			
			if (dueDateString.IsEmpty()) {
				dueDate.SetStatus(COleDateTime::null);
			}
			else {
				int dotPos = dueDateString.ReverseFind('.');
				if (dotPos != -1)
				{
					dueDateString = dueDateString.Left(dotPos);
				}

				if (!dueDate.ParseDateTime(dueDateString)) {
					CString errorMsg;
					errorMsg.Format(_T("Failed to parse DueDate: %s"), dueDateString);
					AfxMessageBox(errorMsg);
					return;
				}
			}
		}
		else {
			AfxMessageBox(_T("Task not found"));
			return;
		}
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		e->Delete();
		return;
	}

	CEditTaskDlg editTaskDlg(this);
	editTaskDlg.SetTaskData(selectedTaskId, title, category, dueDate, description);

	if (editTaskDlg.DoModal() == IDOK) {
		LoadTasksFromDatabase();
	}
}

void CPlannerAppDlg::OnBtnClickedDeleteTask() {
	int selectedRow = m_TaskListCtrl.GetSelectionMark();
	if (selectedRow == -1) {
		AfxMessageBox(_T("Please select a task for delete"));
		return;
	}

	CString taskId = m_TaskListCtrl.GetItemText(selectedRow, 0);

	CString message;
	message.Format(_T("Are you sure you want to delete the task with Id: %s"), taskId);
	if (AfxMessageBox(message, MB_YESNO | MB_ICONQUESTION) == IDOK) {
		return;
	}

	try
	{
		if (!m_db.IsOpen()) {
			ConnectToDatabase();
		}

		CString query;
		query.Format(_T("delete from Tasks where TaskId = '%s'"), taskId);
		m_db.ExecuteSQL(query);
	}
	catch (CDBException* ex)
	{
		AfxMessageBox(ex->m_strError);
		ex->Delete();
		return;
	}

	m_TaskListCtrl.DeleteItem(selectedRow);
	AfxMessageBox(_T("Task deleted successfully"));
}

void CPlannerAppDlg::LoadTasksFromDatabase() {
	m_TaskListCtrl.DeleteAllItems();

	try
	{
		if (!m_db.IsOpen()) {
			ConnectToDatabase();
		}

		CRecordset recordSet(&m_db);
		recordSet.Open(CRecordset::forwardOnly, _T("select TaskId, Title, Category, DueDate, Description from Tasks"));
		int row = 0;
		while (!recordSet.IsEOF()) {
			CString taskId, title, category, dueDate, description;

			try
			{
				recordSet.GetFieldValue(_T("TaskId"), taskId);
				recordSet.GetFieldValue(_T("Title"), title);
				recordSet.GetFieldValue(_T("Category"), category);
				recordSet.GetFieldValue(_T("DueDate"), dueDate);
				recordSet.GetFieldValue(_T("Description"), description);
			}
			catch (CDBException* ex)
			{
				AfxMessageBox(_T("Incorrect field name or field index."));
				ex->Delete();
				return;
			}

			m_TaskListCtrl.InsertItem(row, taskId);
			m_TaskListCtrl.SetItemText(row, 1, title);
			m_TaskListCtrl.SetItemText(row, 2, category);
			m_TaskListCtrl.SetItemText(row, 3, dueDate);
			m_TaskListCtrl.SetItemText(row, 4, description);

			recordSet.MoveNext();
			row++;
		}
	}
	catch (CDBException* e)
	{
		AfxMessageBox(e->m_strError);
		e->Delete();
	}
}

void CPlannerAppDlg::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent == TIMER_CHECK_NOTIFICATIONS) {
		CheckForNotification();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CPlannerAppDlg::ShowNotification(const CString& title, const CString& dueDate) {
	m_notifyIconData.uFlags = NIF_INFO;
	wcscpy_s(m_notifyIconData.szInfo, 256, _T("The task is due soon:") + title + _T("at") + dueDate);
	wcscpy_s(m_notifyIconData.szInfoTitle, 64, L"Task reminder");
	m_notifyIconData.dwInfoFlags = NIF_ICON;

	Shell_NotifyIcon(NIM_MODIFY, &m_notifyIconData);
}

void CPlannerAppDlg::CheckForNotification() {
	try
	{
		if (!m_db.IsOpen()) {
			ConnectToDatabase();
		}

		COleDateTime currentTime = COleDateTime::GetCurrentTime();
		COleDateTime thresholdTime = currentTime + COleDateTimeSpan(0, 0, 30, 0);

		CString query;
		query.Format(_T("select TaskId, Title, format(DueDate, 'yyyy-MM-dd HH:mm:ss') as DueDate from Tasks where DueDate is not null"));

		CRecordset recordSet(&m_db);
		recordSet.Open(CRecordset::forwardOnly, query);

		while (!recordSet.IsEOF()) {
			CString taskId,title, dueDateString;
			COleDateTime dueDate;

			try
			{
				recordSet.GetFieldValue(_T("TaskId"), taskId);
				recordSet.GetFieldValue(_T("Title"), title);
				recordSet.GetFieldValue(_T("DueDate"), dueDateString);


				int dotPos = dueDateString.ReverseFind('.');
				if (dotPos != -1) {
					dueDateString = dueDateString.Left(dotPos);
				}
				dueDateString.Replace(_T("T"), _T(" "));


				if (!dueDate.ParseDateTime(dueDateString)) {
					AfxMessageBox(_T("Failed to parse DueDate"));
					recordSet.MoveNext();
					continue;
				}

				if (dueDate >= currentTime && dueDate <= thresholdTime) {
					ShowNotification(title, dueDate.Format(_T("%Y-%m-%d %H:%M:%S")));
				}
			}
			catch (CDBException* e)
			{
				AfxMessageBox(_T("Database error while retrieving tasks: ") + e->m_strError);
				e->Delete();
			}
			
			recordSet.MoveNext();
		}
	}
	catch (CDBException* ex)
	{
		AfxMessageBox(_T("Database error: ") + ex->m_strError);
		ex->Delete();
	}
}

void CPlannerAppDlg::OnPaint()
{
	CPaintDC dc(this);

	Graphics graphics(dc.m_hDC);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	CRect rect;
	GetClientRect(&rect);

	Gdiplus::Image image(L"res\\img_bg.jpg");
	if (image.GetWidth() > 0 && image.GetHeight() > 0) {
		/*float xScale = static_cast<float>(rect.Width()) / image.GetWidth();
		float yScale = static_cast<float>(rect.Height()) / image.GetHeight();
		float scale = min(xScale, yScale);

		int scaledWidth = static_cast<int>(image.GetWidth() * scale);
		int scaledHeight = static_cast<int>(image.GetHeight() * scale);

		int xOffset = (rect.Width() - scaledWidth) / 2;
		int yOffset = (rect.Height() - scaledHeight) / 2;*/

		graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
		graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
	}
	else {
		AfxMessageBox(_T("Failed to load image"));
	}
}

void CPlannerAppDlg::OnDestroy() {
	CDialogEx::OnDestroy();

	Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);

	KillTimer(TIMER_CHECK_NOTIFICATIONS);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPlannerAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

