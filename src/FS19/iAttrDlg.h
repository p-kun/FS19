/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iAttrDlg.h
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
#ifndef _IATTRDLG_H_
#define _IATTRDLG_H_


class xFont;

// ==========================================================================
// -- iAttrDlg --
// --------------------------------------------------------------------------
class iAttrDlg : public xDialog
{
public:
  void  SetAttribute( dirent *entry );
  DWORD GetAttribute() { return mp_entry->dwFileAttributes; }
  BOOL  GetCheckDir() { return mbEnableDir; }
  void  SetAttributes( void );

public:
  iAttrDlg( HWND hParent );
  ~iAttrDlg();

private:
  void OnCommand( int id, HWND hwndCtl, UINT codeNotify );
  BOOL OnInitDialog( HWND hwndFocus, LPARAM lParam );
  void OnPaint();

private:
  xButton*  mpCheckBoxs[ 5 ];
  dirent*   mp_entry;
  xFont*    mpFont;
  BOOL      mbEnableDir;
  DWORD     mAttribute;
};
// --------------------------------------------------------------------------


#endif//_IATTRDLG_H_
