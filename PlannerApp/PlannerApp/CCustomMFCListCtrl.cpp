#include "pch.h"
#include "CCustomMFCListCtrl.h"
#include <afxwin.h>

BEGIN_MESSAGE_MAP(CCustomMFCListCtrl,CMFCListCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CCustomMFCListCtrl::PreSubclassWindow() {
	CMFCListCtrl::PreSubclassWindow();

	LONG lStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, lStyle | WS_EX_TRANSPARENT);
}

BOOL CCustomMFCListCtrl::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}