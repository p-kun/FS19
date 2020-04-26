/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: CopyFuncs.cpp
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
#include "_box.h"
#include "dirent.h"
#include "iKakuninDlg.h"
#include "iRenameDlg.h"
#include "SubFuncs.h"
#include "logf.h"
#include "CopyFuncs.h"

#define ACTION      TCOPYACTION


// ==========================================================================
// --
// --------------------------------------------------------------------------
static void Message( HWND hWnd, int id, ACTION *act )
{
  ::SendMessage( hWnd, WM_COMMAND, MAKEWPARAM( ( UINT )( ICOPY_BASE_ID ),( UINT )( id ) ), ( LPARAM )act );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static void PathAppend( TCHAR *buf, size_t size, const TCHAR *path1, const TCHAR *path2 )
{
  if (_tcslen(path1) + 1 + _tcslen(path2) + 1 >= MAX_PATH)
    {
      assert(0);
    }

  _tcscpy_s( buf, size, path1 );
  PathAppend( buf, path2 );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL GetFileInfo( const TCHAR *name, DWORD *attr, FILETIME *time, LARGE_INTEGER *size )
{
  HANDLE    hFile;
  FILETIME  creationTime;   // Creation date
  FILETIME  accessTime;     // Last access date

  hFile = ::CreateFile( name, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0 );

  if ( hFile == INVALID_HANDLE_VALUE )
    {
      return FALSE;
    }

  if ( attr )
    {
      *attr = GetFileAttributes( name );
    }

  if ( time )
    {
      ::GetFileTime( hFile, &creationTime, &accessTime, time );
    }

  if ( size )
    {
      size->LowPart = GetFileSize( hFile, ( DWORD* )&size->HighPart );
    }

  ::CloseHandle( hFile );

  return TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int PumpMessage( HWND hWnd )
{
  MSG   msg;
  int   res = 0;

  while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
    {
      if ( msg.message == WM_QUIT )
        {
          res = 1;
          break;
        }
      else if ( msg.message == WM_KEYDOWN )
        {
          if ( msg.wParam == VK_ESCAPE )
            {
              if ( ::MessageBox( hWnd, _T( "Copying in progress. Do you want to cancel it?" ), _T( "what will you do?" ), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDYES )
                {
                  res = 1;
                  break;
                }
            }
        }

      if ( GetMessage( &msg, NULL, 0, 0 ) <= 0 )
        {
          break;
        }

      ::TranslateMessage( &msg );
      ::DispatchMessage ( &msg );
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Copy callback function
// --------------------------------------------------------------------------
DWORD CALLBACK CopyProgressRoutine(
  LARGE_INTEGER TotalFileSize,          // total file size, in bytes
  LARGE_INTEGER TotalBytesTransferred,  // total number of bytes transferred
  LARGE_INTEGER StreamSize,             // total number of bytes for this stream
  LARGE_INTEGER StreamBytesTransferred, // total number of bytes transferred for this stream
  DWORD         dwStreamNumber,         // the current stream
  DWORD         dwCallbackReason,       // reason for callback
  HANDLE        hSourceFile,            // handle to the source file
  HANDLE        hDestinationFile,       // handle to the destination file
  LPVOID        lpData                  // passed by CopyFileEx
)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
{
  ACTION* act = ( ACTION* )lpData;
  DWORD   res = PROGRESS_CONTINUE;

  if ( dwCallbackReason == CALLBACK_STREAM_SWITCH )
    {
      act->in1st.QuadPart += TotalFileSize.QuadPart;
    }

  act->first = dwCallbackReason;
  act->trans.QuadPart = act->in1st.QuadPart + TotalBytesTransferred.QuadPart - TotalFileSize.QuadPart;
  act->file_total.QuadPart = TotalFileSize.QuadPart;
  act->file_trans.QuadPart = TotalBytesTransferred.QuadPart;

  Message( act->hWnd, TCOPYACTION_EXEC, act );

  if ( PumpMessage( act->hWnd ) )
    {
      act->cancel = TRUE;
      res = PROGRESS_CANCEL;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static dirent *CreateEntry( HANDLE hHeap, dirent* top, const TCHAR *name, DWORD attr, FILETIME *time, LARGE_INTEGER *size )
{
  dirent* elm;
  size_t  len = _tcslen( name ) + 1 ;

  elm = ( dirent* )::HeapAlloc( hHeap, 0, sizeof( dirent ) + len * sizeof( TCHAR ) );

  if ( elm )
    {
      _tcscpy_s( elm->d_name, len, name );
      elm->hHandle          = hHeap;
      elm->dwFileAttributes = attr;
      elm->ftCreationTime   = *time;
      elm->nFileSizeHigh    = size->HighPart;
      elm->nFileSizeLow     = size->LowPart;
      elm->next             = top;
      elm->d_cnt            = ( int )( PathFindFileName( elm->d_name ) - elm->d_name );
      elm->state            = -1;
      elm->bookMark         = TCOPYBMARK_FAIL_IF_EXISTS;
      elm->param            = elm;
    }

  return elm;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static dirent *CreateEntry( HANDLE hHeap, dirent* top, const TCHAR *dir, WIN32_FIND_DATA *fd, int d_len = 0 )
{
  dirent* elm = NULL;
  size_t  len = _tcslen( dir ) + 1 ;

  if ( len < MAX_PATH )
    {
      elm = ( dirent* )::HeapAlloc( hHeap, 0, sizeof( dirent ) + len * sizeof( TCHAR ) );
    }

  if ( elm )
    {
      _tcscpy_s( elm->d_name, len, dir );

      elm->hHandle          = hHeap;
      elm->dwFileAttributes = fd->dwFileAttributes;
      elm->ftCreationTime   = fd->ftLastWriteTime;
      elm->nFileSizeHigh    = fd->nFileSizeHigh;
      elm->nFileSizeLow     = fd->nFileSizeLow;
      elm->next             = top;
      elm->d_cnt            = d_len;
      elm->state            = -1;
      elm->bookMark         = TCOPYBMARK_FAIL_IF_EXISTS;
      elm->param            = elm;
    }

  return elm;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static dirent *open_sub( dirent *top )
{
  HANDLE          hFile;
  WIN32_FIND_DATA fd;
  TCHAR           src[ MAX_PATH ];
  TCHAR*          dir;
  dirent*         elm;
  HANDLE          hHeap;

  hHeap = top->hHandle;
  dir   = top->d_name;
  elm   = top->next;

  PathAppend( src, MAX_PATH, dir, _T( "*.*" ) );

  hFile = ::FindFirstFile( src, &fd );

  if ( INVALID_HANDLE_VALUE != hFile )
    {
      do
        {
          if ( _tcscmp( fd.cFileName, _T( "." ) )== 0 || _tcscmp( fd.cFileName, _T( ".." ) ) == 0 )
            {
              continue;
            }

          PathAppend( src, MAX_PATH, dir, fd.cFileName );

          elm = CreateEntry( hHeap, elm, src, &fd, top->d_cnt );

          if ( elm == NULL )
            {
              break;
            }
        }
      while( ::FindNextFile( hFile, &fd ) );

      ::FindClose( hFile );
    }

  return elm;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
dirent *ReadClips( HWND hWnd, HANDLE hHeap, const TCHAR *dir_path )
{
  int           cnt;
  int           i;
  TCHAR         src[ MAX_PATH * 2 ];
  HDROP         hDrop;
  DWORD         attr;
  FILETIME      time;
  LARGE_INTEGER size;
  dirent*       elm;
  dirent*       top = NULL;

  if ( !IsClipboardFormatAvailable( CF_HDROP ) )
    {
      return FALSE;
    }

  OpenClipboard( hWnd );

  hDrop = ( HDROP )GetClipboardData( CF_HDROP );

  if ( hDrop != NULL )
    {
      /* Get number of files */

      cnt = ::DragQueryFile( hDrop, ( unsigned )-1, NULL, 0 );

      for ( i = 0; i < cnt; i++ )
        {
          ::DragQueryFile( hDrop, i, src, MAX_PATH * 2 );

          attr = 0;
          GetFileInfo( src, &attr, &time, &size );
          top = CreateEntry( hHeap, top, src, attr, &time, &size );
        }
    }

  CloseClipboard();

  for ( elm = top; elm; elm = elm->next )
    {
      if ( elm->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
          elm->next = open_sub( elm );
        }
    }

  return top;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int CheckClips( HWND hWnd )
{
  int     cnt = 0;
  HDROP   hDrop;

  if ( !IsClipboardFormatAvailable( CF_HDROP ) )
    {
      return 0;
    }

  OpenClipboard( hWnd );

  hDrop = ( HDROP )GetClipboardData( CF_HDROP );

  if ( hDrop != NULL )
    {
      /* Get number of files */

      cnt = ::DragQueryFile( hDrop, ( unsigned )-1, NULL, 0 );
    }

  CloseClipboard();

  return cnt;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static BOOL Reame( dirent *elm, TCHAR *new_name )
{
  dirent* trans = NULL;
  BOOL    res   = FALSE;
  size_t  len;
  size_t  size;
  TCHAR   buf[MAX_PATH];

  _tcscpy_s(buf, MAX_PATH, elm->d_name);

  PathRemoveFileSpec(buf);
  PathRemoveBackslash(buf);

  if (_tcslen(buf) + _tcslen(new_name) + 1 >= MAX_PATH)
    {
      return FALSE;
    }

  PathAppend(buf, new_name);

  /* String length = Path name without file name + delimiter + new file name + delimiter */

  len = _tcslen(buf) + 1;

  /* Number of bytes to reserve for the number of characters */

  size = sizeof( dirent ) + len * sizeof( TCHAR );

  trans = ( dirent* )::HeapAlloc( elm->hHandle, 0, size );

  if ( !trans )
    {
      return FALSE;
    }

  /* New file name copy */

  ZeroMemory( trans, size );

  *trans          = *elm;
  trans->next     = elm->next;
  elm->next       = trans;
  elm->param      = trans;
  elm->bookMark   = TCOPYBMARK_RENAME;
  trans->param    = NULL;
  trans->bookMark = TCOPYBMARK_NULL;

  _tcscpy_s( trans->d_name, len, buf );

  return TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL CheckCopyInfo( HWND hWnd, dirent* top, const TCHAR *dir )
{
  TCHAR               trans[ MAX_PATH ];
  dirent*             elm;
  iKakuninDlg*        kakunin;
  BOOL                res = TRUE;
  FILETIME            time;
  int                 theAll   = 0;
  int                 selected = 0;
  DWORD               attr     = 0;
  LARGE_INTEGER       size;

  kakunin = new iKakuninDlg( hWnd );

  for ( elm = top; elm && res; elm = elm->next )
    {
      PathAppend( trans, MAX_PATH, dir, elm->d_name + elm->d_cnt );

      if ( !GetFileInfo( trans, &attr, &time, &size ) )
        {
          continue;
        }

      if ( ( elm->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && attr == elm->dwFileAttributes )
        {
          if ( PathFileExists( trans ) )
            {
              elm->bookMark = TCOPYBMARK_EXIST_DIRECTORY;
            }
          continue;
        }

      /* Process when there is a file with the same name */

      kakunin->SetSourceFileInfo( elm->d_name, &elm->ftCreationTime, elm->nFileSizeHigh, elm->nFileSizeLow );
      kakunin->SetTransFileInfo( trans, &time, &size );

      if ( theAll == 0 )
        {
          kakunin->selected = selected;

          if ( kakunin->Modal() == IDCANCEL )
            {
              res = FALSE;
              break;
            }

          if ( kakunin->thatsall )
            {
              theAll = 1;
            }

          selected = kakunin->selected;
        }

      /* Processing result */

      if ( selected == IKAKU_RENAME )
        {
          Reame( elm, kakunin->mNewPath );
        }
      else if ( selected == IKAKU_NEW )
        {
          if ( CompareFileTime( &elm->ftCreationTime, &time ) > 0 )
            {
              /* The destination file is older */

              elm->bookMark = TCOPYBMARK_OVERWRITE;
            }
          else
            {
              elm->bookMark = TCOPYBMARK_NOT_COPY_FOR_DATE;
            }
        }
      else if ( selected == IKAKU_OVERWRITE )
        {
          elm->bookMark = TCOPYBMARK_OVERWRITE;
        }
      else if ( selected == IKAKU_NOTCOPY )
        {
          elm->bookMark = TCOPYBMARK_NOT_COPY_FOR_FILE;
        }
    }

  delete kakunin;

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL CopyExec( HWND hWnd, dirent* top, const TCHAR *dir )
{
  return TRUE;
}
// --------------------------------------------------------------------------
