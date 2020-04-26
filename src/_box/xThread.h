/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xThread.h
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    コントロールの基底クラス
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#ifndef _XTHREAD_H_
#define _XTHREAD_H_

/*===========================================================================

  -- xThread クラス --

 ===========================================================================*/
class xThread
{
public:
  void  Create( DWORD creationFlags = 0 );
  void  End();
  DWORD GetThID() { return mThID; }   // スレッドＩＤ取得
  BOOL  IsEnd()   { return mIsEnd; }  // 終了フラグの取得

  int   GetPriority() { return ::GetThreadPriority( mhThread ); }
  BOOL  SetPriority( int priority ) { return ::SetThreadPriority( mhThread, priority ); }
  int   Wait( DWORD dwMilliseconds = 0 );

protected:
  virtual void Main() {}  // スレッドの内容を記述

private:
  HANDLE  mhThread;       // スレッドのハンドル
  DWORD mThID;            // スレッドのＩＤ
  BOOL  mIsEnd;           // スレッド終了フラグ

private:

  /* スレッドの実態 */

  static DWORD WINAPI ThreadFunc( LPVOID obj );

public:
  xThread();
  virtual ~xThread();

};
/*==========================================================================*/

#endif  /* _XTHREAD_H_ */
