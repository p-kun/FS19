#ifndef _ITREEVIEW_H_
#define _ITREEVIEW_H_

#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <tchar.h>
#include <shlwapi.h>
#include <malloc.h>
#include "iPt.h"
#include "SubFuncs.h"
#include "ContextMenu.h"

/* Private */

#define FIND_DATA           WIN32_FIND_DATA
#define OFFSET_Y            2
#define LINE_STYLE          PS_DOT  /*PS_SOLID*/
#define RIGHT_LINE_ENABLE   1
#define WIN10_LINES         1


// ==========================================================================
// -- アプリケーションクラスの作成
// --------------------------------------------------------------------------
class iTreeView
{
private:
  enum {
    KUN_MAX       = 18000,
    KUN_OKINI_MAX = 40,
  };

private:
  typedef struct M_KUN_LIST_T
  {
    int           depth;
    int           total;
    int           d_sub;
    int           d_cnt;
    unsigned int  attr;
    BOOL          is_func;
    TCHAR*        d_name;
    TCHAR*        d_name2;
    HANDLE        handle;
    int           idx;
  }
  M_KUN_LIST;

  typedef struct M_KUN_T
  {
    int           okini_count;
    M_KUN_LIST    okini_list[ KUN_OKINI_MAX ];
    int           count;
    M_KUN_LIST    list[ 1 ];
  }
  M_KUN;

public:
  // ======================================================================
  // -- クラスの構築
  // ----------------------------------------------------------------------
  iTreeView()
  {
    int   size;

    mhHeap = HeapCreate( HEAP_NO_SERIALIZE, 100000, 0 );

    size = sizeof( M_KUN ) + sizeof( M_KUN_LIST ) * KUN_MAX;

    mp_kun = ( M_KUN* )HeapAlloc( mhHeap, HEAP_NO_SERIALIZE, size );

    memset( mp_kun, 0, size );

    mp_kun->count = 0;

    SetFavorites();
    SetRoot();
    GetEnableDrive();

    m_cur = m_top = m_line = 0;

    m_sub = -1;

    m_enable = FALSE;
    m_focus1 = FALSE;

    _tcscpy_s( m_marks[ 0 ], MAX_PATH, L"" );
    _tcscpy_s( m_marks[ 1 ], MAX_PATH, L"" );

    mhWnd = NULL;

    m_height = 10;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- クラスの破棄
  // ----------------------------------------------------------------------
  ~iTreeView()
  {
    if ( mhHeap )
      {
        HeapDestroy( mhHeap );
      }
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  BOOL Ooo( int cur, int depth )
  {
    BOOL  res = FALSE;

    for ( int i = cur + 1; i < mp_kun->count; i++ )
      {
        if ( mp_kun->list[ i ].depth == depth )
          {
            res = TRUE;
            break;
          }

        if ( mp_kun->list[ i ].depth < depth )
          {
            break;
          }
      }

    return res;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  void Draw( HWND hWnd, HDC hDC, int x, int y, int cx, int cy )
  {
    xTmpDC  dc( hDC );
    int     height;
    int     yy;
    int     i = 0;
    int     j = 0;
    int     lock;
    int     lines;
    int     depth = 0;
    TCHAR   buf[ MAX_PATH * 2 ];
    int     offset_y = OFFSET_Y;
    int     offset_x = 2;
    int     offset_t = 20;
    int     is_no1 = -1;
    int     is_no2 = -1;
    int     line_x;
    xBmpDC  bmp;
    iPt     pt;
    DWORD   ild_mode;
    iico    ico;

    mhWnd = hWnd;

    m_currc.top    = y;
    m_currc.left   = x;
    m_currc.right  = x + cx;
    m_currc.bottom = y + cy;

    dc.SetSelectFont( pt.fontname3, pt.fontsize3 );

    height = dc.tm.tmHeight + 4;

    if ( height < DPIX(18) )
      {
        height = DPIX(18);
      }

    m_height = height;

    lines = ( cy - offset_y ) / height;

    if ( m_line != lines )
      {
        m_line = lines;
        m_cur = Aja( m_cur );
      }

    dc.FillRect( x, y, cx, offset_y, pt.COL_BASE_BAR );
    dc.FillRect( x, y + m_line * height + offset_y, cx, cy - ( m_line * height + offset_y ), pt.COL_BASE_BAR );

#if RIGHT_LINE_ENABLE
    cx--;
    dc.Line( cx, y, cx, y + cy, pt.COL_BASE_BAR );
    cx--;
    dc.Line( cx, y, cx, y + cy, pt.COL_BASE_BAR );
    cx--;
    dc.Line( cx, y, cx, y + cy - /*1*/0, pt.COL_SPLIT_LINE );
#endif

    bmp.Create( dc, cx, height );
    bmp.SetBkMode( TRANSPARENT );
    bmp.SetSelectFont( pt.fontname3, pt.fontsize3 );

    ico.attach(bmp, IMG_TREEVIEW);

    if ( m_focus1 )
      {
        bmp.SetTextColor( 0 );
        ild_mode = ILD_NORMAL;
      }
    else
      {
        bmp.SetTextColor( RGB( 128, 128, 128 ) );
        ild_mode = ILD_BLEND50;
      }

    yy = y + offset_y;
    x += offset_x;

    for( i = m_top; i < m_top + m_line; i++ )
      {
        bmp.FillRect( 0, 0, cx, height, pt.COL_BASE_BAR );

        if( i < mp_kun->count && mp_kun->list[ i ].d_name )
          {
            if ( m_cur == i )
              {
                if ( !m_enable )
                  {
                    FillRect( bmp, x, 0, cx - x - offset_x, height, pt.COL_SELECTED_CUR );
                  }
                else
                  {
                    FillRect( bmp, x, 0, cx - x - offset_x, height, pt.COL_SELECTED_WAKU, pt.COL_SELECTED );
                  }
              }

            if ( m_sub == i && m_sub != m_cur )
              {
                FillRect( bmp, x, 0, cx - x - offset_x, height, pt.COL_SELECTED_CUR );
              }

            depth = mp_kun->list[ i ].depth * offset_t;

            if ( mp_kun->list[ i ].d_name2 )
              {
                _tcscpy_s( buf, MAX_PATH, mp_kun->list[ i ].d_name2 );
              }
            else
              {
                _tcscpy_s( buf, MAX_PATH, mp_kun->list[ i ].d_name );
              }

            PathCompactPath( dc, buf, cx - ( x + 4 + DPIX(16) + 4 + depth + offset_x ) );

            for ( j = 0; j < mp_kun->list[ i ].depth; j++ )
              {
                if ( Ooo( i, j + 1 ) )
                  {
                    lock = 1;
                  }
                else
                  {
                    lock = 0;
                  }

                line_x = x + j * offset_t + 8;

                if ( j == mp_kun->list[ i ].depth - 1 )
                  {
                    bmp.Lined( line_x, height / 2 - 1, line_x + 8, height / 2 - 1, pt.COL_SPLIT_LINE2 );

                    if ( i + 1 >= mp_kun->count || mp_kun->list[ i + 1 ].depth < mp_kun->list[ i ].depth || lock == 0 )
                      {
                        bmp.Lined( line_x, 0, line_x, height / 2, pt.COL_SPLIT_LINE2 );
                      }
                    else
                      {
                        bmp.Lined( line_x, 0, line_x, height, pt.COL_SPLIT_LINE2 );
                      }
                  }
                else
                  {
                    if ( lock )
                      {
                        bmp.Lined( line_x, 0, line_x, height, pt.COL_SPLIT_LINE2 );
                      }
                  }
              }

            bmp.TextOut( x + 4 + DPIX(16) + 4 + depth, ( height - bmp.tm.tmHeight ) / 2, buf );

            bmp.ImageList_DrawEx((HIMAGELIST)mp_kun->list[ i ].handle,
                                 mp_kun->list[ i ].idx,
                                 x + 4 + depth,
                                 ( height - DPIX(16) ) / 2,
                                 DPIX(16),
                                 DPIX(16),
                                 CLR_NONE,
                                 CLR_NONE,
                                 ild_mode );

            MakePath( i, buf );

            if ( is_no1 < 0 && _tcsicmp( buf, m_marks[ 0 ] ) == 0 )
              {
                is_no1 = i;
              }

            if ( is_no2 < 0 && _tcsicmp( buf, m_marks[ 1 ] ) == 0 )
              {
                is_no2 = i;
              }

            if ( mp_kun->list[ i ].d_sub > 0 && m_enable )
              {
                if ( mp_kun->list[ i ].d_cnt == -1 )
                  {
                    ico.draw(x + 2 + depth, height - DPIY(16), 0);
                  }
                else
                  {
                    ico.draw(x + 2 + depth, height - DPIY(16), 1);
                  }
              }
          }

        dc.BitBlt( 0, yy, cx, height, bmp, 0, 0, SRCCOPY );

        yy += height;
      }
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  BOOL KeyDown( int vk, int ctrl, int shift, int alt )
  {
    TCHAR   buf[ MAX_PATH ];
    int     depth;
    int     cur;
    int     last_top = m_top;
    int     last_cur = m_cur;
    int     last_cnt = mp_kun->count;
    BOOL    res = FALSE;

    switch( vk )
      {
        case VK_UP:
          if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
            {
              for ( cur = m_cur - 1; cur >= 0; cur-- )
                {
                  if ( mp_kun->list[ cur ].depth == mp_kun->list[ m_cur ].depth )
                    {
                      m_cur = Aja( cur );
                      break;
                    }
                }
            }
          else
            {
              m_cur = Aja( m_cur - 1 );
            }
          break;
        case VK_DOWN:
          if ( HIBYTE( GetKeyState( VK_SHIFT ) ) )
            {
              for ( cur = m_cur + 1; cur < mp_kun->count; cur++ )
                {
                  if ( mp_kun->list[ cur ].depth == mp_kun->list[ m_cur ].depth )
                    {
                      m_cur = Aja( cur );
                      break;
                    }
                }
            }
          else
            {
              m_cur = Aja( m_cur + 1 );
            }
          break;
        case VK_RETURN:
          break;
        case VK_LEFT:
          if ( mp_kun->list[ m_cur ].d_cnt > 0 )
            {
              closedir( m_cur );
            }
          else
            {
              depth = mp_kun->list[ m_cur ].depth;

              if ( depth > 0 )
                {
                  for ( m_cur--; m_cur >= 0; m_cur-- )
                    {
                      if ( depth > mp_kun->list[ m_cur ].depth )
                        {
                          break;
                        }
                    }
                }
            }
          m_cur = Aja( m_cur );
          break;
        case VK_RIGHT:
          if( mp_kun->list[ m_cur ].d_cnt == -1 )
            {
              MakePath( m_cur, buf );
              readdir( buf, mp_kun->list[ m_cur ].depth, m_cur );
              m_cur = Aja( m_cur );
            }
          break;
        case VK_PRIOR:
          m_cur = Aja( m_cur - m_line / 2 );
          break;
        case VK_NEXT:
          m_cur = Aja( m_cur + m_line / 2 );
          break;
        case VK_HOME:
          m_cur = Aja( 0 );
          break;
        case VK_END:
          m_cur = Aja( mp_kun->count );
          break;
        default:
          break;
      }

    if( last_top != m_top || last_cur != m_cur || last_cnt != mp_kun->count )
      {
        res = TRUE;
      }

    return res;
  }
  // ----------------------------------------------------------------------


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


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  static int Compare( const void *tar1, const void *tar2 )
  {
    M_KUN_LIST* list1 = ( M_KUN_LIST* )tar1;
    M_KUN_LIST* list2 = ( M_KUN_LIST* )tar2;

    return StrCmp( list1->d_name, list2->d_name );
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  int SetFileInfo( M_KUN_LIST *info, const TCHAR *d_name, const TCHAR *f_name, BOOL isDisplayName = FALSE )
  {
    SHFILEINFO  shfi;
    TCHAR       drive[ 8 ];
    TCHAR       tmp[ MAX_PATH ];

    _tcscpy_s( tmp, MAX_PATH, d_name );
    ::PathAppend( tmp, f_name );

    _stprintf_s( drive, _T( "%c:\\" ), _T( 'A' ) + PathGetDriveNumber( d_name ) );

    if ( !isDisplayName && ( PathIsUNC( d_name ) || GetDriveType( drive ) == DRIVE_REMOTE ) )
      {
        GetTempDirectoryIconIndex( &info->handle, ( DWORD* )&info->idx );
        info->d_name2 = NULL;
      }
    else
      {
        info->handle = ( HIMAGELIST )::SHGetFileInfo(
             tmp,
             FILE_ATTRIBUTE_ARCHIVE,
            &shfi,
             sizeof( SHFILEINFO ),
             SHGFI_DISPLAYNAME | SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

        info->idx = shfi.iIcon;

        if (_tcsicmp(f_name, shfi.szDisplayName) != 0)
          {
            info->d_name2 = malloc_entry( shfi.szDisplayName );
          }
        else
          {
            info->d_name2 = NULL;
          }
      }

    return 0;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  int checkunderdir( const TCHAR *cur_dir, const TCHAR *f_name )
  {
    HANDLE        hFile;
    FIND_DATA     fd;
    TCHAR        *temp;
    int           d_cnt = 0;
    int           f_cnt = 0;
    const TCHAR  *card  = L"*";
    size_t        len;

    len = 1 + _tcslen(cur_dir)
        + 1 + _tcslen(f_name)
        + 1 + _tcslen(card)
        + 1;  /* '\0' */

    temp = (TCHAR *)alloca(len * sizeof(TCHAR));

    _tcscpy_s( temp, len, cur_dir );

    ::PathAppend( temp, f_name );
    ::PathAppend( temp, card );

    hFile = ::FindFirstFile( temp, &fd );

    if( INVALID_HANDLE_VALUE != hFile )
      {
        do
          {
            f_cnt++;
            if ( _tcscmp( fd.cFileName, _T( "." ) ) == 0 || _tcscmp( fd.cFileName, _T( ".." ) ) == 0 )
              {
                continue;
              }

            if( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
              {
                d_cnt++;
                break;
              }
          }
        while ( ::FindNextFile( hFile, &fd ) );

        ::FindClose( hFile );
      }

    if ( f_cnt == 0 )
      {
        d_cnt = -1;
      }

    return d_cnt;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  int readdir( const TCHAR *cur_dir, int depth, int cur )
  {
    HANDLE        hFile;
    FIND_DATA     fd;
    TCHAR        *temp;
    int           err = 0;
    int           i;
    int           no = cur;
    int           f_counter = 0;
    int           d_counter = 0;
    int           d_sub;
    const TCHAR  *card  = L"*";
    size_t        len;

    if ( !CheckPath( cur_dir, 4, mhWnd, 1 ) )
      {
        return 0;
      }

    len = 1 + _tcslen(cur_dir)
        + 1 + _tcslen(card)
        + 1;  /* '\0' */

    temp = (TCHAR *)alloca(len * sizeof(TCHAR));

    _tcscpy_s( temp, len, cur_dir );

    ::PathAppend( temp, card );

    hFile = ::FindFirstFile( temp, &fd );

    if( INVALID_HANDLE_VALUE != hFile )
      {
        do
          {
            if ( _tcscmp( fd.cFileName, _T( "." ) ) == 0 || _tcscmp( fd.cFileName, _T( ".." ) ) == 0 )
              {
                continue;
              }

            f_counter++;

            if ( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
              {
                continue;
              }

            if ( mp_kun->count + 1 >= KUN_MAX )
              {
                err = 1;
                break;
              }

            if ( ( d_sub = checkunderdir( cur_dir, fd.cFileName ) ) == -1 )
              {
                continue;
              }

            for( i = mp_kun->count; i > no; i-- )
              {
                mp_kun->list[ i ] = mp_kun->list[ i - 1 ];
              }

            mp_kun->count++;
            no++;
            Init_P_KUN( no );
            mp_kun->list[ no ].depth  = depth + 1;
            mp_kun->list[ no ].attr   = fd.dwFileAttributes;
            mp_kun->list[ no ].d_name = malloc_entry( fd.cFileName );
            mp_kun->list[ no ].d_sub  = d_sub;
            SetFileInfo( &mp_kun->list[ no ], cur_dir, fd.cFileName );
            d_counter++;
          }
        while( ::FindNextFile( hFile, &fd ) );

        ::FindClose( hFile );
      }

    mp_kun->list[ cur ].d_cnt = d_counter;
    mp_kun->list[ cur ].d_sub = d_counter;
    mp_kun->list[ cur ].total = f_counter;

    if ( d_counter > 1 )
      {
        qsort( ( void* )&mp_kun->list[ cur + 1 ], d_counter, sizeof( M_KUN_LIST ), Compare );
      }

    return f_counter;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  int closedir( int cur )
  {
    int   i;
    int   count = mp_kun->count;
    int   depth = 0;

    if ( mp_kun->list[ cur ].d_cnt > 0 )
      {
        mp_kun->list[ cur ].d_cnt = -1;
        depth = mp_kun->list[ cur ].depth;
        cur++;

        for ( i = cur; i < count; i++ )
          {
            if ( depth >= mp_kun->list[ i ].depth )
              {
                break;
              }

            if ( mp_kun->list[ i ].d_name )
              {
                HeapFree( mhHeap, HEAP_NO_SERIALIZE, mp_kun->list[ i ].d_name );
              }

            if ( mp_kun->list[ i ].d_name2 )
              {
                HeapFree( mhHeap, HEAP_NO_SERIALIZE, mp_kun->list[ i ].d_name2 );
              }

            mp_kun->count--;
          }

        for ( ; i < count; i++, cur++ )
          {
            mp_kun->list[ cur ] = mp_kun->list[ i ];
          }
      }

    return mp_kun->count;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  int MakePath( int cur, TCHAR *buf )
  {
    int   depth;
    TCHAR last_buf[ MAX_PATH ] = _T( "" );

    do
      {
        depth = mp_kun->list[ cur ].depth;

        _tcscpy_s( buf, MAX_PATH, mp_kun->list[ cur ].d_name );
        ::PathAppend( buf, last_buf );
        _tcscpy_s( last_buf, MAX_PATH, buf );

        while ( cur > 0 )
          {
            cur--;

            if ( mp_kun->list[ cur ].depth < depth )
              {
                break;
              }
          }
      }
    while( depth );

    return cur;
  }
  // ----------------------------------------------------------------------


  // ======================================================================
  // -- 
  // ----------------------------------------------------------------------
  TCHAR* malloc_entry( const TCHAR *entry )
  {
    TCHAR*  target;
    size_t  len = _tcslen( entry ) + 1;

    target = ( TCHAR* )HeapAlloc( mhHeap, HEAP_NO_SERIALIZE, len * sizeof( TCHAR ) );

    if ( target )
      {
        _tcscpy_s( target, len, entry );
      }

    return target;
  }
  // ----------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void FillRect( HDC hDC, int x, int y, int cx, int cy, COLORREF fg )
  {
    FillRect( hDC, x, y, x + cx, y + cy, fg, fg );
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void FillRect( HDC hDC, int x, int y, int cx, int cy, COLORREF waku, COLORREF fg )
  {
    xTmpDC  dc( hDC );

    dc.Rectangle( x, y, x + cx, y + cy, waku, fg );
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  int GetCurrentPath( TCHAR *d_name )
  {
    return MakePath( m_cur, d_name );
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  BOOL SetFocus( BOOL enable )
  {
    if ( m_focus1 )
      {
        m_enable = enable;
      }

    return m_enable;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  BOOL SetFocusLV2( BOOL enable )
  {
    SetFocus( enable );

    m_focus1 = enable;

    return m_focus1;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void Redraw( HWND hWnd = NULL, RECT *rc = NULL )
  {
    xDC*  dc;

    if ( hWnd == NULL )
      {
        hWnd = mhWnd;
      }

    if ( rc == NULL )
      {
        rc = &m_currc;
      }

    dc = new xDC( hWnd );

    Draw( hWnd, *dc, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top );

    delete dc;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  int Aja( int cur )
  {
    int   top = m_top + 4;
    int   btm = m_top + m_line - 4;

    if ( cur <= 0 )
      {
        cur = 0;
      }
    else if ( cur >= mp_kun->count )
      {
        cur = mp_kun->count - 1;
      }

    if ( cur < m_cur )
      {
        for ( ; cur >= 0; cur-- )
          {
            if ( mp_kun->list[ cur ].d_name != NULL )
              {
                break;
              }
          }
      }
    else
      {
        for ( ; cur < mp_kun->count; cur++ )
          {
            if ( mp_kun->list[ cur ].d_name != NULL )
              {
                break;
              }
          }
      }

    if ( cur <= 0 )
      {
        for ( cur = 0; cur < mp_kun->count; cur++ )
          {
            if ( mp_kun->list[ cur ].d_name != NULL )
              {
                break;
              }
          }
      }
    else if ( cur >= mp_kun->count )
      {
        for ( cur = mp_kun->count - 1; cur >= 0; cur-- )
          {
            if ( mp_kun->list[ cur ].d_name != NULL )
              {
                break;
              }
          }
      }

    if ( cur <= 0 )
      {
        cur = 0;
      }
    else if ( cur >= mp_kun->count )
      {
        cur = mp_kun->count - 1;
      }

    if ( mp_kun->count > m_line )
      {
        if ( top > cur )
          {
            m_top = cur - 4;

            if ( m_top < 0 )
              {
                m_top = 0;
              }
          }
        else if ( btm < cur )
          {
            m_top = cur + 4 - m_line;

            if ( m_top > mp_kun->count - m_line )
              {
                m_top = mp_kun->count - m_line;
              }
          }

        if ( mp_kun->count - m_top < m_line )
          {
            m_top = mp_kun->count - m_line;
          }
      }
    else
      {
        if ( m_top > 0 )
          {
            m_top = 0;
          }
      }

    return cur;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  BOOL SetCurrentPath( TCHAR *d_name_org )
  {
    TCHAR  *cp;
    TCHAR  *d_name;
    TCHAR   drive[ 8 ];
    BOOL    res = TRUE;
    int     i = 0;
    int     depth;
    int     cur = -1;
    BOOL    isUNC = FALSE;
    size_t  len1;
    size_t  len2 = 0;

    len1 = 1 + _tcslen(d_name_org)
         + 1   /* '\\' */
         + 1;  /* '\0' */

    d_name = (TCHAR *)alloca(len1 * sizeof(TCHAR));

    _tcscpy_s( d_name, len1, d_name_org );

    PathAddBackslash( d_name );

    if ( !PathIsRoot( d_name ) )
      {
        PathRemoveBackslash( d_name );
      }

    len1 = _tcslen( d_name );

    if ( len1 == 0 )
      {
        return res;
      }

    if ( PathIsUNC( d_name ) )
      {
        isUNC = TRUE;
      }
    else
      {
        _stprintf_s( drive, _T( "%c:\\" ), _T( 'A' ) + PathGetDriveNumber( d_name ) );

        if ( GetDriveType( drive ) == DRIVE_REMOTE )
          {
            isUNC = TRUE;
          }
      }

    for ( i = 0; i < mp_kun->count; i++ )
      {
        if ( mp_kun->list[ i ].d_name == NULL )
          {
            continue;
          }

        len2 = _tcslen( mp_kun->list[ i ].d_name );

        if ( len1 < len2 )
          {
            continue;
          }

        if ( _tcsnicmp( d_name, mp_kun->list[ i ].d_name, len2 ) != 0 )
          {
            continue;
          }

        if ( *( d_name + len2 ) == _T( '\\' ) )
          {
            len2++;
          }

        if ( mp_kun->list[ i ].d_cnt == -1 && len1 >len2 && !isUNC )
          {
            readdir( mp_kun->list[ i ].d_name, mp_kun->list[ i ].depth, i );
          }

        cur = i;
        break;
      }

    if ( isUNC )
      {
        return res;
      }

    TCHAR *buf;
    TCHAR *tmp;

    len1 = 1 + _tcslen(d_name) + 1 /* '\0'*/;

    buf = (TCHAR *)alloca(len1 * sizeof(TCHAR));

    for ( cp = PathFindNextComponent( d_name + len2 ), depth = 0; cp; cp = PathFindNextComponent( cp ), depth++ )
      {
        _tcsncpy_s( buf, len1, d_name, cp - d_name );

        if ( !PathIsDirectory( buf ) )
          {
            res = FALSE;
            break;
          }

        PathAddBackslash( buf );

        if ( !PathIsRoot( buf ) )
          {
            PathRemoveBackslash( buf );
          }

        if ( depth >= 0 )
          {
            tmp = PathFindFileName( buf );
          }
        else
          {
            tmp = buf;
          }

        for ( ; i < mp_kun->count; i++ )
          {
            if ( StrCmpI( mp_kun->list[ i ].d_name, tmp ) == 0 )
              {
                if ( mp_kun->list[ i ].d_cnt == -1 && StrCmpI( d_name, buf ) != 0 )
                  {
                    readdir( buf, mp_kun->list[ i ].depth, i );
                  }

                cur = i;
                break;
              }
          }

        if ( i == mp_kun->count && depth == 0 )
          {
            if ( mp_kun->count + 1 >= KUN_MAX )
              {
                break;
              }
          }
      }

    if ( cur != -1 )
      {
        m_cur = Aja( cur );
      }

    return res;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  BOOL SetMarkPath( int no, TCHAR *d_name )
  {
    _tcscpy_s( m_marks[ no ], MAX_PATH, d_name );

    PathAddBackslash( m_marks[ no ] );

    if( !PathIsRoot( m_marks[ no ] ) )
      {
        PathRemoveBackslash( m_marks[ no ] );
      }

    return TRUE;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnLButtonDown( int x, int y )
  {
    int   cur = m_top + ( y - OFFSET_Y ) / m_height;
    int   xx;
    int   offset_x = 2;
    int   offset_t = 14;

    xx = mp_kun->list[ cur ].depth * offset_t + offset_x;

    if ( xx < x && x < xx + 16 )
      {
        OnLButtonDblClk( x, y, 1 );
      }
    else
      {
        m_cur = Aja( cur );
        KeyDown( VK_RETURN, 0, 0, 0 );
      }
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnLButtonUp( int x, int y )
  {
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  int GetTotalPage( void )
  {
    if ( mp_kun->count < m_line )
      {
        return 0;
      }

    return mp_kun->count - m_line;
  }


  // --------------------------------------------------------------------------
  int GetCurrentPage( void )
  {
    return m_top;
  }


  // --------------------------------------------------------------------------
  int PageDown( void )
  {
    if ( GetTotalPage() > 0 )
      {
        m_top += 3;

        if ( m_top > mp_kun->count - m_line )
          {
            m_top = mp_kun->count - m_line;
          }
      }

    return m_top;
  }


  // --------------------------------------------------------------------------
  int PageUp( void )
  {
    if ( GetTotalPage() > 0 )
      {
        m_top -= 3;

        if ( m_top < 0 )
          {
            m_top = 0;
          }
      }

    return m_top;
  }


  // --------------------------------------------------------------------------
  int PagePos( int pos )
  {
    m_top = pos;

    return m_top;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnLButtonDblClk( int x, int y, UINT flags )
  {
    TCHAR buf[ MAX_PATH ];
    int   cur = m_top + ( y - OFFSET_Y ) / m_height;

    if ( cur == m_cur || flags == 1 )
      {
        if( mp_kun->list[ cur ].d_cnt == -1 )
          {
            MakePath( cur, buf );
            readdir( buf, mp_kun->list[ cur ].depth, cur );
            cur = Aja( cur );
          }
        else
          {
            closedir( cur );
            cur = Aja( cur );
          }
      }
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnRButtonDown( int x, int y )
  {
    int   cur = m_top + ( y - OFFSET_Y ) / m_height;

    m_cur = Aja( cur );

    Redraw();
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnRButtonUp( int x, int y, UINT flags = 1 )
  {
    int   cur = m_top + ( y - m_currc.top - OFFSET_Y ) / m_height;
    TCHAR buf[ MAX_PATH ];
    POINT pos;

    if ( m_cur == cur )
      {
        pos.x = x;
        pos.y = y;

        MakePath( m_cur, buf );

        ClientToScreen( mhWnd, &pos );

        ContextMenu( mhWnd, pos.x, pos.y, buf, L"" );
      }
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void SetRoot( void )
  {
    TCHAR   buf[ MAX_PATH ];
    TCHAR   drv[ 8 ];
    TCHAR*  cp;
    int     i = mp_kun->count;
    DWORD   drives = 0;
    TCHAR   net_path[ MAX_PATH ];
    DWORD   length = MAX_PATH;

    GetLogicalDriveStrings( MAX_PATH, buf );      // -- ドライブの一覧取得

    for ( cp = buf; *cp != _T( '\0' ); cp += ( _tcslen( cp ) + 1 ) )
      {
        drv[ 0 ] = cp[ 0 ];
        drv[ 1 ] = cp[ 1 ];
        drv[ 2 ] = '\0';

        if( WNetGetConnection( drv, net_path, &length ) != NO_ERROR )
          {
            if ( !PathFileExists( cp ) )
              {
                continue;
              }
          }

        mp_kun->count++;
        mp_kun->list[ i ].depth  = 0;
        mp_kun->list[ i ].total  = -1;
        mp_kun->list[ i ].d_cnt  = -1;
        mp_kun->list[ i ].d_name = malloc_entry( cp );

        if( GetDriveType( cp ) == DRIVE_FIXED )
          {
            mp_kun->list[ i ].d_sub = checkunderdir( cp, L"" );
          }
        else
          {
            mp_kun->list[ i ].d_sub = 0;
          }

        SetFileInfo( &mp_kun->list[ i ], cp, L"", TRUE );
        i++;

        drives |= ( 1 << ( cp[ 0 ] - L'A' ) );
      }

    m_drives = drives;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  M_KUN_LIST *SetSplit( TCHAR *d_name = NULL, TCHAR *d_name2 = NULL )
  {
    int         i = mp_kun->count++;
    M_KUN_LIST* p_kun;

    Init_P_KUN( i );

    p_kun = &mp_kun->list[ i ];

    if ( d_name )
      {
        p_kun->d_name = malloc_entry( d_name );
        SetFileInfo( p_kun, d_name, L"" );
        p_kun->d_sub  = checkunderdir( d_name, L"" );
      }

    if ( d_name2 )
      {
        p_kun->d_name = malloc_entry( d_name2 );
      }

    return p_kun;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void SetFavorites( void )
  {
    TCHAR       buf[ MAX_PATH ];
    M_KUN_LIST* p_kun;
    iPt         pt;
    int         i;
    iico        ico(IMG_TREEVIEW);

    SetSplit();

    for ( i = 0; i < pt.libraly_num; i++ )
      {
        p_kun = SetSplit();
        p_kun->d_name  = malloc_entry( pt.libralypath[ i ] );
        p_kun->d_name2 = malloc_entry( pt.libralyname[ i ] );
        p_kun->handle  = ico;
        p_kun->idx     = 2;
        p_kun->d_cnt   = 0;
      }

    if ( pt.libraly_num > 0 )
      {
        SetSplit();
      }

    if ( SHGetSpecialFolderPath( NULL, buf, CSIDL_DESKTOPDIRECTORY, FALSE ) )
      {
        SetSplit( buf );
      }

    SetSplit();

    if ( SHGetSpecialFolderPath( NULL, buf, CSIDL_FAVORITES, FALSE ) )
      {
        SetSplit( buf );
      }

    if ( SHGetSpecialFolderPath( NULL, buf, CSIDL_PERSONAL, FALSE ) )
      {
        SetSplit( buf );
      }

    if ( SHGetSpecialFolderPath( NULL, buf, CSIDL_PROFILE, FALSE ) )
      {
        SetSplit( buf );
      }

    SetSplit();
    SetSplit();
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  DWORD GetEnableDrive( void )
  {
    DWORD  i,
           d,
           bit;
    TCHAR  buf[ 10 ];
    TCHAR *net_path;
    DWORD  length;

    d = ::GetLogicalDrives();

    for( i = 0; i < 32; i++ )
      {
        bit = ( 1 << i );

        if ( !( bit & d ) )
          {
            continue;
          }

        buf[ 0 ] = L'A' + ( TCHAR )i;
        buf[ 1 ] = L':';
        buf[ 2 ] = L'\0';
        buf[ 3 ] = L'\0';

        length = 0;

        if (WNetGetConnection(buf, NULL, &length) != ERROR_MORE_DATA)
          {
            continue;
          }

        length++;

        net_path = (TCHAR *)alloca(length * sizeof(TCHAR));

        if (WNetGetConnection(buf, net_path, &length) != NO_ERROR)
          {
            continue;
          }

        buf[ 2 ] = L'\\';

        if ( GetDriveType( buf ) == DRIVE_REMOTE )
          {
            continue;
          }

        if ( !PathFileExists( buf ) )
          {
            d = d & ~bit;
          }
      }

    return d;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  int Update( void )
  {
    if ( m_drives != GetEnableDrive() )
      {
        Refresh();
        return 1;
      }

    return 0;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void Refresh( void )
  {
    int   i;
    int   cur = m_cur;

    /* All close */

    for ( i = 0; i < mp_kun->count; i++ )
      {
        if ( mp_kun->list[ i ].d_name )
          {
            HeapFree( mhHeap, HEAP_NO_SERIALIZE, mp_kun->list[ i ].d_name );
          }

        if( mp_kun->list[ i ].d_name2 )
          {
            HeapFree( mhHeap, HEAP_NO_SERIALIZE, mp_kun->list[ i ].d_name2 );
          }
      }

    mp_kun->count = 0;

    SetFavorites();
    SetRoot();

    m_cur = Aja( cur );
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnMouseMove( int x, int y )
  {
    if ( m_height )
      {
        m_sub = m_top + ( y - m_currc.top - OFFSET_Y ) / m_height;
        Redraw();
      }
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnMouseLeave( void )
  {
    if ( m_sub != -1 )
      {
        m_sub = -1;
        Redraw();
      }
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void Init_P_KUN( int idx )
  {
    mp_kun->list[ idx ].depth   = 0;
    mp_kun->list[ idx ].total   = -1;
    mp_kun->list[ idx ].d_cnt   = -1;
    mp_kun->list[ idx ].d_name  = NULL;
    mp_kun->list[ idx ].d_name2 = NULL;
    mp_kun->list[ idx ].d_sub   = 0;
    mp_kun->list[ idx ].attr    = 0;
    mp_kun->list[ idx ].is_func = FALSE;
    mp_kun->list[ idx ].handle  = FALSE;
    mp_kun->list[ idx ].idx     = 0;
  }
  // --------------------------------------------------------------------------


  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void OnTimer( UINT id )
  {
  }
  // --------------------------------------------------------------------------


  private:
  HWND    mhWnd;
  int     m_cur;
  int     m_sub;
  int     m_top;
  int     m_line;
  HANDLE  mhHeap;
  M_KUN*  mp_kun;
  BOOL    m_enable;
  BOOL    m_focus1;
  TCHAR   m_marks[ 2 ][ MAX_PATH ];
  int     m_height;
  RECT    m_currc;
  DWORD   m_drives;
};
// --------------------------------------------------------------------------

#endif // _ITREEVIEW_H_
