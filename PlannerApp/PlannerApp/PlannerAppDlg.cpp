
// PlannerAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PlannerApp.h"
#include "PlannerAppDlg.h"
#include "afxdialogex.h"
#include "afxdb.h"
#include "afxvisualmanagerwindows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CEditTaskDlg.h"


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
}

BEGIN_MESSAGE_MAP(CPlannerAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD_TASK, &CPlannerAppDlg::OnBtnClickedAddTask)
	ON_BN_CLICKED(IDC_EDIT_TASK, &CPlannerAppDlg::OnBtnClickedEditTask)
END_MESSAGE_MAP()


// CPlannerAppDlg message handlers

BOOL CPlannerAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


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
	m_addButton.SetTextColor(RGB(255, 255, 255));
	m_addButton.SetWindowTextW(_T("Add a new task"));


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


BOOL CPlannerAppDlg::OnEraseBkgnd(CDC* pDC) {
	CRect rect;
	GetClientRect(rect);
	CBrush brush(RGB(255, 217, 179));
	pDC->FillRect(&rect, &brush);
	return TRUE;
}


void CPlannerAppDlg::ConnectToDatabase() {
	CDatabase db;
	try
	{
		db.Open(_T("PlannerDSN"), FALSE, FALSE, _T("ODBC;"));
		AfxMessageBox(_T("Database connection successful"));

		CRecordset rs(&db);
		rs.Open(CRecordset::forwardOnly, _T("select * from Tasks"));
		while (!rs.IsEOF()) {
			CString title;
			rs.GetFieldValue(_T("Title"), title);
			AfxMessageBox(title);
			rs.MoveNext();
		}

		db.Close();
	}
	catch (CDBException* ex)
	{
		AfxMessageBox(ex->m_strError);
		ex->Delete();
	}
}

void CPlannerAppDlg::OnBtnClickedAddTask() {
	CAddTaskDlg addTaskDlg;
	if (addTaskDlg.DoModal() == IDOK) {
		AfxMessageBox(_T(""));
	}
	else {
		AfxMessageBox(_T("Add new task cancelled."));
	}
}

void CPlannerAppDlg::OnBtnClickedEditTask() {
	CEditTaskDlg editTaskDlg;
	if (editTaskDlg.DoModal() == IDOK) {
		AfxMessageBox(_T(""));
	}
	else {
		AfxMessageBox(_T("Edit task canceled"));
	}
}

void CPlannerAppDlg::OnBtnClickedDeleteTask() {

}

void CPlannerAppDlg::UpdateTaskList() {

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

