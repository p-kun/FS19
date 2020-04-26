/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iNamelist.h
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
#ifndef _INAMELIST_H_
#define _INAMELIST_H_

#include "dirent.h"
#include "iicon.h"
#include "iSimpleQue.h"

/* 定義一覧 */

#define INAM_MODE_NORMAL	0
#define INAM_MODE_SEACH		1

enum {
  INAM_KIND_NAME,
  INAM_KIND_TIME,
  INAM_KIND_SIZE,
  INAM_KIND_EXT,
};

// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iNamelist
{
public:
	iNamelist();
	~iNamelist();

public:
	virtual void	Cancel( void );
	virtual void	NamelistAction( void* ) {}

public:
  static  void  SetSortMode( int order, int kind );
  static  void  GetSortMode( int *order, int *kind );

public:
	int				Scandir( const TCHAR dir[], const TCHAR *prev_dir = NULL, void *param = NULL, int seach_mode = INAM_MODE_NORMAL );
	TCHAR*		GetCurrentDirectory( void ) { return m_dir; }
	void			Push( const TCHAR *path, const TCHAR *back_path = NULL );
	TCHAR*		Back( const TCHAR **back_path = NULL );
	TCHAR*		Step( void );
	BOOL			IsPushDir( const TCHAR *bk_path );
	BOOL			RootExists( const TCHAR *path );
	BOOL			RootCmp( const TCHAR *path );
	TCHAR*		ForceScandir( void );
	BOOL			CheckRootPath( const TCHAR **dir );
	int				GetTotalFile( void ) { return m_total; }
	void			SetWildcard( const TCHAR *cardlist );
	void			SeachLock( void );
	int				GetFileCount() { return m_fcnt; }
	int				GetDirCount() { return m_dcnt; }
	int				GetTotalCount() { return m_dcnt + m_fcnt; }
	ULARGE_INTEGER	GetTotalFileSize() { return m_fsize; }
	dirent**	BinSeach( TCHAR *dir, dirent *namelist );
  int       Sort( int cur_line );

  /* Directry history  */

  iSimpleQue  m_step_que;
  iSimpleQue  m_back_que;

protected:
	enum {
		WC_LIST_MAX = 8,
	};

protected:
	dirent**	m_namelist;
	int				m_total;
	int				m_fcnt;
	int				m_dcnt;
	TCHAR			m_dir[ MAX_PATH ];
	int				m_seach_mode;
	BOOL			m_cancel;
	//
	TCHAR			m_historys[ 16 ][ MAX_PATH ];
	TCHAR			m_historys_back[ 16 ][ MAX_PATH ];
	int				m_history_no;
	//
	TCHAR*		m_argv[ WC_LIST_MAX ];
	TCHAR			m_argc;
	TCHAR			m_cardtemp[ MAX_PATH ];
	ULARGE_INTEGER	m_fsize;
	BOOL			m_isRemote;

public:
	BOOL			m_lock;

public:
	TCHAR			mCopeingFileName[ MAX_PATH ];
	TCHAR			mCopeingCurrentDirectory[ MAX_PATH ];
	int				mCopeingFileIndex;
	int				mCopeingCounter;
};
// --------------------------------------------------------------------------

typedef struct TACTION_T {
	//
	BOOL*		    cancel;
	HWND		    hWnd;
	DWORD		    time;
	int			    argc;
	TCHAR**		  argv;
	iNamelist*	base;
	int			    s_mode;
	dirent*		  entry;
	int			    stat;
}
TACTION;

#endif//_INAMELIST_H_
