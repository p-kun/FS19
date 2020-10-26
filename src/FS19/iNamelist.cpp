/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iNamelist.cpp
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
#include <shlwapi.h>
#include <TCHAR.H>
#include <malloc.H>
#include "dirent.h"
#include "savedir.h"
#include "strverscmp.h"
#include "SubFuncs.h"
#include "iBaseView.h"
#include "logf.h"
#include "iPt.h"
#include "xgit.h"
#include "iHistory.h"
#include "iNamelist.h"

/* Private parameters */

static int	s_order = 1;
static int	s_kind  = INAM_KIND_EXT;

/* Public functions */

extern DWORD GetSHA1(const TCHAR *in_file, BYTE *sha, DWORD sha_len);

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iNamelist::iNamelist()
{
  m_namelist = NULL;
  m_total    = 0;
  m_cancel   = FALSE;
  m_lock     = FALSE;
  m_isRemote = FALSE;

  ZeroMemory( m_dir, sizeof( m_dir ) );

  m_seach_mode = INAM_MODE_NORMAL;

  for ( int i = 0; i < 16; i++ )
    {
      _tcscpy_s( m_historys[ i ], MAX_PATH, _T( "" ) );
      _tcscpy_s( m_historys_back[ i ], MAX_PATH, _T( "" ) );
    }
  m_history_no = 0;

  for ( int i = 0; i < WC_LIST_MAX; i++ )
    {
      m_argv[ i ] = NULL;
    }

  m_argc       = 0;
  m_fcnt       = 0;
  m_dcnt       = 0;

  mCopeingFileName[ 0 ]         = '\0';
  mCopeingCurrentDirectory[ 0 ] = '\0';
  mCopeingFileIndex             = -1;
  mCopeingCounter               = 0;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iNamelist::~iNamelist()
{
  delete_scandir( m_namelist );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
static TCHAR *RemoveFileSpec( TCHAR *buf, const TCHAR *name )
{
  _tcscpy_s( buf, MAX_PATH, name );

  PathRemoveFileSpec( buf );

  return PathFindFileName( name );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int sort_up( const TCHAR *pFn1, const TCHAR *pFn2 )
{
  int   res = 0;

  if ( _tcscmp( PathFindFileName( pFn1 ), _T( ".." ) ) == 0 )
    {
      res = -1;
    }
  else if ( _tcscmp( PathFindFileName( pFn2 ), _T( ".." ) ) == 0 )
    {
      res = 1;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int sort_attr( DWORD attr1, DWORD attr2 )
{
  if ( attr1 & FILE_ATTRIBUTE_SYSTEM )
    {
      attr1 = FILE_ATTRIBUTE_SYSTEM;
    }
  else if ( attr1 & FILE_ATTRIBUTE_HIDDEN )
    {
      attr1 = FILE_ATTRIBUTE_HIDDEN;
    }
  else if ( attr1 & FILE_ATTRIBUTE_READONLY )
    {
      attr1 = FILE_ATTRIBUTE_READONLY;
    }
  else
    {
      attr1 = 0;
    }

  if ( attr2 & FILE_ATTRIBUTE_SYSTEM )
    {
      attr2 = FILE_ATTRIBUTE_SYSTEM;
    }
  else if ( attr2 & FILE_ATTRIBUTE_HIDDEN )
    {
      attr2 = FILE_ATTRIBUTE_HIDDEN;
    }
  else if ( attr2 & FILE_ATTRIBUTE_READONLY )
    {
      attr2 = FILE_ATTRIBUTE_READONLY;
    }
  else
    {
      attr2 = 0;
    }

  return attr2 - attr1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int sort_dir( DWORD attr1, DWORD attr2 )
{
  attr1 &= FILE_ATTRIBUTE_DIRECTORY;
  attr2 &= FILE_ATTRIBUTE_DIRECTORY;

  return attr2 - attr1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- CVS対応ファイルでソート
// --------------------------------------------------------------------------
static int sort_cvs( int cvs1, int cvs2 )
{
  return cvs2 - cvs1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- CVS対応ファイルでソート
// --------------------------------------------------------------------------
static int sort_cvs3( int cvs1, int cvs2 )
{
  if ( cvs1 )
    {
      cvs1 = 1;
    }

  if ( cvs2 )
    {
      cvs2 = 1;
    }

  return cvs2 - cvs1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- CVS対応ファイルでソート
// --------------------------------------------------------------------------
static int sort_cvs2( int cvs1, int cvs2 )
{
  if ( cvs2 == MARK_MODIFY ||
       cvs2 == MARK_ADD    ||
       cvs2 == MARK_MARGE  ||
       cvs2 == MARK_CONF )
    {
      cvs2 = 1;
    }
  else
    {
      cvs2 = 0;
    }

  if ( cvs1 == MARK_MODIFY ||
       cvs1 == MARK_ADD    ||
       cvs1 == MARK_MARGE  ||
       cvs1 == MARK_CONF )
    {
      cvs1 = 1;
    }
  else
    {
      cvs1 = 0;
    }

  return cvs2 - cvs1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int sort_idx( int idx1, int idx2 )
{
  return idx2 -idx1;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int sort_time( const dirent *dir1, const dirent *dir2 )
{
  int   res = 0;

  if (dir1->ftLastWriteTime.dwHighDateTime > dir2->ftLastWriteTime.dwHighDateTime)
    {
      res = -1;
    }
  else if (dir1->ftLastWriteTime.dwHighDateTime < dir2->ftLastWriteTime.dwHighDateTime)
    {
      res = 1;
    }
  else
    {
      if (dir1->ftLastWriteTime.dwLowDateTime > dir2->ftLastWriteTime.dwLowDateTime)
        {
          res = -1;
        }
      else if (dir1->ftLastWriteTime.dwLowDateTime < dir2->ftLastWriteTime.dwLowDateTime)
        {
          res = 1;
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int sort_size( const dirent *dir1, const dirent *dir2 )
{
  int   res = 0;

  if (dir1->nFileSizeHigh > dir2->nFileSizeHigh)
    {
      res = -1;
    }
  else if (dir1->nFileSizeHigh < dir2->nFileSizeHigh)
    {
      res = 1;
    }
  else
    {
      if (dir1->nFileSizeLow > dir2->nFileSizeLow)
        {
          res = -1;
        }
      else if (dir1->nFileSizeLow < dir2->nFileSizeLow)
        {
          res = 1;
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int Compar1( const dirent **entry1, const dirent **entry2 )
{
  const dirent* dir1 = *entry1;
  const dirent* dir2 = *entry2;
  TCHAR         buf1[ MAX_PATH * 2 ];
  TCHAR         buf2[ MAX_PATH * 2 ];
  int           res;
  TCHAR*        nam1 = ( TCHAR* )PathFindFileName ( dir1->d_name );
  TCHAR*        nam2 = ( TCHAR* )PathFindFileName ( dir2->d_name );
  TCHAR*        ext1 = ( TCHAR* )PathFindExtension( dir1->d_name );
  TCHAR*        ext2 = ( TCHAR* )PathFindExtension( dir2->d_name );

  res = sort_up( nam1, nam2 );

  if (res)
    {
      return res;
    }

  RemoveFileSpec( buf1, dir1->d_name );
  RemoveFileSpec( buf2, dir2->d_name );

  res = strverscmp( buf1, buf2 );

  if (res)
    {
      return res;
    }

  res = sort_dir( dir1->dwFileAttributes, dir2->dwFileAttributes );

  if ( res )
    {
      return res;
    }

  res = sort_attr( dir1->dwFileAttributes, dir2->dwFileAttributes );

  if ( res )
    {
      return res;
    }

  res = _tcsicmp( ext1, ext2 );

  if ( res )
    {
      return res;
    }

  return strverscmp( nam1, nam2 );
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
              if ( ::MessageBox( hWnd,
                                 _T( "Searching. Would you like to cancel?" ),
                                 _T( "What are you doing?" ),
                                 MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDYES )
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
// -- 
// --------------------------------------------------------------------------
static int Filter1( dirent *entry )
{
  TCHAR*    path = PathFindFileName( entry->d_name );
  int       res  = 0;
  TACTION*  act  = ( TACTION* )entry->param;

  if ( entry->cAlternateFileName[ 0 ] == _T( '\0' ) )
    {
      memcpy( entry->cAlternateFileName, path, sizeof( entry->cAlternateFileName ) );
    }

  entry->bookMark = 0;

  if ( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
      if( _tcscmp( path, _T( ".." ) ) == 0 )
        {
          if( entry->depth == 0 )
            {
              res = DENT_FLT_MALLOC;
            }
        }
      else
        {
          if ( entry->depth < 20 )
            {
              res = DENT_FLT_RECURSION/* | DENT_FLT_MALLOC*/;
            }
          else
            {
              res = DENT_FLT_MALLOC;
            }
        }
    }
  else
    {
      if ( act->argc > 0 )
        {
          for ( int i = 0; i < act->argc; i++ )
            {
              if ( PathMatchSpec( path, act->argv[ i ] ) )
                {
                  res = DENT_FLT_MALLOC;
                  break;
                }
            }
        }
    }

  if ( act )
    {
      if ( *( act->cancel ) )
        {
          res = DENT_FLT_CANCEL;
        }
      else
        {
          if ( PumpMessage( act->hWnd ) )
            {
              *( act->cancel ) = 1;
              res = DENT_FLT_CANCEL;
            }
        }
    }

  act->entry = NULL;

  act->base->NamelistAction( act );

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int Compar0( const dirent **entry1, const dirent **entry2 )
{
  const dirent* dir1 = *entry1;
  const dirent* dir2 = *entry2;
  int           res  = 0;
  TCHAR*        nam1 = ( TCHAR* )dir1->d_name;
  TCHAR*        nam2 = ( TCHAR* )dir2->d_name;
  TCHAR*        ext1 = ( TCHAR* )PathFindExtension( dir1->d_name );
  TCHAR*        ext2 = ( TCHAR* )PathFindExtension( dir2->d_name );
//unsigned int  idx1 = dir1->userData >> 24;
//unsigned int  idx2 = dir2->userData >> 24;
  unsigned int  idx1 = dir1->userData;
  unsigned int  idx2 = dir2->userData;

  /* Directory ".." */

  res = sort_up( nam1, nam2 );

  if ( res ) return res;

  /* Directory */

  res = sort_dir( dir1->dwFileAttributes, dir2->dwFileAttributes );

  if ( res ) return res;

  /* Attributey */

  res = sort_attr( dir1->dwFileAttributes, dir2->dwFileAttributes );

  if ( res ) return res;

  /* Overray icon */

  res = sort_idx(idx1, idx2);

  if( res ) return res;

  if (s_kind == INAM_KIND_SIZE)
    {
      /* File size */

      res = sort_size( dir1, dir2 ) * s_order;
    }
  else if (s_kind == INAM_KIND_TIME)
    {
      /* Date time */

      res = sort_time( dir1, dir2 ) * s_order;
    }
  else if (s_kind == INAM_KIND_EXT)
    {
      /* Extention */

      res = _tcsicmp( ext1, ext2 ) * s_order;
    }

  if ( res ) return res;

  /* Filename */

  return strverscmp( nam1, nam2 ) * s_order;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int Filter0( dirent *entry )
{
  TCHAR*    path = PathFindFileName( entry->d_name );
  TACTION*  act  = ( TACTION* )entry->param;

  if ( entry->cAlternateFileName[ 0 ] == _T( '\0' ) )
    {
      memcpy( entry->cAlternateFileName, path, sizeof( entry->cAlternateFileName ) );
    }

  entry->bookMark = 0;

  act->base->NamelistAction( act );

  return DENT_FLT_MALLOC;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static int D_Filter0(dirent *entry)
{
  D_NODE*   d_node;
  D_NODE*   d_parent;
  int       i;
  int       d_cnt = 0;
  dirent*   top;
  TACTION*  act;

  if (!entry)
    {
      return 0;
    }

  d_parent = savedir(entry->d_name);
  act      = (TACTION *)entry->param;

  if (d_parent)
    {
      if (!d_parent->exist)
        {
          if (entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
              GetTempDirectoryIconIndex((HANDLE *)&d_parent->hHandle, &d_parent->d_no);
            }
          else
            {
              GetTempFileIconIndex((HANDLE *)&d_parent->hHandle, &d_parent->d_no);
            }
        }

      entry->hHandle  = d_parent->hHandle;
      entry->userData = d_parent->usr_data;

      for (top = entry->next, i = 0; i < entry->d_cnt && top && i < 1000; i++, top = top->next)
        {
          if ((d_node = node_search(d_parent, PathFindFileName(top->d_name))) == NULL)
            {
              break;
            }

          if (!d_node->exist || d_node->hHandle == NULL)
            {
              if (top->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                  GetTempDirectoryIconIndex((HANDLE *)&d_node->hHandle, &d_node->d_no);
                }
              else
                {
                  d_cnt++;
                  GetTempFileIconIndex((HANDLE *)&d_node->hHandle, &d_node->d_no);
                }
            }

          top->hHandle  = d_node->hHandle;
          top->userData = d_node->usr_data;
        }

      for (; i < entry->d_cnt && top; i++, top = top->next)
        {
          if (top->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
              GetTempDirectoryIconIndex((HANDLE *)&top->hHandle, &top->userData);
            }
          else
            {
              d_cnt++;
              GetTempFileIconIndex((HANDLE *)&top->hHandle, &top->userData);
            }
        }
    }

  act->entry = entry;
  act->base->NamelistAction(act);

  return 0;
}
// --------------------------------------------------------------------------

/*--------------------------------------------------------------------------*/
static bool CheckSHA1(const BYTE *sha_1, const BYTE *sha_2, int sha_len)
{
  int   i;

  for (i = 0; i < sha_len; i++)
    {
      if (sha_1[i] != sha_2[i])
        {
          break;
        }
    }

  return (i == sha_len);
}

/*--------------------------------------------------------------------------*/
static void clear_sub_dir(D_NODE *node)
{
  while (node)
    {
      if (node->subd)
        {
          node->usr_data = 0;

          clear_sub_dir(node->subd);
        }
      else
        {
          node->usr_data = 0;
        }

      node = node->next;
    }
}

/*--------------------------------------------------------------------------*/
void GitNodeCallback(const GIT_NODE *p_node,
                     const TCHAR    *root,
                     void           *p_param)
{
  struct _stat    buf;
  SAVEDIR_PATTERN pat = SAVEDIR_INVALID;
  DWORD           dat = 1;
  D_NODE         *p_top;

  if (p_param == NULL)
    {
      return;
    }

  p_top = *((D_NODE **)p_param);

  if (p_top == NULL)
    {
      p_top = savedir(root);

      if (p_top == NULL)
        {
          return;
        }

      *((D_NODE **)p_param) = p_top;

      ::SetCurrentDirectory(root);

      /*  Clear list */

      clear_sub_dir(p_top);
    }

  /* Get file timestump */

  _wstat(p_node->path, &buf);

  if (buf.st_mtime != p_node->mtime_hi || buf.st_ctime != p_node->ctime_hi)
    {
      BYTE  sha[20];
      DWORD sha_len;

      sha_len = GetSHA1(p_node->path, sha, sizeof(sha));

      if (sha_len > 0 && !CheckSHA1(sha, p_node->sha1, sha_len))
        {
          pat = SAVEDIR_SRCOR;
          dat = 3;
        }
    }

  if (dat)
    {
      p_top->usr_data |= dat;
    }

  savedir(p_top, p_node->path, dat, pat);
}

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iNamelist::Scandir( const TCHAR dir[], const TCHAR *prev_dir, void *param, int seach_mode )
{
  int             i;
  int             total;
  dirent**        namelist;
  int             current = 0;
  TACTION         act;
  static TCHAR    buf[ MAX_PATH ];
  TCHAR*          cp;
  TCHAR           drive[ 8 ];
  iPt             pt;
  ULARGE_INTEGER  size;
  int ( *filter )( dirent *entry );
  int ( *compar )( const dirent **entry1, const dirent **entry2 );

  m_cancel = FALSE;

  if ( seach_mode < 0 )
    {
      seach_mode = m_seach_mode;
    }

  act.cancel = &m_cancel;
  act.hWnd   = ( HWND )param;
  act.base   = this;
  act.time   = 0/*GetLocalTimeD()*/;
  act.argc   = m_argc;
  act.argv   = m_argv;
  act.s_mode = seach_mode;
  act.entry  = NULL;
  act.stat   = 0;

  if ( m_dir[ 0 ] != dir[ 0 ] )
    {
      _stprintf_s( drive, _T( "%c:\\" ), _T( 'A' ) + PathGetDriveNumber( dir ) );

      if ( PathIsUNC( dir ) || GetDriveType( drive ) == DRIVE_REMOTE )
        {
          m_isRemote = TRUE;
        }
      else
        {
          m_isRemote = FALSE;
        }
    }

  /* Search exception folders */

  for (i = 0; i < pt.search_folder_num; i++)
    {
      if (_tcsstr(dir, pt.search_folders[i]))
        {
          m_isRemote = FALSE;
        }
    }

  if ( _tcscmp( dir, _T( ".." ) ) == 0 )
    {
      PathCombine( buf, m_dir, _T( ".." ) );
    }
  else
    {
      _tcscpy_s( buf, MAX_PATH, dir );
    }

  if ( seach_mode == INAM_MODE_SEACH )
    {
      filter = Filter1;
      compar = Compar1;
    }
  else
    {
      filter = Filter0;
      compar = Compar0;
    }

  /* Current folder search */

  void *params = NULL;
  TCHAR tmp[MAX_PATH];

  ::GetCurrentDirectory(MAX_PATH, tmp);

  scan_git_dir(buf, GitNodeCallback, (void *)&params);

  ::SetCurrentDirectory(tmp);

  total = scandir( buf, &namelist, filter, D_Filter0, NULL, &act );

  qsort( namelist, total, sizeof( dirent* ), ( int(*)( const void*, const void* ) )compar );

  if ( total > 0 )
    {
      _tcscpy_s( m_dir, MAX_PATH, buf );

      m_dcnt = 0;
      m_fcnt = 0;

      m_fsize.QuadPart = 0;

      if ( prev_dir )
        {
          for ( i = 0; i < total; i++ )
            {
              if ( _tcsicmp( namelist[ i ]->d_name, prev_dir ) == 0 )
                {
                  current = i;
                  break;
                }
            }
        }

      for ( i = 0; i < total; i++ )
        {
          if ( namelist[ i ]->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
              if ( _tcscmp( PathFindFileName( namelist[ i ]->d_name ), _T( ".." ) ) != 0 )
                {
                  m_dcnt++;
                }
            }
          else
            {
              m_fcnt++;
              size.LowPart  = namelist[ i ]->nFileSizeLow;
              size.HighPart = namelist[ i ]->nFileSizeHigh;
              m_fsize.QuadPart += size.QuadPart;
            }
        }

      if ( m_total && total )
        {
          int   len = (int)(PathFindFileName( m_namelist[ 0 ]->d_name ) - m_namelist[ 0 ]->d_name);
          int   j;

          if ( StrCmpNI( m_namelist[ 0 ]->d_name, dir, len ) == 0 )
            {
              for ( i = 0; i < m_total; i++ )
                {
                  if( m_namelist[ i ]->bookMark == 0 )
                    {
                      continue;
                    }
                  for ( j = 0; j < total; j++ )
                    {
                      if( namelist[ j ]->bookMark )
                        {
                          continue;
                        }

                      if ( StrCmpI( m_namelist[ i ]->d_name + len, namelist[ j ]->d_name + len ) == 0 )
                        {
                          namelist[ j ]->bookMark = m_namelist[ i ]->bookMark;
                          break;
                        }
                    }
                }
            }
        }

      delete_scandir( m_namelist );

      m_namelist = namelist;
      m_total    = total;

      m_seach_mode = seach_mode;

      for ( mCopeingFileIndex = -1, i = 0; i < total; i++ )
        {
          if ( !( namelist[ i ]->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
            {
              if( StrCmpI( mCopeingFileName, namelist[ i ]->d_name ) == 0 )
                {
                  mCopeingFileIndex = i;
                  break;
                }

              continue;
            }

          if ( ( cp = StrStr( mCopeingFileName, namelist[ i ]->d_name ) ) != NULL )
            {
              if ( *( cp + _tcslen( namelist[ i ]->d_name ) ) == '\\' )
                {
                  mCopeingFileIndex = i;
                  break;
                }
            }
        }
    }
  else
    {
      if( *act.cancel )
        {
          current = -1;
        }
      else
        {
          _tcscpy_s( m_dir, MAX_PATH, buf );
          m_total = 0;
        }
    }

  m_lock = FALSE;

  return current;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iNamelist::Cancel( void )
{
  m_cancel = TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iNamelist::Push( const TCHAR *path, const TCHAR *back_path )
{
  if ( !IsPushDir( path ) )
    {
      return;
    }

#if 0
  TCHAR*  cp = m_back_que.top();

  m_back_que.push( path );

  if ( cp && _tcsicmp( cp, path ) == 0 )
    {
      return;
    }

  m_step_que.clear();
#else
  iHistory  iHis( path );

  if ( _tcsicmp( m_historys[ m_history_no ], path ) == 0 )
    {
      return;
    }

  if ( back_path )
    {
      _tcscpy_s( m_historys_back[ m_history_no ], MAX_PATH, back_path );
    }
  else
    {
      _tcscpy_s( m_historys_back[ m_history_no ], MAX_PATH, path );
    }

  m_history_no++;
  m_history_no &= 0x0F;
  _tcscpy_s( m_historys[ m_history_no ], MAX_PATH, path );
  _tcscpy_s( m_historys[ ( m_history_no + 1 ) & 0x0F ], MAX_PATH, _T( "" ) );
#endif
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iNamelist::Back( const TCHAR **back_path )
{
#if 0
  if ( m_back_que.size() <= 1 )
    {
      return NULL;
    }

  TCHAR*  cp = m_back_que.top();

  m_step_que.push( cp );
  m_back_que.pop();

  if ( back_path )
    {
      *back_path = m_step_que.top();
    }

  return m_back_que.top();
#else
  if ( m_historys[ ( m_history_no - 1 ) & 0xF ][ 0 ] == '\0' )
    {
      return NULL;
    }

  m_history_no--;
  m_history_no &= 0x0F;

  if ( back_path )
    {
      *back_path = m_historys_back[ m_history_no ];
    }

  return m_historys[ m_history_no ];
#endif
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iNamelist::Step( void )
{
#if 0
  TCHAR*  cp = m_step_que.top();

  if ( cp == NULL )
    {
      return NULL;
    }

  m_back_que.push( cp );
  m_step_que.pop();

  return m_step_que.top();
#else
  if ( m_historys[ ( m_history_no + 1 ) & 0xF ][ 0 ] == '\0' )
    {
      return NULL;
    }

  m_history_no++;
  m_history_no &= 0x0F;

  return m_historys[ m_history_no ];
#endif
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iNamelist::IsPushDir(const TCHAR *bk_path)
{
  if(PathIsRoot(bk_path))
    {
      return false;
    }

  BOOL    res = TRUE;
  TCHAR  *buf;
  size_t  len;

  len = _tcslen(bk_path) + 1;

  buf = (TCHAR *)alloca(len * sizeof(TCHAR));

  _tcscpy_s(buf, len, bk_path);

  PathStripToRoot(buf);

  switch (GetDriveType(buf))
    {
      case DRIVE_RAMDISK:
      case DRIVE_REMOTE:
      case DRIVE_FIXED:
      case DRIVE_NO_ROOT_DIR:
        break;
      case DRIVE_UNKNOWN:
      case DRIVE_CDROM:
        res = FALSE;
        break;
      default:
        break;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iNamelist::RootExists(const TCHAR *path)
{
  TCHAR  *tmp;
  size_t  len;

  len = _tcslen(path)  + 1;

  tmp = (TCHAR *)alloca(len * sizeof(TCHAR));

  _tcscpy_s(tmp, len, path);

  PathStripToRoot(tmp);

  return PathFileExists(tmp);
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iNamelist::RootCmp(const TCHAR *path)
{
  TCHAR  *tmp1;
  TCHAR  *tmp2;
  size_t  len1;
  size_t  len2;

  len1 = _tcslen(path)  + 1;
  len2 = _tcslen(m_dir) + 1;

  tmp1 = (TCHAR *)alloca(len1 * sizeof(TCHAR));
  tmp2 = (TCHAR *)alloca(len2 * sizeof(TCHAR));

  _tcscpy_s(tmp1, len1, path);
  _tcscpy_s(tmp2, len2, m_dir);

  PathStripToRoot(tmp1);
  PathStripToRoot(tmp2);

  return (_tcsicmp(tmp1, tmp2) == 0);
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iNamelist::ForceScandir(void)
{
  static TCHAR  buf[MAX_PATH];
  TCHAR        *dir;

  dir = m_historys[m_history_no & 0xF];

  if (*dir == '\0')
    {
      ::GetModuleFileName(NULL, buf, MAX_PATH);

      PathRemoveFileSpec(buf);

      dir = buf;
    }

  return dir;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iNamelist::CheckRootPath(const TCHAR **dir)
{
  BOOL  res = TRUE;

  if (RootExists(*dir))
    {
      return res;
    }

  if (m_namelist == NULL)
    {
      *dir = ForceScandir();
    }
  else
    {
      if (RootCmp(*dir))
        {
          *dir = ForceScandir();
        }
      else
        {
          res = FALSE;
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iNamelist::SetWildcard(const TCHAR *cardlist)
{
  int    i = 0;
  TCHAR *cp;
  TCHAR *ctxt;

  _tcscpy_s(m_cardtemp, MAX_PATH, cardlist);

  m_argc = 0;

  for (cp = _tcstok_s(m_cardtemp, _T( ";" ), &ctxt); cp; cp = _tcstok_s(NULL, _T(";"), &ctxt))
    {
      if (i >= WC_LIST_MAX)
        {
          break;
        }

      m_argv[i] = cp;
      m_argc++;
      i++;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iNamelist::SeachLock(void)
{
  m_lock = TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
dirent **iNamelist::BinSeach(TCHAR* dir, dirent *target)
{
  dirent*   entry;
  dirent**  res   = NULL;
  DWORD     cvs[] = {0, MARK_SVN, MARK_MODIFY, MARK_ADD, MARK_MARGE, MARK_CONF};
  DWORD     i;
  size_t    len;
  dirent*   next = (dirent *)target->param;

  len = _tcslen(dir) + 1 + _tcslen(next->d_name) + 1;

  entry = (dirent *)alloca(sizeof(dirent) + len * sizeof(TCHAR));

  *entry = *target;

  _tcscpy_s(entry->d_name, len, dir);
  ::PathAppend(entry->d_name, next->d_name + next->d_cnt);

  for (i = 0; i < sizeof(cvs) / sizeof(DWORD); i++)
    {
      res = (dirent **)bsearch(&entry,
                                m_namelist,
                                m_total,
                                sizeof(dirent*),
                                (int(*)(const void*, const void*))Compar0);
      if (res != NULL)
        {
          break;
        }
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
int iNamelist::Sort(int cur_line)
{
  TCHAR*  d_name;
  int     new_line = cur_line;

  d_name = m_namelist[cur_line]->d_name;

  qsort(m_namelist, m_total, sizeof(dirent *), (int(*)(const void *, const void *))Compar0);

  if (cur_line >= m_total)
    {
      return 0;
    }

  for (int i = 0; i < m_total; i++)
    {
      if (_tcscmp(d_name, m_namelist[i]->d_name) == 0)
        {
          new_line = i;
          break;
        }
    }

  return new_line;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iNamelist::SetSortMode(int order, int kind)
{
  s_order = order;
  s_kind  = kind;
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void iNamelist::GetSortMode(int *order, int *kind)
{
  if (order)
    {
      *order = s_order;
    }

  if (kind)
    {
      *kind = s_kind;
    }
}
// --------------------------------------------------------------------------
