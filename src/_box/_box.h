/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: _box.h
 *  Created  : 19/08/24 08:28:31
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
#ifndef __BOX_H_
#define __BOX_H_

// SDKDDKVer.h をインクルードすると、利用できる最も高いレベルの Windows プラットフォームが定義されます。
// 以前の Windows プラットフォーム用にアプリケーションをビルドする場合は、WinSDKVer.h をインクルードし、
// サポートしたいプラットフォームに _WIN32_WINNT マクロを設定してから SDKDDKVer.h をインクルードします。

#include <SDKDDKVer.h>

/* Windows ヘッダーからほとんど使用されていない部分を除外する */

//#define WIN32_LEAN_AND_MEAN

/* Windows ヘッダー ファイル */

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <locale.h>
#include <olectl.h>
#include <shellapi.h>
#include <iphlpapi.h>
#include <IcmpAPI.h>
#include <aclapi.h>
#include <gdiplus.h>
#include <Uxtheme.h>

/* C ランタイム ヘッダー ファイル */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include "_boxCore.h"
#include "_boxMsgCore.h"
#include "_boxApp.h"
#include "xDcBase.h"
#include "xControls.h"
#include "xDialog.h"
#include "xThread.h"
#include "xFont.h"

#endif // __BOX_H_
