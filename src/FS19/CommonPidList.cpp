/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: CommonPidList.cpp
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
#include "CommonPidList.h"


// ==========================================================================
// -- PIDリストを解放する
//   ConvPath2PIDList関数、DuplicatePIDList関数で取得したPIDリストを解放する
// --------------------------------------------------------------------------
void DeletePIDList( LPITEMIDLIST pID )
{
  LPMALLOC    pMalloc = NULL;

  if ( SUCCEEDED( ::SHGetMalloc( &pMalloc ) ) )
    {
      pMalloc->Free( pID );
      pMalloc->Release();
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- PIDリストの情報を取得する
//    pID   [I] 情報を取得したいPIDリスト
//    size  [O] PIDリストの全サイズ
//    cnt   [O] PIDリストのPIDの個数
//    argv  [O] 各PIDの位置（ポインタ）
//    idxds [O] 各PIDの位置（インデックス）
// --------------------------------------------------------------------------
int GetPIDListInfo( LPITEMIDLIST pID, int *size, int *cnt, LPITEMIDLIST *argv, int *idxs )
{
  int   cb;

  *size = 0;
  *cnt  = 0;

  while ( ( cb = pID->mkid.cb ) > 0 )
    {
      ( *cnt )++;

      ( *size ) += cb;

      *idxs++ = ( *size );
      *argv++ = pID;

      pID = ( LPITEMIDLIST )( ( BYTE* )pID + cb );
    }

  ( *size ) += 2;

  return *cnt;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- PIDリストを複製する
// --------------------------------------------------------------------------
LPITEMIDLIST DuplicatePIDList( LPITEMIDLIST pID )
{
  int           size;
  int           cnt;
  int           idx[ 50 ];
  LPITEMIDLIST  argv[ 50 ];
  LPMALLOC      pMalloc = NULL;
  LPITEMIDLIST  newID   = NULL;

  GetPIDListInfo( pID, &size, &cnt, argv, idx );

  if ( FAILED( ::SHGetMalloc( &pMalloc ) ) )
    {
      return newID;
    }

  newID = ( LPITEMIDLIST )pMalloc->Alloc( size );

  if ( newID )
    {
      memcpy( newID, pID, size );
    }

  pMalloc->Release();

  return newID;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ファイルパスからPIDリストに変換する
// --------------------------------------------------------------------------
LPITEMIDLIST ConvPath2PIDList( IShellFolder *pFolder, TCHAR path[] )
{
  IShellFolder* pDesktopFolder = NULL;
  LPITEMIDLIST  pID = NULL;
  OLECHAR       olePath[ MAX_PATH ];
  ULONG         size = 0;
  ULONG         attr = 0;

#ifndef UNICODE
  MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, path, -1, olePath, MAX_PATH );
#else
  _tcscpy_s( olePath, MAX_PATH, path );
#endif

  if ( pFolder == NULL )
    {
      if( SUCCEEDED( SHGetDesktopFolder( &pDesktopFolder ) ) )
        {
          pDesktopFolder->ParseDisplayName( NULL, NULL, olePath, &size, &pID, &attr );
          
          pDesktopFolder->Release();
        }
    }
  else
    {
      pFolder->ParseDisplayName( NULL, NULL, olePath, &size, &pID, &attr );
    }

  return pID;
}
// --------------------------------------------------------------------------


#define _TRY( x ) { if( FAILED( x ) ) { throw; } }


// ==========================================================================
// -- フルパスからPIDリストに変換する
// --------------------------------------------------------------------------
LPITEMIDLIST ConvPath2PIDList( HWND hWnd, TCHAR path[] )
{
  IShellFolder* pDesktop = NULL;
  IShellFolder* pFolder  = NULL;
  LPMALLOC      pMalloc  = NULL;
  LPITEMIDLIST  pID = NULL;
  OLECHAR       oleName[ MAX_PATH ];
  OLECHAR       olePath[ MAX_PATH ];
  ULONG         size = 0;
  ULONG         attr = 0;

  _tcscpy_s( olePath, MAX_PATH, path );
  _tcscpy_s( oleName, MAX_PATH, path );

  PathRemoveFileSpec( olePath );
  PathStripPath( oleName );

  try
    {
      _TRY( ::SHGetMalloc( &pMalloc ) );

      _TRY( SHGetDesktopFolder( &pDesktop ) );

      _TRY( pDesktop->ParseDisplayName( hWnd, NULL, olePath, &size, &pID, &attr ) );

      _TRY( pDesktop->BindToObject( pID, 0, IID_IShellFolder, ( void** )&pFolder ) );

      pMalloc->Free( pID );
      pID = NULL;

      _TRY( pFolder->ParseDisplayName( NULL, NULL, oleName, &size, &pID, &attr ) );
    }
  catch ( int )
    {
      if ( pFolder )
        {
          pFolder->Release();
        }

      if ( pDesktop )
        {
          pDesktop->Release();
        }

      if ( pMalloc )
        {
          if ( pID )
            {
              pMalloc->Free( pID );
            }

          pMalloc->Release();
        }
    }

  return pID;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- PIDリストをディレクトリーとファイルで分ける
// --------------------------------------------------------------------------
LPITEMIDLIST SplitPIDList( LPITEMIDLIST pID, LPITEMIDLIST *pID1, LPITEMIDLIST *pID2 )
{
  int           size;
  int           cnt;
  int           idxs[ 50 ];
  LPITEMIDLIST  argv[ 50 ];
  LPMALLOC      pMalloc = NULL;
  LPITEMIDLIST  newID   = NULL;

  GetPIDListInfo( pID, &size, &cnt, argv, idxs );

  cnt--;

  if ( cnt < 0 )
    {
      goto _ERR_EXIT;
    }

  *pID1 = NULL;
  *pID2 = NULL;

  if ( ::SHGetMalloc( &pMalloc ) != NOERROR )
    {
      goto _ERR_EXIT;
    }

  newID = ( LPITEMIDLIST )pMalloc->Alloc( size + 2 );

  if ( newID == NULL )
    {
      goto _ERR_EXIT;
    }

  memcpy( newID, pID, size );

  *( ( BYTE* )newID + idxs[ cnt - 1 ] )     = 0;
  *( ( BYTE* )newID + idxs[ cnt - 1 ] + 1 ) = 0;

  *pID1 = newID;

  size = argv[ cnt ]->mkid.cb + 2;

  newID = ( LPITEMIDLIST )pMalloc->Alloc( size );

  if ( newID == NULL )
    {
      goto _ERR_EXIT;
    }

  memcpy( newID, argv[ cnt ], size );

  *pID2 = newID;

  pMalloc->Release();

  return newID;

_ERR_EXIT:
  if( *pID1 )   pMalloc->Free( *pID1 );
  if( *pID2 )   pMalloc->Free( *pID2 );
  if( pMalloc ) pMalloc->Release();

  return NULL;
  
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- パスのリストからPIDのリストを作成する
// --------------------------------------------------------------------------
LPITEMIDLIST *ConvPath2PIDLists( IShellFolder *pFolder, TCHAR **names, int num )
{
  int           i;
  LPITEMIDLIST* pListTop = NULL;
  LPITEMIDLIST* pListTmp = NULL;
  LPITEMIDLIST  pID      = NULL;
  LPMALLOC      pMalloc  = NULL;

  if ( ::SHGetMalloc( &pMalloc ) != NOERROR )
    {
      goto _ERR_EXIT;
    }

  pListTop = ( LPITEMIDLIST* )pMalloc->Alloc( sizeof( LPITEMIDLIST ) * ( num + 1 ) );
  pListTmp = pListTop;

  if ( pListTmp == NULL )
    {
      goto _ERR_EXIT;
    }

  for ( i = 0; i < num; i++ )
    {
      pID = ConvPath2PIDList( pFolder, names[ i ] );

      if ( pID )
        {
          *pListTmp++ = pID;
        }
    }

  *pListTmp = NULL;

  pMalloc->Release();

  return pListTop;

_ERR_EXIT:
  if ( pMalloc )
    {
      pMalloc->Release();
    }

  DeletePIDLists( pListTop );

  return NULL;
  
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- パスのリストからPIDのリストを作成する
// --------------------------------------------------------------------------
LPITEMIDLIST *ConvPath2PIDLists( TCHAR **names, int num )
{
  int           i;
  LPITEMIDLIST* pListTop = NULL;
  LPITEMIDLIST* pListTmp = NULL;
  LPITEMIDLIST  pID      = NULL;
  LPMALLOC      pMalloc  = NULL;

  try
    {
      if ( FAILED( ::SHGetMalloc( &pMalloc ) ) )
        {
          throw;
        }

      if ( ( pListTop = ( LPITEMIDLIST* )pMalloc->Alloc( sizeof( LPITEMIDLIST ) * ( num + 1 ) ) ) == NULL )
        {
          throw;
        }

      pListTmp = pListTop;

      for ( i = 0; i < num; i++ )
        {
          pID = ConvPath2PIDList( names[ i ] );

          if ( pID )
            {
              *pListTmp++ = pID;
            }
        }

      *pListTmp = NULL;
    }
  catch( int )
    {
      DeletePIDLists( pListTop );
    }

  if( pMalloc ) pMalloc->Release();

  return pListTop;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 複数のPIDをリストにしたものを解放する
//    ConvPath2PIDLists関数で取得したものを解放
// --------------------------------------------------------------------------
void DeletePIDLists( LPITEMIDLIST *pLists )
{
  LPITEMIDLIST* pTop    = pLists;
  LPMALLOC      pMalloc = NULL;

  if ( FAILED( ::SHGetMalloc( &pMalloc ) ) )
    {
      return;
    }

  while ( *pLists )
    {
      pMalloc->Free( *pLists );
      pLists++;
    }

  if ( pTop )
    {
      pMalloc->Free( pTop );
    }

  pMalloc->Release();

}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
LPITEMIDLIST ConvPath2PIDList( TCHAR path[] )
{
  LPITEMIDLIST  pidl = NULL;
  LPSHELLFOLDER pDesktopFolder;
  ULONG         chEaten;
  ULONG         dwAttributes = 0;

  /* Get a pointer to the Desktop's IShellFolder interface. */

  if ( FAILED( SHGetDesktopFolder( &pDesktopFolder ) ) )
    {
      return pidl;
    }

  /* Convert the path to an ITEMIDLIST. */

  if ( FAILED( pDesktopFolder->ParseDisplayName( NULL, NULL, path, &chEaten, &pidl, &dwAttributes ) ) )
    {
      /* 失敗 */

      pidl = NULL;
    }

  pDesktopFolder->Release();

  return pidl;
}
// --------------------------------------------------------------------------
