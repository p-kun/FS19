LRESULT _boxMsgCore::MsgHandle0001( WPARAM wParam, LPARAM lParam ) {
	return ((OnCreate)((LPCREATESTRUCT)(lParam)) ? 0L : (LRESULT)-1L);
}
LRESULT _boxMsgCore::MsgHandle0002( WPARAM wParam, LPARAM lParam ) {
	return ((OnDestroy)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0003( WPARAM wParam, LPARAM lParam ) {
	return ((OnMove)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0005( WPARAM wParam, LPARAM lParam ) {
	return ((OnSize)((UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0006( WPARAM wParam, LPARAM lParam ) {
	return ((OnActivate)((UINT)LOWORD(wParam), (HWND)(lParam), (BOOL)HIWORD(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0007( WPARAM wParam, LPARAM lParam ) {
	return ((OnSetFocus)((HWND)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0008( WPARAM wParam, LPARAM lParam ) {
	return ((OnKillFocus)((HWND)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle000A( WPARAM wParam, LPARAM lParam ) {
	return ((OnEnable)((BOOL)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle000B( WPARAM wParam, LPARAM lParam ) {
	return ((OnSetRedraw)((BOOL)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle000C( WPARAM wParam, LPARAM lParam ) {
	return ((OnSetText)((LPCTSTR)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle000D( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(int)(OnGetText)((int)(wParam), (LPTSTR)(lParam));
}
LRESULT _boxMsgCore::MsgHandle000E( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(int)(OnGetTextLength)();
}
LRESULT _boxMsgCore::MsgHandle000F( WPARAM wParam, LPARAM lParam ) {
	return ((OnPaint)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0010( WPARAM wParam, LPARAM lParam ) {
	return ((OnClose)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0011( WPARAM wParam, LPARAM lParam ) {
	return MAKELRESULT((BOOL)(OnQueryEndSession)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0013( WPARAM wParam, LPARAM lParam ) {
	return MAKELRESULT((BOOL)(OnQueryOpen)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0016( WPARAM wParam, LPARAM lParam ) {
	return ((OnEndSession)((BOOL)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0012( WPARAM wParam, LPARAM lParam ) {
	return ((OnQuit)((int)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0014( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(BOOL)(OnEraseBkgnd)((HDC)(wParam));
}
LRESULT _boxMsgCore::MsgHandle0015( WPARAM wParam, LPARAM lParam ) {
	return ((OnSysColorChange)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0018( WPARAM wParam, LPARAM lParam ) {
	return ((OnShowWindow)((BOOL)(wParam), (UINT)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle001A( WPARAM wParam, LPARAM lParam ) {
	return ((OnWinIniChange)((LPCTSTR)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle001B( WPARAM wParam, LPARAM lParam ) {
	return ((OnDevModeChange)((LPCTSTR)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle001C( WPARAM wParam, LPARAM lParam ) {
	return ((OnActivateApp)((BOOL)(wParam), (DWORD)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle001D( WPARAM wParam, LPARAM lParam ) {
	return ((OnFontChange)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle001E( WPARAM wParam, LPARAM lParam ) {
	return ((OnTimeChange)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle001F( WPARAM wParam, LPARAM lParam ) {
	return ((OnCancelMode)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0020( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(BOOL)(OnSetCursor)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
}
LRESULT _boxMsgCore::MsgHandle0021( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(int)(OnMouseActivate)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
}
LRESULT _boxMsgCore::MsgHandle0022( WPARAM wParam, LPARAM lParam ) {
	return ((OnChildActivate)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0023( WPARAM wParam, LPARAM lParam ) {
	return ((OnQueueSync)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0024( WPARAM wParam, LPARAM lParam ) {
	return ((OnGetMinMaxInfo)((LPMINMAXINFO)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0027( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(BOOL)(OnIconEraseBkgnd)((HDC)(wParam));
}
LRESULT _boxMsgCore::MsgHandle0028( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HWND)(OnNextDlgCtl)((HWND)(wParam), (BOOL)(lParam));
}
LRESULT _boxMsgCore::MsgHandle002A( WPARAM wParam, LPARAM lParam ) {
	return ((OnSpoolerStatus)((UINT)(wParam), (int)(short)LOWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle002B( WPARAM wParam, LPARAM lParam ) {
	return ((OnDrawItem)((const DRAWITEMSTRUCT *)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle002C( WPARAM wParam, LPARAM lParam ) {
	return ((OnMeasureItem)((MEASUREITEMSTRUCT *)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle002D( WPARAM wParam, LPARAM lParam ) {
	return ((OnDeleteItem)((const DELETEITEMSTRUCT *)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle002E( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(int)(OnVkeyToItem)((UINT)LOWORD(wParam), (HWND)(lParam), (int)(short)HIWORD(wParam));
}
LRESULT _boxMsgCore::MsgHandle002F( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(int)(OnCharToItem)((UINT)LOWORD(wParam), (HWND)(lParam), (int)(short)HIWORD(wParam));
}
LRESULT _boxMsgCore::MsgHandle0030( WPARAM wParam, LPARAM lParam ) {
	return ((OnSetFont)((HFONT)(wParam), (BOOL)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0031( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HFONT)(OnGetFont)();
}
LRESULT _boxMsgCore::MsgHandle0037( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(OnQueryDragIcon)();
}
LRESULT _boxMsgCore::MsgHandle0039( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(int)(OnCompareItem)((const COMPAREITEMSTRUCT *)(lParam));
}
LRESULT _boxMsgCore::MsgHandle0041( WPARAM wParam, LPARAM lParam ) {
	return ((OnCompacting)((UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0044( WPARAM wParam, LPARAM lParam ) {
	return ((OnCommNotify)((int)(wParam), (UINT)LOWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0046( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(BOOL)(OnWindowPosChanging)((LPWINDOWPOS)(lParam));
}
LRESULT _boxMsgCore::MsgHandle0047( WPARAM wParam, LPARAM lParam ) {
	return ((OnWindowPosChanged)((const LPWINDOWPOS)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0048( WPARAM wParam, LPARAM lParam ) {
	return ((OnPower)((int)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle007B( WPARAM wParam, LPARAM lParam ) {
	return ((OnContextMenu)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle007E( WPARAM wParam, LPARAM lParam ) {
	return ((OnDisplayChange)((UINT)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0081( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(BOOL)(OnNCCreate)((LPCREATESTRUCT)(lParam));
}
LRESULT _boxMsgCore::MsgHandle0082( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCDestroy)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0083( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(UINT)(OnNCCalcSize)((BOOL)(wParam), (NCCALCSIZE_PARAMS *)(lParam));
}
LRESULT _boxMsgCore::MsgHandle0084( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(UINT)(OnNCHitTest)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
}
LRESULT _boxMsgCore::MsgHandle0085( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCPaint)((HRGN)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0086( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(BOOL)(OnNCActivate)((BOOL)(wParam), 0L, 0L);
}
LRESULT _boxMsgCore::MsgHandle0087( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(UINT)(OnGetDlgCode)((LPMSG)(lParam));
}
LRESULT _boxMsgCore::MsgHandle00A0( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCMouseMove)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A1( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCLButtonDown)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A2( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCLButtonUp)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A3( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCLButtonDown)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A4( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCRButtonDown)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A5( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCRButtonUp)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A6( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCRButtonDown)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A7( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCMButtonDown)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A8( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCMButtonUp)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle00A9( WPARAM wParam, LPARAM lParam ) {
	return ((OnNCMButtonDown)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0100( WPARAM wParam, LPARAM lParam ) {
	return ((OnKeyDown)((UINT)(wParam), TRUE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0101( WPARAM wParam, LPARAM lParam ) {
	return ((OnKeyUp)((UINT)(wParam), FALSE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0102( WPARAM wParam, LPARAM lParam ) {
	return ((OnChar)((TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0103( WPARAM wParam, LPARAM lParam ) {
	return ((OnDeadChar)((TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0104( WPARAM wParam, LPARAM lParam ) {
	return ((OnSysKeyDown)((UINT)(wParam), TRUE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0105( WPARAM wParam, LPARAM lParam ) {
	return ((OnSysKeyUp)((UINT)(wParam), FALSE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0106( WPARAM wParam, LPARAM lParam ) {
	return ((OnSysChar)((int)(wParam), (int)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0107( WPARAM wParam, LPARAM lParam ) {
	return ((OnSysDeadChar)((int)(wParam), (int)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0110( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(UINT)(BOOL)(OnInitDialog)((HWND)(wParam), lParam);
}
LRESULT _boxMsgCore::MsgHandle0111( WPARAM wParam, LPARAM lParam ) {
	return ((OnCommand)((int)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0112( WPARAM wParam, LPARAM lParam ) {
	return ((OnSysCommand)((UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0113( WPARAM wParam, LPARAM lParam ) {
	return ((OnTimer)((UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0114( WPARAM wParam, LPARAM lParam ) {
	return ((OnHScroll)((HWND)(lParam), (UINT)(LOWORD(wParam)), (int)(short)HIWORD(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0115( WPARAM wParam, LPARAM lParam ) {
	return ((OnVScroll)((HWND)(lParam), (UINT)(LOWORD(wParam)),  (int)(short)HIWORD(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0116( WPARAM wParam, LPARAM lParam ) {
	return ((OnInitMenu)((HMENU)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0117( WPARAM wParam, LPARAM lParam ) {
	return ((OnInitMenuPopup)((HMENU)(wParam), (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle011F( WPARAM wParam, LPARAM lParam ) {
	return ((OnMenuSelect)((HMENU)(lParam), (HIWORD(wParam) & MF_POPUP) ? 0L : (int)(LOWORD(wParam)), (HIWORD(wParam) & MF_POPUP) ? GetSubMenu((HMENU)lParam, LOWORD(wParam)) : 0L, (UINT)(((short)HIWORD(wParam) == -1) ? 0xFFFFFFFF : HIWORD(wParam))), 0L);
}
LRESULT _boxMsgCore::MsgHandle0120( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(OnMenuChar)((UINT)(LOWORD(wParam)), (UINT)HIWORD(wParam), (HMENU)(lParam));
}
LRESULT _boxMsgCore::MsgHandle0121( WPARAM wParam, LPARAM lParam ) {
	return ((OnEnterIdle)((UINT)(wParam), (HWND)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0132( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HBRUSH)(OnCtlColorMsgBox)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_MSGBOX);
}
LRESULT _boxMsgCore::MsgHandle0133( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HBRUSH)(OnCtlColorEditr)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_EDIT);
}
LRESULT _boxMsgCore::MsgHandle0134( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HBRUSH)(OnCtlColorListBox)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_LISTBOX);
}
LRESULT _boxMsgCore::MsgHandle0135( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HBRUSH)(OnCtlColorBtn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_BTN);
}
LRESULT _boxMsgCore::MsgHandle0136( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HBRUSH)(OnCtlColorDlg)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_DLG);
}
LRESULT _boxMsgCore::MsgHandle0137( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HBRUSH)(OnCtlColorScrollBar)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_SCROLLBAR);
}
LRESULT _boxMsgCore::MsgHandle0138( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HBRUSH)(OnCtlColorStatic)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_STATIC);
}
LRESULT _boxMsgCore::MsgHandle0200( WPARAM wParam, LPARAM lParam ) {
	return ((OnMouseMove)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0201( WPARAM wParam, LPARAM lParam ) {
	return ((OnLButtonDown)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0202( WPARAM wParam, LPARAM lParam ) {
	return ((OnLButtonUp)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0203( WPARAM wParam, LPARAM lParam ) {
	return ((OnLButtonDown)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0204( WPARAM wParam, LPARAM lParam ) {
	return ((OnRButtonDown)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0205( WPARAM wParam, LPARAM lParam ) {
	return ((OnRButtonUp)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0206( WPARAM wParam, LPARAM lParam ) {
	return ((OnRButtonDown)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0207( WPARAM wParam, LPARAM lParam ) {
	return ((OnMButtonDown)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0208( WPARAM wParam, LPARAM lParam ) {
	return ((OnMButtonUp)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0209( WPARAM wParam, LPARAM lParam ) {
	return ((OnMButtonDown)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle020A( WPARAM wParam, LPARAM lParam ) {
	return ((OnMouseWheel)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0210( WPARAM wParam, LPARAM lParam ) {
	return ((OnParentNotify)((UINT)LOWORD(wParam), (HWND)(lParam), (UINT)HIWORD(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0214( WPARAM wParam, LPARAM lParam ) {
	return ((OnSizing)((UINT)(wParam), (LPRECT)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0216( WPARAM wParam, LPARAM lParam ) {
	return ((OnMoving)((UINT)(wParam), (LPRECT)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0219( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(BOOL)(OnDeviceChange)((UINT)(wParam), (DWORD)(wParam));
}
LRESULT _boxMsgCore::MsgHandle0220( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(MDICreate)((LPMDICREATESTRUCT)(lParam));
}
LRESULT _boxMsgCore::MsgHandle0221( WPARAM wParam, LPARAM lParam ) {
	return ((MDIDestroy)((HWND)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0222( WPARAM wParam, LPARAM lParam ) {
	return ((MDIActivate)((BOOL)(lParam == (LPARAM)mhWnd), (HWND)(lParam), (HWND)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0223( WPARAM wParam, LPARAM lParam ) {
	return ((MDIRestore)((HWND)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0224( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(HWND)(MDINext)((HWND)(wParam), (BOOL)lParam);
}
LRESULT _boxMsgCore::MsgHandle0225( WPARAM wParam, LPARAM lParam ) {
	return ((MDIMaximize)((HWND)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0226( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(MDITile)((UINT)(wParam));
}
LRESULT _boxMsgCore::MsgHandle0227( WPARAM wParam, LPARAM lParam ) {
	return (LRESULT)(DWORD)(MDICascade)((UINT)(wParam));
}
LRESULT _boxMsgCore::MsgHandle0228( WPARAM wParam, LPARAM lParam ) {
	return ((MDIIconArrange)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0229( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(MDIGetActive)();
}
LRESULT _boxMsgCore::MsgHandle0230( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(MDISetMenu)((BOOL)(wParam), (HMENU)(wParam), (HMENU)(lParam));
}
LRESULT _boxMsgCore::MsgHandle0233( WPARAM wParam, LPARAM lParam ) {
	return ((OnDropFiles)((HDROP)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0300( WPARAM wParam, LPARAM lParam ) {
	return ((OnCut)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0301( WPARAM wParam, LPARAM lParam ) {
	return ((OnCopy)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0302( WPARAM wParam, LPARAM lParam ) {
	return ((OnPaste)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0303( WPARAM wParam, LPARAM lParam ) {
	return ((OnClear)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0304( WPARAM wParam, LPARAM lParam ) {
	return ((OnUndo)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0305( WPARAM wParam, LPARAM lParam ) {
	return (int)(LRESULT)(UINT_PTR)(HANDLE)(OnRenderFormat)((UINT)(wParam));
}
LRESULT _boxMsgCore::MsgHandle0306( WPARAM wParam, LPARAM lParam ) {
	return ((OnRenderAllFormats)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0307( WPARAM wParam, LPARAM lParam ) {
	return ((OnDestroyClipboard)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0308( WPARAM wParam, LPARAM lParam ) {
	return ((OnDrawClipboard)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0309( WPARAM wParam, LPARAM lParam ) {
	return ((OnPaintClipboard)((HWND)(wParam), (const LPPAINTSTRUCT)GlobalLock((HGLOBAL)(lParam))), GlobalUnlock((HGLOBAL)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle030A( WPARAM wParam, LPARAM lParam ) {
	return ((OnVScrollClipboard)((HWND)(wParam), (UINT)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle030B( WPARAM wParam, LPARAM lParam ) {
	return ((OnSizeClipboard)((HWND)(wParam), (const LPRECT)GlobalLock((HGLOBAL)(lParam))), GlobalUnlock((HGLOBAL)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle030C( WPARAM wParam, LPARAM lParam ) {
	return ((OnAskCBFormatName)((int)(wParam), (LPTSTR)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle030D( WPARAM wParam, LPARAM lParam ) {
	return ((OnChangeCBChain)((HWND)(wParam), (HWND)(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle030E( WPARAM wParam, LPARAM lParam ) {
	return ((OnHScrollClipboard)((HWND)(wParam), (UINT)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle030F( WPARAM wParam, LPARAM lParam ) {
	return MAKELRESULT((BOOL)(OnQueryNewPalette)(), 0L);
}
LRESULT _boxMsgCore::MsgHandle0310( WPARAM wParam, LPARAM lParam ) {
	return ((OnPaletteIsChanging)((HWND)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0311( WPARAM wParam, LPARAM lParam ) {
	return ((OnPaletteChanged)((HWND)(wParam)), 0L);
}
LRESULT _boxMsgCore::MsgHandle0312( WPARAM wParam, LPARAM lParam ) {
	return ((OnHotKey)((int)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
}
