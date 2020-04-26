/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iRenameDlg.h
 *  Created  : 08/06/04(水) 08:28:31
 *
 *  Function
 *    リネームダイアログ
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:49 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#ifndef _IRENAMEDLG_H_
#define _IRENAMEDLG_H_

class xFont;

// ===========================================================================
// -- iRenameDlgクラス
// ---------------------------------------------------------------------------
class iRenameDlg : public xDialog
{
public:
  void  SetFileInfo( dirent* pFD );
  TCHAR rename[ MAX_PATH ];

public:
  iRenameDlg( HWND hParent );
  ~iRenameDlg();

private:
  BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam );
  void OnCommand( int id, HWND hwndCtl, UINT codeNotify );
  void OnPaint();

private:
  xEdit     mEdit;
  xFont*    mpFont;
  TCHAR     m_name[ MAX_PATH ];
  TCHAR     m_dir[ MAX_PATH ];
  dirent    m_entry;
};
// ---------------------------------------------------------------------------

#endif//_IRENAMEDLG_H_
