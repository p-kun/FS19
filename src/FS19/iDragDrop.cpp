/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iDragDrop.cpp
 *  Created  : 08/04/09(水) 16:34:42
 *
 *  Function
 *
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.3 $
 *  $Date: 2008/07/28 13:50:15 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#include "_box.h"
#include "iDragDrop.h"


// ==========================================================================
// -- QueryInterface
// --------------------------------------------------------------------------
iDragDrop::iDragDrop()
{
	FORMATETC*	p = mFmes;

	SetFormatetc( p++, CF_TEXT );
	SetFormatetc( p++, CF_BITMAP );
	SetFormatetc( p++, CF_METAFILEPICT );
	SetFormatetc( p++, CF_SYLK );
	SetFormatetc( p++, CF_DIF );
	SetFormatetc( p++, CF_TIFF );
	SetFormatetc( p++, CF_OEMTEXT );
	SetFormatetc( p++, CF_DIB );
	SetFormatetc( p++, CF_PALETTE );
	SetFormatetc( p++, CF_PENDATA );
	SetFormatetc( p++, CF_RIFF );
	SetFormatetc( p++, CF_WAVE );
	SetFormatetc( p++, CF_UNICODETEXT );
	SetFormatetc( p++, CF_ENHMETAFILE );
	SetFormatetc( p++, CF_HDROP );
	SetFormatetc( p++, CF_LOCALE );
	SetFormatetc( p++, CF_MAX );
	SetFormatetc( p++, CF_OWNERDISPLAY );
	SetFormatetc( p++, CF_DSPTEXT );
	SetFormatetc( p++, CF_DSPBITMAP );
	SetFormatetc( p++, CF_DSPMETAFILEPICT );
	SetFormatetc( p++, CF_DSPENHMETAFILE );
	SetFormatetc( p++, CF_PRIVATEFIRST );
	SetFormatetc( p++, CF_PRIVATELAST );
	SetFormatetc( p++, CF_GDIOBJFIRST );
	SetFormatetc( p++, CF_GDIOBJLAST );
	SetFormatetc( p++, 0 );

	mRefCnt = 1;
}
// --------------------------------------------------------------------------



// ==========================================================================
// -- DoDragDrop
// --------------------------------------------------------------------------
void iDragDrop::DoDragDrop( const TCHAR *path, size_t size )
{
//IDropSource*	pDropSource;
//IDataObject*	pDataObject;
	DWORD			    param = 0;

	mpPath = path;
	mSize  = size;

//QueryInterface( IID_IDropSource, ( void** )&pDropSource );
//QueryInterface( IID_IDataObject, ( void** )&pDataObject );

	if ( SUCCEEDED( ::DoDragDrop( this, this, DROPEFFECT_COPY, &param ) ) )
    {
  		;
  	}
	else
    {
  		;
  	}

//	pDropSource->Release();
//	pDataObject->Release();
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Register
// --------------------------------------------------------------------------
void iDragDrop::Register( HWND hWnd )
{
  /* IDropTarget*	pDropTarget;
   * iDragDrop	drop;
   * QueryInterface( IID_IDropSource, ( void** )&pDropTarget );
   */

	::RegisterDragDrop( hWnd, this );
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- QueryInterface
// --------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE iDragDrop::QueryInterface( REFIID riid, void **pObj )
{
	if ( IsEqualIID( riid, IID_IUnknown ) || IsEqualIID( riid, IID_IDropTarget ) )
    {
  		*pObj = static_cast< IDropTarget* >( this );
  	}
	else if ( IsEqualIID( riid, IID_IDropSource ) )
    {
  		*pObj = static_cast< IDropSource* >( this );
  	}
	else if ( IsEqualIID( riid, IID_IDataObject ) )
    {
  		*pObj = static_cast< IDataObject* >( this );
  	}
	else if ( IsEqualIID( riid, IID_IEnumFORMATETC ) )
    {
  		*pObj = static_cast< IEnumFORMATETC* >( this );
  	}
	else
    {
  		*pObj = NULL;

  		return E_NOINTERFACE;
  	}

	AddRef();			// 参照カウンタをインクリメントする

	return S_OK;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 参照カウンタを単純にインクリメントして、その値を返却
// --------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE iDragDrop::AddRef()
{
	mRefCnt++;

	return mRefCnt;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- 参照カウンタをデクリメント
// --------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE iDragDrop::Release()
{
	if ( mRefCnt > 0 )
    {
  		mRefCnt--;
  	}

	return S_OK;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- ドラッグを継続するかどうかを決める
// --------------------------------------------------------------------------
STDMETHODIMP iDragDrop::QueryContinueDrag( BOOL isEsc, DWORD state )
{
	/* ESCが押された場合やマウスのボタンが両方押されたときは中止 */

	if ( isEsc || ( MK_LBUTTON | MK_RBUTTON ) == ( state & ( MK_LBUTTON | MK_RBUTTON ) ) )
    {
  		return DRAGDROP_S_CANCEL;
  	}

	/* マウスボタンが離されたときはドロップ */

	if ( ( state & ( MK_LBUTTON | MK_RBUTTON ) ) == 0 )
    {
  		return DRAGDROP_S_DROP;
  	}

	return S_OK;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- GiveFeedback
// --------------------------------------------------------------------------
STDMETHODIMP iDragDrop::DragOver( DWORD grfKeyState, POINTL pt, DWORD *pdwEffect )
{
	return E_NOTIMPL;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- GiveFeedback
// --------------------------------------------------------------------------
STDMETHODIMP iDragDrop::GiveFeedback( DWORD fb )
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- GetData
// --------------------------------------------------------------------------
STDMETHODIMP iDragDrop::GetData( FORMATETC* pFormatetc, STGMEDIUM* pMedium )
{
	HDROP	  hDrop;
	TCHAR*	cp;

	if ( pFormatetc->tymed != TYMED_HGLOBAL )
    {
  		return DV_E_FORMATETC;
  	}

	if ( pFormatetc->cfFormat != CF_HDROP )
    {
  		return DV_E_FORMATETC;
  	}

	hDrop = ( HDROP )::GlobalAlloc( GHND, mSize );

	cp = ( TCHAR* )::GlobalLock( hDrop );

	memcpy( cp, mpPath, mSize );

	::GlobalUnlock( hDrop );

	pMedium->hGlobal		= hDrop;
	pMedium->tymed			= pFormatetc->tymed;
	pMedium->pUnkForRelease	= NULL;

	return S_OK;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- QueryGetData
// --------------------------------------------------------------------------
STDMETHODIMP iDragDrop::QueryGetData( FORMATETC* p )
{
	if ( p->tymed != TYMED_HGLOBAL )
    {
  		return DV_E_FORMATETC;
  	}

	if ( p->cfFormat != CF_HDROP )
    {
  		return DV_E_FORMATETC;
  	}

	return S_OK;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- GiveFeedback
// --------------------------------------------------------------------------
STDMETHODIMP iDragDrop::EnumFormatEtc( DWORD dwDirection, IEnumFORMATETC** p )
{
	if ( dwDirection == DATADIR_GET )
    {
  		QueryInterface( IID_IEnumFORMATETC,( void** )p );
  		Reset();

  		return S_OK;
  	}

	return E_NOTIMPL;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Next
// --------------------------------------------------------------------------
STDMETHODIMP iDragDrop::Next( ULONG celt, FORMATETC *rgelt, ULONG *pcelt )
{
	if ( pcelt != NULL )
    {
  		*pcelt = 0;
  	}

	if ( celt <= 0 || rgelt == NULL )
    {
  		return S_FALSE;
  	}

	if ( mpFmes->cfFormat == 0 )
    {
  		return S_FALSE;
  	}

	*rgelt = *mpFmes++;

	return S_OK;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- Reset
// --------------------------------------------------------------------------
STDMETHODIMP iDragDrop::Reset()
{
	mpFmes = mFmes;

	return S_OK;
}
// --------------------------------------------------------------------------


// ==========================================================================
// -- SetFormatetc
// --------------------------------------------------------------------------
void iDragDrop::SetFormatetc( FORMATETC* p, CLIPFORMAT cf )
{
	p->cfFormat	= cf;
	p->ptd		= NULL;
	p->dwAspect	= DVASPECT_CONTENT;
	p->lindex	= -1;
	p->tymed	= TYMED_HGLOBAL;
}
// --------------------------------------------------------------------------
