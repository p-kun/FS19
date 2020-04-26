/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: ssnprintf.h
 *  Created  : 08/06/12(木) 17:56:03
 *
 *  Function
 *    Printf関数
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.6 $
 *  $Date: 2008/10/07 07:21:07 $
 *  $Author: uj03760 $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#ifndef _SSNPRINTF_H_
#define _SSNPRINTF_H_

#include <stdarg.h>

TCHAR *int2str( TCHAR *buf, int max, int len, long long val, int base, int is_big, TCHAR space, int interv, int is_left, int sign );
TCHAR *strncpy_sc( TCHAR *buf, int max, const TCHAR *src, int len, int left );
TCHAR *strncpy_sc( TCHAR *cp, const TCHAR *src, int len );

int ssnprintf( TCHAR *buf, int max, const TCHAR *fmt, ... );
int vssnprintf( TCHAR *buf, int max, const TCHAR *fmt, va_list ap );

#endif // _SSNPRINTF_H_
