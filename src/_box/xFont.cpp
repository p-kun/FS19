/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xFont.cpp
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
#include <windows.h>
#include <tchar.h>
#include "xFont.h"


// ==========================================================================
// -- xFontクラスの構築
// --------------------------------------------------------------------------
xFont::xFont()
{
  mhFont = NULL;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- xFontクラスの破棄
// --------------------------------------------------------------------------
xFont::~xFont()
{
  DeleteObject();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- フォント作成 - Level1
// --------------------------------------------------------------------------
void xFont::Create( const TCHAR *pFace, int h, int weigth, int w, int angle )
{
  LOGFONT log;

  log.lfHeight         = h;                         // フォント高さ
  log.lfWidth          = w;                         // 文字幅
  log.lfEscapement     = angle;                     // テキストの角度

  if ( weigth )
    {
      log.lfWeight     = FW_SEMIBOLD;               // フォントの重さ（太さ）
    }
  else
    {
      log.lfWeight     = FW_REGULAR;                // フォントの重さ（太さ）
    }

  wcscpy_s( log.lfFaceName, LF_FACESIZE, pFace );   // 書体名

  log.lfOrientation    = 0;                         // ベースラインとｘ軸との角度
  log.lfItalic         = FALSE;                     // イタリック体
  log.lfUnderline      = FALSE;                     // アンダーライン
  log.lfStrikeOut      = FALSE;                     // 打ち消し線
  log.lfCharSet        = SHIFTJIS_CHARSET;          // 文字セット
  log.lfOutPrecision   = OUT_DEFAULT_PRECIS;        // 出力精度
  log.lfClipPrecision  = CLIP_DEFAULT_PRECIS;       // クリッピング精度
  log.lfQuality        = PROOF_QUALITY;             // 出力品質
  log.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;   // ピッチとファミリー

  CreateIndirect( &log );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- フォント作成 - Level2
// --------------------------------------------------------------------------
void xFont::CreateIndirect( LOGFONT* pLogFont )
{
  /* Delete if the font has already been created */

  DeleteObject();

  mhFont = ::CreateFontIndirect( pLogFont );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- オブジェクトの選択解除とオブジェクトのデリート
// --------------------------------------------------------------------------
void xFont::DeleteObject()
{
  if ( mhFont )
    {
      ::DeleteObject( mhFont );
      mhFont = NULL;
    }
}
// --------------------------------------------------------------------------
