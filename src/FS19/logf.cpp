#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <stdio.h>
#include "logf.h"


// ==========================================================================
// --
// --------------------------------------------------------------------------
void logf2(
	TCHAR*			  fnam,
	int				    l_no,
	const TCHAR*	fmt,	// フォーマット文字列
	...						      // 可変引数
	)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
{
	va_list		  ap;
	FILE*		    fp;
	TCHAR		    buf[ MAX_PATH ];
	static int	first = 1;
	SYSTEMTIME	systime;

	va_start( ap, fmt );

	::GetModuleFileName( NULL, buf, MAX_PATH );

	PathRenameExtension( buf, _T( ".log" ) );

	if (_tfopen_s(&fp, buf, _T("a")) == 0)
    {
		  if ( first )
        {
			    first = 0;
			    ::GetLocalTime( &systime );
			    _ftprintf( fp, _T( "\n------ First log. %02d-%02d-%02d %02d:%02d:%02d ------\n" ),
				    systime.wYear % 100,
				    systime.wMonth,
				    systime.wDay,
				    systime.wHour,
				    systime.wMinute,
				    systime.wSecond );
  		  }

      _ftprintf( fp, _T( "%s(%d) " ), fnam, l_no );
		  _vftprintf( fp, fmt, ap );
		  _ftprintf( fp, _T( "\n" ) );
		
		  fclose( fp );
	  }

	va_end( ap );
}
// --------------------------------------------------------------------------


// ==========================================================================
// --
// --------------------------------------------------------------------------
void *_allocs( void *top, size_t size, int line_no, const TCHAR *fname )
{
	return top;
}
// --------------------------------------------------------------------------
