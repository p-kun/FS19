#include <windows.h>
#include <gdiplus.h>
#include <tchar.h>
#include "GdiLoadImage.h"

#pragma comment (lib, "gdiplus.lib")

using namespace Gdiplus;

bool GdiLoadImage(HDC hDC, TCHAR path[])
{
#if 1

  /*  GDI+ object (necessary for image development) */

  GdiplusStartupInput startup;
  uintptr_t           token;

  /* Initial setting of GDI + */

  if (GdiplusStartup(&token, &startup, NULL) !=  Ok)
    {
      return false;
    }

  Bitmap *pBitmap = Bitmap::FromFile(path);

  int   width  = 0;
  int   height = 0;

  if (pBitmap && pBitmap->GetLastStatus() == Ok)
    {
      width  = pBitmap->GetWidth();
      height = pBitmap->GetHeight();

      Graphics  MyGraphics(hDC);

      MyGraphics.DrawImage(pBitmap, 0, 0, width, height);

      /* Release of GDI+ */

      delete pBitmap;
    }

  GdiplusShutdown(token);

#endif

#if 0
    // 文字コードをワイド文字列に変換
    // 【注意】本来はこの箇所は文字列バッファ長の考慮の他に文字列終端コードを処理するよりセキュアな対応が好ましいです。
    wchar_t     path[ MAX_PATH ];
    size_t      pathLength = 0;
 
    if( mbstowcs_s( &amp;pathLength,            // [out]    変換された文字数
                    &amp;path[0],               // [out]    変換されたワイド文字列を格納するバッファのアドレス(変換先)
                    MAX_PATH,               // [in]     出力側のバッファのサイズ(単位:文字数)
                    fileName,               // [in]     マルチバイト文字列のアドレス(変換元)
                    _TRUNCATE ) != 0 ) {    // [in]     出力先に格納するワイド文字の最大数
        return false;
    }
 
    //  GDI+オブジェクト（画像展開に必要）
    Gdiplus::GdiplusStartupInput    gdiplusStartupInput;
    ULONG_PTR                       gdiplusToken;
 
    //---- GDI+の初期設定
    if( Gdiplus::GdiplusStartup(&amp;gdiplusToken, &amp;gdiplusStartupInput, NULL) !=  Gdiplus::Ok ) {
        return false;
    }
 
    //-------------------------------------------------------------
    // 画像の読み込み
    //-------------------------------------------------------------
    bool    result = false;
    int     width  = 0;         // 画像の幅
    int     height = 0;         //  〃  高さ
 
    //--- 画像ファイルを開く
    //  【対応画像形式】  BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
    Gdiplus::Bitmap*    pBitmap = Gdiplus::Bitmap::FromFile(path);
    if( pBitmap &amp;&amp; pBitmap-&gt;GetLastStatus() == Gdiplus::Ok ) {
        //---- 画像サイズ分の領域確保
        width  = pBitmap-&gt;GetWidth();
        height = pBitmap-&gt;GetHeight();
 
        // ■ロードする画像のメモリの解像度を変更/設定（この位置に任意に記述して下さい）
        if( /* createBuffer(width, height)*/ ) {
            result = true;
        }
    }
 
    //---- 画像イメージの読み込み
    if( result == true ) {
        for( int y=0; y&lt;height; y++ ) {
            for( int x=0; x&lt;width; x++ ) {
                Gdiplus::Color  srcColor;
                pBitmap-&gt;GetPixel(x, y, &amp;srcColor);
 
                unsigned char r = srcColor.GetR();
                unsigned char g = srcColor.GetG();
                unsigned char b = srcColor.GetB();
                unsigned char a = srcColor.GetA();
 
                // ■ピクセルへの描画（この位置に任意に記述して下さい）
                // setPixel(x, y,  r, g, b, a);
            }
        }
    }
 
    delete pBitmap;
 
    //---- GDI+の解放
    Gdiplus::GdiplusShutdown(gdiplusToken);
 
    return result;
#endif
  return true;
}
