/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: xDcBase.h
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
#ifndef _XDCBASE_H_
#define _XDCBASE_H_


// ==========================================================================
//
// -- xDcBase クラス --
//
// --------------------------------------------------------------------------
class xDcBase
{
public:
  xDcBase();
  virtual ~xDcBase();

public:
  operator HDC () const { return mhDC; }

public:

  /* テキスト関連 */

  BOOL TextOut( int x, int y, const TCHAR *str );
  BOOL TextOut( int x, int y, const TCHAR *str, int count );
  BOOL ExtTextOut( int x, int y, const TCHAR *pString, int *pWidths );
  BOOL ExtTextOut( int x, int y, const TCHAR *pString, int witdth );
  BOOL SetTextColor( COLORREF color );
  BOOL SetBkColor( COLORREF color );
  int  SetBkMode( int bkMode );
  BOOL GetTextExtentPoint32( const TCHAR *pString, SIZE *pSize );
  BOOL GetTextExtentPoint32( const TCHAR *pString, int len, SIZE *pSize );
  BOOL GetTextMetrics( TEXTMETRIC *pTM = NULL );
  void Printf( int x, int y, const TCHAR *buf, ... );
  void Printf( RECT *pRC, UINT format, const TCHAR *buf, ... );
  int  DrawText( LPCTSTR lpString, LPRECT lpRect, UINT uFormat );
  BOOL GetTextExtentPoint32( TCHAR *str, LPSIZE lpSize );

  /* ビットマップ関連 */

  int SetDIBitsToDevice( int x, int y, BITMAPINFO *pBitmapInfo, void *pData );
  int SetDIBitsToDevice( int x, int y, int srcX, int srcY, int width, int height, BITMAPINFO *pBitmapInfo, void *pData );
  int SetDIBitsToDevice( int x, int y, BITMAPINFO *pBitmapInfo );
  int StretchDIBits( int x, int y, int dispCx, int dispCy, BITMAPINFO *pBitmapInfo, void *pData = NULL );

  /* グラフィック関連 */

  int  FillRect( const RECT *pRC, COLORREF color );
  int  FillRect( int x, int y, int cx, int cy, COLORREF color );
  BOOL Line( int sx, int sy, int ex, int ey );
  BOOL Line( int sx, int sy, int ex, int ey, COLORREF colPen, int style = PS_SOLID );
  BOOL Line( int ex, int ey );
  BOOL Lined( int sx, int sy, int ex, int ey, COLORREF colPen );
  BOOL Rectangle( int left, int top, int right, int bottom );
  BOOL Rectangle( RECT *pRC );
  BOOL Rectangle( int x, int y, int cx, int cy, COLORREF pen, COLORREF brush );
  BOOL Rectangle( const RECT *pRC, COLORREF pen, COLORREF brush );

  BOOL DrawEdge( RECT *pRC, UINT edge, UINT grfFlags );
  BOOL DrawEdge( int left, int top, int right, int bottom, UINT edge, UINT grfFlags );
  BOOL Ellipse( int nLeftRect, int nTopRect, int nRightRect, int nBottomRect );
  BOOL Ellipse( int x, int y, int cx, int cy, COLORREF pen, COLORREF brush );
  BOOL Ellipse( RECT *pRC, COLORREF pen, COLORREF brush );
  BOOL RoundRect( int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nWidth, int nHeight, COLORREF colBk, COLORREF colPen, int style = PS_SOLID );
  BOOL RoundRect( const RECT *pRC, int nWidth, int nHeight, COLORREF colBk, COLORREF colPen, int style = PS_SOLID );
  void BitBlt( int x1, int y1, int cx, int cy, HDC hDC, int x2, int y2, DWORD flags = SRCCOPY );
  void BitBlt( RECT *p1, HDC hDC, RECT *p2, DWORD flags = SRCCOPY );
  BOOL StretchBlt( int nDstX, int nDstY, int nDstCx, int nDstCt, HDC hDcSrc, int nSrcX, int nSrcY, int nSrcCx, int nSrcCy, DWORD rop = SRCCOPY );
  BOOL SetStretchBltMode( int iStretchMode );
  int  GetStretchBltMode();

  /* アイコン関連 */

  BOOL ImageList_DrawEx( void *himl, int index, int x, int y, int cx, int cy, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle = ILD_NORMAL );
  BOOL ImageList_DrawEx( void *himl, int index, int x, int y, int cx, int cy, UINT fStyle = ILD_NORMAL );

  /* その他 */

  HGDIOBJ SelectObject( HGDIOBJ hGdiObject );
  HDC     CreateCompatibleDC();
  HBITMAP CreateCompatibleBitmap( int cx, int cy );
  void    AttachDC( HDC hDC ) { mhDC = hDC; }

  void SetSelectPen( COLORREF color, int weight = 1, int style = PS_SOLID );
  void SetSelectBrush( COLORREF color );
  void SetSelectFont( const TCHAR *fontname, int fontsize, int bold = FW_REGULAR );
  void FreePen( void );
  void FreeBrush( void );
  void FreeFont( void );

public:
  TEXTMETRIC  tm;

protected:
  HDC       mhDC;
  HPEN      mhPen;
  HBRUSH    mhBrush;
  HFONT     mhFont;
  HGDIOBJ   mhBackupPen;
  HGDIOBJ   mhBackupBrush;
  HGDIOBJ   mhBackupFont;

protected:
  enum
  {
    STRINGBUFFER = 1024
  };

};
// ==========================================================================


// ==========================================================================
//
// -- xDC クラス --
//
// --------------------------------------------------------------------------
class xDC : public xDcBase
{
public:
  xDC( HWND hWnd ) { mhWnd = hWnd; mhDC = ::GetDC( hWnd ); }
  ~xDC() { ::ReleaseDC( mhWnd, mhDC ); }

private:
  HWND  mhWnd;
};
// ==========================================================================


// ==========================================================================
//
// -- xPaintDC クラス --
//
// --------------------------------------------------------------------------
class xPaintDC : public xDcBase
{
public:
  xPaintDC( HWND hWnd ) { mhWnd = hWnd; mhDC = ::BeginPaint( hWnd, &mPS ); }
  ~xPaintDC() { ::EndPaint( mhWnd, &mPS ); }

public:
  HWND    mhWnd;
  PAINTSTRUCT mPS;
};
// ==========================================================================


// ==========================================================================
//
// -- xTmpDC クラス --
//
// --------------------------------------------------------------------------
class xTmpDC : public xDcBase
{
public:
  xTmpDC( HWND hWnd, HDC hDC ) : mhWnd( hWnd ) { mhDC = hDC; }
  xTmpDC( HDC hDC ) : mhWnd( NULL ) { mhDC = hDC; ::GetTextMetrics( hDC, &tm ); }
  ~xTmpDC() {}

private:
  HWND    mhWnd;
};
// ==========================================================================


// ==========================================================================
//
// -- xBmpDC クラス --
//
// --------------------------------------------------------------------------
class xBmpDC : public xDcBase
{
public:
  xBmpDC()
  {
    mhBitmap  = NULL;
    mhDC      = NULL;
    mhPrevBmp = NULL;
  }

  xBmpDC( HDC hDC, int cx, int cy )
  {
    mhDC      = ::CreateCompatibleDC( hDC );
    mhBitmap  = ::CreateCompatibleBitmap( hDC, cx, cy );
    mhPrevBmp = SelectObject( mhBitmap );
  }

  ~xBmpDC()
  {
    DeleteObject();
  }

  void Create( HDC hDC, int cx, int cy )
  {
    DeleteObject();
    mhDC      = ::CreateCompatibleDC( hDC );
    mhBitmap  = ::CreateCompatibleBitmap( hDC, cx, cy );
    mhPrevBmp = SelectObject( mhBitmap );
  }

  void DeleteObject( void )
  {
    if ( mhBitmap )
      {
        SelectObject( mhPrevBmp );
        ::DeleteObject( mhBitmap );
        mhBitmap = NULL;
      }

    if ( mhDC )
      {
        ::DeleteDC( mhDC );
        mhDC = NULL;
      }
  }

  void Remove( void )
  {
    SelectObject( mhPrevBmp );
  }

  void Select( void )
  {
    mhPrevBmp = SelectObject( mhBitmap );
  }

public:
  operator HBITMAP () const { return mhBitmap; }

private:
  HBITMAP mhBitmap;
  HGDIOBJ mhPrevBmp;

};
// ==========================================================================

#endif // _XDCBASE_H_
