/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iComList.h
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
#ifndef _ICOMLIST_H_
#define _ICOMLIST_H_


// ===========================================================================
// -- iComList クラス --
// ---------------------------------------------------------------------------
class iComList : public xListBox
{
public:
	void  SetLogData( int index, const TCHAR *pDispName, int type = -1 );

public:
	iComList();
	~iComList();

private:
	int   DrawItem( const DRAWITEMSTRUCT *lpDrawItem );
	int   MeasureItem( MEASUREITEMSTRUCT *lpMeasureItem );

private:
	void  DispDirectory( HDC hDC, int x, int cy, int height, TCHAR *str );

protected:
	void  FillRect( HDC hDC, const RECT *pRC, COLORREF fg );
  void  FillRect( HDC hDC, const RECT *pRC, COLORREF pen, COLORREF fg );

private:
	DWORD	date_time;

};
// ---------------------------------------------------------------------------


#endif//_ICOMLIST_H_
