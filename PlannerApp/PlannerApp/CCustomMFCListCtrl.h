#pragma once
#include "afxdialogex.h"

class CCustomMFCListCtrl : public CMFCListCtrl
{
public:
	CCustomMFCListCtrl(){}
	virtual ~CCustomMFCListCtrl(){}

protected:
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
};

