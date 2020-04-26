/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: formatinc.h
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
#ifndef _FORMATINC_H_
#define _FORMATINC_H_


DWORD  FormatColor( dirent *entry );
DWORD  FormatColor( DWORD attr );
DWORD  FormatColorSub1( dirent *entry );
DWORD  FormatColorSub2( dirent *entry );
int    FormatInc( TCHAR buf[], dirent *entry, int max_len, int str_len, int *str_offset );
int    FormatInc( TCHAR buf[], dirent *entry, int offset, int max_len, int str_len, int lens[] );
TCHAR* CheckFileName( TCHAR *cp, TCHAR *src, int len );
TCHAR* CheckFileName( TCHAR *cp, TCHAR *src, int cnt, int len );


#endif//_FORMATINC_H_
