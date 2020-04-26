/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: ContextMenu.h
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
#ifndef _CONTEXTMENU_H_
#define _CONTEXTMENU_H_


void ContextMenu( HWND hWnd, int x, int y, TCHAR dir[], const TCHAR name[] );
void ContextMenu( HWND hWnd, int x, int y, TCHAR dir[], TCHAR* names[], int cnt );
void ContextMenu( HWND hWnd, int x, int y, TCHAR* names[], int cnt );
void ContextMenuMsg( UINT msg, WPARAM wParam, LPARAM lParam );


#endif//_CONTEXTMENU_H_
