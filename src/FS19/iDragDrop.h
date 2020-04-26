/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iDragDrop.h
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
#ifndef _IDRAGDROP_H_
#define _IDRAGDROP_H_


// ==========================================================================
//
// -- ドラッグドロップクラス
//
// ==========================================================================
class iDragDrop : IDropTarget, IDropSource, IDataObject, IEnumFORMATETC
{
  // -- 公開します
  //
public:
  void  DoDragDrop( const TCHAR *path, size_t size );
  void  Register( HWND hWnd );


public:
  iDragDrop();


private:
  // -- IUnknown
  //
  virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID, void** );
  virtual ULONG STDMETHODCALLTYPE AddRef();
  virtual ULONG STDMETHODCALLTYPE Release();

  // -- IDropTarget
  //
  STDMETHOD( DragEnter )( IDataObject*, DWORD, POINTL, DWORD* )
    { return E_NOTIMPL; }
  STDMETHOD( DragOver )( DWORD, POINTL, DWORD* );
  STDMETHOD( DragLeave )()
    { return E_NOTIMPL; }
  STDMETHOD( Drop )( IDataObject*, DWORD, POINTL, DWORD* )
    { return E_NOTIMPL; }
  
  // -- IDropSource
  //
  STDMETHOD( QueryContinueDrag )( BOOL, DWORD );
  STDMETHOD( GiveFeedback )( DWORD );

  // -- IDataObject
  //
  STDMETHOD( GetData )( FORMATETC*, STGMEDIUM* );
  STDMETHOD( QueryGetData )( FORMATETC* );
  STDMETHOD( EnumFormatEtc )( DWORD,IEnumFORMATETC** );
  //
  STDMETHOD( GetDataHere )( FORMATETC*, STGMEDIUM* )
    { return E_NOTIMPL; }
  STDMETHOD( GetCanonicalFormatEtc )( FORMATETC*, FORMATETC* )
    { return E_NOTIMPL; }
  STDMETHOD( SetData )( FORMATETC*, STGMEDIUM*, BOOL )
    { return E_NOTIMPL; }
  //
  STDMETHOD( DAdvise )( FORMATETC*, DWORD, IAdviseSink*,PDWORD )
    { return OLE_E_ADVISENOTSUPPORTED; }
  STDMETHOD( DUnadvise )( DWORD )
    { return OLE_E_ADVISENOTSUPPORTED; }
  STDMETHOD( EnumDAdvise )( IEnumSTATDATA** )
    { return OLE_E_ADVISENOTSUPPORTED; }

  // -- IEnumFORMATETC
  //
  STDMETHOD( Next )( ULONG, FORMATETC*, ULONG* );
  STDMETHOD( Reset )();
  //
  STDMETHOD( Skip )( ULONG )
    { return E_NOTIMPL; }
  STDMETHOD( Clone )( IEnumFORMATETC** )
    { return E_NOTIMPL; }


private:
  enum {
    //
    CRIP_FORMAT_MAX = 30,
  };


  // -- その他の関数一覧...
  //
private:
  void  SetFormatetc( FORMATETC* p, CLIPFORMAT cf );


  // -- その他の変数一覧...
  //
private:
  ULONG   mRefCnt;        // 参照カウンタ
  //
  FORMATETC mFmes[ CRIP_FORMAT_MAX ];
  FORMATETC*  mpFmes;
  //
  IUnknown* mpUnk;
  //
  const TCHAR*  mpPath;
  size_t      mSize;

};
// ==========================================================================


#endif//_IDRAGDROP_H_
