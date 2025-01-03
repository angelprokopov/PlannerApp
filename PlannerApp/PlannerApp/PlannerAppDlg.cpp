
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



CPlannerAppDlg::CPlannerAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLANNERAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD_TASK, &CPlannerAppDlg::OnBtnClickedAddTask)
	ON_BN_CLICKED(IDC_EDIT_TASK, &CPlannerAppDlg::OnBtnClickedEditTask)
	ON_BN_CLICKED(IDC_DELETE_TASK, &CPlannerAppDlg::OnBtnClickedDeleteTask)
END_MESSAGE_MAP()


// CPlannerAppDlg message handlers

BOOL CPlannerAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	HRESULT hResult = m_bgImage.Load(_T("res\\img\\above-art-background-black.jpg"));
	if (FAILED(hResult)) {
		AfxMessageBox(_T("Failed to load background image"));
	}

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

	m_TaskListCtrl.InsertColumn(0, _T("Task Id"), LVCFMT_LEFT, 60);
	m_TaskListCtrl.InsertColumn(1, _T("Title"), LVCFMT_LEFT, 150);
	m_TaskListCtrl.InsertColumn(2, _T("Category"), LVCFMT_LEFT, 100);
	m_TaskListCtrl.InsertColumn(3, _T("DueDate"), LVCFMT_LEFT, 120);
	m_TaskListCtrl.InsertColumn(4, _T("Description"), LVCFMT_LEFT, 200);

	m_TaskListCtrl.SetExtendedStyle(m_TaskListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if (ConnectToDatabase()) {
		LoadTasksFromDatabase();
	}

	// Load images
	try
	{
		m_AddImage.Load(_T("res\\img\\add.png"));
		m_addButton.SetImage(m_AddImage);

		m_EditImage.Load(_T("res\\img\\edit.png"));
		m_editButton.SetImage(m_EditImage);

		m_DeleteImage.Load(_T("res\\img\\delete.png"));
		m_deleteButton.SetImage(m_DeleteImage);
	}
	catch (CFileException* ex)
	{
		AfxMessageBox(_T("Failed to load image from file"));
		ex->Delete();
		return FALSE;
	}

	m_addButton.SetFaceColor(RGB(0, 120, 215));
	m_addButton.SetTextColor(RGB(179, 89, 0));
	m_addButton.SetWindowText(_T("Add new task"));

	m_editButton.SetFaceColor(RGB(0, 120, 215));
	m_editButton.SetTextColor(RGB(179, 89, 0));
	m_editButton.SetWindowText(_T("Edit task"));

	m_deleteButton.SetFaceColor(RGB(0, 120, 215));
	m_deleteButton.SetTextColor(RGB(179, 89, 0));
	m_deleteButton.SetWindowText(_T("Delete task"));


	memset(&m_NotifyIconData, 0, sizeof(m_NotifyIconData));
	m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyIconData.hWnd = this->GetSafeHwnd();
	m_NotifyIconData.uID = 1;
	m_NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_NotifyIconData.uCallbackMessage = WM_APP + 1;
	m_NotifyIconData.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
	wcscpy_s(m_NotifyIconData.szTip, L"Planner App");

	Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);

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


//BOOL CPlannerAppDlg::OnEraseBkgnd(CDC* pDC) {
//	CRect rect;
//	GetClientRect(rect);
//	CBrush brush(RGB(255, 217, 179));
//	pDC->FillRect(&rect, &brush);
//	return TRUE;
//}


BOOL CPlannerAppDlg::ConnectToDatabase() {
	try
	{
		if (!m_db.IsOpen()) {
			m_db.OpenEx(_T("DSN=PlannerDSN"), CDatabase::noOdbcDialog);
		}
		return true;
	}
	catch (CDBException* ex)
	{
		AfxMessageBox(ex->m_strError);
		ex->Delete();
		return false;
	}
}

void CPlannerAppDlg::OnBtnClickedAddTask() {
	CAddTaskDlg addTaskDlg;
	if (addTaskDlg.DoModal() == IDOK) {
		AfxMessageBox(_T(""));
	}
	else {
		AfxMessageBox(_T("Add task canceled"));
	}
}

void CPlannerAppDlg::OnBtnClickedEditTask() {
	int selectedRow = m_TaskListCtrl.GetSelectionMark();
	if (selectedRow == -1) {
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

	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPlannerAppDlg::ShowNotification(const CString& message) {
	m_NotifyIconData.uFlags = NIF_INFO;
	wcscpy_s(m_NotifyIconData.szInfo, message);
	wcscpy_s(m_NotifyIconData.szInfoTitle, L"Upcoming task");
	m_NotifyIconData.dwInfoFlags = NIF_INFO;
	Shell_NotifyIcon(NIM_MODIFY, &m_NotifyIconData);
}

void CPlannerAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		if (!m_bgImage.IsNull()) {
			m_bgImage.Draw(dc.m_hDC, rect);
		}
		else {
			CDialogEx::OnPaint();
		}

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CPlannerAppDlg::OnDestroy() {
	CDialogEx::OnDestroy();

	Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);

	KillTimer(TIMER_CHECK_NOTIFICATIONS);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPlannerAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

