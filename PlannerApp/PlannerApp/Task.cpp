#include "pch.h"
#include "Task.h"

Task::Task() : title(_T("")), category(_T("")), dueDate(CTime::GetCurrentTime()), description(_T("")) {}

Task::Task(CString t, CString c, CTime d, CString desc) : title(t), category(c), dueDate(d), description(desc) {}

CString Task::ToString() const {
	CString result;
	result.Format(_T("Title: %s\nCategory: %s\nDue Date: %s\nDescription: %s"), title, category, dueDate.Format(_T("%Y-%m-%d %H:%M:%S")));

	return result;
}