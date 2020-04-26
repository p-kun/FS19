BOOL _boxMsgCore::OnCreate(LPCREATESTRUCT lpCreateStruct) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_CREATE, 0L, (LPARAM)(LPCREATESTRUCT)(lpCreateStruct)); }
BOOL _boxMsgCore::SendCreate(LPCREATESTRUCT lpCreateStruct) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_CREATE, 0L, (LPARAM)(LPCREATESTRUCT)(lpCreateStruct)); }
void _boxMsgCore::OnDestroy() { (void)(DefaultMessage)((mhWnd), WM_DESTROY, 0L, 0L); }
void _boxMsgCore::SendDestroy() { (void)(::SendMessage)((mhWnd), WM_DESTROY, 0L, 0L); }
void _boxMsgCore::OnMove(int x, int y) { (void)(DefaultMessage)((mhWnd), WM_MOVE, 0L, MAKELPARAM((x), (y))); }
void _boxMsgCore::SendMove(int x, int y) { (void)(::SendMessage)((mhWnd), WM_MOVE, 0L, MAKELPARAM((x), (y))); }
void _boxMsgCore::OnSize(UINT state, int cx, int cy) { (void)(DefaultMessage)((mhWnd), WM_SIZE, (WPARAM)(UINT)(state), MAKELPARAM((cx), (cy))); }
void _boxMsgCore::SendSize(UINT state, int cx, int cy) { (void)(::SendMessage)((mhWnd), WM_SIZE, (WPARAM)(UINT)(state), MAKELPARAM((cx), (cy))); }
void _boxMsgCore::OnActivate(UINT state, HWND hwndActDeact, BOOL fMinimized) { (void)(DefaultMessage)((mhWnd), WM_ACTIVATE, MAKEWPARAM((state), (fMinimized)), (LPARAM)(HWND)(hwndActDeact)); }
void _boxMsgCore::SendActivate(UINT state, HWND hwndActDeact, BOOL fMinimized) { (void)(::SendMessage)((mhWnd), WM_ACTIVATE, MAKEWPARAM((state), (fMinimized)), (LPARAM)(HWND)(hwndActDeact)); }
void _boxMsgCore::OnSetFocus(HWND hwndOldFocus) { (void)(DefaultMessage)((mhWnd), WM_SETFOCUS, (WPARAM)(HWND)(hwndOldFocus), 0L); }
void _boxMsgCore::SendSetFocus(HWND hwndOldFocus) { (void)(::SendMessage)((mhWnd), WM_SETFOCUS, (WPARAM)(HWND)(hwndOldFocus), 0L); }
void _boxMsgCore::OnKillFocus(HWND hwndNewFocus) { (void)(DefaultMessage)((mhWnd), WM_KILLFOCUS, (WPARAM)(HWND)(hwndNewFocus), 0L); }
void _boxMsgCore::SendKillFocus(HWND hwndNewFocus) { (void)(::SendMessage)((mhWnd), WM_KILLFOCUS, (WPARAM)(HWND)(hwndNewFocus), 0L); }
void _boxMsgCore::OnEnable(BOOL fEnable) { (void)(DefaultMessage)((mhWnd), WM_ENABLE, (WPARAM)(BOOL)(fEnable), 0L); }
void _boxMsgCore::SendEnable(BOOL fEnable) { (void)(::SendMessage)((mhWnd), WM_ENABLE, (WPARAM)(BOOL)(fEnable), 0L); }
void _boxMsgCore::OnSetRedraw(BOOL fRedraw) { (void)(DefaultMessage)((mhWnd), WM_SETREDRAW, (WPARAM)(BOOL)(fRedraw), 0L); }
void _boxMsgCore::SendSetRedraw(BOOL fRedraw) { (void)(::SendMessage)((mhWnd), WM_SETREDRAW, (WPARAM)(BOOL)(fRedraw), 0L); }
void _boxMsgCore::OnSetText(LPCTSTR lpszText) { (void)(DefaultMessage)((mhWnd), WM_SETTEXT, 0L, (LPARAM)(LPCTSTR)(lpszText)); }
void _boxMsgCore::SendSetText(LPCTSTR lpszText) { (void)(::SendMessage)((mhWnd), WM_SETTEXT, 0L, (LPARAM)(LPCTSTR)(lpszText)); }
INT _boxMsgCore::OnGetText(int cchTextMax, LPTSTR lpszText) { return (int)(DWORD)(DefaultMessage)((mhWnd), WM_GETTEXT, (WPARAM)(int)(cchTextMax), (LPARAM)(LPTSTR)(lpszText)); }
INT _boxMsgCore::SendGetText(int cchTextMax, LPTSTR lpszText) { return (int)(DWORD)(::SendMessage)((mhWnd), WM_GETTEXT, (WPARAM)(int)(cchTextMax), (LPARAM)(LPTSTR)(lpszText)); }
INT _boxMsgCore::OnGetTextLength() { return (int)(DWORD)(DefaultMessage)((mhWnd), WM_GETTEXTLENGTH, 0L, 0L); }
INT _boxMsgCore::SendGetTextLength() { return (int)(DWORD)(::SendMessage)((mhWnd), WM_GETTEXTLENGTH, 0L, 0L); }
void _boxMsgCore::OnPaint() { (void)(DefaultMessage)((mhWnd), WM_PAINT, 0L, 0L); }
void _boxMsgCore::SendPaint() { (void)(::SendMessage)((mhWnd), WM_PAINT, 0L, 0L); }
void _boxMsgCore::OnClose() { (void)(DefaultMessage)((mhWnd), WM_CLOSE, 0L, 0L); }
void _boxMsgCore::SendClose() { (void)(::SendMessage)((mhWnd), WM_CLOSE, 0L, 0L); }
BOOL _boxMsgCore::OnQueryEndSession() { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_QUERYENDSESSION, 0L, 0L); }
BOOL _boxMsgCore::SendQueryEndSession() { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_QUERYENDSESSION, 0L, 0L); }
BOOL _boxMsgCore::OnQueryOpen() { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_QUERYOPEN, 0L, 0L); }
BOOL _boxMsgCore::SendQueryOpen() { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_QUERYOPEN, 0L, 0L); }
void _boxMsgCore::OnEndSession(BOOL fEnding) { (void)(DefaultMessage)((mhWnd), WM_ENDSESSION, (WPARAM)(BOOL)(fEnding), 0L); }
void _boxMsgCore::SendEndSession(BOOL fEnding) { (void)(::SendMessage)((mhWnd), WM_ENDSESSION, (WPARAM)(BOOL)(fEnding), 0L); }
void _boxMsgCore::OnQuit(int exitCode) { (void)(DefaultMessage)((mhWnd), WM_QUIT, (WPARAM)(exitCode), 0L); }
void _boxMsgCore::SendQuit(int exitCode) { (void)(::SendMessage)((mhWnd), WM_QUIT, (WPARAM)(exitCode), 0L); }
BOOL _boxMsgCore::OnEraseBkgnd(HDC hdc) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_ERASEBKGND, (WPARAM)(HDC)(hdc), 0L); }
BOOL _boxMsgCore::SendEraseBkgnd(HDC hdc) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_ERASEBKGND, (WPARAM)(HDC)(hdc), 0L); }
void _boxMsgCore::OnSysColorChange() { (void)(DefaultMessage)((mhWnd), WM_SYSCOLORCHANGE, 0L, 0L); }
void _boxMsgCore::SendSysColorChange() { (void)(::SendMessage)((mhWnd), WM_SYSCOLORCHANGE, 0L, 0L); }
void _boxMsgCore::OnShowWindow(BOOL fShow, UINT status) { (void)(DefaultMessage)((mhWnd), WM_SHOWWINDOW, (WPARAM)(BOOL)(fShow), (LPARAM)(UINT)(status)); }
void _boxMsgCore::SendShowWindow(BOOL fShow, UINT status) { (void)(::SendMessage)((mhWnd), WM_SHOWWINDOW, (WPARAM)(BOOL)(fShow), (LPARAM)(UINT)(status)); }
void _boxMsgCore::OnWinIniChange(LPCTSTR lpszSectionName) { (void)(DefaultMessage)((mhWnd), WM_WININICHANGE, 0L, (LPARAM)(LPCTSTR)(lpszSectionName)); }
void _boxMsgCore::SendWinIniChange(LPCTSTR lpszSectionName) { (void)(::SendMessage)((mhWnd), WM_WININICHANGE, 0L, (LPARAM)(LPCTSTR)(lpszSectionName)); }
void _boxMsgCore::OnDevModeChange(LPCTSTR lpszDeviceName) { (void)(DefaultMessage)((mhWnd), WM_DEVMODECHANGE, 0L, (LPARAM)(LPCTSTR)(lpszDeviceName)); }
void _boxMsgCore::SendDevModeChange(LPCTSTR lpszDeviceName) { (void)(::SendMessage)((mhWnd), WM_DEVMODECHANGE, 0L, (LPARAM)(LPCTSTR)(lpszDeviceName)); }
void _boxMsgCore::OnActivateApp(BOOL fActivate, DWORD dwThreadId) { (void)(DefaultMessage)((mhWnd), WM_ACTIVATEAPP, (WPARAM)(BOOL)(fActivate), (LPARAM)(dwThreadId)); }
void _boxMsgCore::SendActivateApp(BOOL fActivate, DWORD dwThreadId) { (void)(::SendMessage)((mhWnd), WM_ACTIVATEAPP, (WPARAM)(BOOL)(fActivate), (LPARAM)(dwThreadId)); }
void _boxMsgCore::OnFontChange() { (void)(DefaultMessage)((mhWnd), WM_FONTCHANGE, 0L, 0L); }
void _boxMsgCore::SendFontChange() { (void)(::SendMessage)((mhWnd), WM_FONTCHANGE, 0L, 0L); }
void _boxMsgCore::OnTimeChange() { (void)(DefaultMessage)((mhWnd), WM_TIMECHANGE, 0L, 0L); }
void _boxMsgCore::SendTimeChange() { (void)(::SendMessage)((mhWnd), WM_TIMECHANGE, 0L, 0L); }
void _boxMsgCore::OnCancelMode() { (void)(DefaultMessage)((mhWnd), WM_CANCELMODE, 0L, 0L); }
void _boxMsgCore::SendCancelMode() { (void)(::SendMessage)((mhWnd), WM_CANCELMODE, 0L, 0L); }
BOOL _boxMsgCore::OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_SETCURSOR, (WPARAM)(HWND)(hwndCursor), MAKELPARAM((codeHitTest), (msg))); }
BOOL _boxMsgCore::SendSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_SETCURSOR, (WPARAM)(HWND)(hwndCursor), MAKELPARAM((codeHitTest), (msg))); }
int _boxMsgCore::OnMouseActivate(HWND hwndTopLevel, UINT codeHitTest, UINT msg) { return (int)(DWORD)(DefaultMessage)((mhWnd), WM_MOUSEACTIVATE, (WPARAM)(HWND)(hwndTopLevel), MAKELPARAM((codeHitTest), (msg))); }
int _boxMsgCore::SendMouseActivate(HWND hwndTopLevel, UINT codeHitTest, UINT msg) { return (int)(DWORD)(::SendMessage)((mhWnd), WM_MOUSEACTIVATE, (WPARAM)(HWND)(hwndTopLevel), MAKELPARAM((codeHitTest), (msg))); }
void _boxMsgCore::OnChildActivate() { (void)(DefaultMessage)((mhWnd), WM_CHILDACTIVATE, 0L, 0L); }
void _boxMsgCore::SendChildActivate() { (void)(::SendMessage)((mhWnd), WM_CHILDACTIVATE, 0L, 0L); }
void _boxMsgCore::OnQueueSync() { (void)(DefaultMessage)((mhWnd), WM_QUEUESYNC, 0L, 0L); }
void _boxMsgCore::SendQueueSync() { (void)(::SendMessage)((mhWnd), WM_QUEUESYNC, 0L, 0L); }
void _boxMsgCore::OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo) { (void)(DefaultMessage)((mhWnd), WM_GETMINMAXINFO, 0L, (LPARAM)(LPMINMAXINFO)(lpMinMaxInfo)); }
void _boxMsgCore::SendGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo) { (void)(::SendMessage)((mhWnd), WM_GETMINMAXINFO, 0L, (LPARAM)(LPMINMAXINFO)(lpMinMaxInfo)); }
BOOL _boxMsgCore::OnIconEraseBkgnd(HDC hdc) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_ICONERASEBKGND, (WPARAM)(HDC)(hdc), 0L); }
BOOL _boxMsgCore::SendIconEraseBkgnd(HDC hdc) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_ICONERASEBKGND, (WPARAM)(HDC)(hdc), 0L); }
HWND _boxMsgCore::OnNextDlgCtl(HWND hwndSetFocus, BOOL fNext) { return (HWND)(UINT_PTR)(DefaultMessage)((mhWnd), WM_NEXTDLGCTL, (WPARAM)(HWND)(hwndSetFocus), (LPARAM)(fNext)); }
HWND _boxMsgCore::SendNextDlgCtl(HWND hwndSetFocus, BOOL fNext) { return (HWND)(UINT_PTR)(::SendMessage)((mhWnd), WM_NEXTDLGCTL, (WPARAM)(HWND)(hwndSetFocus), (LPARAM)(fNext)); }
void _boxMsgCore::OnSpoolerStatus(UINT status, int cJobInQueue) { (void)(DefaultMessage)((mhWnd), WM_SPOOLERSTATUS, (WPARAM)(status), MAKELPARAM((cJobInQueue), 0)); }
void _boxMsgCore::SendSpoolerStatus(UINT status, int cJobInQueue) { (void)(::SendMessage)((mhWnd), WM_SPOOLERSTATUS, (WPARAM)(status), MAKELPARAM((cJobInQueue), 0)); }
void _boxMsgCore::OnDrawItem(const DRAWITEMSTRUCT * lpDrawItem) { (void)(DefaultMessage)((mhWnd), WM_DRAWITEM, (WPARAM)(((const DRAWITEMSTRUCT *)lpDrawItem)->CtlID), (LPARAM)(const DRAWITEMSTRUCT *)(lpDrawItem)); }
void _boxMsgCore::SendDrawItem(const DRAWITEMSTRUCT * lpDrawItem) { (void)(::SendMessage)((mhWnd), WM_DRAWITEM, (WPARAM)(((const DRAWITEMSTRUCT *)lpDrawItem)->CtlID), (LPARAM)(const DRAWITEMSTRUCT *)(lpDrawItem)); }
void _boxMsgCore::OnMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem) { (void)(DefaultMessage)((mhWnd), WM_MEASUREITEM, (WPARAM)(((MEASUREITEMSTRUCT *)lpMeasureItem)->CtlID), (LPARAM)(MEASUREITEMSTRUCT *)(lpMeasureItem)); }
void _boxMsgCore::SendMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem) { (void)(::SendMessage)((mhWnd), WM_MEASUREITEM, (WPARAM)(((MEASUREITEMSTRUCT *)lpMeasureItem)->CtlID), (LPARAM)(MEASUREITEMSTRUCT *)(lpMeasureItem)); }
void _boxMsgCore::OnDeleteItem(const DELETEITEMSTRUCT * lpDeleteItem) { (void)(DefaultMessage)((mhWnd), WM_DELETEITEM, (WPARAM)(((const DELETEITEMSTRUCT *)(lpDeleteItem))->CtlID), (LPARAM)(const DELETEITEMSTRUCT *)(lpDeleteItem)); }
void _boxMsgCore::SendDeleteItem(const DELETEITEMSTRUCT * lpDeleteItem) { (void)(::SendMessage)((mhWnd), WM_DELETEITEM, (WPARAM)(((const DELETEITEMSTRUCT *)(lpDeleteItem))->CtlID), (LPARAM)(const DELETEITEMSTRUCT *)(lpDeleteItem)); }
int _boxMsgCore::OnVkeyToItem(UINT vk, HWND hwndListbox, int iCaret) { return (int)(DWORD)(DefaultMessage)((mhWnd), WM_VKEYTOITEM, MAKEWPARAM((vk), (iCaret)), (LPARAM)(hwndListbox)); }
int _boxMsgCore::SendVkeyToItem(UINT vk, HWND hwndListbox, int iCaret) { return (int)(DWORD)(::SendMessage)((mhWnd), WM_VKEYTOITEM, MAKEWPARAM((vk), (iCaret)), (LPARAM)(hwndListbox)); }
int _boxMsgCore::OnCharToItem(UINT ch, HWND hwndListbox, int iCaret) { return (int)(DWORD)(DefaultMessage)((mhWnd), WM_CHARTOITEM, MAKEWPARAM((UINT)(ch), (UINT)(iCaret)), (LPARAM)(hwndListbox)); }
int _boxMsgCore::SendCharToItem(UINT ch, HWND hwndListbox, int iCaret) { return (int)(DWORD)(::SendMessage)((mhWnd), WM_CHARTOITEM, MAKEWPARAM((UINT)(ch), (UINT)(iCaret)), (LPARAM)(hwndListbox)); }
void _boxMsgCore::OnSetFont(HFONT hfont, BOOL fRedraw) { (void)(DefaultMessage)((mhWnd), WM_SETFONT, (WPARAM)(HFONT)(hfont), (LPARAM)(BOOL)(fRedraw)); }
void _boxMsgCore::SendSetFont(HFONT hfont, BOOL fRedraw) { (void)(::SendMessage)((mhWnd), WM_SETFONT, (WPARAM)(HFONT)(hfont), (LPARAM)(BOOL)(fRedraw)); }
HFONT _boxMsgCore::OnGetFont() { return (HFONT)(UINT_PTR)(DefaultMessage)((mhWnd), WM_GETFONT, 0L, 0L); }
HFONT _boxMsgCore::SendGetFont() { return (HFONT)(UINT_PTR)(::SendMessage)((mhWnd), WM_GETFONT, 0L, 0L); }
HICON _boxMsgCore::OnQueryDragIcon() { return (HICON)(DefaultMessage)((mhWnd), WM_QUERYDRAGICON, 0L, 0L); }
HICON _boxMsgCore::SendQueryDragIcon() { return (HICON)(::SendMessage)((mhWnd), WM_QUERYDRAGICON, 0L, 0L); }
int _boxMsgCore::OnCompareItem(const COMPAREITEMSTRUCT * lpCompareItem) { return (int)(DWORD)(DefaultMessage)((mhWnd), WM_COMPAREITEM, (WPARAM)(((const COMPAREITEMSTRUCT *)(lpCompareItem))->CtlID), (LPARAM)(const COMPAREITEMSTRUCT *)(lpCompareItem)); }
int _boxMsgCore::SendCompareItem(const COMPAREITEMSTRUCT * lpCompareItem) { return (int)(DWORD)(::SendMessage)((mhWnd), WM_COMPAREITEM, (WPARAM)(((const COMPAREITEMSTRUCT *)(lpCompareItem))->CtlID), (LPARAM)(const COMPAREITEMSTRUCT *)(lpCompareItem)); }
void _boxMsgCore::OnCompacting(UINT compactRatio) { (void)(DefaultMessage)((mhWnd), WM_COMPACTING, (WPARAM)(UINT)(compactRatio), 0L); }
void _boxMsgCore::SendCompacting(UINT compactRatio) { (void)(::SendMessage)((mhWnd), WM_COMPACTING, (WPARAM)(UINT)(compactRatio), 0L); }
void _boxMsgCore::OnCommNotify(int cid, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_COMMNOTIFY, (WPARAM)(cid), MAKELPARAM((flags), 0)); }
void _boxMsgCore::SendCommNotify(int cid, UINT flags) { (void)(::SendMessage)((mhWnd), WM_COMMNOTIFY, (WPARAM)(cid), MAKELPARAM((flags), 0)); }
BOOL _boxMsgCore::OnWindowPosChanging(LPWINDOWPOS lpwpos) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_WINDOWPOSCHANGING, 0L, (LPARAM)(LPWINDOWPOS)(lpwpos)); }
BOOL _boxMsgCore::SendWindowPosChanging(LPWINDOWPOS lpwpos) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_WINDOWPOSCHANGING, 0L, (LPARAM)(LPWINDOWPOS)(lpwpos)); }
void _boxMsgCore::OnWindowPosChanged(const LPWINDOWPOS lpwpos) { (void)(DefaultMessage)((mhWnd), WM_WINDOWPOSCHANGED, 0L, (LPARAM)(const LPWINDOWPOS)(lpwpos)); }
void _boxMsgCore::SendWindowPosChanged(const LPWINDOWPOS lpwpos) { (void)(::SendMessage)((mhWnd), WM_WINDOWPOSCHANGED, 0L, (LPARAM)(const LPWINDOWPOS)(lpwpos)); }
void _boxMsgCore::OnPower(int code) { (void)(DefaultMessage)((mhWnd), WM_POWER, (WPARAM)(int)(code), 0L); }
void _boxMsgCore::SendPower(int code) { (void)(::SendMessage)((mhWnd), WM_POWER, (WPARAM)(int)(code), 0L); }
void _boxMsgCore::OnContextMenu(HWND hwndContext, UINT xPos, UINT yPos) { (void)(DefaultMessage)((mhWnd), WM_CONTEXTMENU, (WPARAM)(HWND)(hwndContext), MAKELPARAM((UINT)(xPos), (UINT)(yPos))); }
void _boxMsgCore::SendContextMenu(HWND hwndContext, UINT xPos, UINT yPos) { (void)(::SendMessage)((mhWnd), WM_CONTEXTMENU, (WPARAM)(HWND)(hwndContext), MAKELPARAM((UINT)(xPos), (UINT)(yPos))); }
void _boxMsgCore::OnDisplayChange(UINT bitsPerPixel, UINT cxScreen, UINT cyScreen) { (void)(DefaultMessage)((mhWnd), WM_DISPLAYCHANGE, (WPARAM)(UINT)(bitsPerPixel), (LPARAM)MAKELPARAM((UINT)(cxScreen), (UINT)(cyScreen))); }
void _boxMsgCore::SendDisplayChange(UINT bitsPerPixel, UINT cxScreen, UINT cyScreen) { (void)(::SendMessage)((mhWnd), WM_DISPLAYCHANGE, (WPARAM)(UINT)(bitsPerPixel), (LPARAM)MAKELPARAM((UINT)(cxScreen), (UINT)(cyScreen))); }
BOOL _boxMsgCore::OnNCCreate(LPCREATESTRUCT lpCreateStruct) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_NCCREATE, 0L, (LPARAM)(LPCREATESTRUCT)(lpCreateStruct)); }
BOOL _boxMsgCore::SendNCCreate(LPCREATESTRUCT lpCreateStruct) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_NCCREATE, 0L, (LPARAM)(LPCREATESTRUCT)(lpCreateStruct)); }
void _boxMsgCore::OnNCDestroy() { (void)(DefaultMessage)((mhWnd), WM_NCDESTROY, 0L, 0L); }
void _boxMsgCore::SendNCDestroy() { (void)(::SendMessage)((mhWnd), WM_NCDESTROY, 0L, 0L); }
UINT _boxMsgCore::OnNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp) { return (UINT)(DWORD)(DefaultMessage)((mhWnd), WM_NCCALCSIZE, (WPARAM)(fCalcValidRects), (LPARAM)(NCCALCSIZE_PARAMS *)(lpcsp)); }
UINT _boxMsgCore::SendNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp) { return (UINT)(DWORD)(::SendMessage)((mhWnd), WM_NCCALCSIZE, (WPARAM)(fCalcValidRects), (LPARAM)(NCCALCSIZE_PARAMS *)(lpcsp)); }
UINT _boxMsgCore::OnNCHitTest(int x, int y) { return (UINT)(DWORD)(DefaultMessage)((mhWnd), WM_NCHITTEST, 0L, MAKELPARAM((x), (y))); }
UINT _boxMsgCore::SendNCHitTest(int x, int y) { return (UINT)(DWORD)(::SendMessage)((mhWnd), WM_NCHITTEST, 0L, MAKELPARAM((x), (y))); }
void _boxMsgCore::OnNCPaint(HRGN hrgn) { (void)(DefaultMessage)((mhWnd), WM_NCPAINT, (WPARAM)(HRGN)(hrgn), 0L); }
void _boxMsgCore::SendNCPaint(HRGN hrgn) { (void)(::SendMessage)((mhWnd), WM_NCPAINT, (WPARAM)(HRGN)(hrgn), 0L); }
BOOL _boxMsgCore::OnNCActivate(BOOL fActive, HWND hwndActDeact, BOOL fMinimized) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_NCACTIVATE, (WPARAM)(BOOL)(fActive), 0L); }
BOOL _boxMsgCore::SendNCActivate(BOOL fActive, HWND hwndActDeact, BOOL fMinimized) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_NCACTIVATE, (WPARAM)(BOOL)(fActive), 0L); }
UINT _boxMsgCore::OnGetDlgCode(LPMSG lpmsg) { return (UINT)(DWORD)(DefaultMessage)((mhWnd), WM_GETDLGCODE, (lpmsg ? lpmsg->wParam : 0), (LPARAM)(LPMSG)(lpmsg)); }
UINT _boxMsgCore::SendGetDlgCode(LPMSG lpmsg) { return (UINT)(DWORD)(::SendMessage)((mhWnd), WM_GETDLGCODE, (lpmsg ? lpmsg->wParam : 0), (LPARAM)(LPMSG)(lpmsg)); }
void _boxMsgCore::OnNCMouseMove(int x, int y, UINT codeHitTest) { (void)(DefaultMessage)((mhWnd), WM_NCMOUSEMOVE, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendNCMouseMove(int x, int y, UINT codeHitTest) { (void)(::SendMessage)((mhWnd), WM_NCMOUSEMOVE, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnNCLButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) { (void)(DefaultMessage)((mhWnd), (fDoubleClick) ? WM_NCLBUTTONDBLCLK : WM_NCLBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendNCLButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) { (void)(::SendMessage)((mhWnd), (fDoubleClick) ? WM_NCLBUTTONDBLCLK : WM_NCLBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnNCLButtonUp(int x, int y, UINT codeHitTest) { (void)(DefaultMessage)((mhWnd), WM_NCLBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendNCLButtonUp(int x, int y, UINT codeHitTest) { (void)(::SendMessage)((mhWnd), WM_NCLBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnNCRButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) { (void)(DefaultMessage)((mhWnd), (fDoubleClick) ? WM_NCRBUTTONDBLCLK : WM_NCRBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)) ); }
void _boxMsgCore::SendNCRButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) { (void)(::SendMessage)((mhWnd), (fDoubleClick) ? WM_NCRBUTTONDBLCLK : WM_NCRBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)) ); }
void _boxMsgCore::OnNCRButtonUp(int x, int y, UINT codeHitTest) { (void)(DefaultMessage)((mhWnd), WM_NCRBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)) ); }
void _boxMsgCore::SendNCRButtonUp(int x, int y, UINT codeHitTest) { (void)(::SendMessage)((mhWnd), WM_NCRBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)) ); }
void _boxMsgCore::OnNCMButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) { (void)(DefaultMessage)((mhWnd), (fDoubleClick) ? WM_NCMBUTTONDBLCLK : WM_NCMBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)) ); }
void _boxMsgCore::SendNCMButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) { (void)(::SendMessage)((mhWnd), (fDoubleClick) ? WM_NCMBUTTONDBLCLK : WM_NCMBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)) ); }
void _boxMsgCore::OnNCMButtonUp(int x, int y, UINT codeHitTest) { (void)(DefaultMessage)((mhWnd), WM_NCMBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)) ); }
void _boxMsgCore::SendNCMButtonUp(int x, int y, UINT codeHitTest) { (void)(::SendMessage)((mhWnd), WM_NCMBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)) ); }
void _boxMsgCore::OnKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_KEYDOWN, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags))); }
void _boxMsgCore::SendKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags) { (void)(::SendMessage)((mhWnd), WM_KEYDOWN, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags))); }
void _boxMsgCore::OnKeyUp(UINT vk, BOOL fDown, int cRepeat, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_KEYUP, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags))); }
void _boxMsgCore::SendKeyUp(UINT vk, BOOL fDown, int cRepeat, UINT flags) { (void)(::SendMessage)((mhWnd), WM_KEYUP, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags))); }
void _boxMsgCore::OnChar(TCHAR ch, int cRepeat) { (void)(DefaultMessage)((mhWnd), WM_CHAR, (WPARAM)(TCHAR)(ch), MAKELPARAM((cRepeat),0)); }
void _boxMsgCore::SendChar(TCHAR ch, int cRepeat) { (void)(::SendMessage)((mhWnd), WM_CHAR, (WPARAM)(TCHAR)(ch), MAKELPARAM((cRepeat),0)); }
void _boxMsgCore::OnDeadChar(TCHAR ch, int cRepeat) { (void)(DefaultMessage)((mhWnd), WM_DEADCHAR, (WPARAM)(TCHAR)(ch), MAKELPARAM((cRepeat),0)); }
void _boxMsgCore::SendDeadChar(TCHAR ch, int cRepeat) { (void)(::SendMessage)((mhWnd), WM_DEADCHAR, (WPARAM)(TCHAR)(ch), MAKELPARAM((cRepeat),0)); }
void _boxMsgCore::OnSysKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_SYSKEYDOWN, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags))); }
void _boxMsgCore::SendSysKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags) { (void)(::SendMessage)((mhWnd), WM_SYSKEYDOWN, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags))); }
void _boxMsgCore::OnSysKeyUp(UINT vk, BOOL fDown, int cRepeat, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_SYSKEYUP, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags))); }
void _boxMsgCore::SendSysKeyUp(UINT vk, BOOL fDown, int cRepeat, UINT flags) { (void)(::SendMessage)((mhWnd), WM_SYSKEYUP, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags))); }
void _boxMsgCore::OnSysChar(int ch, int cRepeat) { (void)(DefaultMessage)((mhWnd), WM_SYSCHAR, (WPARAM)(ch), (LPARAM)(cRepeat)); }
void _boxMsgCore::SendSysChar(int ch, int cRepeat) { (void)(::SendMessage)((mhWnd), WM_SYSCHAR, (WPARAM)(ch), (LPARAM)(cRepeat)); }
void _boxMsgCore::OnSysDeadChar(int ch, int cRepeat) { (void)(DefaultMessage)((mhWnd), WM_SYSDEADCHAR, (WPARAM)(ch), (LPARAM)(cRepeat)); }
void _boxMsgCore::SendSysDeadChar(int ch, int cRepeat) { (void)(::SendMessage)((mhWnd), WM_SYSDEADCHAR, (WPARAM)(ch), (LPARAM)(cRepeat)); }
BOOL _boxMsgCore::OnInitDialog(HWND hwndFocus, LPARAM lParam) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_INITDIALOG, (WPARAM)(HWND)(hwndFocus), (lParam)); }
BOOL _boxMsgCore::SendInitDialog(HWND hwndFocus, LPARAM lParam) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_INITDIALOG, (WPARAM)(HWND)(hwndFocus), (lParam)); }
void _boxMsgCore::OnCommand(int id, HWND hwndCtl, UINT codeNotify) { (void)(DefaultMessage)((mhWnd), WM_COMMAND, MAKEWPARAM((UINT)(id),(UINT)(codeNotify)), (LPARAM)(HWND)(hwndCtl)); }
void _boxMsgCore::SendCommand(int id, HWND hwndCtl, UINT codeNotify) { (void)(::SendMessage)((mhWnd), WM_COMMAND, MAKEWPARAM((UINT)(id),(UINT)(codeNotify)), (LPARAM)(HWND)(hwndCtl)); }
void _boxMsgCore::OnSysCommand(UINT cmd, int x, int y) { (void)(DefaultMessage)((mhWnd), WM_SYSCOMMAND, (WPARAM)(UINT)(cmd), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendSysCommand(UINT cmd, int x, int y) { (void)(::SendMessage)((mhWnd), WM_SYSCOMMAND, (WPARAM)(UINT)(cmd), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnTimer(UINT id) { (void)(DefaultMessage)((mhWnd), WM_TIMER, (WPARAM)(UINT)(id), 0L); }
void _boxMsgCore::SendTimer(UINT id) { (void)(::SendMessage)((mhWnd), WM_TIMER, (WPARAM)(UINT)(id), 0L); }
void _boxMsgCore::OnHScroll(HWND hwndCtl, UINT code, int pos) { (void)(DefaultMessage)((mhWnd), WM_HSCROLL, MAKEWPARAM((UINT)(int)(code),(UINT)(int)(pos)), (LPARAM)(HWND)(hwndCtl)); }
void _boxMsgCore::SendHScroll(HWND hwndCtl, UINT code, int pos) { (void)(::SendMessage)((mhWnd), WM_HSCROLL, MAKEWPARAM((UINT)(int)(code),(UINT)(int)(pos)), (LPARAM)(HWND)(hwndCtl)); }
void _boxMsgCore::OnVScroll(HWND hwndCtl, UINT code, int pos) { (void)(DefaultMessage)((mhWnd), WM_VSCROLL, MAKEWPARAM((UINT)(int)(code), (UINT)(int)(pos)), (LPARAM)(HWND)(hwndCtl)); }
void _boxMsgCore::SendVScroll(HWND hwndCtl, UINT code, int pos) { (void)(::SendMessage)((mhWnd), WM_VSCROLL, MAKEWPARAM((UINT)(int)(code), (UINT)(int)(pos)), (LPARAM)(HWND)(hwndCtl)); }
void _boxMsgCore::OnInitMenu(HMENU hMenu) { (void)(DefaultMessage)((mhWnd), WM_INITMENU, (WPARAM)(HMENU)(hMenu), 0L); }
void _boxMsgCore::SendInitMenu(HMENU hMenu) { (void)(::SendMessage)((mhWnd), WM_INITMENU, (WPARAM)(HMENU)(hMenu), 0L); }
void _boxMsgCore::OnInitMenuPopup(HMENU hMenu, UINT item, BOOL fSystemMenu) { (void)(DefaultMessage)((mhWnd), WM_INITMENUPOPUP, (WPARAM)(HMENU)(hMenu), MAKELPARAM((item),(fSystemMenu))); }
void _boxMsgCore::SendInitMenuPopup(HMENU hMenu, UINT item, BOOL fSystemMenu) { (void)(::SendMessage)((mhWnd), WM_INITMENUPOPUP, (WPARAM)(HMENU)(hMenu), MAKELPARAM((item),(fSystemMenu))); }
void _boxMsgCore::OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_MENUSELECT, MAKEWPARAM((item), (flags)), (LPARAM)(HMENU)((hmenu) ? (hmenu) : (hmenuPopup))); }
void _boxMsgCore::SendMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags) { (void)(::SendMessage)((mhWnd), WM_MENUSELECT, MAKEWPARAM((item), (flags)), (LPARAM)(HMENU)((hmenu) ? (hmenu) : (hmenuPopup))); }
DWORD _boxMsgCore::OnMenuChar(UINT ch, UINT flags, HMENU hmenu) { return (DWORD)(DefaultMessage)((mhWnd), WM_MENUCHAR, MAKEWPARAM(flags, (WORD)(ch)), (LPARAM)(HMENU)(hmenu)); }
DWORD _boxMsgCore::SendMenuChar(UINT ch, UINT flags, HMENU hmenu) { return (DWORD)(::SendMessage)((mhWnd), WM_MENUCHAR, MAKEWPARAM(flags, (WORD)(ch)), (LPARAM)(HMENU)(hmenu)); }
void _boxMsgCore::OnEnterIdle(UINT source, HWND hwndSource) { (void)(DefaultMessage)((mhWnd), WM_ENTERIDLE, (WPARAM)(UINT)(source), (LPARAM)(HWND)(hwndSource)); }
void _boxMsgCore::SendEnterIdle(UINT source, HWND hwndSource) { (void)(::SendMessage)((mhWnd), WM_ENTERIDLE, (WPARAM)(UINT)(source), (LPARAM)(HWND)(hwndSource)); }
HBRUSH _boxMsgCore::OnCtlColorMsgBox(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(DefaultMessage)((mhWnd), WM_CTLCOLORMSGBOX, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::SendCtlColorMsgBox(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(::SendMessage)((mhWnd), WM_CTLCOLORMSGBOX, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::OnCtlColorEditr(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(DefaultMessage)((mhWnd), WM_CTLCOLOREDIT, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::SendCtlColorEditr(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(::SendMessage)((mhWnd), WM_CTLCOLOREDIT, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::OnCtlColorListBox(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(DefaultMessage)((mhWnd), WM_CTLCOLORLISTBOX, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::SendCtlColorListBox(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(::SendMessage)((mhWnd), WM_CTLCOLORLISTBOX, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::OnCtlColorBtn(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(DefaultMessage)((mhWnd), WM_CTLCOLORBTN, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::SendCtlColorBtn(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(::SendMessage)((mhWnd), WM_CTLCOLORBTN, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::OnCtlColorDlg(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(DefaultMessage)((mhWnd), WM_CTLCOLORDLG, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::SendCtlColorDlg(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(::SendMessage)((mhWnd), WM_CTLCOLORDLG, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::OnCtlColorScrollBar(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(DefaultMessage)((mhWnd), WM_CTLCOLORSCROLLBAR, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::SendCtlColorScrollBar(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(::SendMessage)((mhWnd), WM_CTLCOLORSCROLLBAR, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::OnCtlColorStatic(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(DefaultMessage)((mhWnd), WM_CTLCOLORSTATIC, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
HBRUSH _boxMsgCore::SendCtlColorStatic(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(UINT_PTR)(::SendMessage)((mhWnd), WM_CTLCOLORSTATIC, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild)); }
void _boxMsgCore::OnMouseMove(int x, int y, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_MOUSEMOVE, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendMouseMove(int x, int y, UINT flags) { (void)(::SendMessage)((mhWnd), WM_MOUSEMOVE, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnLButtonDown(BOOL fDoubleClick, int x, int y, UINT flags) { (void)(DefaultMessage)((mhWnd), (fDoubleClick) ? WM_LBUTTONDBLCLK : WM_LBUTTONDOWN, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendLButtonDown(BOOL fDoubleClick, int x, int y, UINT flags) { (void)(::SendMessage)((mhWnd), (fDoubleClick) ? WM_LBUTTONDBLCLK : WM_LBUTTONDOWN, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnLButtonUp(int x, int y, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_LBUTTONUP, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendLButtonUp(int x, int y, UINT flags) { (void)(::SendMessage)((mhWnd), WM_LBUTTONUP, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnRButtonDown(BOOL fDoubleClick, int x, int y, UINT flags) { (void)(DefaultMessage)((mhWnd), (fDoubleClick) ? WM_RBUTTONDBLCLK : WM_RBUTTONDOWN, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendRButtonDown(BOOL fDoubleClick, int x, int y, UINT flags) { (void)(::SendMessage)((mhWnd), (fDoubleClick) ? WM_RBUTTONDBLCLK : WM_RBUTTONDOWN, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnRButtonUp(int x, int y, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_RBUTTONUP, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendRButtonUp(int x, int y, UINT flags) { (void)(::SendMessage)((mhWnd), WM_RBUTTONUP, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnMButtonDown(BOOL fDoubleClick, int x, int y, UINT flags) { (void)(DefaultMessage)((mhWnd), (fDoubleClick) ? WM_MBUTTONDBLCLK : WM_MBUTTONDOWN, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendMButtonDown(BOOL fDoubleClick, int x, int y, UINT flags) { (void)(::SendMessage)((mhWnd), (fDoubleClick) ? WM_MBUTTONDBLCLK : WM_MBUTTONDOWN, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnMButtonUp(int x, int y, UINT flags) { (void)(DefaultMessage)((mhWnd), WM_MBUTTONUP, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::SendMButtonUp(int x, int y, UINT flags) { (void)(::SendMessage)((mhWnd), WM_MBUTTONUP, (WPARAM)(UINT)(flags), MAKELPARAM((x), (y))); }
void _boxMsgCore::OnMouseWheel(int x, int y, int zDelta, UINT fwKeys) { (void)(DefaultMessage)((mhWnd), WM_MOUSEWHEEL, MAKEWPARAM((fwKeys),(zDelta)), MAKELPARAM((x),(y))); }
void _boxMsgCore::SendMouseWheel(int x, int y, int zDelta, UINT fwKeys) { (void)(::SendMessage)((mhWnd), WM_MOUSEWHEEL, MAKEWPARAM((fwKeys),(zDelta)), MAKELPARAM((x),(y))); }
void _boxMsgCore::OnParentNotify(UINT msg, HWND hwndChild, int idChild) { (void)(DefaultMessage)((mhWnd), WM_PARENTNOTIFY, MAKEWPARAM(msg, idChild), (LPARAM)(hwndChild)); }
void _boxMsgCore::SendParentNotify(UINT msg, HWND hwndChild, int idChild) { (void)(::SendMessage)((mhWnd), WM_PARENTNOTIFY, MAKEWPARAM(msg, idChild), (LPARAM)(hwndChild)); }
void _boxMsgCore::OnSizing(UINT nSide, LPRECT lpRect) { (void)(DefaultMessage)((mhWnd), WM_SIZING, (WPARAM)(nSide), (WPARAM)(lpRect)); }
void _boxMsgCore::SendSizing(UINT nSide, LPRECT lpRect) { (void)(::SendMessage)((mhWnd), WM_SIZING, (WPARAM)(nSide), (WPARAM)(lpRect)); }
void _boxMsgCore::OnMoving(UINT nSide, LPRECT lpRect) { (void)(DefaultMessage)((mhWnd), WM_MOVING, (WPARAM)(nSide), (WPARAM)(lpRect)); }
void _boxMsgCore::SendMoving(UINT nSide, LPRECT lpRect) { (void)(::SendMessage)((mhWnd), WM_MOVING, (WPARAM)(nSide), (WPARAM)(lpRect)); }
BOOL _boxMsgCore::OnDeviceChange(UINT uEvent, DWORD dwEventData) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_DEVICECHANGE, (WPARAM)(UINT)(uEvent), (LPARAM)(DWORD)(dwEventData)); }
BOOL _boxMsgCore::SendDeviceChange(UINT uEvent, DWORD dwEventData) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_DEVICECHANGE, (WPARAM)(UINT)(uEvent), (LPARAM)(DWORD)(dwEventData)); }
HWND _boxMsgCore::MDICreate(const LPMDICREATESTRUCT lpmcs) { return (HWND)(DefaultMessage)((mhWnd), WM_MDICREATE, 0L, (LPARAM)(LPMDICREATESTRUCT)(lpmcs)); }
HWND _boxMsgCore::SendMDICreate(const LPMDICREATESTRUCT lpmcs) { return (HWND)(::SendMessage)((mhWnd), WM_MDICREATE, 0L, (LPARAM)(LPMDICREATESTRUCT)(lpmcs)); }
void _boxMsgCore::MDIDestroy(HWND hwndDestroy) { (void)(DefaultMessage)((mhWnd), WM_MDIDESTROY, (WPARAM)(hwndDestroy), 0L); }
void _boxMsgCore::SendMDIDestroy(HWND hwndDestroy) { (void)(::SendMessage)((mhWnd), WM_MDIDESTROY, (WPARAM)(hwndDestroy), 0L); }
void _boxMsgCore::MDIActivate(BOOL fActive, HWND hwndActivate, HWND hwndDeactivate) { (void)(DefaultMessage)(mhWnd, WM_MDIACTIVATE, (WPARAM)(hwndDeactivate), (LPARAM)(hwndActivate)); }
void _boxMsgCore::SendMDIActivate(BOOL fActive, HWND hwndActivate, HWND hwndDeactivate) { (void)(::SendMessage)(mhWnd, WM_MDIACTIVATE, (WPARAM)(hwndDeactivate), (LPARAM)(hwndActivate)); }
void _boxMsgCore::MDIRestore(HWND hwndRestore) { (void)(DefaultMessage)((mhWnd), WM_MDIRESTORE, (WPARAM)(hwndRestore), 0L); }
void _boxMsgCore::SendMDIRestore(HWND hwndRestore) { (void)(::SendMessage)((mhWnd), WM_MDIRESTORE, (WPARAM)(hwndRestore), 0L); }
HWND _boxMsgCore::MDINext(HWND hwndCur, BOOL fPrev) { return (HWND)(UINT_PTR)(DefaultMessage)((mhWnd), WM_MDINEXT, (WPARAM)(hwndCur), (LPARAM)(fPrev)); }
HWND _boxMsgCore::SendMDINext(HWND hwndCur, BOOL fPrev) { return (HWND)(UINT_PTR)(::SendMessage)((mhWnd), WM_MDINEXT, (WPARAM)(hwndCur), (LPARAM)(fPrev)); }
void _boxMsgCore::MDIMaximize(HWND hwndMaximize) { (void)(DefaultMessage)((mhWnd), WM_MDIMAXIMIZE, (WPARAM)(hwndMaximize), 0L); }
void _boxMsgCore::SendMDIMaximize(HWND hwndMaximize) { (void)(::SendMessage)((mhWnd), WM_MDIMAXIMIZE, (WPARAM)(hwndMaximize), 0L); }
BOOL _boxMsgCore::MDITile(UINT cmd) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_MDITILE, (WPARAM)(cmd), 0L); }
BOOL _boxMsgCore::SendMDITile(UINT cmd) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_MDITILE, (WPARAM)(cmd), 0L); }
BOOL _boxMsgCore::MDICascade(UINT cmd) { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_MDICASCADE, (WPARAM)(cmd), 0L); }
BOOL _boxMsgCore::SendMDICascade(UINT cmd) { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_MDICASCADE, (WPARAM)(cmd), 0L); }
void _boxMsgCore::MDIIconArrange() { (void)(DefaultMessage)((mhWnd), WM_MDIICONARRANGE, 0L, 0L); }
void _boxMsgCore::SendMDIIconArrange() { (void)(::SendMessage)((mhWnd), WM_MDIICONARRANGE, 0L, 0L); }
HWND _boxMsgCore::MDIGetActive() { return (HWND)(UINT_PTR)(DefaultMessage)((mhWnd), WM_MDIGETACTIVE, 0L, 0L); }
HWND _boxMsgCore::SendMDIGetActive() { return (HWND)(UINT_PTR)(::SendMessage)((mhWnd), WM_MDIGETACTIVE, 0L, 0L); }
HMENU _boxMsgCore::MDISetMenu(BOOL fRefresh, HMENU hmenuFrame, HMENU hmenuWindow) { return (HMENU)(UINT_PTR)(DefaultMessage)((mhWnd), WM_MDISETMENU, (WPARAM)((fRefresh) ? (hmenuFrame) : 0), (LPARAM)(hmenuWindow)); }
HMENU _boxMsgCore::SendMDISetMenu(BOOL fRefresh, HMENU hmenuFrame, HMENU hmenuWindow) { return (HMENU)(UINT_PTR)(::SendMessage)((mhWnd), WM_MDISETMENU, (WPARAM)((fRefresh) ? (hmenuFrame) : 0), (LPARAM)(hmenuWindow)); }
void _boxMsgCore::OnDropFiles(HDROP hdrop) { (void)(DefaultMessage)((mhWnd), WM_DROPFILES, (WPARAM)(HDROP)(hdrop), 0L); }
void _boxMsgCore::SendDropFiles(HDROP hdrop) { (void)(::SendMessage)((mhWnd), WM_DROPFILES, (WPARAM)(HDROP)(hdrop), 0L); }
void _boxMsgCore::OnCut() { (void)(DefaultMessage)((mhWnd), WM_CUT, 0L, 0L); }
void _boxMsgCore::SendCut() { (void)(::SendMessage)((mhWnd), WM_CUT, 0L, 0L); }
void _boxMsgCore::OnCopy() { (void)(DefaultMessage)((mhWnd), WM_COPY, 0L, 0L); }
void _boxMsgCore::SendCopy() { (void)(::SendMessage)((mhWnd), WM_COPY, 0L, 0L); }
void _boxMsgCore::OnPaste() { (void)(DefaultMessage)((mhWnd), WM_PASTE, 0L, 0L); }
void _boxMsgCore::SendPaste() { (void)(::SendMessage)((mhWnd), WM_PASTE, 0L, 0L); }
void _boxMsgCore::OnClear() { (void)(DefaultMessage)((mhWnd), WM_CLEAR, 0L, 0L); }
void _boxMsgCore::SendClear() { (void)(::SendMessage)((mhWnd), WM_CLEAR, 0L, 0L); }
void _boxMsgCore::OnUndo() { (void)(DefaultMessage)((mhWnd), WM_UNDO, 0L, 0L); }
void _boxMsgCore::SendUndo() { (void)(::SendMessage)((mhWnd), WM_UNDO, 0L, 0L); }
HANDLE _boxMsgCore::OnRenderFormat(UINT fmt) { return (HANDLE)(UINT_PTR)(DefaultMessage)((mhWnd), WM_RENDERFORMAT, (WPARAM)(UINT)(fmt), 0L); }
HANDLE _boxMsgCore::SendRenderFormat(UINT fmt) { return (HANDLE)(UINT_PTR)(::SendMessage)((mhWnd), WM_RENDERFORMAT, (WPARAM)(UINT)(fmt), 0L); }
void _boxMsgCore::OnRenderAllFormats() { (void)(DefaultMessage)((mhWnd), WM_RENDERALLFORMATS, 0L, 0L); }
void _boxMsgCore::SendRenderAllFormats() { (void)(::SendMessage)((mhWnd), WM_RENDERALLFORMATS, 0L, 0L); }
void _boxMsgCore::OnDestroyClipboard() { (void)(DefaultMessage)((mhWnd), WM_DESTROYCLIPBOARD, 0L, 0L); }
void _boxMsgCore::SendDestroyClipboard() { (void)(::SendMessage)((mhWnd), WM_DESTROYCLIPBOARD, 0L, 0L); }
void _boxMsgCore::OnDrawClipboard() { (void)(DefaultMessage)((mhWnd), WM_DRAWCLIPBOARD, 0L, 0L); }
void _boxMsgCore::SendDrawClipboard() { (void)(::SendMessage)((mhWnd), WM_DRAWCLIPBOARD, 0L, 0L); }
void _boxMsgCore::OnPaintClipboard(HWND hwndCBViewer, const LPPAINTSTRUCT lpPaintStruct) { (void)(DefaultMessage)((mhWnd), WM_PAINTCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), (LPARAM)(LPPAINTSTRUCT)(lpPaintStruct)); }
void _boxMsgCore::SendPaintClipboard(HWND hwndCBViewer, const LPPAINTSTRUCT lpPaintStruct) { (void)(::SendMessage)((mhWnd), WM_PAINTCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), (LPARAM)(LPPAINTSTRUCT)(lpPaintStruct)); }
void _boxMsgCore::OnVScrollClipboard(HWND hwndCBViewer, UINT code, int pos) { (void)(DefaultMessage)((mhWnd), WM_VSCROLLCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), MAKELPARAM((code), (pos))); }
void _boxMsgCore::SendVScrollClipboard(HWND hwndCBViewer, UINT code, int pos) { (void)(::SendMessage)((mhWnd), WM_VSCROLLCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), MAKELPARAM((code), (pos))); }
void _boxMsgCore::OnSizeClipboard(HWND hwndCBViewer, const LPRECT lprc) { (void)(DefaultMessage)((mhWnd), WM_SIZECLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), (LPARAM)(LPRECT)(lprc)); }
void _boxMsgCore::SendSizeClipboard(HWND hwndCBViewer, const LPRECT lprc) { (void)(::SendMessage)((mhWnd), WM_SIZECLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), (LPARAM)(LPRECT)(lprc)); }
void _boxMsgCore::OnAskCBFormatName(int cchMax, LPTSTR rgchName) { (void)(DefaultMessage)((mhWnd), WM_ASKCBFORMATNAME, (WPARAM)(int)(cchMax), (LPARAM)(rgchName)); }
void _boxMsgCore::SendAskCBFormatName(int cchMax, LPTSTR rgchName) { (void)(::SendMessage)((mhWnd), WM_ASKCBFORMATNAME, (WPARAM)(int)(cchMax), (LPARAM)(rgchName)); }
void _boxMsgCore::OnChangeCBChain(HWND hwndRemove, HWND hwndNext) { (void)(DefaultMessage)((mhWnd), WM_CHANGECBCHAIN, (WPARAM)(HWND)(hwndRemove), (LPARAM)(HWND)(hwndNext)); }
void _boxMsgCore::SendChangeCBChain(HWND hwndRemove, HWND hwndNext) { (void)(::SendMessage)((mhWnd), WM_CHANGECBCHAIN, (WPARAM)(HWND)(hwndRemove), (LPARAM)(HWND)(hwndNext)); }
void _boxMsgCore::OnHScrollClipboard(HWND hwndCBViewer, UINT code, int pos) { (void)(DefaultMessage)((mhWnd), WM_HSCROLLCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), MAKELPARAM((code), (pos))); }
void _boxMsgCore::SendHScrollClipboard(HWND hwndCBViewer, UINT code, int pos) { (void)(::SendMessage)((mhWnd), WM_HSCROLLCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), MAKELPARAM((code), (pos))); }
BOOL _boxMsgCore::OnQueryNewPalette() { return (BOOL)(DWORD)(DefaultMessage)((mhWnd), WM_QUERYNEWPALETTE, 0L, 0L); }
BOOL _boxMsgCore::SendQueryNewPalette() { return (BOOL)(DWORD)(::SendMessage)((mhWnd), WM_QUERYNEWPALETTE, 0L, 0L); }
void _boxMsgCore::OnPaletteIsChanging(HWND hwndPaletteChange) { (void)(DefaultMessage)((mhWnd), WM_PALETTEISCHANGING, (WPARAM)(HWND)(hwndPaletteChange), 0L); }
void _boxMsgCore::SendPaletteIsChanging(HWND hwndPaletteChange) { (void)(::SendMessage)((mhWnd), WM_PALETTEISCHANGING, (WPARAM)(HWND)(hwndPaletteChange), 0L); }
void _boxMsgCore::OnPaletteChanged(HWND hwndPaletteChange) { (void)(DefaultMessage)((mhWnd), WM_PALETTECHANGED, (WPARAM)(HWND)(hwndPaletteChange), 0L); }
void _boxMsgCore::SendPaletteChanged(HWND hwndPaletteChange) { (void)(::SendMessage)((mhWnd), WM_PALETTECHANGED, (WPARAM)(HWND)(hwndPaletteChange), 0L); }
void _boxMsgCore::OnHotKey(int idHotKey, UINT fuModifiers, UINT vk) { (void)(DefaultMessage)((mhWnd), WM_HOTKEY, (WPARAM)(idHotKey), MAKELPARAM((fuModifiers), (vk))); }
void _boxMsgCore::SendHotKey(int idHotKey, UINT fuModifiers, UINT vk) { (void)(::SendMessage)((mhWnd), WM_HOTKEY, (WPARAM)(idHotKey), MAKELPARAM((fuModifiers), (vk))); }
