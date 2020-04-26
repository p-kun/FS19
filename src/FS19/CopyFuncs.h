/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: CopyFuncs.h
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
#ifndef _COPYFUNCS_H_
#define _COPYFUNCS_H_

typedef struct TCOPYACTION_T {
	//
	HWND			hWnd;
	LARGE_INTEGER	total;
	LARGE_INTEGER	in1st;
	LARGE_INTEGER	trans;
	LARGE_INTEGER	file_total;
	LARGE_INTEGER	file_trans;
	DWORD			attr;
	TCHAR*			src_name;
	TCHAR*			des_name;
	BOOL			cancel;
	BOOL			first;
	int				total_count;
	int				counter;
	int				status;
}
TCOPYACTION;

enum {
	TCOPYACTION_START,
	TCOPYACTION_EXEC,
	TCOPYACTION_END,
	TCOPYACTION_COMP,
	TCOPYACTION_CANCEL,
	TCOPYACTION_ERROR
};

enum {
	TCOPYBMARK_NULL,				        // Invalid. Do not copy
	TCOPYBMARK_OVERWRITE,			      // Overwrite
	TCOPYBMARK_FAIL_IF_EXISTS,		  // Do not copy if the same name exists
	TCOPYBMARK_RENAME,				      // Changing the name
	TCOPYBMARK_EXIST_DIRECTORY,		  // Directory already exists
	TCOPYBMARK_NOT_COPY_FOR_DATE,	  // Do not copy because the date is newer than the copy source
	TCOPYBMARK_NOT_COPY_FOR_FILE,	  // Do not copy because there is already a file with the same name
};

dirent* ReadClips( HWND hWnd, HANDLE hHeap, const TCHAR *dir_path );
BOOL    CopyExec( HWND hWnd, dirent* top, const TCHAR *dir );
BOOL    CheckCopyInfo( HWND hWnd, dirent* top, const TCHAR *dir );
int     CheckClips( HWND hWnd );

#define ICOPY_BASE_ID   0x1888

#endif//_COPYFUNCS_H_
