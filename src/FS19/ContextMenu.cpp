/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: ContextMenu.cpp
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


// -- Private:
//
static IContextMenu2* _pMenu2 = NULL;


// ==========================================================================
// -- コンテキストメニューを表示します
// --------------------------------------------------------------------------
void ContextMenu( HWND hWnd, int x, int y, TCHAR dir[], const TCHAR name[] )
{
  IContextMenu*       pMenu    = NULL;
  IContextMenu2*      pMenu2   = NULL;
  IShellFolder*       pDesktop = NULL;
  IShellFolder*       pFolder  = NULL;
  LPMALLOC            pMalloc  = NULL;
  LPITEMIDLIST        pID1     = NULL;
  LPITEMIDLIST        pID2     = NULL;
  LPITEMIDLIST        pID3     = NULL;
  HMENU               hMenu    = NULL;
  CMINVOKECOMMANDINFO cmi;
  OLECHAR             olePath[ MAX_PATH ];
  ULONG               size;
  ULONG               attr = 0;
  int                 cmd;

  if ( ::SHGetMalloc( &pMalloc ) != NOERROR )
    {
      goto _EXIT_;
    }

  if ( FAILED( SHGetDesktopFolder( &pDesktop ) ) )
    {
      goto _EXIT_;
    }

#ifndef UNICODE
  MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, dir, -1, olePath, MAX_PATH );
#else
  _tcscpy_s( olePath, MAX_PATH, dir );
#endif

  if ( FAILED( pDesktop->ParseDisplayName( hWnd, NULL, olePath, &size, &pID1, &attr ) ) )
    {
      goto _EXIT_;
    }

  if ( *name == '\0' )
    {
      if ( SplitPIDList( pID1, &pID2, &pID3 ) == NULL )
        {
          goto _EXIT_;
        }

      pMalloc->Free( pID1 );
      pID1 = pID2;
      pID2 = pID3;
      pID3 = NULL;

      if ( FAILED( pDesktop->BindToObject( pID1, 0, IID_IShellFolder, ( void** )&pFolder ) ) )
        {
          goto _EXIT_;
        }
    }
  else
    {
      if ( FAILED( pDesktop->BindToObject( pID1, 0, IID_IShellFolder, ( void** )&pFolder ) ) )
        {
          goto _EXIT_;
        }

#ifndef UNICODE
      MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, name, -1, olePath, MAX_PATH );
#else
      _tcscpy_s( olePath, MAX_PATH, name );
#endif
      if ( FAILED( pFolder->ParseDisplayName( hWnd, NULL, olePath, &size, &pID2, &attr ) ) )
        {
          goto _EXIT_;
        }
    }

  if ( FAILED( pFolder->GetUIObjectOf( NULL, 1, ( const ITEMIDLIST** )&pID2, IID_IContextMenu, 0, ( LPVOID* )&pMenu ) ) )
    {
      goto _EXIT_;
    }

  if ( FAILED( pMenu->QueryInterface( IID_IContextMenu2, ( void** )&pMenu2 ) ) )
    {
      goto _EXIT_;
    }

  if ( ( hMenu = CreatePopupMenu() ) == 0 )
    {
      goto _EXIT_;
    }

  if ( FAILED( pMenu2->QueryContextMenu( hMenu, 0, 1, 0x7fff, CMF_EXPLORE ) ) )
    {
      goto _EXIT_;
    }

  _pMenu2 = pMenu2;
  cmd     = TrackPopupMenu( hMenu, TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, x, y, 0, hWnd, NULL );
  _pMenu2 = NULL;

  if ( cmd )
    {
      cmi.cbSize       = sizeof( CMINVOKECOMMANDINFO );
      cmi.fMask        = 0;
      cmi.hwnd         = hWnd;
      cmi.lpVerb       = ( LPCSTR )MAKEINTRESOURCE( cmd - 1 );
      cmi.lpParameters = 0;
      cmi.lpDirectory  = 0;
      cmi.nShow        = SW_SHOWNORMAL;
      cmi.dwHotKey     = 0;
      cmi.hIcon        = NULL;

      pMenu2->InvokeCommand( &cmi );    // 次のコマンドで実行します
    }

_EXIT_:
  if( pMenu )    pMenu->Release();
  if( pMenu2 )   pMenu2->Release();
  if( pFolder )  pFolder->Release();
  if( pDesktop ) pDesktop->Release();
  if( hMenu )    DestroyMenu( hMenu );

  if ( pMalloc )
    {
      if( pID1 ) pMalloc->Free( pID1 );
      if( pID2 ) pMalloc->Free( pID2 );
      if( pID3 ) pMalloc->Free( pID3 );

      pMalloc->Release();
    }

}
// --------------------------------------------------------------------------


// ==========================================================================
// -- コンテキストメニュー表示中のメッセージ処理
// --------------------------------------------------------------------------
void ContextMenuMsg( UINT msg, WPARAM wParam, LPARAM lParam )
{
  if ( _pMenu2 )
    {
      _pMenu2->HandleMenuMsg( msg, wParam, lParam );
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- コンテキストメニューを表示します(ファイルの複数選択)
// --------------------------------------------------------------------------
void ContextMenu( HWND hWnd, int x, int y, TCHAR dir[], TCHAR* names[], int cnt )
{
  IContextMenu*       pMenu    = NULL;
  IContextMenu2*      pMenu2   = NULL;
  IShellFolder*       pDesktop = NULL;
  IShellFolder*       pFolder  = NULL;
  LPMALLOC            pMalloc  = NULL;
  LPITEMIDLIST        pID1     = NULL;
  LPITEMIDLIST        pID2     = NULL;
  LPITEMIDLIST        pID3     = NULL;
  LPITEMIDLIST*       pList    = NULL;
  HMENU               hMenu = NULL;
  CMINVOKECOMMANDINFO cmi;
  OLECHAR             olePath[ MAX_PATH ];
  ULONG               size;
  ULONG               attr = 0;
  int                 cmd;

  if ( ::SHGetMalloc( &pMalloc ) != NOERROR )
    {
      goto _EXIT_;
    }

  if ( FAILED( SHGetDesktopFolder( &pDesktop ) ) )
    {
      goto _EXIT_;
    }

#ifndef UNICODE
  MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, dir, -1, olePath, MAX_PATH );
#else
  _tcscpy_s( olePath, MAX_PATH, dir );
#endif

  if ( FAILED( pDesktop->ParseDisplayName( hWnd, NULL, olePath, &size, &pID1, &attr ) ) )
    {
      goto _EXIT_;
    }

  if ( FAILED( pDesktop->BindToObject( pID1, 0, IID_IShellFolder, ( void** )&pFolder ) ) )
    {
      goto _EXIT_;
    }

  pList = ConvPath2PIDLists( pFolder, names, cnt );

  if ( pList == NULL )
    {
      goto _EXIT_;
    }

  if ( FAILED( pFolder->GetUIObjectOf( NULL, cnt, ( const ITEMIDLIST** )pList, IID_IContextMenu, 0, ( LPVOID* )&pMenu ) ) )
    {
      goto _EXIT_;
    }

  if ( FAILED( pMenu->QueryInterface( IID_IContextMenu2, ( void** )&pMenu2 ) ) )
    {
      goto _EXIT_;
    }

  if ( ( hMenu = CreatePopupMenu() ) == 0 )
    {
      goto _EXIT_;
    }

  if ( FAILED( pMenu2->QueryContextMenu( hMenu, 0, 1, 0x7fff, CMF_EXPLORE ) ) )
    {
      goto _EXIT_;
    }

  _pMenu2 = pMenu2;
  cmd     = TrackPopupMenu( hMenu, TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, x, y, 0, hWnd, NULL );
  _pMenu2 = NULL;

  if ( cmd )
    {
      cmi.cbSize       = sizeof( CMINVOKECOMMANDINFO );
      cmi.fMask        = 0;
      cmi.hwnd         = hWnd;
      cmi.lpVerb       = ( LPCSTR )MAKEINTRESOURCE( cmd - 1 );
      cmi.lpParameters = 0;
      cmi.lpDirectory  = 0;
      cmi.nShow        = SW_SHOWNORMAL;
      cmi.dwHotKey     = 0;
      cmi.hIcon        = NULL;

      pMenu2->InvokeCommand( &cmi );    // 次のコマンドで実行します
    }

_EXIT_:
  if( pMenu )    pMenu->Release();
  if( pMenu2 )   pMenu2->Release();
  if( pFolder )  pFolder->Release();
  if( pDesktop ) pDesktop->Release();
  if( hMenu )    DestroyMenu( hMenu );

  if ( pMalloc )
    {
      if( pID1 ) pMalloc->Free( pID1 );
      if( pID2 ) pMalloc->Free( pID2 );
      if( pID3 ) pMalloc->Free( pID3 );

      pMalloc->Release();
    }

  DeletePIDLists( pList );

}
// --------------------------------------------------------------------------


// ==========================================================================
// -- コンテキストメニューを表示します(ファイルの複数選択)
// --------------------------------------------------------------------------
void ContextMenu( HWND hWnd, int x, int y, TCHAR* names[], int cnt )
{
  IContextMenu*       pMenu    = NULL;
  IContextMenu2*      pMenu2   = NULL;
  IShellFolder*       pDesktop = NULL;
  LPMALLOC            pMalloc  = NULL;
  LPITEMIDLIST        pID1     = NULL;
  LPITEMIDLIST        pID2     = NULL;
  LPITEMIDLIST        pID3     = NULL;
  LPITEMIDLIST*       pList    = NULL;
  HMENU               hMenu    = NULL;
  CMINVOKECOMMANDINFO cmi;
  int                 cmd;

  if ( ::SHGetMalloc( &pMalloc ) != NOERROR )
    {
      goto _EXIT_;
    }

  if ( FAILED( ::SHGetDesktopFolder( &pDesktop ) ) )
    {
      goto _EXIT_;
    }

  pList = ConvPath2PIDLists( names, cnt );

  if ( pList == NULL )
    {
      goto _EXIT_;
    }

  if ( FAILED( pDesktop->GetUIObjectOf( NULL, cnt, ( const ITEMIDLIST** )pList, IID_IContextMenu, 0, ( LPVOID* )&pMenu ) ) )
    {
      goto _EXIT_;
    }

  if ( FAILED( pMenu->QueryInterface( IID_IContextMenu2, ( void** )&pMenu2 ) ) )
    {
      goto _EXIT_;
    }

  if ( ( hMenu = CreatePopupMenu() ) == 0 )
    {
      goto _EXIT_;
    }

  if ( FAILED( pMenu2->QueryContextMenu( hMenu, 0, 1, 0x7fff, CMF_EXPLORE ) ) )
    {
      goto _EXIT_;
    }

  _pMenu2 = pMenu2;
  cmd     = TrackPopupMenu( hMenu, TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, x, y, 0, hWnd, NULL );
  _pMenu2 = NULL;

  if ( cmd )
    {
      cmi.cbSize       = sizeof( CMINVOKECOMMANDINFO );
      cmi.fMask        = 0;
      cmi.hwnd         = hWnd;
      cmi.lpVerb       = ( LPCSTR )MAKEINTRESOURCE( cmd - 1 );
      cmi.lpParameters = 0;
      cmi.lpDirectory  = 0;
      cmi.nShow        = SW_SHOWNORMAL;
      cmi.dwHotKey     = 0;
      cmi.hIcon        = NULL;

      pMenu2->InvokeCommand( &cmi );    // 次のコマンドで実行します
    }

_EXIT_:
  if( pMenu )    pMenu->Release();
  if( pMenu2 )   pMenu2->Release();
  if( pDesktop ) pDesktop->Release();
  if( hMenu )    DestroyMenu( hMenu );

  if ( pMalloc )
    {
      if( pID1 ) pMalloc->Free( pID1 );
      if( pID2 ) pMalloc->Free( pID2 );
      if( pID3 ) pMalloc->Free( pID3 );

      pMalloc->Release();
    }

  DeletePIDLists( pList );

}
// --------------------------------------------------------------------------
