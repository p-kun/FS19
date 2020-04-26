/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xDcBase.cpp
 *  Created  : 08/06/28(金) 10:58:04
 *
 *  Function
 *    未定
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:48 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <malloc.h>
#include <commctrl.h>
#include "xDcBase.h"


#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "WS2_32.Lib")
#pragma comment(lib, "IPHlpApi.Lib")
#pragma comment(lib, "Uxtheme.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "mpr.lib")
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "iphlpapi.lib")


// ==========================================================================
// --
// --------------------------------------------------------------------------
xDcBase::xDcBase()
{
  mhPen         = NULL;
  mhBrush       = NULL;
  mhFont        = NULL;
  mhBackupPen   = NULL;
  mhBackupBrush = NULL;
  mhBackupFont  = NULL;
}
// --------------------------------------------------------------------------
xDcBase::~xDcBase()
{
  FreePen();
  FreeBrush();
  FreeFont();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 直線の表示
//  [I] sx : 始点（Ｘ）
//    sy : 始点（Ｙ）
//    ex : 終点（Ｘ）
//    ey : 終点（Ｙ）
//  PS_SOLID            0
//  PS_DASH             1       /* -------  */
//  PS_DOT              2       /* .......  */
//  PS_DASHDOT          3       /* _._._._  */
//  PS_DASHDOTDOT       4       /* _.._.._  */
//  PS_NULL             5
//  PS_INSIDEFRAME      6
//  PS_USERSTYLE        7
//  PS_ALTERNATE        8
//
//  [R] 0     : 失敗
//      0以外 : 関数成功
// --------------------------------------------------------------------------
BOOL xDcBase::Line( int sx, int sy, int ex, int ey )
{
  ::MoveToEx( mhDC, sx, sy, NULL );

  return ::LineTo( mhDC, ex, ey );
}
// --------------------------------------------------------------------------
BOOL xDcBase::Line( int sx, int sy, int ex, int ey, COLORREF colPen, int style )
{
  HPEN  hPen = ::CreatePen( style, 1, colPen );
  HPEN  prePen;

  prePen = ( HPEN )SelectObject( hPen );

  Line( sx, sy, ex, ey );

  SelectObject( prePen );
  DeleteObject( hPen );

  return TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 直線の表示
//  [I] ex : 終点（Ｘ）
//    ey : 終点（Ｙ）
//  [R] 0     : 失敗
//    0以外 : 関数成功
// --------------------------------------------------------------------------
BOOL xDcBase::Line( int ex, int ey )
{
  return ::LineTo( mhDC, ex, ey );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 文字列の表示
//  [I] x       : 位置（Ｘ）
//    y       : 位置（Ｙ）
//    pString : 文字列
//  [R] 0     : 失敗
//      0以外 : 関数成功
// --------------------------------------------------------------------------
BOOL xDcBase::TextOut( int x, int y, const TCHAR *pString )
{
  return ::TextOut( mhDC, x, y, pString, ( int )_tcslen( pString ) );
}
// --------------------------------------------------------------------------
BOOL xDcBase::TextOut( int x, int y, const TCHAR *pString, int count )
{
  return ::TextOut( mhDC, x, y, pString, count );
}
// --------------------------------------------------------------------------
BOOL xDcBase::ExtTextOut( int x, int y, const TCHAR *pString, int *pWidths )
{
  return ::ExtTextOut( mhDC, x, y, 0, NULL, pString, ( int )_tcslen( pString ), pWidths );
}
// --------------------------------------------------------------------------
BOOL xDcBase::ExtTextOut( int x, int y, const TCHAR *pString, int width )
{
  int           len = ( int )_tcslen( pString );
  const TCHAR*  cp;
  int           i;
  int*          widths;

  widths = ( int* )alloca( len * sizeof( int* ) * sizeof( TCHAR ) + 1 );

#ifdef _UNICODE
  cp = pString;
  i = 0;

  while ( *cp )
    {
      if ( *cp > 0xFF )
        {
          widths[ i ] = width * 2;
        }
      else
        {
          widths[ i ] = width;
        }
      cp++;
      i++;
    }
#else
  for ( i = 0; i < len; i++ )
    {
      widths[ i ] = width;
    }
#endif

  return ::ExtTextOut( mhDC, x, y, 0, NULL, pString, len, widths );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Printf
//  [I] x   : 位置（Ｘ）
//    y   : 位置（Ｙ）
//    buf : 文字列（フォーマット付き）
// --------------------------------------------------------------------------
void xDcBase::Printf( int x, int y, const TCHAR *buf, ... )
{
  TCHAR   outstr[ STRINGBUFFER + 1 ];
  va_list ap;

  va_start( ap, buf );

  outstr[ 0 ] = '\0';
  wvsprintf( outstr, buf, ap );

  va_end( ap );

  TextOut( x, y, outstr );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Printf
//  [I] x   : 位置（Ｘ）
//    y   : 位置（Ｙ）
//    pRC : 長方形の論理座標が入った、RECT 構造体
//    frm : フォーマット
//    buf : 文字列（フォーマット付き）
// --------------------------------------------------------------------------
void xDcBase::Printf( RECT *pRC, UINT format, const TCHAR *buf, ... )
{
  TCHAR outstr[ STRINGBUFFER + 1 ];
  va_list ap;

  va_start( ap, buf );

  outstr[ 0 ] = '\0';
  wvsprintf( outstr, buf, ap );

  va_end( ap );

  DrawText( outstr, pRC, format );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ビットマップデータを指定したデバイスコンテキストに表示
// --------------------------------------------------------------------------
int xDcBase::SetDIBitsToDevice( int x, int y, BITMAPINFO *pBitmapInfo, void *pData )
{
  int   cx = pBitmapInfo->bmiHeader.biWidth;
  int   cy = pBitmapInfo->bmiHeader.biHeight;

  return ::SetDIBitsToDevice( mhDC, x, y, cx, cy, 0, 0, 0, cy, pData, pBitmapInfo, DIB_RGB_COLORS );
}
// --------------------------------------------------------------------------
int xDcBase::SetDIBitsToDevice( int x, int y, BITMAPINFO *pBitmapInfo )
{
  int     cx    = pBitmapInfo->bmiHeader.biWidth;
  int     cy    = pBitmapInfo->bmiHeader.biHeight;
  RGBQUAD*  pData = pBitmapInfo->bmiColors;

  pData += pBitmapInfo->bmiHeader.biClrUsed;

  return ::SetDIBitsToDevice( mhDC, x, y, cx, cy, 0, 0, 0, cy, pData, pBitmapInfo, DIB_RGB_COLORS );
}
// --------------------------------------------------------------------------
int xDcBase::SetDIBitsToDevice( int x, int y, int srcX, int srcY, int width, int height, BITMAPINFO *pBitmapInfo, void *pData )
{
  return ::SetDIBitsToDevice( mhDC, x, y, width, height, srcX, srcY, 0, pBitmapInfo->bmiHeader.biHeight, pData, pBitmapInfo, DIB_RGB_COLORS );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- デバイスコンテキストに、指定されたオブジェクトを選択します
// --------------------------------------------------------------------------
HGDIOBJ xDcBase::SelectObject( HGDIOBJ hGdiObject )
{
  return ::SelectObject( mhDC, hGdiObject );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 文字列の色
// --------------------------------------------------------------------------
BOOL xDcBase::SetTextColor( COLORREF color )
{
  return ::SetTextColor( mhDC, color );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 文字列のバックグラウンドの色
// --------------------------------------------------------------------------
BOOL xDcBase::SetBkColor( COLORREF color )
{
  return ::SetBkColor( mhDC, color );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 文字列の幅と高さを計算します。
// --------------------------------------------------------------------------
BOOL xDcBase::GetTextExtentPoint32( const TCHAR *pString, SIZE *pSize )
{
  return ::GetTextExtentPoint32( mhDC, pString, ( int )_tcslen( pString ), pSize );
}
// --------------------------------------------------------------------------
BOOL xDcBase::GetTextExtentPoint32( const TCHAR *pString, int len, SIZE *pSize )
{
  return ::GetTextExtentPoint32( mhDC, pString, len, pSize );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 長方形を指定された色で塗りつぶします
// --------------------------------------------------------------------------
int xDcBase::FillRect( const RECT *pRC, COLORREF color )
{
  HBRUSH  hBrush = ::CreateSolidBrush( color );
  int     res;

  res = ::FillRect( mhDC, pRC, hBrush );

  DeleteObject( hBrush );

  return res;
}
// --------------------------------------------------------------------------
int xDcBase::FillRect( int x, int y, int cx, int cy, COLORREF color )
{
  HBRUSH  hBrush = ::CreateSolidBrush( color );
  int     res;
  RECT    rc;

  rc.left   = x;
  rc.top    = y;
  rc.right  = cx + x;
  rc.bottom = cy + y;

  res = ::FillRect( mhDC, &rc, hBrush );

  DeleteObject( hBrush );

  return res;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 現在選択されているフォントの情報を取得します。
// --------------------------------------------------------------------------
BOOL xDcBase::GetTextMetrics( TEXTMETRIC *pTM )
{
  if ( pTM == NULL )
    {
      pTM = &tm;
    }

  return ::GetTextMetrics( mhDC, pTM );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 四角を書きます
// --------------------------------------------------------------------------
BOOL xDcBase::Rectangle( int x, int y, int cx, int cy )
{
  return ::Rectangle( mhDC, x, y, x + cx, y + cy );
}
// --------------------------------------------------------------------------
BOOL xDcBase::Rectangle( RECT *pRC )
{
  return ::Rectangle( mhDC, pRC->left, pRC->top, pRC->right - pRC->left, pRC->bottom - pRC->top );
}
// --------------------------------------------------------------------------
BOOL xDcBase::Rectangle( int x, int y, int cx, int cy, COLORREF pen, COLORREF brush )
{
  HBRUSH  hBrush = ::CreateSolidBrush( brush );
  HPEN    hPen   = ::CreatePen( PS_SOLID, 1, pen );
  HBRUSH  preBrush;
  HPEN    prePen;
  BOOL    res;

  preBrush = ( HBRUSH )SelectObject( hBrush );
  prePen   = (  HPEN  )SelectObject( hPen );

  res = ::Rectangle( mhDC, x, y, x + cx, y + cy );

  SelectObject( preBrush );
  SelectObject( prePen );

  DeleteObject( hBrush );
  DeleteObject( hPen );

  return res;
}
// --------------------------------------------------------------------------
BOOL xDcBase::Rectangle( const RECT *pRC, COLORREF pen, COLORREF brush )
{
  return Rectangle( pRC->left,
                    pRC->top,
                    pRC->right  - pRC->left,
                    pRC->bottom - pRC->top,
                    pen,
                    brush );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 長方形の辺を描画します
//  edge     :  BDR_RAISEDINNER 辺の内側を隆起させます 
//        BDR_SUNKENINNER 辺の内側を陥没させます 
//  grfFlags :  BDR_RAISEDOUTER 辺の外側を隆起させます 
//        BDR_SUNKENOUTER 辺の外側を陥没させます 
// --------------------------------------------------------------------------
BOOL xDcBase::DrawEdge( RECT *pRC, UINT edge, UINT grfFlags )
{
  return ::DrawEdge( mhDC, pRC, edge, grfFlags );
}
// --------------------------------------------------------------------------
BOOL xDcBase::DrawEdge( int left, int top, int right, int bottom, UINT edge, UINT grfFlags )
{
  RECT  rc;

  rc.left   = left;
  rc.top    = top;
  rc.right  = right;
  rc.bottom = bottom;

  return ::DrawEdge( mhDC, &rc, edge, grfFlags );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 背景モードを設定します
//  bkMode : OPAQUE      現在の背景色で背景を塗りつぶします。 
//           TRANSPARENT 背景をそのまま残します 
// --------------------------------------------------------------------------
int xDcBase::SetBkMode( int bkMode )
{
  return ::SetBkMode( mhDC, bkMode );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 楕円形を描画します。
// --------------------------------------------------------------------------
BOOL xDcBase::Ellipse( int nLeftRect, int nTopRect, int nRightRect, int nBottomRect )
{
  return ::Ellipse( mhDC, nLeftRect, nTopRect, nRightRect, nBottomRect );
}
// --------------------------------------------------------------------------
BOOL xDcBase::Ellipse( int x, int y, int cx, int cy, COLORREF pen, COLORREF brush )
{
  HBRUSH  hBrush = ::CreateSolidBrush( brush );
  HPEN    hPen   = ::CreatePen( PS_SOLID, 1, pen );
  HBRUSH  preBrush;
  HPEN    prePen;
  BOOL    res;

  preBrush = ( HBRUSH )SelectObject( hBrush );
  prePen   = (  HPEN  )SelectObject( hPen );

  res = ::Ellipse( mhDC, x, y, x + cx, y + cy );

  SelectObject( preBrush );
  SelectObject( prePen );

  DeleteObject( hBrush );
  DeleteObject( hPen );

  return res;
}
// --------------------------------------------------------------------------
BOOL xDcBase::Ellipse( RECT *pRC, COLORREF pen, COLORREF brush )
{
  return Ellipse( pRC->left, pRC->top, pRC->right, pRC->bottom, pen, brush );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 角の丸い長方形を描画します。
// --------------------------------------------------------------------------
BOOL xDcBase::RoundRect(
  int     nLeftRect,
  int     nTopRect,
  int     nRightRect,
  int     nBottomRect,
  int     nWidth,
  int     nHeight,
  COLORREF  colBk,
  COLORREF  colPen,
  int     style )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
{
  HBRUSH  hBrush = ::CreateSolidBrush( colBk );
  HPEN    hPen   = ::CreatePen( style, 1, colPen );
  HBRUSH  preBrush;
  HPEN    prePen;
  BOOL    res;

  preBrush = ( HBRUSH )SelectObject( hBrush );
  prePen   = (  HPEN  )SelectObject( hPen );
  res = ::RoundRect( mhDC, nLeftRect, nTopRect, nRightRect, nBottomRect, nWidth, nHeight );

  SelectObject( preBrush );
  SelectObject( prePen );

  DeleteObject( hBrush );
  DeleteObject( hPen );

  return res;
}
// --------------------------------------------------------------------------
BOOL xDcBase::RoundRect( const RECT *pRC, int nWidth, int nHeight, COLORREF colBk, COLORREF colPen, int style )
{
  return RoundRect( pRC->left, pRC->top, pRC->right, pRC->bottom, nWidth, nHeight, colBk, colPen, style );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ビットマップデータを指定したデバイスコンテキストに表示
// --------------------------------------------------------------------------
int xDcBase::StretchDIBits( int x, int y, int dispCx, int dispCy, BITMAPINFO *pBitmapInfo, void *pData )
{
  int   w = pBitmapInfo->bmiHeader.biWidth;
  int   h = pBitmapInfo->bmiHeader.biHeight;

  if ( pData == NULL )
    {
      pData = pBitmapInfo->bmiColors + pBitmapInfo->bmiHeader.biClrUsed;
    }

  return ::StretchDIBits(
    mhDC,
    x,
    y,
    dispCx,
    dispCy,
    0,
    0,
    w,
    h,
    pData,
    pBitmapInfo,
    DIB_RGB_COLORS,
    SRCCOPY );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 指定された長方形内に、フォーマットしたテキストを表示します
// --------------------------------------------------------------------------
int xDcBase::DrawText( LPCTSTR lpString, LPRECT lpRect, UINT uFormat )
{
  return ::DrawText( mhDC, lpString, -1, lpRect, uFormat );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- APIのラップ
// --------------------------------------------------------------------------
HDC   xDcBase::CreateCompatibleDC() { return ::CreateCompatibleDC( mhDC ); }
HBITMAP xDcBase::CreateCompatibleBitmap( int cx, int cy ) { return ::CreateCompatibleBitmap( mhDC, cx, cy ); }
void  xDcBase::BitBlt( int x1, int y1, int cx, int cy, HDC hDC, int x2, int y2, DWORD flags )
  { ::BitBlt( mhDC, x1, y1, cx, cy, hDC, x2, y2, flags ); }
void  xDcBase::BitBlt( RECT *p1, HDC hDC, RECT *p2, DWORD flags )
  { ::BitBlt( mhDC, p1->left, p1->top, p1->right - p1->left, p1->bottom - p1->top, hDC, p2->left, p2->top, flags ); }
BOOL  xDcBase::StretchBlt( int nDstX, int nDstY, int nDstCx, int nDstCt, HDC hDcSrc, int nSrcX, int nSrcY, int nSrcCx, int nSrcCy, DWORD rop )
  { return ::StretchBlt( mhDC, nDstX, nDstY, nDstCx, nDstCt, hDcSrc, nSrcX, nSrcY, nSrcCx, nSrcCy, rop ); }
// --------------------------------------------------------------------------


// ==========================================================================
// -- 伸縮モードの変更、取得
// BLACKONWHITE 標準設定。低画質だが高速。拡大または等倍だけならこのままで
// COLORONCOLOR 実用的な画質で高速。縮小が必要な時に使おう
// HALFTONE     高画質だが低速。ビューワーなどで使おう
// --------------------------------------------------------------------------
BOOL xDcBase::SetStretchBltMode( int iStretchMode )
{
  return ::SetStretchBltMode( mhDC, iStretchMode );
}
// --------------------------------------------------------------------------
int xDcBase::GetStretchBltMode()
{
  return ::GetStretchBltMode( mhDC );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void xDcBase::SetSelectPen( COLORREF color, int weight, int style )
{
  FreePen();
  mhPen       = ::CreatePen( style, weight, color );
  mhBackupPen = SelectObject( mhPen );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void xDcBase::SetSelectBrush( COLORREF color )
{
  FreeBrush();
  mhBrush       = ::CreateSolidBrush( color );
  mhBackupBrush = SelectObject( mhBrush );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void xDcBase::SetSelectFont( const TCHAR *fontname, int fontsize, int bold )
{
  LOGFONT log;

  FreeFont();

  ZeroMemory( &log, sizeof( LOGFONT ) );

  log.lfHeight         = fontsize;                    // フォント高さ
  log.lfWeight         = bold;                        // フォントの重さ(太さ)
  log.lfCharSet        = SHIFTJIS_CHARSET;            // 文字セット
  log.lfOutPrecision   = OUT_DEFAULT_PRECIS;          // 出力精度
  log.lfClipPrecision  = CLIP_DEFAULT_PRECIS;         // クリッピング精度
  log.lfQuality        = PROOF_QUALITY;               // 出力品質
  log.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;     // ピッチとファミリー
  wcscpy_s( log.lfFaceName, LF_FACESIZE, fontname );  // 書体名

  mhFont       = ::CreateFontIndirect( &log );
  mhBackupFont = SelectObject( mhFont );

  GetTextMetrics( &tm );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void xDcBase::FreePen( void )
{
  if ( mhBackupPen )
    {
      SelectObject( mhBackupPen );
      mhBackupPen = NULL;
    }

  if ( mhPen )
    {
      DeletePen( mhPen );
      mhPen = NULL;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void xDcBase::FreeBrush( void )
{
  if ( mhBackupBrush )
    {
      SelectObject( mhBackupBrush );
      mhBackupBrush = NULL;
    }

  if ( mhBrush )
    {
      DeleteBrush( mhBrush );
      mhBrush = NULL;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
void xDcBase::FreeFont( void )
{
  if ( mhBackupFont )
    {
      SelectObject( mhBackupFont );
      mhBackupFont = NULL;
    }

  if ( mhFont )
    {
      DeleteFont( mhFont );
      mhFont = NULL;
    }
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL xDcBase::Lined( int sx, int sy, int ex, int ey, COLORREF colPen )
{
  HPEN      hPen;
  LOGBRUSH  log;
  HPEN      prePen;
  DWORD     ll[ 2 ] = { 1, 1 };

  log.lbStyle = BS_SOLID;
  log.lbColor = colPen;
  log.lbHatch = NULL;    // Sundown: lbHatch could hold a HANDLE

  hPen = ::ExtCreatePen( /*PS_COSMETIC | PS_USERSTYLE*/PS_GEOMETRIC | PS_ENDCAP_FLAT | PS_USERSTYLE, 1, &log, 2, ll );

  prePen = ( HPEN )SelectObject( hPen );

  Line( sx, sy, ex, ey );

  SelectObject( prePen );
  DeleteObject( hPen );

  return TRUE;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL xDcBase::GetTextExtentPoint32( TCHAR *str, LPSIZE lpSize )
{
  return ::GetTextExtentPoint32( mhDC, str, ( int )_tcslen( str ), lpSize );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL xDcBase::ImageList_DrawEx( void*    pHiml,
                                int      index,
                                int      x,
                                int      y,
                                int      cx,
                                int      cy,
                                COLORREF rgbBk,
                                COLORREF rgbFg,
                                UINT     fStyle )
{
  HIMAGELIST himl = ( HIMAGELIST )pHiml;

  return ::ImageList_DrawEx( himl, index, mhDC, x, y, cx, cy, rgbBk, rgbFg, fStyle );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
BOOL xDcBase::ImageList_DrawEx( void* pHiml,
                                int   index,
                                int   x,
                                int   y,
                                int   cx,
                                int   cy,
                                UINT  fStyle )
{
  HIMAGELIST himl = ( HIMAGELIST )pHiml;

  return ::ImageList_DrawEx( himl, index, mhDC, x, y, cx, cy, CLR_NONE, CLR_NONE, fStyle );
}
// --------------------------------------------------------------------------
