/****************************************************************************
 * Included files
 ****************************************************************************/

#include "_box.h"
#include "dirent.h"
#include "SubFuncs.h"
#include "ipt.h"
#include "iicon.h"

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/

#define ICONSIZE        16
#define ICON_DPI_SIZE   DPIX(ICONSIZE)

/****************************************************************************
 * Private types
 ****************************************************************************/

typedef struct _ICONICON_T_ {
  //
  HIMAGELIST  handle;
  int         index;
}
ICONICON;

/****************************************************************************
 * Private data
 ****************************************************************************/

static HIMAGELIST _hImage[ IMG_MAX_NUM ] = { NULL };
static ICONICON   _temp_dir;
static ICONICON   _temp_file;

/****************************************************************************
 * Private functions
 ****************************************************************************/

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
static HIMAGELIST GetImageList( int idx = 0 )
{
  assert(idx != IMG_INIT);

  static const TCHAR *image_name[ IMG_MAX_NUM ] =
  {
    L"IDB_IMAGELIST01",
    L"IDB_IMAGELIST02",
    L"IDB_IMAGELIST03",
    L"IDB_IMAGELIST04",
    L"IDB_IMAGELIST05",
    L"IDB_IMAGELIST06",
    L"IDB_IMAGELIST07",
    L"IDB_IMAGELIST08",
    L"IDB_IMAGELIST09",
    L"IDB_IMAGELIST10",
  };

  if ( _hImage[ idx ] == NULL )
    {
      _hImage[ idx ] = ImageList_LoadImage( ::GetModuleHandle( NULL ),
                                            image_name[ idx ],
                                            ICONSIZE,
                                            ICONSIZE,
                                            RGB( 0, 128, 0 ),
                                            IMAGE_BITMAP,
                                            LR_CREATEDIBSECTION );
    }

  return _hImage[ idx ];
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iico::initialization( void )
{
  TCHAR       buf[ MAX_PATH ];
  TCHAR       tmp[ MAX_PATH ];
  SHFILEINFO  shfi;

  GetTempPath( MAX_PATH, buf );

  ZeroMemory( &shfi, sizeof( shfi ) );

  _temp_dir.handle = ( HIMAGELIST )::SHGetFileInfo(
    buf,
    FILE_ATTRIBUTE_ARCHIVE,
    &shfi,
    sizeof( SHFILEINFO ),
    SHGFI_SMALLICON | SHGFI_SYSICONINDEX/* | SHGFI_USEFILEATTRIBUTES*/ );
  _temp_dir.index = shfi.iIcon;

  GetTempFileName( buf, _T( "fs8" ), 0, tmp );

  _temp_file.handle = ( HIMAGELIST )::SHGetFileInfo(
    tmp,
    FILE_ATTRIBUTE_ARCHIVE,
    &shfi,
    sizeof( SHFILEINFO ),
    SHGFI_SMALLICON | SHGFI_SYSICONINDEX/* | SHGFI_USEFILEATTRIBUTES*/ );
  _temp_file.index = shfi.iIcon;

  DeleteFile( tmp );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iico::destroy( void )
{
  for (int i = 0; i < IMG_MAX_NUM; i++)
    {
      if( _hImage[ i ] )
        {
          ImageList_Destroy( _hImage[ i ] );
          _hImage[ i ] = NULL;
        }
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void GetTempDirectoryIconIndex( HANDLE *handle, DWORD *index )
{
  *index  = _temp_dir.index;
  *handle = _temp_dir.handle;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void GetTempFileIconIndex( HANDLE *handle, DWORD *index )
{
  *index  = _temp_file.index;
  *handle = _temp_file.handle;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iico::iico(int init)
{
  if (init == IMG_INIT)
    {
      initialization();
    }

  mIndex = init;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iico::iico(HDC hDC, int idx)
{
  attach(hDC, idx);
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iico::~iico()
{
  if (mIndex == IMG_INIT)
    {
      destroy();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iico::draw(int x, int y, int idx, BOOL isActive)
{
  HIMAGELIST  hImange = ::GetImageList(mIndex);
  int         cx;
  int         cy;

  ImageList_GetIconSize(hImange, &cx, &cy);

  if (::ImageList_GetImageCount(hImange) > idx)
    {
      ::ImageList_DrawEx(hImange, idx, mhDC, x, y, cx, cy, rgbBk, rgbFg, isActive ? ILD_NORMAL : ILD_BLEND50);
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iico::attach(HDC hDC, int idx)
{
  mhDC   = hDC;
  mIndex = idx;
  rgbBk  = CLR_NONE;
  rgbFg  = CLR_NONE;
  fStyle = ILD_NORMAL;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
iico::operator void* () const
{
  return GetImageList(mIndex);
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void iico::draw(int x, int y, dirent *entry, BOOL isActive, BOOL isRemote)
{
  HIMAGELIST  image;
  DWORD       index;
  DWORD       sub_index;
  xTmpDC      dc( mhDC );
  UINT        style = ILD_BLEND50;
  iico        ico(dc, IMG_MARK);

  if ( !isActive )
    {
    }
  else if ( entry->dwFileAttributes & ( FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY ) )
    {
    }
  else
    {
      style = ILD_NORMAL;
    }

  if ( _tcscmp( PathFindFileName( entry->d_name ), _T( ".." ) ) == 0 )
    {
      return;
    }

  image     = ( HIMAGELIST )entry->hHandle;
  index     =  entry->userData & 0x00FFFFFF;
  sub_index = (entry->userData & 0x0F000000) >> 24;

  dc.ImageList_DrawEx(image, index, x, y, ICON_DPI_SIZE, ICON_DPI_SIZE, INDEXTOOVERLAYMASK(sub_index) | style);

  /* チェックマーク表示 */

  if ( isRemote )
    {
      ico.draw(x - 3, y, 5);
    }

  if ( entry->bookMark )
    {
      ico.draw(x - 3, y, 1);
    }
  else if ( entry->dwFileAttributes & FILE_ATTRIBUTE_SYSTEM )
    {
      ico.draw(x - 3, y, 3);
    }
  else if ( entry->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
    {
      ico.draw(x - 3, y, 2);
    }
  else if ( entry->dwFileAttributes & FILE_ATTRIBUTE_READONLY )
    {
      ico.draw(x - 3, y, 4);
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
HANDLE iico::get_instance(dirent *entry, DWORD *d_no, BOOL isRemotea)
{
  return isRemotea ? get_remote_instance(entry, d_no) : get_instance(entry, d_no);
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
HANDLE iico::get_instance(dirent *entry, DWORD *d_no)
{
  SHFILEINFO  shfi;
  HANDLE      handle;

  handle = ( HIMAGELIST )::SHGetFileInfo(
      entry->d_name,
      FILE_ATTRIBUTE_ARCHIVE,
     &shfi,
      sizeof( SHFILEINFO ),
      SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_OVERLAYINDEX | SHGFI_ICON);

  if (handle)
    {
      *d_no = shfi.iIcon;

      ::DestroyIcon(shfi.hIcon);
    }
  else
    {
      *d_no = 0;

      handle = _hImage[IMG_MARK];
    }

  return handle;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
HANDLE iico::get_remote_instance(dirent *entry, DWORD *d_no)
{
  HANDLE  handle;
  DWORD   attr = 0;

  if ( entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
      *d_no  = _temp_dir.index;
      handle = _temp_dir.handle;
    }
  else
    {
      if( GetShortcutInfo( entry->d_name, NULL, &attr ) )
        {
          if ( attr & FILE_ATTRIBUTE_DIRECTORY )
            {
              *d_no  = _temp_dir.index;
              handle = _temp_dir.handle;
            }
          else
            {
              *d_no  = _temp_file.index;
              handle = _temp_file.handle;
            }
        }
      else
        {
          *d_no  = _temp_file.index;
          handle = _temp_file.handle;
        }
    }

  return handle;
}
// --------------------------------------------------------------------------
