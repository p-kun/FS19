/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iMainApp.cpp
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */

/****************************************************************************
 * Included files
 ****************************************************************************/

#include "_box.h"
#include "dirent.h"
#include "savedir.h"
#include "iIcon.h"
#include "CopyFuncs.h"
#include "iSubApp.h"
#include "ContextMenu.h"
#include "iPt.h"
#include "iSettingDialog.h"
#include "iBaseView.h"
#include "iNamelist.h"
#include "iSeachView.h"
#include "iShortcutView.h"
#include "iDirectoryView.h"
#include "SubFuncs.h"
#include "iMkDirDlg.h"
#include "iRenameDlg.h"
#include "iFindDlg.h"
#include "iEjectDskDlg.h"
#include "iLogDskDlg.h"
#include "iAttrDlg.h"
#include "logf.h"
#include "iTreeView.h"
#include "timerid.h"
#include "xping.h"
#include "iMainApp.h"

/****************************************************************************
 * Private pre-processor definitions
 ****************************************************************************/

/****************************************************************************
 * Private types
 ****************************************************************************/

typedef struct _DWM_BLURBEHIND
{
  DWORD dwFlags;
  BOOL  fEnable;
  HRGN  hRgnBlur;
  BOOL  fTransitionOnMaximized;
} DWM_BLURBEHIND, *PDWM_BLURBEHIND;

typedef HRESULT ( WINAPI *t_DwmExtendFrameIntoClientArea )( HWND hwnd, const MARGINS *pMarInset );
typedef HRESULT ( WINAPI *t_DwmEnableBlurBehindWindow )( HWND hwnd, const DWM_BLURBEHIND *pBlurBehind );

/****************************************************************************
 * Public type declarations
 ****************************************************************************/

#define FIND_DATA         WIN32_FIND_DATA
#define DWM_BB_ENABLE     0x00000001
#define DWM_BB_BLURREGION 0x00000002

/****************************************************************************
 * Public function prototypes
 ****************************************************************************/

/****************************************************************************
 * Public data
 ****************************************************************************/

/****************************************************************************
 * Private data
 ****************************************************************************/

/****************************************************************************
 * Private functions
 ****************************************************************************/

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static void DwmExtendFrameIntoClientArea( HWND hwnd, const MARGINS *pMarInset )
{
  HMODULE dll = LoadLibrary( TEXT( "dwmapi.dll" ) );

  if( dll ) {
    t_DwmExtendFrameIntoClientArea dwmEfica = reinterpret_cast< t_DwmExtendFrameIntoClientArea >( GetProcAddress( dll, "DwmExtendFrameIntoClientArea" ) );
    dwmEfica( hwnd, pMarInset );
    FreeLibrary( dll );
  }
}


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static void DwmEnableBlurBehindWindow( HWND hwnd, const DWM_BLURBEHIND *pBlurBehind )
{
  HMODULE dll = LoadLibrary( TEXT( "dwmapi.dll" ) );

  if( dll ) {
    t_DwmEnableBlurBehindWindow dwmEfica = reinterpret_cast< t_DwmEnableBlurBehindWindow >( GetProcAddress( dll, "DwmEnableBlurBehindWindow" ) );
    dwmEfica( hwnd, pBlurBehind );
    FreeLibrary( dll );
  }
}


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iMainApp::iMainApp()
{
  mpFuncs = ( PT_FUNC_TYPE* )::HeapAlloc( GetProcessHeap(), HEAP_NO_SERIALIZE, sizeof( PT_FUNC_TYPE* ) * ( PT_FUNC_MAX ) * 2 );

  mpFuncs[ PT_FUNC_NON ]         = &iMainApp::Dummy;
  mpFuncs[ PT_FUNC_EDITOR ]      = &iMainApp::FuncEditor;
  mpFuncs[ PT_FUNC_LOGDISK ]     = &iMainApp::FuncLogDsk;
  mpFuncs[ PT_FUNC_REFLESH ]     = &iMainApp::FuncRefresh;
  mpFuncs[ PT_FUNC_EXEC ]        = &iMainApp::FuncExec;
  mpFuncs[ PT_FUNC_BACK ]        = &iMainApp::FuncBack;
  mpFuncs[ PT_FUNC_STEP ]        = &iMainApp::FuncStep;
  mpFuncs[ PT_FUNC_EJECT ]       = &iMainApp::FuncEject;
  mpFuncs[ PT_FUNC_COPY ]        = &iMainApp::FuncCopy;
  mpFuncs[ PT_FUNC_PAST ]        = &iMainApp::FuncPast;
  mpFuncs[ PT_FUNC_RENAME ]      = &iMainApp::FuncRename;
  mpFuncs[ PT_FUNC_OPTION ]      = &iMainApp::FuncOption;
  mpFuncs[ PT_FUNC_SELEXE ]      = &iMainApp::FuncSelExe;
  mpFuncs[ PT_FUNC_DELETE ]      = &iMainApp::FuncDelete;
  mpFuncs[ PT_FUNC_INSERT ]      = &iMainApp::FuncInsert;
  mpFuncs[ PT_FUNC_APPS ]        = &iMainApp::FuncApps;
  mpFuncs[ PT_FUNC_PROPERTIES ]  = &iMainApp::FuncProperties;
  mpFuncs[ PT_FUNC_ATTRIBUTE ]   = &iMainApp::FuncAttribute;
  mpFuncs[ PT_FUNC_SHORTCUT1 ]   = &iMainApp::FuncShortcut1;
  mpFuncs[ PT_FUNC_SHORTCUT2 ]   = &iMainApp::FuncShortcut2;
  mpFuncs[ PT_FUNC_SHORTCUT3 ]   = &iMainApp::FuncShortcut3;
  mpFuncs[ PT_FUNC_SHORTCUT4 ]   = &iMainApp::FuncShortcut4;
  mpFuncs[ PT_FUNC_SETCOPYVIEW ] = &iMainApp::FuncSetCopyView;
  mpFuncs[ PT_FUNC_FIND ]        = &iMainApp::FuncFind;
  mpFuncs[ PT_FUNC_SORT ]        = &iMainApp::FuncSort;

  m_desktopfolder[ 0 ] = '\0';

  /* Do not issue OS messages */

  mOsMessageMode = SetErrorMode( SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS );

  OleInitialize( NULL );

  mhNextClipWnd = NULL;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iMainApp::~iMainApp()
{
  SetErrorMode( mOsMessageMode );

  ::HeapFree( GetProcessHeap(), HEAP_NO_SERIALIZE, mpFuncs );

  OleUninitialize();
}
// --------------------------------------------------------------------------


// ===========================================================================
// -- コマンドの処理
// ---------------------------------------------------------------------------
void iMainApp::OnCommand( int id, HWND hwndCtl, UINT codeNotify )
{
  POINT     pos;

  GetCursorPos( &pos );

  if ( id == ISHORTCUT_BASE_ID )
    {
      switch ( codeNotify )
        {
          case ISHORTCUTID_SETTING:
            FuncOption();
            break;
          case ISHORTCUTID_BACK:
            Back();
            break;
          case ISHORTCUTID_STEP:
            Step();
            break;
          case ISHORTCUTID_COPY:
            FuncCopy();
            break;
          case ISHORTCUTID_PAST:
            FuncPast();
            break;
          case ISHORTCUTID_RENAME:
            FuncRename();
            break;
          case ISHORTCUTID_DELETE:
            FuncDelete();
            break;
          case ISHORTCUTID_PROPERTIES:
            FuncProperties();
            break;
          case ISHORTCUTID_REFRESH:
            FuncRefresh();
            break;
          case ISHORTCUTID_R_FOLDER:
            DoContextMenu( pos.x, pos.y, GetSelected() );
            break;
          case ISHORTCUTID_EJECT:
            FuncEject();
            break;
        }
    }
  else if ( id == IDIRECTORY_BASE_ID && codeNotify == IDIRECTORYID_R_FOLDER )
    {
      DoContextMenu( pos.x, pos.y, GetSelected() );
    }
  else if ( id == ISEACHVIEW_BASE_ID )
    {
      if ( codeNotify != 0xFFFF )
        {
          DoContextMenu( pos.x, pos.y );
        }
      else
        {
          DoContextMenu( pos.x, pos.y, GetSelected() );
        }
    }
  else if ( id == ICOPY_BASE_ID )
    {
      LetsCopy( ( TCOPYACTION* )hwndCtl, codeNotify );
    }
}
// ---------------------------------------------------------------------------


// ==========================================================================
// -- メイン処理
// --------------------------------------------------------------------------
#ifdef _DEBUG
int _tmain()
#else
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInst, PSTR szCmdLine, int iCmdShow )
#endif
{
  iMainApp* app;
  xping*    pin;
  iico*     ico;
  iPt       pt;

  pin = new xping(MAKEWORD(2, 2));

  app = new iMainApp;

  ico = new iico(IMG_INIT);

#ifdef _DEBUG
  app->Initialize();
#else
  app->Initialize( hInstance );
#endif

  app->Create( pt.x, pt.y, pt.cx, pt.cy );
  app->ShowWindow( pt.show );

  app->Run();

  delete ico;
  delete app;
  delete pin;

  delete_savedir();

  pt.Write();

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- WM_COMMANDの処理
// --------------------------------------------------------------------------
BOOL iMainApp::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
  iPt   pt;

  SetColumn( pt.column );

  SetFonts( pt.fontname1, pt.fontsize1,
            pt.fontname2, pt.fontsize2,
            pt.fontname2, pt.fontsize3,
            pt.fontname2, pt.fontsize4 );

  mhNextClipWnd = SetClipboardViewer( mhWnd );

  MARGINS   margins;

  margins.cyTopHeight    = TITLE_HEIGHT;
  margins.cyBottomHeight = 0;
  margins.cxLeftWidth    = 0;
  margins.cxRightWidth   = 0;

  DwmExtendFrameIntoClientArea( mhWnd, &margins );

  return TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iMainApp::OnShowWindow( BOOL fShow, UINT status )
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iMainApp::DoContextMenu( int x, int y )
{
  POINT   pos;
  TCHAR*  dir;
  dirent* entry;
  dirent* top;
  int     cnt;
  int     i;
  TCHAR** names;
  TCHAR*  cp;

  pos.x = x;
  pos.y = y;

  dir = GetCurrentDirectory();

  cnt = GetMarkedNum( &top );

  names = ( TCHAR** )HeapAlloc( GetProcessHeap(), HEAP_NO_SERIALIZE, sizeof( TCHAR* ) * ( cnt + 1 ) );

  for ( entry = top, i = 0; i < cnt; i++ )
    {
      cp = entry->d_name + _tcslen( dir );

      while ( *cp == '\\' )
        {
          cp++;
        }

      names[ i ] = cp;
      entry = entry->next;
    }

  if ( cnt == 1 )
    {
      if ( _tcscmp( names[ 0 ], _T( ".." ) ) == 0 )
        {
          ContextMenu( mhWnd, pos.x, pos.y, dir, _T( "" ) );
        }
      else
        {
          ContextMenu( mhWnd, pos.x, pos.y, dir, names[ 0 ] );
        }
    }
  else
    {
      ContextMenu( mhWnd, pos.x, pos.y, dir, names, cnt );
    }

  ::HeapFree( GetProcessHeap(), HEAP_NO_SERIALIZE, names );

}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iMainApp::DoContextMenu( int x, int y, const TCHAR *target )
{
  ContextMenu( mhWnd, x, y, ( TCHAR* )target, _T( "" ) );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iMainApp::OnInitMenuPopup( HMENU hMenu, UINT item, BOOL fSystemMenu )
{
  ContextMenuMsg( WM_INITMENUPOPUP, ( WPARAM )( HMENU )( hMenu ), MAKELPARAM( ( item ), ( fSystemMenu ) ) );
}
// --------------------------------------------------------------------------
void iMainApp::OnDrawItem( const DRAWITEMSTRUCT * lpDrawItem )
{
  ContextMenuMsg( WM_DRAWITEM, ( WPARAM )( ( ( const DRAWITEMSTRUCT* )lpDrawItem )->CtlID ), ( LPARAM )( const DRAWITEMSTRUCT* )( lpDrawItem ) );
  iSubApp::OnDrawItem( lpDrawItem );
}
// --------------------------------------------------------------------------
void iMainApp::OnMeasureItem( MEASUREITEMSTRUCT * lpMeasureItem )
{
  ContextMenuMsg( WM_MEASUREITEM, ( WPARAM )( ( ( MEASUREITEMSTRUCT* )lpMeasureItem )->CtlID ), ( LPARAM )( MEASUREITEMSTRUCT* )( lpMeasureItem ) );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iMainApp::OnDestroy()
{
  iPt   pt;

  pt.SetPlacement( mhWnd );

  pt.tool_size = m_tool_size;

  GetCurrentDirectorys( pt.path1, pt.path2, MAX_PATH );

  pt.page_no = GetCurrentPageNo();

  KillTimer( mhWnd, ISCROLL_TIMERID );
  KillTimer( mhWnd, ISUBAPP_TIMEID );
  KillTimer( mhWnd, ISUBAPP_SSSS );
  KillTimer( mhWnd, IINFO_LEAVE_TIMERID );
  KillTimer( mhWnd, IINFO_WAIT_TIMERID );
  KillTimer( mhWnd, ISEACH_TIMERID );

  if ( mhNextClipWnd )
    {
      ChangeClipboardChain( mhWnd, mhNextClipWnd );
    }

  _boxApp::OnDestroy();
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncEditor( TCHAR *param )
{
  iPt     pt;
  TCHAR   buf[ MAX_PATH ];
  dirent* entry;

  entry = GetCurrentEntry();

  if ( _tcscmp( PathFindFileName( entry->d_name ), _T( ".." ) ) != 0 && !( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && !PathIsRoot( entry->d_name ) )
    {
      wsprintf( buf, _T( "%s \"%s\"" ), pt.GetEditorPath(), entry->d_name );
      WinExec( buf );
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncLogDsk( TCHAR *param )
{
  iLogDskDlg  dlg( mhWnd );

  dlg.SetCurrent( GetCurrentDirectory() );

  if ( dlg.Modal() ==IDOK )
    {
      SetDirectory( dlg.logdrv, NULL, 1 );
      Invalidate();
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncRefresh( TCHAR *param )
{
  Refresh();

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncExec( TCHAR *param )
{
  dirent*   entry;
  iBaseView base;

  entry = GetCurrentEntry();

  if ( entry && !PathIsRoot( entry->d_name ) )
    {
      base.ShellExecute( entry->d_name );
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncBack( TCHAR *param )
{
  Back();

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncStep( TCHAR *param )
{
  Step();

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncEject( TCHAR *param )
{
  iEjectDskDlg  dlg( mhWnd );

  if ( dlg.CheckEjectDrive() )
    {
      StopNotification();

      dlg.Modal();

      SetDirectorys();
      Invalidate();

      StartNotification();
    }
  else
    {
      MessageBox( _T( "There is no drive that can be ejected!" ), _T( "eject..." ), MB_ICONINFORMATION );
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncCopy( TCHAR *param )
{
  dirent* entry;
  HDROP   hDrop;

  if ( GetMarkedNum( &entry ) > 0 )
    {
      hDrop = ( HDROP )MakeHDropFiles( entry );

      if ( hDrop )
        {
          if ( OpenClipboard( mhWnd ) == 0 )
            {
              GlobalFree( hDrop );
            }
          else
            {
              EmptyClipboard();
              SetClipboardData( CF_HDROP, hDrop );
              CloseClipboard();
            }
        }
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncPast( TCHAR *param )
{
  if ( m_lock == FALSE )
    {
      if ( CheckClips( mhWnd ) > 0 )
        {
          m_lock = TRUE;
          StartCopyInfo();
          m_lock = FALSE;
        }
    }
  else
    {
      MessageBox( _T( "Copying! Can't run" ), _T( "Copy..." ), MB_ICONERROR );
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncRename( TCHAR *param )
{
  dirent*     entry;
  iRenameDlg  dlg( mhWnd );
  iPt         pt;
  TCHAR       buf[MAX_PATH];
  TCHAR       title[] = L"rename...";
  TCHAR       msg01[] = L"Cannot be changed!\r";
  TCHAR       msg02[] = L"A file with the same name exists.";

  entry = GetCurrentEntry();

  do
    {
      if (entry == NULL)
        {
          break;
        }

      if (_tcscmp( PathFindFileName( entry->d_name ), _T( ".." ) ) == 0)
        {
          break;
        }

      if (PathIsRoot( entry->d_name ))
        {
          break;
        }

      dlg.SetFileInfo( entry );

      if ( dlg.Modal() != IDOK )
        {
          break;
        }

      if (PathFileExists(dlg.rename))
        {
          _stprintf_s(buf, MAX_PATH, L"\"%s\"\r\r%s%s", dlg.rename, msg01, msg02);
          MessageBox( buf, title, MB_ICONSTOP );
          break;
        }

      if (!::MoveFile( entry->d_name, dlg.rename ))
        {
          _stprintf_s(buf, MAX_PATH, L"\"%s\"\r\r%s", dlg.rename, msg01);
          MessageBox( buf, title, MB_ICONSTOP );
          break;
        }

      SetDirectory( GetCurrentDirectory(), dlg.rename );
      Invalidate();
    }
  while(0);

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncOption( TCHAR *param )
{
  iSettingDialog  dlg( mhWnd );
  LOGFONT*        logfont1;
  LOGFONT*        logfont2;
  LOGFONT*        logfont3;
  LOGFONT*        logfont4;
  int             sort_kind;
  int             sort_num;
  iPt             pt;

  dlg.SetParam( pt.column, 0, 0 );
  dlg.SetLogFont( 0, pt.fontname1, pt.fontsize1 );
  dlg.SetLogFont( 1, pt.fontname2, pt.fontsize2 );
  dlg.SetLogFont( 2, pt.fontname3, pt.fontsize3 );
  dlg.SetLogFont( 3, pt.fontname4, pt.fontsize4 );

  if ( dlg.Modal() )
    {
      dlg.GetParam( &pt.column, &sort_num, &sort_kind );

      logfont1 = dlg.GetLogFont( 0 );
      logfont2 = dlg.GetLogFont( 1 );
      logfont3 = dlg.GetLogFont( 2 );
      logfont4 = dlg.GetLogFont( 3 );

      _tcscpy_s( pt.fontname1, LF_FACESIZE, logfont1->lfFaceName );
      pt.fontsize1 = logfont1->lfHeight;
      _tcscpy_s( pt.fontname2, LF_FACESIZE, logfont2->lfFaceName );
      pt.fontsize2 = logfont2->lfHeight;
      _tcscpy_s( pt.fontname3, LF_FACESIZE, logfont3->lfFaceName );
      pt.fontsize3 = logfont3->lfHeight;
      _tcscpy_s( pt.fontname4, LF_FACESIZE, logfont4->lfFaceName );
      pt.fontsize4 = logfont4->lfHeight;

      SetFonts( pt.fontname1, pt.fontsize1,
                pt.fontname2, pt.fontsize2,
                pt.fontname3, pt.fontsize3,
                pt.fontname4, pt.fontsize4 );

      SetColumn( pt.column );

      Invalidate();
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncSelExe( TCHAR *param )
{
  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncDelete( TCHAR *param )
{
  int     count;
  dirent* entry;

  count = GetMarkedNum( &entry );

  if ( count > 0 )
    {
      ShDelete( mhWnd, entry, count );
      SetDirectorys();
      Invalidate();
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncInsert( TCHAR *param )
{
  iMkDirDlg dlg( mhWnd );
  iPt       pt;

  dlg.SetFont( pt.fontname2, pt.fontsize2 );
  dlg.SetDirectory( GetCurrentDirectory() );

  if ( dlg.Modal() == IDOK )
    {
      SetDirectory( GetCurrentDirectory(), dlg.GetSel() );
      Invalidate();
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncSetCopyView( TCHAR *param )
{
  if ( GetTotalCopyFile() > 0 )
    {
      SetCopyView();
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncFind( TCHAR *param )
{
  iFindDlg  dlg( mhWnd );

  if ( dlg.Modal() == IDOK )
    {
      SelectSeachMode( dlg.m_target );
      Invalidate();
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncSort( TCHAR *param )
{
  int   order;
  int   kind;
  int   new_kind;

  iNamelist::GetSortMode(&order, &kind);

  switch (*param)
    {
      case '1':
        new_kind = INAM_KIND_EXT;
        break;
      case '2':
        new_kind = INAM_KIND_TIME;
        break;
      case '3':
        new_kind = INAM_KIND_SIZE;
        break;
      default:
        new_kind = INAM_KIND_NAME;
        break;
    }

  if (kind == new_kind)
    {
      order *= -1;
    }

  iNamelist::SetSortMode(order, new_kind);

  Sort();
  Invalidate();

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncShortcut1( TCHAR *param )
{
  SetDirectory( GetDesktopFolder() );
  Invalidate();

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncShortcut2( TCHAR *param )
{
  iPt   pt;
  TCHAR buf[ MAX_PATH ];
  TCHAR tmp[ MAX_PATH ];

  ::GetCurrentDirectory( MAX_PATH, tmp );
  ::SetCurrentDirectory( GetCurrentDirectory() );

  wsprintf( buf, _T( "%s -GREPMODE -GOPT=SP2K -GREPDLG -GFILE=\"*.c;*.cpp;*.h\" -GFOLDER=\"%s\"" ), pt.GetEditorPath(), GetCurrentDirectory() );

  WinExec( buf );

  ::SetCurrentDirectory( tmp );

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncShortcut3( TCHAR *param )
{
  iPt   pt;

  SetDirectory( pt.shortcut3 );
  Invalidate();

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncShortcut4( TCHAR *param )
{
  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncProperties( TCHAR *param )
{
  SHELLEXECUTEINFO  sei;
  dirent*           entry;

  entry = GetCurrentEntry();

  ZeroMemory( &sei, sizeof( sei ) );

  sei.cbSize = sizeof( sei );
  sei.lpFile = entry->d_name;
  sei.lpVerb = TEXT( "properties" );
  sei.fMask  = SEE_MASK_INVOKEIDLIST | SEE_MASK_NOCLOSEPROCESS;

  ShellExecuteEx( &sei );

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncAttribute( TCHAR *param )
{
  iAttrDlg  dlg( mhWnd );
  dirent*   entry;

  GetMarkedNum( &entry );

  if ( entry )
    {
      dlg.SetAttribute( entry );

      if ( dlg.Modal() == IDOK )
        {
          dlg.SetAttributes();
          SetDirectory( GetCurrentDirectory(), entry->d_name );
          Invalidate();
        }
    }

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iMainApp::FuncApps( TCHAR *param )
{
  POINT pos;

  GetCurrentPosition( ( int* )&pos.x, ( int* )&pos.y );

  ClientToScreen( mhWnd, &pos );

  DoContextMenu( pos.x, pos.y );

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iMainApp::OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags )
{
  int   f_no;
  iPt   pt;

  f_no = pt.GetFuncNo( vk, HIBYTE( GetKeyState( VK_CONTROL ) ), HIBYTE( GetKeyState( VK_SHIFT ) ), 0 );

  ( this->*( mpFuncs[ f_no ] ) )( (TCHAR *)&vk );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iMainApp::GetDesktopFolder( void )
{
  LPITEMIDLIST  pidlist;

  if ( m_desktopfolder[ 0 ] == '\0' )
    {
      SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOPDIRECTORY, &pidlist );
      SHGetPathFromIDList( pidlist, m_desktopfolder );
      CoTaskMemFree( pidlist );
    }

  return m_desktopfolder;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
static dirent *theSetEntry( HANDLE hHeap, dirent *entry, const TCHAR *name )
{
  HANDLE    hFile;
  size_t    size;
  dirent*   new_entry;
  FIND_DATA fd;

  size = sizeof( dirent ) + ( _tcslen( name ) + 1 ) * sizeof( TCHAR );

  new_entry = ( dirent* )HeapAlloc( hHeap, HEAP_NO_SERIALIZE, size );

  if ( new_entry )
    {
      ZeroMemory( new_entry, size );
    
      hFile = ::FindFirstFile( name, &fd );
    
      if ( INVALID_HANDLE_VALUE != hFile )
        {
          new_entry->next = entry;
          new_entry->size = ( DWORD )size;
          //
          new_entry->dwFileAttributes = fd.dwFileAttributes;
          new_entry->ftCreationTime   = fd.ftCreationTime;
          new_entry->ftLastAccessTime = fd.ftLastAccessTime;
          new_entry->ftLastWriteTime  = fd.ftLastWriteTime;
          new_entry->nFileSizeHigh    = fd.nFileSizeHigh;
          new_entry->nFileSizeLow     = fd.nFileSizeLow;
          //
          StrCpy( new_entry->cAlternateFileName, fd.cAlternateFileName );
          StrCpy( new_entry->d_name, name );
        }

      ::FindClose( hFile );
    }

  return new_entry;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
static dirent *theSetEntry( HANDLE hHeap, dirent *entry, const TCHAR *dir, FIND_DATA *fd )
{
  size_t    size;
  dirent*   new_entry;

  size = sizeof( dirent ) + ( _tcslen( dir ) + 1 + _tcslen( fd->cFileName ) + 1 ) * sizeof( TCHAR );

  new_entry = ( dirent* )HeapAlloc( hHeap, HEAP_NO_SERIALIZE, size );

  if ( new_entry )
    {
      ZeroMemory( new_entry, size );

      new_entry->next = entry;
      new_entry->size = ( DWORD )size;
      //
      new_entry->dwFileAttributes = fd->dwFileAttributes;
      new_entry->ftCreationTime   = fd->ftCreationTime;
      new_entry->ftLastAccessTime = fd->ftLastAccessTime;
      new_entry->ftLastWriteTime  = fd->ftLastWriteTime;
      new_entry->nFileSizeHigh    = fd->nFileSizeHigh;
      new_entry->nFileSizeLow     = fd->nFileSizeLow;
      //
      StrCpy( new_entry->cAlternateFileName, fd->cAlternateFileName );
      StrCpy( new_entry->d_name, dir );
      PathAddBackslash( new_entry->d_name );
      StrCat( new_entry->d_name, fd->cFileName );
    }

  return new_entry;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static dirent *open_sub( HANDLE hHeap, dirent *in_top )
{
  HANDLE    hFile;
  FIND_DATA fd;
  dirent*   top_entry = in_top->next;
  TCHAR     buf[ MAX_PATH ];

  wsprintf( buf, _T( "%s\\*.*" ), in_top->d_name );

  hFile = ::FindFirstFile( buf, &fd );

  if ( INVALID_HANDLE_VALUE == hFile )
    {
      return top_entry;
    }

  do
    {
      if ( _tcscmp( fd.cFileName, _T( "." ) ) == 0 || _tcscmp( fd.cFileName, _T( ".." ) ) == 0 )
        {
          continue;
        }
    
      top_entry = theSetEntry( hHeap, top_entry, in_top->d_name, &fd );
    
      if ( top_entry == NULL )
        {
          break;
        }
    }
  while( ::FindNextFile( hFile, &fd ) );

  ::FindClose( hFile );

  return top_entry;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
static BOOL FileTimeCmp( FILETIME *filetime, TCHAR *fname )
{
  HANDLE    hFile;
  FILETIME  creationTime;     // Create date
  FILETIME  lastAccessTime;   // Last access date
  FILETIME  lastWriteTime;    // Last write date
  BOOL      res = FALSE;

  hFile = CreateFile( fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

  ::GetFileTime( hFile, &creationTime, &lastAccessTime, &lastWriteTime );

  CloseHandle( hFile );

  if ( CompareFileTime( filetime, &lastWriteTime ) > 0 )
    {
      res = TRUE;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Read drop file list from clipboard
// --------------------------------------------------------------------------
int iMainApp::ReadClip()
{
  HDROP     hDrop;
  HANDLE    hHeap;
  int       cnt;
  int       i;
  dirent*   entry;
  dirent*   top_entry = NULL;
  TCHAR*    dir;
  int       len;
  FILE*     fp;
  TCHAR     buf[ MAX_PATH * 2 ];
  TCHAR     tmp[ MAX_PATH * 4 ];
  errno_t   err;

  if ( !IsClipboardFormatAvailable( CF_HDROP ) )
    {
      return 0;
    }

  OpenClipboard( mhWnd );

  hDrop = ( HDROP )GetClipboardData( CF_HDROP );

  if ( hDrop == NULL )
    {
      return 0;
    }

  hHeap = HeapCreate( HEAP_NO_SERIALIZE, 10000, 0 );

  if ( hHeap )
    {
      /* Get number of files */

      cnt = ::DragQueryFile( hDrop, ( unsigned )-1, NULL, 0 );

      for ( i = 0; i < cnt; i++ )
        {
          ::DragQueryFile( hDrop, i, buf, MAX_PATH );
      
          top_entry = theSetEntry( hHeap, top_entry, buf );
        }
    }

  GlobalUnlock( hDrop );
  CloseClipboard();

  dir = GetCurrentDirectory();

  len = ( int )( PathFindFileName( top_entry->d_name ) - top_entry->d_name );

  GetTempPath( MAX_PATH, buf );

  GetTempFileName( buf, _T( "fs8" ), 0, tmp );

  err = _tfopen_s(&fp, tmp, _T( "w" ) );

  if ( err )
    {
      for ( entry = top_entry; entry; entry = entry->next )
        {
          if ( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
              entry->next = open_sub( hHeap, entry );
            }
          else
            {
              _tcscpy_s( buf, MAX_PATH, dir );
              PathAddBackslash( buf );
              _tcscat_s( buf, MAX_PATH, entry->d_name + len );

              if ( PathFileExists( buf ) )
                {
                  if( FileTimeCmp( &entry->ftLastWriteTime, buf ) > 0 )
                    {
                    }
                }

              ShCopy( mhWnd, entry->d_name, buf );

              _ftprintf( fp, L"\"%s\" \"%s\"\n", entry->d_name, buf );
            }
        }

      fclose( fp );
    }

  if ( hHeap )
    {
      HeapDestroy( hHeap );
    }

  DeleteFile( tmp );

  SetDirectory();
  Invalidate();

  return 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Handling dropped files
// --------------------------------------------------------------------------
void iMainApp::OnDropFiles( HDROP hDrop )
{
  int             i;
  int             cnt;
  size_t          len  = 0;
  TCHAR*          pTop = NULL;
  TCHAR*          cp;
  TCHAR           buf[ MAX_PATH ];
  SHFILEOPSTRUCT  fileOp;

  /* Get number of files */

  cnt = ::DragQueryFile( hDrop, ( unsigned )-1, NULL, 0 );

  /* Calculate region size */

  for ( i = 0; i < cnt; i++ )
    {
      ::DragQueryFile( hDrop, i, buf, MAX_PATH );

      len += _tcslen( buf );
      len++;   /* delimiter */
    }

  cp = pTop = ( TCHAR* )HeapAlloc( GetProcessHeap(), HEAP_NO_SERIALIZE, len * sizeof( TCHAR ) );

  if ( cp == NULL )
    {
      return;
    }

  for( i = 0; i < cnt; i++ )
    {
      ::DragQueryFile( hDrop, i, buf, MAX_PATH );

      len = _tcslen( buf ) + 1;

      _tcscpy_s( cp, len, buf );

      cp += len;
    }

  /* Put double NULL character */

  *cp = '\0';

  /* Set value of SHFILEOPSTRUCT structure member */

  fileOp.hwnd                   = mhWnd;
  fileOp.wFunc                  = FO_COPY;
  fileOp.pFrom                  = pTop;
  fileOp.pTo                    = GetCurrentDirectory();
  fileOp.fAnyOperationsAborted  = FALSE;
  fileOp.hNameMappings          = NULL;
  fileOp.lpszProgressTitle      = NULL;
  fileOp.fFlags =
//    FOF_ALLOWUNDO |
      FOF_RENAMEONCOLLISION |
//    FOF_NOCONFIRMATION |
//    FOF_NOERRORUI |
//    FOF_SILENT |
//    FOF_NOCONFIRMATION |
      0;

  ::HeapFree( GetProcessHeap(), HEAP_NO_SERIALIZE, pTop );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iMainApp::LetsCopy( TCOPYACTION *act, int id )
{
  if( id == TCOPYACTION_EXEC )
    {
      SetCopyInfo( act );
    }
  else if( id == TCOPYACTION_COMP )
    {
      CompCopyInfo( act );
    }
  else if( id == TCOPYACTION_END )
    {
      EndCopyInfo();
    }
  else if( id == TCOPYACTION_START )
    {
    }
  else if( id == TCOPYACTION_CANCEL )
    {
      CancelCopyInfo();
    }
}
// --------------------------------------------------------------------------
