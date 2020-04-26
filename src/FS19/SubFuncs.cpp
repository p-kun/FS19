/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: SubFuncs.cpp
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
#include "ssnprintf.h"
#include "dirent.h"
#include "logf.h"
#include "SubFuncs.h"

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/

#define VOLUMEFORMAT  TEXT("\\\\.\\%c:")
#define ROOTFORMAT    TEXT("%c:\\")
#define ERRORFORMAT   TEXT("Error %d: %s\n")
#define LOCK_TIMEOUT  2000       // 10 Seconds
#define LOCK_RETRIES  20

/****************************************************************************
 * Private types
 ****************************************************************************/

/****************************************************************************
 * Public type declarations
 ****************************************************************************/

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

static HANDLE OpenVolume( TCHAR cDriveLetter );
static BOOL   LockVolume( HANDLE hVolume );
static BOOL   DismountVolume( HANDLE hVolume );
static BOOL   PreventRemovalOfVolume( HANDLE hVolume, BOOL fPrevent );
static BOOL   AutoEjectVolume( HANDLE hVolume );
static BOOL   CloseVolume( HANDLE hVolume );


// ==========================================================================
// --
// --------------------------------------------------------------------------
TCHAR *MakeFileList( dirent *top_entry, int count )
{
  int     i;
  size_t  len  = 0;
  TCHAR*  pTop = NULL;
  TCHAR*  cp;
  dirent* entry;

  if ( count == 0 )
    {
      return pTop;
    }

  for ( i = 0, entry = top_entry; i < count && entry; i++, entry = entry->next )
    {
      len += _tcslen( entry->d_name );
      len++;
    }

  cp = pTop = ( TCHAR* )::HeapAlloc( GetProcessHeap(), 0, ( len + 1 ) * sizeof( TCHAR ) );

  if ( !cp )
    {
      return pTop;
    }

  for ( i = 0, entry = top_entry; i < count && entry; i++, entry = entry->next )
    {
      len = _tcslen( entry->d_name ) + 1;

      _tcscpy_s( cp, len, entry->d_name );

      cp += len;
    }

  /* Put double NULL character */

  *cp = '\0';

  return pTop;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- File to trash
// --------------------------------------------------------------------------
BOOL ShDelete( HWND hWnd, dirent *top_entry, int count )
{
  SHFILEOPSTRUCT  fileOp;
  TCHAR*          pTop;
  BOOL            bResult;

  pTop = MakeFileList( top_entry, count );

  if ( pTop == NULL )
    {
      return FALSE;
    }

  /* Set value of SHFILEOPSTRUCT structure member */

  fileOp.hwnd                  = hWnd;
  fileOp.wFunc                 = FO_DELETE;
  fileOp.pFrom                 = pTop;
  fileOp.pTo                   = NULL;
  fileOp.fAnyOperationsAborted = FALSE;
  fileOp.hNameMappings         = NULL;
  fileOp.lpszProgressTitle     = NULL;
  fileOp.fFlags =
    FOF_ALLOWUNDO |
//  FOF_NOCONFIRMATION |
//  FOF_NOERRORUI |
//  FOF_SILENT |
//  FOF_NOCONFIRMATION |
    0;

  if ( SHFileOperation( &fileOp ) == 0 )
    {
      bResult = TRUE;
    }
  else
    {
      bResult = FALSE;
    }

  ::HeapFree( GetProcessHeap(), 0, pTop );

  return bResult;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ファイルをコピー...
// --------------------------------------------------------------------------
BOOL ShCopy( HWND hWnd, const TCHAR *dir, dirent *top_entry, int count, BOOL isCopyMode )
{
  SHFILEOPSTRUCT  fileOp;
  TCHAR*          pTop;
  BOOL            bResult;

  pTop = MakeFileList( top_entry, count );

  if ( pTop == NULL )
    {
      return FALSE;
    }

  /* Set value of SHFILEOPSTRUCT structure member */

  fileOp.hwnd                  = hWnd;
  fileOp.wFunc                 = isCopyMode ? FO_COPY : FO_MOVE;
  fileOp.pFrom                 = pTop;
  fileOp.pTo                   = dir;
  fileOp.fAnyOperationsAborted = FALSE;
  fileOp.hNameMappings         = NULL;
  fileOp.lpszProgressTitle     = NULL;
  fileOp.fFlags =
//    FOF_ALLOWUNDO |
//    FOF_RENAMEONCOLLISION |
//    FOF_NOCONFIRMATION |
//    FOF_NOERRORUI |
//    FOF_SILENT |
//    FOF_NOCONFIRMATION |
    0;

  if ( SHFileOperation( &fileOp ) == 0 )
    {
      bResult = TRUE;
    }
  else
    {
      bResult = FALSE;
    }

  ::HeapFree( GetProcessHeap(), 0, pTop );

  return bResult;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ファイルをコピー...
// --------------------------------------------------------------------------
BOOL ShCopy( HWND hWnd, const TCHAR *src, const TCHAR *des, BOOL isCopyMode )
{
  SHFILEOPSTRUCT  fileOp;
  BOOL            bResult;

  /* Set value of SHFILEOPSTRUCT structure member */

  fileOp.hwnd                  = hWnd;
  fileOp.wFunc                 = isCopyMode ? FO_COPY : FO_MOVE;
  fileOp.pFrom                 = src;
  fileOp.pTo                   = des;
  fileOp.fAnyOperationsAborted = FALSE;
  fileOp.hNameMappings         = NULL;
  fileOp.lpszProgressTitle     = NULL;
  fileOp.fFlags =
    FOF_ALLOWUNDO |
    FOF_RENAMEONCOLLISION |
    FOF_NOCONFIRMATION |
    FOF_NOERRORUI |
    FOF_SILENT |
    0;

  if ( SHFileOperation( &fileOp ) == 0 )
    {
      bResult = TRUE;
    }
  else
    {
      bResult = FALSE;
    }

  return bResult;
}
// --------------------------------------------------------------------------


// ==========================================================================
//
// --------------------------------------------------------------------------
HGLOBAL MakeHDropFiles( dirent *top_entry )
{
  size_t      len;
  HDROP       hDrop;
  DROPFILES*  pFiles;
  WCHAR*      pWc;
  dirent*     entry;

  /* Add header size part in advance */

  len = sizeof( DROPFILES ) + 2;

  for ( entry = top_entry; entry; entry = entry->next )
    {
      len += _tcslen( entry->d_name );
      len++;
    }

#ifndef UNICODE
  hDrop = ( HDROP )::GlobalAlloc( GHND, len );
#else
  hDrop = ( HDROP )::GlobalAlloc( GHND, len * sizeof( WCHAR ) );
#endif

  if ( hDrop )
    {
      pFiles = ( DROPFILES* )::GlobalLock( hDrop );

      ZeroMemory( pFiles, len * sizeof( WCHAR ) );

      pFiles->pFiles  = sizeof( DROPFILES );
      pFiles->pt.x  = 0;
      pFiles->pt.y  = 0;
      pFiles->fNC   = 0;
      pFiles->fWide = 1;

      pFiles++;
      pWc = ( WCHAR* )pFiles;

      for ( entry = top_entry; entry; entry = entry->next )
        {
#ifndef UNICODE
          pWc += ::MultiByteToWideChar( CP_ACP, 0, entry->d_name, -1, pWc, MAX_PATH );
#else
          len = _tcslen( entry->d_name ) + 1;

          _tcscpy_s( pWc, len, entry->d_name );

          pWc += len;
#endif
        }

      ::GlobalUnlock( hDrop );
    }

  return hDrop;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
HANDLE OpenVolume( TCHAR cDriveLetter )
{
  UINT  uDriveType;
  TCHAR szVolumeName[8];
  TCHAR szRootName[5];
  DWORD dwAccessFlags;

  _stprintf_s( szRootName, 5, ROOTFORMAT, cDriveLetter );

  uDriveType = GetDriveType( szRootName );

  switch ( uDriveType )
    {
      case DRIVE_REMOVABLE:
        dwAccessFlags = GENERIC_READ | GENERIC_WRITE;
        break;
      case DRIVE_CDROM:
        dwAccessFlags = GENERIC_READ;
        break;
      default:
        return INVALID_HANDLE_VALUE;
    }

  _stprintf_s( szVolumeName, 8, VOLUMEFORMAT, cDriveLetter );

  return CreateFile( szVolumeName, dwAccessFlags, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
BOOL CloseVolume( HANDLE hVolume )
{
  return CloseHandle( hVolume );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
BOOL LockVolume( HANDLE hVolume )
{
  DWORD dwBytesReturned;
  DWORD dwSleepAmount;
  int   nTryCount;

  dwSleepAmount = LOCK_TIMEOUT / LOCK_RETRIES;

  /* Do this in a loop until a timeout period has expired */

  for ( nTryCount = 0; nTryCount < LOCK_RETRIES; nTryCount++ )
    {
      if ( DeviceIoControl( hVolume, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL ) )
        {
          return TRUE;
        }
      Sleep( dwSleepAmount );
    }

  return FALSE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
BOOL DismountVolume( HANDLE hVolume )
{
  DWORD dwBytesReturned;

  return DeviceIoControl( hVolume, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
BOOL PreventRemovalOfVolume( HANDLE hVolume, BOOL fPreventRemoval )
{
  DWORD dwBytesReturned;
  PREVENT_MEDIA_REMOVAL PMRBuffer;

  PMRBuffer.PreventMediaRemoval = ( BOOLEAN )fPreventRemoval;

  return DeviceIoControl( hVolume, IOCTL_STORAGE_MEDIA_REMOVAL, &PMRBuffer, sizeof(PREVENT_MEDIA_REMOVAL), NULL, 0, &dwBytesReturned, NULL );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
BOOL AutoEjectVolume( HANDLE hVolume )
{
  DWORD dwBytesReturned;

  return DeviceIoControl( hVolume, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0, &dwBytesReturned, NULL );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
BOOL EjectVolume( TCHAR cDriveLetter )
{
  HANDLE  hVolume;
  BOOL  fRemoveSafely = FALSE;
  BOOL  fAutoEject = FALSE;

  /* Open the volume. */

  hVolume = OpenVolume( cDriveLetter );

  if ( hVolume == INVALID_HANDLE_VALUE )
    {
      return FALSE;
    }

  do
    {
      /* Lock and dismount the volume. */

      if ( !LockVolume( hVolume ) )
        {
          break;
        }

      if ( !DismountVolume( hVolume ) )
        {
          break;
        }

      fRemoveSafely = TRUE;

      /* Set prevent removal to false and eject the volume. */

      if ( !PreventRemovalOfVolume( hVolume, FALSE ) )
        {
          break;
        }

      if ( !AutoEjectVolume( hVolume ) )
        {
          break;
        }

      fAutoEject = TRUE;
    }
  while( 0 );

  CloseVolume( hVolume );

  return fAutoEject;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void DispFileStatus( HDC hDC, RECT *rc, dirent *entry, const TCHAR *fontname, int fontsize )
{
  xTmpDC      dc( hDC );
  TCHAR       str[MAX_PATH];
  TCHAR       buf[ 1000 ];
  SHFILEINFO  shfi;
  SYSTEMTIME  st;
  FILETIME    local;
  int         height;

  /* テキスト表示の初期化 */

  dc.SetSelectFont( fontname, fontsize );
  dc.SetBkMode( TRANSPARENT );

  dc.tm.tmHeight += 2;

  ::SHGetFileInfo( entry->d_name, FILE_ATTRIBUTE_ARCHIVE, &shfi, sizeof( SHFILEINFO ), SHGFI_TYPENAME | SHGFI_ICON | SHGFI_ADDOVERLAYS );

  ::DrawIconEx(dc, rc->left, rc->top, shfi.hIcon, DPIX(32), DPIY(32), 0, 0, DI_NORMAL);

  ::DestroyIcon(shfi.hIcon);

  rc->left   += DPIX(40);
  rc->top    += DPIY(10);
  rc->bottom  = rc->top + dc.tm.tmHeight;

  PathCompactPathEx( buf, PathFindFileName( entry->d_name ), ( rc->right - rc->left ) / dc.tm.tmAveCharWidth, 0 );
  dc.Printf( rc, DT_END_ELLIPSIS, buf );

  rc->left = 16;

  /* ファイル情報を表示 */

  rc->top    += 40;
  rc->bottom  = rc->top + dc.tm.tmHeight;

  _stprintf_s( str, MAX_PATH, _T( "Type   : %s" ), shfi.szTypeName );

  PathCompactPathEx( buf, str, ( rc->right - rc->left ) / dc.tm.tmAveCharWidth, 0 );

  dc.TextOut( rc->left, rc->top, buf );

  /* 更新日時の表示 */

  height = dc.tm.tmHeight + DPIY(8);

  rc->top    += height;
  rc->bottom += height;

  FileTimeToLocalFileTime( &entry->ftCreationTime, &local );
  FileTimeToSystemTime( &local, &st );
  _stprintf_s( str, MAX_PATH, _T( "Create : %4d-%02d-%02d %2d:%02d:%02d" ), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
  dc.TextOut( rc->left, rc->top, str );

  rc->top    += height;
  rc->bottom += height;

  FileTimeToLocalFileTime( &entry->ftLastWriteTime, &local );
  FileTimeToSystemTime( &local, &st );
  _stprintf_s( str, MAX_PATH, _T( "Update : %4d-%02d-%02d %2d:%02d:%02d" ), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
  dc.TextOut( rc->left, rc->top, str );

  rc->top    += height;
  rc->bottom += height;

  FileTimeToLocalFileTime( &entry->ftLastAccessTime, &local );
  FileTimeToSystemTime( &local, &st );
  _stprintf_s( str, MAX_PATH, _T( "Access : %4d-%02d-%02d %2d:%02d:%02d" ), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
  dc.TextOut( rc->left, rc->top, str );

  /* ファイルサイズの表示 */

  rc->top    += height;
  rc->bottom += height;

  if ( !( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
    {
      ssnprintf( buf, 64, _T( "Size   : %,u byte" ), entry->nFileSizeLow );
      dc.TextOut( rc->left, rc->top, buf );
      rc->top    += height;
      rc->bottom += height;
    }

  _tcscpy_s( str, MAX_PATH, _T( "Place  : " ) );
  _tcscat_s( str, MAX_PATH, entry->d_name );

  PathRemoveFileSpec( str );
  PathCompactPathEx( buf, str, ( rc->right - rc->left ) / dc.tm.tmAveCharWidth, 0 );

  dc.TextOut( rc->left, rc->top, buf );

}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void StrTrimRight( TCHAR *str, const TCHAR *toks )
{
  const TCHAR*  cp;
        TCHAR*  target = NULL;

  while( *str )
    {
      for ( cp = toks; *cp; cp++ )
        {
          if( *cp == *str )
            {
              if ( target == NULL )
                {
                  target = str;
                }
              break;
            }
        }

      if ( *cp == '\0' )
        {
          target = NULL;
        }

#ifndef UNICODE
      if( ( ( unsigned int )( ( TCHAR )( *cp ) ^ 0x20 ) - 0xa1 < 0x3c ) )
        {
          str++;
        }
#endif
      str++;
    }

  if ( target )
    {
      *target = '\0';
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
DWORD GetHostByNameA( LPCSTR name )
{
  LPHOSTENT host = gethostbyname( name );
  DWORD     ip   = 0;

  if ( host )
    {
      ip = *( DWORD* )host->h_addr;
    }

  return ip;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
DWORD GetHostByNameW( LPCWSTR name )
{
  int     len;
  LPSTR   cp;

  len = WideCharToMultiByte( CP_UTF8, 0, name, -1, 0, 0, NULL, NULL );
  cp  = ( LPSTR )alloca( len + 2 );

  WideCharToMultiByte( CP_UTF8, 0, name, -1, cp, len, 0, 0 );

  return GetHostByNameA( cp );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL IsUNCOK( const TCHAR *in_path )
{
  TCHAR             path[ MAX_PATH ];
  TCHAR*            cp;
  BOOL              res = FALSE;
  HANDLE            hicmp;
  TCHAR             send_data[] = /*"ICMP SEND DATA"*/_T( "shinosan" );
  ICMP_ECHO_REPLY*  echo_r;
  DWORD             retval;
  DWORD             len;
  DWORD             ip;
  WSADATA           wsa;
  TCHAR             drv[ 3 ];
  TCHAR             net_path[ MAX_PATH ];
  DWORD             length = MAX_PATH;
  TCHAR*            context;

  if ( !PathIsUNC( in_path ) )
    {
      /* PATHがUNC形式のものではない = 通常のドライブ付パス名 */

      drv[ 0 ] = in_path[ 0 ];
      drv[ 1 ] = in_path[ 1 ];
      drv[ 2 ] = '\0';

      if ( GetDriveType( drv ) != DRIVE_REMOTE )
        {
          /* リモートに割り当てられているドライブではない = 通常の物理ドライブ */

          return TRUE;
        }

      /* リモートに割り当てられているドライブ名をネットワークパスに変換 */

      if ( WNetGetConnection( drv, net_path, &length ) != NO_ERROR )
        {
          return FALSE;
        }

      in_path = net_path;
    }

  _tcscpy_s( path, MAX_PATH, in_path );

  cp = _tcstok_s( path, _T( "\\" ), &context );

  if ( cp == NULL )
    {
      return FALSE;
    }

  WSAStartup( 2, &wsa );

  ip = GetHostByName( cp );

  WSACleanup();

  if ( ip == 0 )
    {
      return FALSE;
    }

  hicmp = IcmpCreateFile();

  len = ( DWORD)( sizeof( ICMP_ECHO_REPLY ) + ( _tcslen( send_data ) + 1 ) * sizeof( TCHAR ) );

  echo_r = ( ICMP_ECHO_REPLY* )alloca( len );

  if ( echo_r )
    {
      ZeroMemory( echo_r, len );

      retval = IcmpSendEcho( hicmp, ip, send_data, ( WORD )_tcslen( send_data ), NULL, echo_r, len, 1500 );

      if ( retval > 0 && echo_r->DataSize > 0 )
        {
          res = TRUE;
        }
    }

  IcmpCloseHandle( hicmp );

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL IsAccessOK( const TCHAR *dir )
{
  PSECURITY_DESCRIPTOR  descriptor;
  BOOL                  res = FALSE;

  if ( GetNamedSecurityInfo( ( TCHAR* )dir, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, NULL, NULL, &descriptor ) == ERROR_SUCCESS )
    {
      res = TRUE;
      LocalFree( descriptor );
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL IsFolderAccessOK( const TCHAR *dir )
{
  BOOL            res = FALSE;
  WIN32_FIND_DATA fd;
  HANDLE          hFile;
  TCHAR           buf[MAX_PATH];

  _tcscpy_s(buf, MAX_PATH, dir);

  PathAppend(buf, _T("*.*"));

  hFile = ::FindFirstFile(buf, &fd);

  if (hFile != INVALID_HANDLE_VALUE)
    {
      ::FindClose(hFile);

      res = TRUE;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL IsFolderExist( const TCHAR *dir )
{
  return PathFileExists( dir );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL IsRootPathOK( const TCHAR *path )
{
  TCHAR   tmp[ MAX_PATH ];

  _tcscpy_s( tmp, MAX_PATH, path );

  PathStripToRoot( tmp );

  return PathFileExists( tmp );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int CheckPath( const TCHAR *in_path, int c_no, HWND hWnd, int msg_on )
{
  int   i;
  TCHAR buf [ 128 ];
  TCHAR path[ MAX_PATH ];

  _tcscpy_s(path, MAX_PATH, in_path);

  PathRemoveBackslash(path);

  struct {
    const TCHAR*  msg1;
    const TCHAR*  msg2;
    BOOL          ( *func )( const TCHAR* dir );
  }
  test_pass[] = {
    { _T( "Location unavailable !!" ), _T( "Possibility of no connection destination." ), IsUNCOK },
    { _T( "Location unavailable !!" ), _T( "No drive reference." ),                       IsRootPathOK },
    { _T( "Location unavailable !!" ), _T( "No folder reference." ),                      IsFolderExist },
    { _T( "Location unavailable !!" ), _T( "operation prohibited." ),                     IsAccessOK },
    { _T( "Location unavailable !!" ), _T( "Not allowed for temporary employees." ),      IsFolderAccessOK },
    { NULL, NULL, NULL },
  };

  for ( i = 0; test_pass[ i ].func && i < c_no; i++ )
    {
      if ( test_pass[ i ].func( path ) )
        {
          continue;
        }

      if ( msg_on )
        {
          _stprintf_s( buf, 128, _T( "\r\nCannot access \"%s\" !\r\n\r\n\r\n%s\r\n\r\n" ), path, test_pass[ i ].msg2 );
          ::MessageBox( hWnd, buf, test_pass[ i ].msg1, MB_ICONSTOP );
        }

      break;
    }

  return i;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL CheckPathRules( const TCHAR *path )
{
  BOOL  res  = FALSE;
  TCHAR top1 = path[ 0 ];
  TCHAR top2 = path[ 1 ];
  TCHAR top3 = path[ 2 ];

  if ( ( 'a' <= top1 && top1 <= 'z' ) || ( 'A' <= top1 && top1 <= 'Z' ) )
    {
      if ( top2 == ':' && top3 == '\\' )
        {
          res = TRUE;
        }
    }
  else if ( top1 == '\\' && top2 == '\\' )
    {
      if ( PathIsUNC( path ) )
        {
          res = TRUE;
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL EasyPictureCheck( BYTE data[] )
{
  BOOL  res = TRUE;

  if ( data[ 0 ] == 0x42 && data[ 1 ] == 0x4D )
    {
    }
  else if ( data[ 0 ] == 0xFF && data[ 1 ] == 0xD8 && data[ 2 ] == 0xFF )
    {
    }
  else if ( data[ 0 ] == 0x47 && data[ 1 ] == 0x49 && data[ 2 ] ==0x46 && data[ 3 ] == 0x38 && data[ 4 ] == 0x39 && data[ 5 ] == 0x61 )
    {
    }
  else if ( data[ 0 ] == 0x47 && data[ 1 ] == 0x49 && data[ 2 ] ==0x46 && data[ 3 ] == 0x38 && data[ 4 ] == 0x37 && data[ 5 ] == 0x61 )
    {
    }
  else if ( data[ 0 ] == 0x89 && data[ 1 ] == 0x50 && data[ 2 ] ==0x4E && data[ 3 ] == 0x47 && data[ 4 ] == 0x0D && data[ 5 ] == 0x0A )
    {
    }
  else
    {
      res = FALSE;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
HGLOBAL AllocAndReadFile( TCHAR *path, DWORD *size )
{
  HGLOBAL hBuf = NULL;
  DWORD   temp;
  LPVOID  lpBuf;
  HANDLE  hFile;
  BYTE    data[ 6 ];

  *size = 0;

  if ( ( hFile = CreateFile( path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ) == NULL )
    {
      return NULL;
    }

  *size = GetFileSize( hFile, NULL );

  if ( *size > 0 )
    {
      ReadFile( hFile, data, 6, &temp, NULL );

      if ( EasyPictureCheck( data ) )
        {
          SetFilePointer( hFile, 0, 0, FILE_BEGIN );

          if ( ( hBuf = GlobalAlloc( GMEM_MOVEABLE, *size ) ) != NULL )
            {
              lpBuf = GlobalLock( hBuf );

              ReadFile( hFile, lpBuf, *size, &temp, NULL );

              GlobalUnlock( hBuf );
            }
        }
    }
  CloseHandle( hFile );

  return hBuf;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BITMAPINFO *LoadImage( dirent *entry )
{
  IStream*      isImage;
  IPicture*     ipicImage;
  HGLOBAL       hBuf;
  HBITMAP       hBmp;
  BITMAP        bmpInfo;
  LPBITMAPINFO  lpbiBmp = NULL;
  LPBYTE        lpBmp;
  DWORD         dwFileSize;
  HRESULT       res;
  TCHAR*        ext = PathFindExtension( entry->d_name );

  do
    {
      if ( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
          break;
        }

      if (_tcsicmp( ext, _T( ".bmp" ) )  != 0
       && _tcsicmp( ext, _T( ".jpg" ) )  != 0
       && _tcsicmp( ext, _T( ".jpeg" ) ) != 0
       && _tcsicmp( ext, _T( ".png" ) )  != 0
       && _tcsicmp( ext, _T( ".gif" ) )  != 0 )
        {
          break;
        }

      if ( ( hBuf = AllocAndReadFile( entry->d_name, &dwFileSize ) ) == NULL )
        {
          break;
        }

      /* Create an IStream from a memory block */

      CreateStreamOnHGlobal( hBuf, TRUE, &isImage );

      /* Create an image object from a stream of image files */

      res = OleLoadPicture( isImage, dwFileSize, FALSE, IID_IPicture, ( LPVOID* )&ipicImage );

      isImage->Release();         /* Stream release */
      GlobalFree( hBuf );         /* Memory block release */

      if ( res != S_OK )
        {
          break;
        }

      ipicImage->get_Handle( ( OLE_HANDLE* )&hBmp );    /* Get handle of image object */

      GetObject( hBmp, sizeof( BITMAP ), &bmpInfo );    /* Get BITMAP information */

      /* Secure memory for DIB */

      if ( ( lpBmp = ( LPBYTE )GlobalAlloc( GPTR, sizeof( BITMAPINFO ) + ( bmpInfo.bmWidth * bmpInfo.bmHeight * 4 ) ) ) != NULL )
        {
          /* BITMAPINFO structure setting for 32-bit DIB */

          lpbiBmp = ( LPBITMAPINFO )lpBmp;
          lpbiBmp->bmiHeader.biSize     = sizeof( BITMAPINFOHEADER );
          lpbiBmp->bmiHeader.biWidth    = bmpInfo.bmWidth;
          lpbiBmp->bmiHeader.biHeight   = bmpInfo.bmHeight;
          lpbiBmp->bmiHeader.biPlanes   = 1;
          lpbiBmp->bmiHeader.biBitCount = 32;

          /* Get DIB pixel column */

          GetDIBits( GetDC( NULL ), hBmp, 0, bmpInfo.bmHeight, lpBmp + sizeof( BITMAPINFO ), lpbiBmp, DIB_RGB_COLORS ); 
        }

      ipicImage->Release();
    }
  while( 0 );

  return lpbiBmp;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL GetShortcutInfo( TCHAR *fnam, TCHAR *lnkfile, DWORD *attr )
{
  BOOL            res = FALSE;
  WIN32_FIND_DATA fd;
  WCHAR           wsz[ MAX_PATH ];
  IShellLink*     psl = NULL;
  IPersistFile*   ppf = NULL;

  if( _tcsicmp( PathFindExtension( fnam ), _T( ".lnk" ) ) != 0 )
    {
      return res;
    }

  CoInitialize( NULL );

  do
    {
      if ( FAILED( CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, ( LPVOID* )&psl ) ) )
        {
          break;
        }

      if( FAILED( psl->QueryInterface( IID_IPersistFile, ( LPVOID* )&ppf ) ) )
        {
          break;
        }
#ifndef UNICODE
      MultiByteToWideChar( CP_ACP, 0, fnam, -1, ( LPWSTR )wsz, MAX_PATH );
#else
      _tcscpy_s( wsz, MAX_PATH, fnam );
#endif
      if ( FAILED( ppf->Load( ( LPCOLESTR )wsz, STGM_READ ) ) )
        {
          break;
        }

      if ( lnkfile )
        {
          psl->GetPath( lnkfile, MAX_PATH, ( WIN32_FIND_DATA* )&fd, 0 );

          if ( attr )
            {
              *attr = fd.dwFileAttributes;
            }
        }

      res = TRUE;
    }
  while( 0 );

  if ( ppf )
    {
      ppf->Release(); 
    }

  if ( psl )
    {
      psl->Release(); 
    }

  CoUninitialize();

  return res; 
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static BOOL CALLBACK MonitorProc(HMONITOR hMonitor, HDC hDC, LPRECT pRC, LPARAM param)
{
  MONITORINFOEX info;
  RECT*         rc = (RECT *)param;

  info.cbSize = sizeof(info);

  GetMonitorInfo(hMonitor, &info);

  rc->left    = 0;
  rc->top     = 0;
  rc->right  += pRC->right  - pRC->left;
  rc->bottom += pRC->bottom - pRC->top;

  return TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL GetDisplayMonitors(RECT *pRC)
{
  return EnumDisplayMonitors(NULL, NULL, MonitorProc, (LPARAM)pRC);
}
// --------------------------------------------------------------------------
