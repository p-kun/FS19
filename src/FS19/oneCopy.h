/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: oneCopy.h
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
#ifndef _ONECOPY_H_
#define _ONECOPY_H_

// ==========================================================================
// -- アプリケーションクラスの作成
// --------------------------------------------------------------------------
class oneCopy : public xThread
{
public:
  typedef struct TCOPYACTION_T
  {
    HWND          hWnd;
    LARGE_INTEGER total;
    LARGE_INTEGER in1st;
    LARGE_INTEGER trans;
    LARGE_INTEGER file_total;
    LARGE_INTEGER file_trans;
    DWORD         attr;
    TCHAR*        src_name;
    TCHAR*        des_name;
    BOOL          cancel;
    BOOL          first;
    int           total_count;
    int           counter;
  }
  ACTION;

public:
  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  oneCopy() {}
  // --------------------------------------------------------------------------

  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  virtual BOOL doCopy( HWND hWnd, dirent *top, TCHAR *dir )
  {
    int   counter = -1;

    _top = top;
    _dir = dir;

    _act.hWnd           = hWnd;
    _act.cancel         = FALSE;
    _act.counter        = 0;
    _act.total_count    = 0;
    _act.total.QuadPart = 0;
    _act.trans.QuadPart = 0;
    _act.in1st.QuadPart = 0;

    Create();

    while( Wait() )
      {
        if( _act.total_count && counter != _act.counter )
          {
            counter = _act.counter;
          }

        if( PumpMessage() )
          {
            _act.cancel = TRUE;
            break;
          }
      }

    End();

    return _act.cancel;
  }
  // --------------------------------------------------------------------------

  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  ~oneCopy() {}
  // --------------------------------------------------------------------------

  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void Main()
  {
    CopyExec( _top, _dir );
  }
  // --------------------------------------------------------------------------

  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  int PumpMessage( HWND hWnd = NULL )
  {
    MSG   msg;
    int   res = 0;

    while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
      {
        if( msg.message == WM_QUIT )
          {
            res = 1;
            break;
          }
        else if( msg.message == WM_KEYDOWN )
          {
            if( msg.wParam == VK_ESCAPE )
              {
                if( ::MessageBox(hWnd,
                                 _T("Copying. Would you like to cancel?" ),
                                 _T( "What are you doing?" ),
                                 MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDYES)
                  {
                    res = 1;
                    break;
                  }
              }
          }

        if( GetMessage( &msg, NULL, 0, 0 ) <= 0 )
          {
            break;
          }

        ::TranslateMessage( &msg );
        ::DispatchMessage ( &msg );
      }

    return res;
  }
  // --------------------------------------------------------------------------

  // ==========================================================================
  // -- コピー時のコールバック関数
  // --------------------------------------------------------------------------
  static DWORD CALLBACK CopyProgressRoutine(
    LARGE_INTEGER TotalFileSize,          // total file size, in bytes
    LARGE_INTEGER TotalBytesTransferred,  // total number of bytes transferred
    LARGE_INTEGER StreamSize,             // total number of bytes for this stream
    LARGE_INTEGER StreamBytesTransferred, // total number of bytes transferred for this stream
    DWORD         dwStreamNumber,         // the current stream
    DWORD         dwCallbackReason,       // reason for callback
    HANDLE        hSourceFile,            // handle to the source file
    HANDLE        hDestinationFile,       // handle to the destination file
    LPVOID        lpData                  // passed by CopyFileEx
  )
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  {
    ACTION* act = ( ACTION* )lpData;
    DWORD res = PROGRESS_CONTINUE;

    if( dwCallbackReason == CALLBACK_STREAM_SWITCH )
      {
        act->in1st.QuadPart += TotalFileSize.QuadPart;
      }

    act->first = dwCallbackReason;
    act->trans.QuadPart = act->in1st.QuadPart + TotalBytesTransferred.QuadPart - TotalFileSize.QuadPart;
    act->file_total.QuadPart = TotalFileSize.QuadPart;
    act->file_trans.QuadPart = TotalBytesTransferred.QuadPart;

    if( act->cancel )
      {
        res = PROGRESS_CANCEL;
      }

    return res;
  }
  // --------------------------------------------------------------------------

  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  BOOL CopyExec( dirent *top, const TCHAR *dir )
  {
    TCHAR         trans[ MAX_PATH ];
    dirent*       elm;
    BOOL          isPoint = 0;
    BOOL          res = TRUE;
    int           action;
    LARGE_INTEGER size;

    for( elm = top; elm; elm = elm->next )
      {
        if( elm->bookMark == TCOPYBMARK_NULL
         || elm->bookMark == TCOPYBMARK_NOT_COPY_FOR_DATE
         || elm->bookMark == TCOPYBMARK_NOT_COPY_FOR_FILE )
          {
            continue;
          }

        size.HighPart = elm->nFileSizeHigh;
        size.LowPart  = elm->nFileSizeLow;
        _act.total.QuadPart += size.QuadPart;
        _act.total_count++;
      }

    for( elm = top; elm && !_act.cancel; elm = elm->next )
      {
        elm->state = 0;

        if( elm->bookMark == TCOPYBMARK_NULL )
          {
            continue;
          }

        _act.counter++;

        if( elm->bookMark == TCOPYBMARK_OVERWRITE )
          {
            action = 0;
          }
        else if( elm->bookMark == TCOPYBMARK_FAIL_IF_EXISTS
              || elm->bookMark == TCOPYBMARK_RENAME )
          {
            action = COPY_FILE_FAIL_IF_EXISTS;
          }
        else
          {
            continue;
          }

        PathAppend( trans, dir, ( ( dirent* )elm->param )->d_name + ( ( dirent* )elm->param )->d_cnt );

        _act.src_name = elm->d_name;
        _act.des_name = trans;
        _act.attr     = elm->dwFileAttributes;

        if( elm->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
          {
            PathAddBackslash( trans );

            if( /*!::MakeSureDirectoryPathExists( trans )*/!::CreateDirectory( trans, NULL ) )
              {
                res = FALSE;
                elm->state = GetLastError();
              }
          }
        else
          {
            SetFileAttributes( trans, 0 );

            if( CopyFileEx( elm->d_name, trans, CopyProgressRoutine, ( void* )&_act, &isPoint, action ) == FALSE )
              {
                res = FALSE;
                elm->state = GetLastError();
              }
          }
      }

    return res;
  }
  // --------------------------------------------------------------------------

  // ==========================================================================
  // -- 
  // --------------------------------------------------------------------------
  void PathAppend( TCHAR *buf, const TCHAR *path1, const TCHAR *path2 )
  {
    StrCpy( buf, path1 );
    ::PathAppend( buf, path2 );
  }
  // --------------------------------------------------------------------------

protected:
  dirent*   _top;
  TCHAR*    _dir;
  ACTION    _act;
};
// --------------------------------------------------------------------------

#endif /* _ONECOPY_H_ */
