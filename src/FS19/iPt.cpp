/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iPt.cpp
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
#include "iHistory.h"
#include "iPt.h"


#define FONT_SIZE       14
#define FONT_DPI_SIZE   DPIX(FONT_SIZE)
#define FONT_NAME   (_T( "ＭＳ ゴシック" ))

int       iPt::OK                       = 0;
int       iPt::x                        = CW_USEDEFAULT;
int       iPt::y                        = CW_USEDEFAULT;
int       iPt::cx                       = CW_USEDEFAULT;
int       iPt::cy                       = CW_USEDEFAULT;
int       iPt::show                     = SW_SHOW;
TCHAR     iPt::path1[ MAX_PATH ]        = _T( "" );
TCHAR     iPt::path2[ MAX_PATH ]        = _T( "" );
int       iPt::fontsize1                = FONT_DPI_SIZE;
TCHAR     iPt::fontname1[ MAX_PATH ]    = FONT_NAME;
int       iPt::fontsize2                = FONT_DPI_SIZE;
TCHAR     iPt::fontname2[ MAX_PATH ]    = FONT_NAME;
int       iPt::fontsize3                = FONT_DPI_SIZE;
TCHAR     iPt::fontname3[ MAX_PATH ]    = FONT_NAME;
int       iPt::fontsize4                = FONT_DPI_SIZE;
TCHAR     iPt::fontname4[ MAX_PATH ]    = FONT_NAME;
int       iPt::column                   = 2;
TCHAR     iPt::editor[ MAX_PATH ]       = _T( "notepad.exe" );
TCHAR     iPt::wclist[ MAX_PATH ]       = _T( "" );
int       iPt::page_no                  = 0;
TCHAR     iPt::shortcut3[ MAX_PATH ]    = _T( "" );
int       iPt::tool_size                = TOOLBER_SIZE;
int       iPt::libraly_num              = 0;
TCHAR     iPt::libralyname[ LIBRALY_NUM ][ MAX_PATH ];
TCHAR     iPt::libralypath[ LIBRALY_NUM ][ MAX_PATH ];
int       iPt::line_pitch               = 0;
int       iPt::COL_MODE                 = PT_COL_MODE1;
COLORREF  iPt::COL_BASE_BAR             = RGB( 255, 255, 255 );
COLORREF  iPt::COL_SCROLL_BAR           = RGB( 205, 205, 205 );
COLORREF  iPt::COL_SCROLL_BAR_SEL       = RGB( 166, 166, 166 );
COLORREF  iPt::COL_BASE_BAR_LOW         = RGB( 250, 250, 255 );
COLORREF  iPt::COL_SPLIT_LINE           = RGB( 235, 235, 235 );
COLORREF  iPt::COL_SPLIT_LINE2          = RGB( 192, 192, 192 );
COLORREF  iPt::COL_SPLIT_LINE_TOP       = RGB(   7,   5,   5 );
COLORREF  iPt::COL_SPLIT_LINE_BTM       = RGB( 137, 145, 145 );
COLORREF  iPt::COL_SPLIT_LINE3          = RGB( 107, 105, 105 );
COLORREF  iPt::COL_SELECTED             = RGB( 204, 232, 255 );
COLORREF  iPt::COL_SELECTED_WAKU        = RGB( 153, 209, 255 );
COLORREF  iPt::COL_SELECTED_CUR         = RGB( 229, 243, 255 );
COLORREF  iPt::COL_PROGRESS_BAR         = RGB(   0, 114, 198 );
COLORREF  iPt::COL_REMAIN_BAR_BLUE      = RGB(  38, 160, 218 );
COLORREF  iPt::COL_REMAIN_BAR_RED       = RGB( 218,  38,  38 );
COLORREF  iPt::COL_TITLE                = RGB(  0,   51, 153 );
#if 0
COLORREF  iPt::COL_FILE_ATTR_SYSTEM     = RGB(   0,   0, 255 );
COLORREF  iPt::COL_FILE_ATTR_HIDDEN     = RGB( 255,   0,   0 );
COLORREF  iPt::COL_FILE_ATTR_READONLY   = RGB(  64, 128,   0 );
COLORREF  iPt::COL_FILE_ATTR_DIRECTORY  = RGB(   0, 128, 255 );
COLORREF  iPt::COL_FILE_ATTR_NORMAL     = RGB(  15,   4,   0 );
#else
COLORREF  iPt::COL_FILE_ATTR_SYSTEM     = RGB(  15,   4,   0 );
COLORREF  iPt::COL_FILE_ATTR_HIDDEN     = RGB(  15,   4,   0 );
COLORREF  iPt::COL_FILE_ATTR_READONLY   = RGB(  15,   4,   0 );
COLORREF  iPt::COL_FILE_ATTR_DIRECTORY  = RGB(  15,   4,   0 );
COLORREF  iPt::COL_FILE_ATTR_NORMAL     = RGB(  15,   4,   0 );
#endif
int       iPt::search_folder_num        = 0;
TCHAR     iPt::search_folders[ FOLDER_NUM ][ MAX_PATH ];

iPt::KEYS iPt::keys[ PT_MAX_FUNCS ] =
{
  { VK_RETURN, 0, 1, 0, PT_FUNC_EDITOR },
  { 'L',       0, 0, 0, PT_FUNC_LOGDISK },
  { 221,       0, 0, 0, PT_FUNC_LOGDISK },
  { VK_F5,     0, 0, 0, PT_FUNC_REFLESH },
  { 'Z',       0, 0, 0, PT_FUNC_EXEC },
  { VK_RETURN, 1, 0, 0, PT_FUNC_BACK },
  { VK_DELETE, 1, 0, 0, PT_FUNC_STEP },
  { 'E',       0, 0, 0, PT_FUNC_EJECT },
  { VK_INSERT, 1, 0, 0, PT_FUNC_COPY },
  { VK_INSERT, 0, 1, 0, PT_FUNC_PAST },
  { VK_F2,     0, 0, 0, PT_FUNC_RENAME },
  { 'X',       0, 0, 0, PT_FUNC_SELEXE },
  { VK_INSERT, 0, 0, 0, PT_FUNC_INSERT },
  { VK_DELETE, 0, 0, 0, PT_FUNC_DELETE },
  { VK_APPS,   0, 0, 0, PT_FUNC_APPS },
  { 'A',       0, 0, 0, PT_FUNC_ATTRIBUTE },
  { VK_F1,     0, 0, 0, PT_FUNC_SHORTCUT1 },
  { VK_F11,    0, 0, 0, PT_FUNC_SHORTCUT2 },
  { 'Q',       0, 0, 0, PT_FUNC_SHORTCUT3 },
  { 'F',       0, 0, 0, PT_FUNC_FIND },
  { '1',       0, 0, 0, PT_FUNC_SORT },
  { '2',       0, 0, 0, PT_FUNC_SORT },
  { '3',       0, 0, 0, PT_FUNC_SORT },
  { '4',       0, 0, 0, PT_FUNC_SORT },
  { 0,         0, 0, 0, PT_FUNC_NON },
};


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iPt::iPt( void )
{
  if ( OK )
    {
      return;
    }

  _tsetlocale( LC_ALL, _T( "" ) );  // ロケールを定義します。

  Read();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iPt::~iPt()
{
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static TCHAR *sstrtok( TCHAR *str = NULL )
{
  wchar_t*  context;

  return _tcstok_s( str, _T( "=\n\r" ), &context );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iPt::AnlFunc( const TCHAR *kind, const TCHAR *param )
{
  if ( _tcscmp( kind, _T( "X" ) ) == 0 )
    {
      x = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "Y" ) ) == 0 )
    {
      y = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "CX" ) ) == 0 )
    {
      cx = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "CY" ) ) == 0 )
    {
      cy = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "SHOW" ) ) == 0 )
    {
      show = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "PATH1" ) ) == 0 )
    {
      _tcscpy_s( path1, MAX_PATH, param );
      PathAddBackslash( path1 );
    }
  else if ( _tcscmp( kind, _T( "PATH2" ) ) == 0 )
    {
      _tcscpy_s( path2, MAX_PATH, param );
      PathAddBackslash( path2 );
    }
  else if ( _tcscmp( kind, _T( "COLUMN" ) ) == 0 )
    {
      column = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "FONTSIZE1" ) ) == 0 )
    {
      fontsize1 = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "FONTSIZE2" ) ) == 0 )
    {
      fontsize2 = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "FONTSIZE3" ) ) == 0 )
    {
      fontsize3 = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "FONTSIZE4" ) ) == 0 )
    {
      fontsize4 = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "FONTNAME1" ) ) == 0 )
    {
      _tcscpy_s( fontname1, MAX_PATH, param );
    }
  else if ( _tcscmp( kind, _T( "FONTNAME2" ) ) == 0 )
    {
      _tcscpy_s( fontname2, MAX_PATH, param );
    }
  else if ( _tcscmp( kind, _T( "FONTNAME3" ) ) == 0 )
    {
      _tcscpy_s( fontname3, MAX_PATH, param );
    }
  else if ( _tcscmp( kind, _T( "FONTNAME4" ) ) == 0 )
    {
      _tcscpy_s( fontname4, MAX_PATH, param );
    }
  else if ( _tcscmp( kind, _T( "EDITOR" ) ) == 0 )
    {
      _tcscpy_s( editor, MAX_PATH, param );
    }
  else if ( _tcscmp( kind, _T( "WCLIST" ) ) == 0 )
    {
      _tcscpy_s( wclist, MAX_PATH, param );
    }
  else if ( _tcscmp( kind, _T( "PAGE_NO" ) ) == 0 )
    {
      page_no = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "SHORTCUT3" ) ) == 0 )
    {
      _tcscpy_s( shortcut3, MAX_PATH, param );
    }
  else if ( _tcscmp( kind, _T( "TOOLBER_SIZE" ) ) == 0 )
    {
      tool_size = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "HISTCNT" ) ) == 0 )
    {
      iHistory  his( NULL );
    }
  else if( _tcscmp( kind, _T( "HISTORY" ) ) == 0 )
    {
      iHistory  his;
      DWORD     dat;

      dat = _tcstoul( param + 1, NULL, 16 );

      if ( *param == _T( '*' ) )
        {
          his.Push( param + HISTORY_PATH_OFFSET, 1, dat );
        }
      else
        {
          his.Push( param + HISTORY_PATH_OFFSET, 0, dat );
        }
    }
  else if ( _tcscmp( kind, _T( "LIBRARY__NUM" ) ) == 0 )
    {
      libraly_num = 0;
    }
  else if ( _tcscmp( kind, _T( "LIBRARY_NAME" ) ) == 0 )
    {
      _tcscpy_s( libralyname[ libraly_num ], MAX_PATH, param );
    }
  else if ( _tcscmp( kind, _T( "LIBRARY_PATH" ) ) == 0 )
    {
      _tcscpy_s( libralypath[ libraly_num ], MAX_PATH, param );
      libraly_num++;
    }
  else if ( _tcscmp( kind, _T( "LINE_PITCH" ) ) == 0 )
    {
      line_pitch = _ttoi( param );
    }
  else if ( _tcscmp( kind, _T( "SEARCH_FOLDER_NUM" ) ) == 0 )
    {
      search_folder_num = 0;
    }
  else if ( _tcscmp( kind, _T( "SEARCH_FOLDERS" ) ) == 0 )
    {
      _tcscpy_s( search_folders[search_folder_num], MAX_PATH, param );
      search_folder_num++;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iPt::Read()
{
  TCHAR   buf[ MAX_PATH ];
  FILE*   fp;
  BOOL    res = FALSE;
  TCHAR*  cp;
  TCHAR*  argv[ 3 ];
  int     i;
  TCHAR*  context;

  /* Init */

  ::GetCurrentDirectory( MAX_PATH, path1 );
  ::GetCurrentDirectory( MAX_PATH, path2 );

  ::GetModuleFileName( NULL, buf, MAX_PATH );

  PathRenameExtension( buf, _T( ".ini" ) );

  if (_tfopen_s(&fp, buf, _T("r")) == 0 )
    {
      while( _fgetts( buf, MAX_PATH, fp ) )
        {
          for( cp = _tcstok_s( buf, L"=\r\n", &context ), i = 0; cp && i < 3; cp = _tcstok_s( NULL, L"=\r\n", &context ), i++ )
            {
              argv[ i ] = cp;
            }

          if ( i > 1 )
            {
              AnlFunc( argv[ 0 ], argv[ 1 ] );
            }
        }

      fclose( fp );

      res = TRUE;

      if ( x > ::GetSystemMetrics( SM_CXVIRTUALSCREEN ) || y > ::GetSystemMetrics( SM_CYVIRTUALSCREEN ) )
        {
          x    = CW_USEDEFAULT;
          y    = CW_USEDEFAULT;
          cx   = CW_USEDEFAULT;
          cy   = CW_USEDEFAULT;
          show = SW_SHOW;
        }
    
      if ( _tcscmp( path1, _T( "" ) ) == 0 )
        {
          ::GetCurrentDirectory( MAX_PATH, path1 );
        }

      if ( _tcscmp( path2, _T( "" ) ) == 0 )
        {
          ::GetCurrentDirectory( MAX_PATH, path2 );
        }
    }

  OK = 1;

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL iPt::Write()
{
  TCHAR     buf[ MAX_PATH ];
  FILE*     fp;
  BOOL      res = FALSE;
  iHistory  his;
  int       i;

  ::GetModuleFileName( NULL, buf, MAX_PATH );

  PathRenameExtension( buf, _T( ".ini" ) );

  if (_tfopen_s(&fp, buf, _T("w")) == 0)
    {
      _ftprintf( fp, _T( "X=%d\n" ),            x );
      _ftprintf( fp, _T( "Y=%d\n" ),            y );
      _ftprintf( fp, _T( "CX=%d\n" ),           cx );
      _ftprintf( fp, _T( "CY=%d\n" ),           cy );
      _ftprintf( fp, _T( "SHOW=%d\n" ),         show );
      _ftprintf( fp, _T( "COLUMN=%d\n" ),       column );
      _ftprintf( fp, _T( "FONTSIZE1=%d\n" ),    fontsize1 );
      _ftprintf( fp, _T( "FONTSIZE2=%d\n" ),    fontsize2 );
      _ftprintf( fp, _T( "FONTSIZE3=%d\n" ),    fontsize3 );
      _ftprintf( fp, _T( "FONTSIZE4=%d\n" ),    fontsize4 );
      _ftprintf( fp, _T( "FONTNAME1=%s\n" ),    fontname1 );
      _ftprintf( fp, _T( "FONTNAME2=%s\n" ),    fontname2 );
      _ftprintf( fp, _T( "FONTNAME3=%s\n" ),    fontname3 );
      _ftprintf( fp, _T( "FONTNAME4=%s\n" ),    fontname4 );
      _ftprintf( fp, _T( "PATH1=%s\n" ),        path1 );
      _ftprintf( fp, _T( "PATH2=%s\n" ),        path2 );
      _ftprintf( fp, _T( "EDITOR=%s\n" ),       editor );
      _ftprintf( fp, _T( "WCLIST=%s\n" ),       wclist );
      _ftprintf( fp, _T( "PAGE_NO=%d\n" ),      page_no );
      _ftprintf( fp, _T( "SHORTCUT3=%s\n" ),    shortcut3 );
      _ftprintf( fp, _T( "TOOLBER_SIZE=%d\n" ), tool_size );
      _ftprintf( fp, _T( "HISTCNT=%d\n" ),      his.GetCount() );

      for ( i = his.GetCount() - 1; i >= 0; i-- )
        {
          _ftprintf( fp, _T( "HISTORY=%s\n" ), his.GetAt( i ) );
        }

      _ftprintf( fp, _T( "LIBRARY__NUM=%d\n" ), libraly_num );

      for ( i = 0; i < libraly_num; i++ )
        {
          _ftprintf( fp, _T( "LIBRARY_NAME=%s\n" ), libralyname[ i ] );
          _ftprintf( fp, _T( "LIBRARY_PATH=%s\n" ), libralypath[ i ] );
        }

      _ftprintf( fp, _T( "LINE_PITCH=%d\n" ),                line_pitch );
      _ftprintf( fp, _T( "COL_MODE=%08X\n" ),                COL_MODE );
      _ftprintf( fp, _T( "COL_BASE_BAR=%08X\n" ),            COL_BASE_BAR );
      _ftprintf( fp, _T( "COL_SCROLL_BAR=%08X\n" ),          COL_SCROLL_BAR );
      _ftprintf( fp, _T( "COL_SCROLL_BAR_SEL=%08X\n" ),      COL_SCROLL_BAR_SEL );
      _ftprintf( fp, _T( "COL_BASE_BAR_LOW=%08X\n" ),        COL_BASE_BAR_LOW );
      _ftprintf( fp, _T( "COL_SPLIT_LINE=%08X\n" ),          COL_SPLIT_LINE );
      _ftprintf( fp, _T( "COL_SPLIT_LINE2=%08X\n" ),         COL_SPLIT_LINE2 );
      _ftprintf( fp, _T( "COL_SPLIT_LINE_TOP=%08X\n" ),      COL_SPLIT_LINE_TOP );
      _ftprintf( fp, _T( "COL_SPLIT_LINE_BTM=%08X\n" ),      COL_SPLIT_LINE_BTM );
      _ftprintf( fp, _T( "COL_SPLIT_LINE3=%08X\n" ),         COL_SPLIT_LINE3 );
      _ftprintf( fp, _T( "COL_SELECTED=%08X\n" ),            COL_SELECTED );
      _ftprintf( fp, _T( "COL_SELECTED_WAKU=%08X\n" ),       COL_SELECTED_WAKU );
      _ftprintf( fp, _T( "COL_SELECTED_CUR=%08X\n" ),        COL_SELECTED_CUR );
      _ftprintf( fp, _T( "COL_FILE_ATTR_SYSTEM=%08X\n" ),    COL_FILE_ATTR_SYSTEM );
      _ftprintf( fp, _T( "COL_FILE_ATTR_HIDDEN=%08X\n" ),    COL_FILE_ATTR_HIDDEN );
      _ftprintf( fp, _T( "COL_FILE_ATTR_READONLY=%08X\n" ),  COL_FILE_ATTR_READONLY );
      _ftprintf( fp, _T( "COL_FILE_ATTR_DIRECTORY=%08X\n" ), COL_FILE_ATTR_DIRECTORY );
      _ftprintf( fp, _T( "COL_FILE_ATTR_NORMAL=%08X\n" ),    COL_FILE_ATTR_NORMAL );

      _ftprintf( fp, _T( "SEARCH_FOLDER_NUM=%d\n" ), search_folder_num );

      for ( i = 0; i < search_folder_num; i++ )
        {
          _ftprintf( fp, _T( "SEARCH_FOLDERS=%s\n" ), search_folders[ i ] );
        }

      fclose( fp );

      res = TRUE;
    }

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iPt::SetPlacement( HWND hWnd )
{
  WINDOWPLACEMENT place;

  ::GetWindowPlacement( hWnd, &place );

  x    = place.rcNormalPosition.left;
  y    = place.rcNormalPosition.top;
  cx   = place.rcNormalPosition.right  - place.rcNormalPosition.left;
  cy   = place.rcNormalPosition.bottom - place.rcNormalPosition.top;
  show = place.showCmd;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
int iPt::GetFuncNo( int vk, int ctrl, int shift, int alt )
{
  int   i;
  int   f_no = 0;

  ctrl  = ( ctrl > 0 );
  shift = ( shift > 0 );
  alt   = ( alt > 0 );

  for ( i = 0; i < PT_MAX_FUNCS; i++ )
    {
      if ( keys[ i ].vk == 0 )
        {
          break;
        }

      if ( keys[ i ].vk  == vk
        && keys[ i ].ctl == ctrl
        && keys[ i ].sft == shift
        && keys[ i ].alt == alt )
        {
          f_no = keys[ i ].f_no;
        }
    }

  return f_no;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
TCHAR *iPt::GetEditorPath( void )
{
  return editor;
}
// --------------------------------------------------------------------------
