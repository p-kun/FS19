/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: CommonPidList.h
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
#ifndef _COMMONPIDLIST_H_
#define _COMMONPIDLIST_H_


void DeletePIDList( LPITEMIDLIST pID );
int GetPIDListInfo( LPITEMIDLIST pID, int *size, int *cnt, LPITEMIDLIST *argv, int *idx );
LPITEMIDLIST DuplicatePIDList( LPITEMIDLIST pID );
LPITEMIDLIST ConvPath2PIDList( IShellFolder *pFolder, TCHAR path[] );
LPITEMIDLIST SplitPIDList( LPITEMIDLIST pID, LPITEMIDLIST *pID1, LPITEMIDLIST *pID2 );
LPITEMIDLIST *ConvPath2PIDLists( IShellFolder *pFolder, TCHAR **names, int num );
void DeletePIDLists( LPITEMIDLIST *pLists );
LPITEMIDLIST ConvPath2PIDList( HWND hWnd, TCHAR path[] );
LPITEMIDLIST *ConvPath2PIDLists( TCHAR **names, int num );
LPITEMIDLIST ConvPath2PIDList( TCHAR path[] );


#endif//_COMMONPIDLIST_H_
