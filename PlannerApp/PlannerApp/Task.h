#pragma once

#include <afx.h>

class Task
{
public:
	CString title;
	CString category;
	CTime dueDate;
	CString description;

	Task();
	Task(CString t, CString c, CTime d, CString desc);

	CString ToString() const;
};

