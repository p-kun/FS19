/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iComButton.h
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
#ifndef _ICOMBUTTON_H_
#define _ICOMBUTTON_H_

//
#define iComButton_KEY	0x8A00


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iComButton : public xButton
{
public:
	void OnKeyDown( UINT vk, BOOL fDown, int cRepeat, UINT flags );
	int  DrawItem( const DRAWITEMSTRUCT *pDIS );
	void FillRect( HDC hDC, const RECT *pRC, COLORREF fg );
	
};
// --------------------------------------------------------------------------


#endif//_ICOMBUTTON_H_
