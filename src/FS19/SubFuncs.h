/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: SubFuncs.h
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
#ifndef _SUBFUNCS_H_
#define _SUBFUNCS_H_

TCHAR*      MakeFileList( dirent *top_entry, int count );
BOOL        ShDelete( HWND hWnd, dirent *top_entry, int count );
BOOL        ShCopy( HWND hWnd, const TCHAR *dir, dirent *top_entry, int count, BOOL isCopyMode = TRUE );
BOOL        ShCopy( HWND hWnd, const TCHAR *src, const TCHAR *des, BOOL isCopyMode = TRUE );
HGLOBAL     MakeHDropFiles( dirent *top_entry );
BOOL        EjectVolume( TCHAR cDriveLetter );
void        DispFileStatus( HDC hDC, RECT *rc, dirent *entry, const TCHAR *fontname, int fontsize );
void        StrTrimRight( TCHAR *str, const TCHAR *toks );
DWORD       GetHostByNameA( LPCSTR name );
DWORD       GetHostByNameW( LPCWSTR name );
#ifdef UNICODE
#define     GetHostByName	GetHostByNameW
#else
#define     GetHostByName	GetHostByNameA
#endif
BOOL        IsUNCOK( const TCHAR *in_path );
BOOL        IsAccessOK( const TCHAR *dir );
BOOL        IsRootPathOK( const TCHAR *path );
int         CheckPath( const TCHAR *path, int c_no = 3, HWND hWnd = NULL, int msg_on = 0 );
BOOL        CheckPathRules( const TCHAR *path );
BITMAPINFO* LoadImage( dirent *entry );
BOOL	      GetShortcutInfo( TCHAR *fnam, TCHAR *lnkfile = NULL, DWORD *attr = NULL );
BOOL        GetDisplayMonitors(RECT *pRC);

#endif//_SUBFUNCS_H_
