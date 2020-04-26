virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct); // [WM_CREATE]
BOOL SendCreate(LPCREATESTRUCT lpCreateStruct);
virtual void OnDestroy(); // [WM_DESTROY]
void SendDestroy();
virtual void OnMove(int x, int y); // [WM_MOVE]
void SendMove(int x, int y);
virtual void OnSize(UINT state, int cx, int cy); // [WM_SIZE]
void SendSize(UINT state, int cx, int cy);
virtual void OnActivate(UINT state, HWND hwndActDeact, BOOL fMinimized); // [WM_ACTIVATE]
void SendActivate(UINT state, HWND hwndActDeact, BOOL fMinimized);
virtual void OnSetFocus(HWND hwndOldFocus); // [WM_SETFOCUS]
void SendSetFocus(HWND hwndOldFocus);
virtual void OnKillFocus(HWND hwndNewFocus); // [WM_KILLFOCUS]
void SendKillFocus(HWND hwndNewFocus);
virtual void OnEnable(BOOL fEnable); // [WM_ENABLE]
void SendEnable(BOOL fEnable);
virtual void OnSetRedraw(BOOL fRedraw); // [WM_SETREDRAW]
void SendSetRedraw(BOOL fRedraw);
virtual void OnSetText(LPCTSTR lpszText); // [WM_SETTEXT]
void SendSetText(LPCTSTR lpszText);
virtual INT OnGetText(int cchTextMax, LPTSTR lpszText); // [WM_GETTEXT]
INT SendGetText(int cchTextMax, LPTSTR lpszText);
virtual INT OnGetTextLength(); // [WM_GETTEXTLENGTH]
INT SendGetTextLength();
virtual void OnPaint(); // [WM_PAINT]
void SendPaint();
virtual void OnClose(); // [WM_CLOSE]
void SendClose();
virtual BOOL OnQueryEndSession(); // [WM_QUERYENDSESSION]
BOOL SendQueryEndSession();
virtual BOOL OnQueryOpen(); // [WM_QUERYOPEN]
BOOL SendQueryOpen();
virtual void OnEndSession(BOOL fEnding); // [WM_ENDSESSION]
void SendEndSession(BOOL fEnding);
virtual void OnQuit(int exitCode); // [WM_QUIT]
void SendQuit(int exitCode);
virtual BOOL OnEraseBkgnd(HDC hdc); // [WM_ERASEBKGND]
BOOL SendEraseBkgnd(HDC hdc);
virtual void OnSysColorChange(); // [WM_SYSCOLORCHANGE]
void SendSysColorChange();
virtual void OnShowWindow(BOOL fShow, UINT status); // [WM_SHOWWINDOW]
void SendShowWindow(BOOL fShow, UINT status);
virtual void OnWinIniChange(LPCTSTR lpszSectionName); // [WM_WININICHANGE]
void SendWinIniChange(LPCTSTR lpszSectionName);
virtual void OnDevModeChange(LPCTSTR lpszDeviceName); // [WM_DEVMODECHANGE]
void SendDevModeChange(LPCTSTR lpszDeviceName);
virtual void OnActivateApp(BOOL fActivate, DWORD dwThreadId); // [WM_ACTIVATEAPP]
void SendActivateApp(BOOL fActivate, DWORD dwThreadId);
virtual void OnFontChange(); // [WM_FONTCHANGE]
void SendFontChange();
virtual void OnTimeChange(); // [WM_TIMECHANGE]
void SendTimeChange();
virtual void OnCancelMode(); // [WM_CANCELMODE]
void SendCancelMode();
virtual BOOL OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg); // [WM_SETCURSOR]
BOOL SendSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg);
virtual int OnMouseActivate(HWND hwndTopLevel, UINT codeHitTest, UINT msg); // [WM_MOUSEACTIVATE]
int SendMouseActivate(HWND hwndTopLevel, UINT codeHitTest, UINT msg);
virtual void OnChildActivate(); // [WM_CHILDACTIVATE]
void SendChildActivate();
virtual void OnQueueSync(); // [WM_QUEUESYNC]
void SendQueueSync();
virtual void OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo); // [WM_GETMINMAXINFO]
void SendGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo);
virtual BOOL OnIconEraseBkgnd(HDC hdc); // [WM_ICONERASEBKGND]
BOOL SendIconEraseBkgnd(HDC hdc);
virtual HWND OnNextDlgCtl(HWND hwndSetFocus, BOOL fNext); // [WM_NEXTDLGCTL]
HWND SendNextDlgCtl(HWND hwndSetFocus, BOOL fNext);
virtual void OnSpoolerStatus(UINT status, int cJobInQueue); // [WM_SPOOLERSTATUS]
void SendSpoolerStatus(UINT status, int cJobInQueue);
virtual void OnDrawItem(const DRAWITEMSTRUCT * lpDrawItem); // [WM_DRAWITEM]
void SendDrawItem(const DRAWITEMSTRUCT * lpDrawItem);
virtual void OnMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem); // [WM_MEASUREITEM]
void SendMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem);
virtual void OnDeleteItem(const DELETEITEMSTRUCT * lpDeleteItem); // [WM_DELETEITEM]
void SendDeleteItem(const DELETEITEMSTRUCT * lpDeleteItem);
virtual int OnVkeyToItem(UINT vk, HWND hwndListbox, int iCaret); // [WM_VKEYTOITEM]
int SendVkeyToItem(UINT vk, HWND hwndListbox, int iCaret);
virtual int OnCharToItem(UINT ch, HWND hwndListbox, int iCaret); // [WM_CHARTOITEM]
int SendCharToItem(UINT ch, HWND hwndListbox, int iCaret);
virtual void OnSetFont(HFONT hfont, BOOL fRedraw); // [WM_SETFONT]
void SendSetFont(HFONT hfont, BOOL fRedraw);
virtual HFONT OnGetFont(); // [WM_GETFONT]
HFONT SendGetFont();
virtual HICON OnQueryDragIcon(); // [WM_QUERYDRAGICON]
HICON SendQueryDragIcon();
virtual int OnCompareItem(const COMPAREITEMSTRUCT * lpCompareItem); // [WM_COMPAREITEM]
int SendCompareItem(const COMPAREITEMSTRUCT * lpCompareItem);
virtual void OnCompacting(UINT compactRatio); // [WM_COMPACTING]
void SendCompacting(UINT compactRatio);
virtual void OnCommNotify(int cid, UINT flags); // [WM_COMMNOTIFY]
void SendCommNotify(int cid, UINT flags);
virtual BOOL OnWindowPosChanging(LPWINDOWPOS lpwpos); // [WM_WINDOWPOSCHANGING]
BOOL SendWindowPosChanging(LPWINDOWPOS lpwpos);
virtual void OnWindowPosChanged(const LPWINDOWPOS lpwpos); // [WM_WINDOWPOSCHANGED]
void SendWindowPosChanged(const LPWINDOWPOS lpwpos);
virtual void OnPower(int code); // [WM_POWER]
void SendPower(int code);
virtual void OnContextMenu(HWND hwndContext, UINT xPos, UINT yPos); // [WM_CONTEXTMENU]
void SendContextMenu(HWND hwndContext, UINT xPos, UINT yPos);
virtual void OnDisplayChange(UINT bitsPerPixel, UINT cxScreen, UINT cyScreen); // [WM_DISPLAYCHANGE]
void SendDisplayChange(UINT bitsPerPixel, UINT cxScreen, UINT cyScreen);
virtual BOOL OnNCCreate(LPCREATESTRUCT lpCreateStruct); // [WM_NCCREATE]
BOOL SendNCCreate(LPCREATESTRUCT lpCreateStruct);
virtual void OnNCDestroy(); // [WM_NCDESTROY]
void SendNCDestroy();
virtual UINT OnNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp); // [WM_NCCALCSIZE]
UINT SendNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp);
virtual UINT OnNCHitTest(int x, int y); // [WM_NCHITTEST]
UINT SendNCHitTest(int x, int y);
virtual void OnNCPaint(HRGN hrgn); // [WM_NCPAINT]
void SendNCPaint(HRGN hrgn);
virtual BOOL OnNCActivate(BOOL fActive, HWND hwndActDeact, BOOL fMinimized); // [WM_NCACTIVATE]
BOOL SendNCActivate(BOOL fActive, HWND hwndActDeact, BOOL fMinimized);
virtual UINT OnGetDlgCode(LPMSG lpmsg); // [WM_GETDLGCODE]
UINT SendGetDlgCode(LPMSG lpmsg);
virtual void OnNCMouseMove(int x, int y, UINT codeHitTest); // [WM_NCMOUSEMOVE]
void SendNCMouseMove(int x, int y, UINT codeHitTest);
virtual void OnNCLButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest); // [WM_NCLBUTTONDOWN]
void SendNCLButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest);
virtual void OnNCLButtonUp(int x, int y, UINT codeHitTest); // [WM_NCLBUTTONUP]
void SendNCLButtonUp(int x, int y, UINT codeHitTest);
virtual void OnNCRButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest); // [WM_NCRBUTTONDOWN]
void SendNCRButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest);
virtual void OnNCRButtonUp(int x, int y, UINT codeHitTest); // [WM_NCRBUTTONUP]
void SendNCRButtonUp(int x, int y, UINT codeHitTest);
virtual void OnNCMButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest); // [WM_NCMBUTTONDOWN]
void SendNCMButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest);
virtual void OnNCMButtonUp(int x, int y, UINT codeHitTest); // [WM_NCMBUTTONUP]
void SendNCMButtonUp(int x, int y, UINT codeHitTest);
virtual void OnKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags); // [WM_KEYDOWN]
void SendKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags);
virtual void OnKeyUp(UINT vk, BOOL fDown, int cRepeat, UINT flags); // [WM_KEYUP]
void SendKeyUp(UINT vk, BOOL fDown, int cRepeat, UINT flags);
virtual void OnChar(TCHAR ch, int cRepeat); // [WM_CHAR]
void SendChar(TCHAR ch, int cRepeat);
virtual void OnDeadChar(TCHAR ch, int cRepeat); // [WM_DEADCHAR]
void SendDeadChar(TCHAR ch, int cRepeat);
virtual void OnSysKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags); // [WM_SYSKEYDOWN]
void SendSysKeyDown(UINT vk, BOOL fDown, int cRepeat, UINT flags);
virtual void OnSysKeyUp(UINT vk, BOOL fDown, int cRepeat, UINT flags); // [WM_SYSKEYUP]
void SendSysKeyUp(UINT vk, BOOL fDown, int cRepeat, UINT flags);
virtual void OnSysChar(int ch, int cRepeat); // [WM_SYSCHAR]
void SendSysChar(int ch, int cRepeat);
virtual void OnSysDeadChar(int ch, int cRepeat); // [WM_SYSDEADCHAR]
void SendSysDeadChar(int ch, int cRepeat);
virtual BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam); // [WM_INITDIALOG]
BOOL SendInitDialog(HWND hwndFocus, LPARAM lParam);
virtual void OnCommand(int id, HWND hwndCtl, UINT codeNotify); // [WM_COMMAND]
void SendCommand(int id, HWND hwndCtl, UINT codeNotify);
virtual void OnSysCommand(UINT cmd, int x, int y); // [WM_SYSCOMMAND]
void SendSysCommand(UINT cmd, int x, int y);
virtual void OnTimer(UINT id); // [WM_TIMER]
void SendTimer(UINT id);
virtual void OnHScroll(HWND hwndCtl, UINT code, int pos); // [WM_HSCROLL]
void SendHScroll(HWND hwndCtl, UINT code, int pos);
virtual void OnVScroll(HWND hwndCtl, UINT code, int pos); // [WM_VSCROLL]
void SendVScroll(HWND hwndCtl, UINT code, int pos);
virtual void OnInitMenu(HMENU hMenu); // [WM_INITMENU]
void SendInitMenu(HMENU hMenu);
virtual void OnInitMenuPopup(HMENU hMenu, UINT item, BOOL fSystemMenu); // [WM_INITMENUPOPUP]
void SendInitMenuPopup(HMENU hMenu, UINT item, BOOL fSystemMenu);
virtual void OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags); // [WM_MENUSELECT]
void SendMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);
virtual DWORD OnMenuChar(UINT ch, UINT flags, HMENU hmenu); // [WM_MENUCHAR]
DWORD SendMenuChar(UINT ch, UINT flags, HMENU hmenu);
virtual void OnEnterIdle(UINT source, HWND hwndSource); // [WM_ENTERIDLE]
void SendEnterIdle(UINT source, HWND hwndSource);
virtual HBRUSH OnCtlColorMsgBox(HDC hdc, HWND hwndChild, int type); // [WM_CTLCOLORMSGBOX]
HBRUSH SendCtlColorMsgBox(HDC hdc, HWND hwndChild, int type);
virtual HBRUSH OnCtlColorEditr(HDC hdc, HWND hwndChild, int type); // [WM_CTLCOLOREDIT]
HBRUSH SendCtlColorEditr(HDC hdc, HWND hwndChild, int type);
virtual HBRUSH OnCtlColorListBox(HDC hdc, HWND hwndChild, int type); // [WM_CTLCOLORLISTBOX]
HBRUSH SendCtlColorListBox(HDC hdc, HWND hwndChild, int type);
virtual HBRUSH OnCtlColorBtn(HDC hdc, HWND hwndChild, int type); // [WM_CTLCOLORBTN]
HBRUSH SendCtlColorBtn(HDC hdc, HWND hwndChild, int type);
virtual HBRUSH OnCtlColorDlg(HDC hdc, HWND hwndChild, int type); // [WM_CTLCOLORDLG]
HBRUSH SendCtlColorDlg(HDC hdc, HWND hwndChild, int type);
virtual HBRUSH OnCtlColorScrollBar(HDC hdc, HWND hwndChild, int type); // [WM_CTLCOLORSCROLLBAR]
HBRUSH SendCtlColorScrollBar(HDC hdc, HWND hwndChild, int type);
virtual HBRUSH OnCtlColorStatic(HDC hdc, HWND hwndChild, int type); // [WM_CTLCOLORSTATIC]
HBRUSH SendCtlColorStatic(HDC hdc, HWND hwndChild, int type);
virtual void OnMouseMove(int x, int y, UINT flags); // [WM_MOUSEMOVE]
void SendMouseMove(int x, int y, UINT flags);
virtual void OnLButtonDown(BOOL fDoubleClick, int x, int y, UINT flags); // [WM_LBUTTONDOWN]
void SendLButtonDown(BOOL fDoubleClick, int x, int y, UINT flags);
virtual void OnLButtonUp(int x, int y, UINT flags); // [WM_LBUTTONUP]
void SendLButtonUp(int x, int y, UINT flags);
virtual void OnRButtonDown(BOOL fDoubleClick, int x, int y, UINT flags); // [WM_RBUTTONDOWN]
void SendRButtonDown(BOOL fDoubleClick, int x, int y, UINT flags);
virtual void OnRButtonUp(int x, int y, UINT flags); // [WM_RBUTTONUP]
void SendRButtonUp(int x, int y, UINT flags);
virtual void OnMButtonDown(BOOL fDoubleClick, int x, int y, UINT flags); // [WM_MBUTTONDOWN]
void SendMButtonDown(BOOL fDoubleClick, int x, int y, UINT flags);
virtual void OnMButtonUp(int x, int y, UINT flags); // [WM_MBUTTONUP]
void SendMButtonUp(int x, int y, UINT flags);
virtual void OnMouseWheel(int x, int y, int zDelta, UINT fwKeys); // [WM_MOUSEWHEEL]
void SendMouseWheel(int x, int y, int zDelta, UINT fwKeys);
virtual void OnParentNotify(UINT msg, HWND hwndChild, int idChild); // [WM_PARENTNOTIFY]
void SendParentNotify(UINT msg, HWND hwndChild, int idChild);
virtual void OnSizing(UINT nSide, LPRECT lpRect); // [WM_SIZING]
void SendSizing(UINT nSide, LPRECT lpRect);
virtual void OnMoving(UINT nSide, LPRECT lpRect); // [WM_MOVING]
void SendMoving(UINT nSide, LPRECT lpRect);
virtual BOOL OnDeviceChange(UINT uEvent, DWORD dwEventData); // [WM_DEVICECHANGE]
BOOL SendDeviceChange(UINT uEvent, DWORD dwEventData);
virtual HWND MDICreate(const LPMDICREATESTRUCT lpmcs); // [WM_MDICREATE]
HWND SendMDICreate(const LPMDICREATESTRUCT lpmcs);
virtual void MDIDestroy(HWND hwndDestroy); // [WM_MDIDESTROY]
void SendMDIDestroy(HWND hwndDestroy);
virtual void MDIActivate(BOOL fActive, HWND hwndActivate, HWND hwndDeactivate); // [WM_MDIACTIVATE]
void SendMDIActivate(BOOL fActive, HWND hwndActivate, HWND hwndDeactivate);
virtual void MDIRestore(HWND hwndRestore); // [WM_MDIRESTORE]
void SendMDIRestore(HWND hwndRestore);
virtual HWND MDINext(HWND hwndCur, BOOL fPrev); // [WM_MDINEXT]
HWND SendMDINext(HWND hwndCur, BOOL fPrev);
virtual void MDIMaximize(HWND hwndMaximize); // [WM_MDIMAXIMIZE]
void SendMDIMaximize(HWND hwndMaximize);
virtual BOOL MDITile(UINT cmd); // [WM_MDITILE]
BOOL SendMDITile(UINT cmd);
virtual BOOL MDICascade(UINT cmd); // [WM_MDICASCADE]
BOOL SendMDICascade(UINT cmd);
virtual void MDIIconArrange(); // [WM_MDIICONARRANGE]
void SendMDIIconArrange();
virtual HWND MDIGetActive(); // [WM_MDIGETACTIVE]
HWND SendMDIGetActive();
virtual HMENU MDISetMenu(BOOL fRefresh, HMENU hmenuFrame, HMENU hmenuWindow); // [WM_MDISETMENU]
HMENU SendMDISetMenu(BOOL fRefresh, HMENU hmenuFrame, HMENU hmenuWindow);
virtual void OnDropFiles(HDROP hdrop); // [WM_DROPFILES]
void SendDropFiles(HDROP hdrop);
virtual void OnCut(); // [WM_CUT]
void SendCut();
virtual void OnCopy(); // [WM_COPY]
void SendCopy();
virtual void OnPaste(); // [WM_PASTE]
void SendPaste();
virtual void OnClear(); // [WM_CLEAR]
void SendClear();
virtual void OnUndo(); // [WM_UNDO]
void SendUndo();
virtual HANDLE OnRenderFormat(UINT fmt); // [WM_RENDERFORMAT]
HANDLE SendRenderFormat(UINT fmt);
virtual void OnRenderAllFormats(); // [WM_RENDERALLFORMATS]
void SendRenderAllFormats();
virtual void OnDestroyClipboard(); // [WM_DESTROYCLIPBOARD]
void SendDestroyClipboard();
virtual void OnDrawClipboard(); // [WM_DRAWCLIPBOARD]
void SendDrawClipboard();
virtual void OnPaintClipboard(HWND hwndCBViewer, const LPPAINTSTRUCT lpPaintStruct); // [WM_PAINTCLIPBOARD]
void SendPaintClipboard(HWND hwndCBViewer, const LPPAINTSTRUCT lpPaintStruct);
virtual void OnVScrollClipboard(HWND hwndCBViewer, UINT code, int pos); // [WM_VSCROLLCLIPBOARD]
void SendVScrollClipboard(HWND hwndCBViewer, UINT code, int pos);
virtual void OnSizeClipboard(HWND hwndCBViewer, const LPRECT lprc); // [WM_SIZECLIPBOARD]
void SendSizeClipboard(HWND hwndCBViewer, const LPRECT lprc);
virtual void OnAskCBFormatName(int cchMax, LPTSTR rgchName); // [WM_ASKCBFORMATNAME]
void SendAskCBFormatName(int cchMax, LPTSTR rgchName);
virtual void OnChangeCBChain(HWND hwndRemove, HWND hwndNext); // [WM_CHANGECBCHAIN]
void SendChangeCBChain(HWND hwndRemove, HWND hwndNext);
virtual void OnHScrollClipboard(HWND hwndCBViewer, UINT code, int pos); // [WM_HSCROLLCLIPBOARD]
void SendHScrollClipboard(HWND hwndCBViewer, UINT code, int pos);
virtual BOOL OnQueryNewPalette(); // [WM_QUERYNEWPALETTE]
BOOL SendQueryNewPalette();
virtual void OnPaletteIsChanging(HWND hwndPaletteChange); // [WM_PALETTEISCHANGING]
void SendPaletteIsChanging(HWND hwndPaletteChange);
virtual void OnPaletteChanged(HWND hwndPaletteChange); // [WM_PALETTECHANGED]
void SendPaletteChanged(HWND hwndPaletteChange);
virtual void OnHotKey(int idHotKey, UINT fuModifiers, UINT vk); // [WM_HOTKEY]
void SendHotKey(int idHotKey, UINT fuModifiers, UINT vk);
