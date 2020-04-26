/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xFont.h
 *  Created  : 08/06/12(木) 17:56:03
 *
 *  Function
 *    フォントクラス
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:48 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#ifndef _IFONT_H_
#define _IFONT_H_

/*===========================================================================

  -- xFont クラス --

 ===========================================================================*/
class xFont : public TEXTMETRIC
{
public:
  operator HFONT() const { return mhFont; }

public:
  xFont();
  virtual ~xFont();

public:
  void Create( const TCHAR* face, int h, int weigth = 0, int w = 0, int angle = 0 );

public:
  virtual void CreateIndirect( LOGFONT* pLogFont );

public:
  HFONT mhFont;

protected:
  void  DeleteObject();
};
/*==========================================================================*/

#endif // _IFONT_H_
