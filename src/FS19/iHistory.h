/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iGdiPlus.h
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
#ifndef _IHISTORY_H_
#define _IHISTORY_H_


#define HISTORY_PATH_OFFSET		10


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iHistory
{
public:
	void   Push( const TCHAR *path, int mark = 0, DWORD date = 0 );
	int    GetCount( void );
	TCHAR* GetAt( int i );
	void   Clean( void );
	int    GetMark( int no );
	void   SetMark( const TCHAR *path );
	void   DeleteLog( const TCHAR *path );
	DWORD  GetLocalDateTime( void );
	void   CmpLocalDateTime( const 	TCHAR *path, DWORD date_time, TCHAR *date_str, size_t len );
	
public:
	iHistory( const TCHAR *path );
	iHistory() {}
	~iHistory();
	
private:
	enum {
		HISTORY_DEPTH = 30,
	};
	
private:
	static int			mCount;
	static int			mHistMark[ HISTORY_DEPTH + 1 ];
	static TCHAR		mHistBuf[ HISTORY_DEPTH + 1 ][ MAX_PATH ];
	static unsigned int	mHistDate[ HISTORY_DEPTH + 1 ];
};
// --------------------------------------------------------------------------


#endif // _IHISTORY_H_
