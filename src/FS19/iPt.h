/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: iPt.h
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
#ifndef _IPT_H_
#define _IPT_H_


enum PT_VKS_T {
	PT_VK_NON			= 0,
	PT_VK_SHIFT,
	PT_VK_CTRL,
	PT_VK_ALT,
};
//
enum PT_FUNCS_T {
	PT_FUNC_NON			= 0,
	PT_FUNC_EDITOR,
	PT_FUNC_LOGDISK,
	PT_FUNC_REFLESH,
	PT_FUNC_EXEC,
	PT_FUNC_BACK,
	PT_FUNC_STEP,
	PT_FUNC_EJECT,
	PT_FUNC_COPY,
	PT_FUNC_PAST,
	PT_FUNC_RENAME,
	PT_FUNC_OPTION,
	PT_FUNC_SELEXE,
	PT_FUNC_DELETE,
	PT_FUNC_INSERT,
	PT_FUNC_APPS,
	PT_FUNC_PROPERTIES,
	PT_FUNC_ATTRIBUTE,
	PT_FUNC_SHORTCUT1,
	PT_FUNC_SHORTCUT2,
	PT_FUNC_SHORTCUT3,
	PT_FUNC_SHORTCUT4,
	PT_FUNC_SETCOPYVIEW,
	PT_FUNC_FIND,
	PT_FUNC_SORT,
	PT_FUNC_MAX,
};
//
enum PT_COL_MODE_T {
	PT_COL_MODE1,
	PT_COL_MODE2,
	PT_COL_MODE3,
};
//
#define PT_MAX_FUNCS	(30)
#define TOOLBER_SIZE	(200)
#define LIBRALY_NUM		(32)
#define FOLDER_NUM    (8)


// ==========================================================================
// -- 
// --------------------------------------------------------------------------
class iPt
{
public:
	iPt();
	virtual ~iPt();

public:
	BOOL	  Read();
	BOOL	  Write();
	void	  SetPlacement( HWND hWnd );
	int		  GetFuncNo( int vk, int ctrl, int shift, int alt );
	TCHAR*	GetEditorPath( void );

public:
	static int		  x;
	static int		  y;
	static int		  cx;
	static int		  cy;
	static int		  show;
	static TCHAR	  path1[ MAX_PATH ];
	static TCHAR	  path2[ MAX_PATH ];
	static int		  fontsize1;
	static TCHAR	  fontname1[ MAX_PATH ];
	static int		  fontsize2;
	static TCHAR	  fontname2[ MAX_PATH ];
	static int		  fontsize3;
	static TCHAR	  fontname3[ MAX_PATH ];
	static int		  fontsize4;
	static TCHAR	  fontname4[ MAX_PATH ];
	static int		  column;
	static TCHAR	  editor[ MAX_PATH ];
	static TCHAR	  wclist[ MAX_PATH ];
	static int		  page_no;
	static int		  OK;
	static TCHAR	  shortcut3[ MAX_PATH ];
	static int		  tool_size;
	static int		  libraly_num;
	static TCHAR	  libralyname[ LIBRALY_NUM ][ MAX_PATH ];
	static TCHAR	  libralypath[ LIBRALY_NUM ][ MAX_PATH ];
	static int		  line_pitch;
	static int		  COL_MODE;
	static COLORREF	COL_BASE_BAR;
	static COLORREF	COL_SCROLL_BAR;
	static COLORREF	COL_SCROLL_BAR_SEL;
	static COLORREF	COL_BASE_BAR_LOW;
	static COLORREF	COL_SPLIT_LINE;
	static COLORREF	COL_SPLIT_LINE2;
	static COLORREF	COL_SPLIT_LINE_TOP;
	static COLORREF	COL_SPLIT_LINE_BTM;
	static COLORREF	COL_SPLIT_LINE3	;
	static COLORREF	COL_SELECTED;
	static COLORREF	COL_SELECTED_WAKU;
	static COLORREF	COL_SELECTED_CUR;
	static COLORREF	COL_PROGRESS_BAR;
	static COLORREF	COL_REMAIN_BAR_BLUE;
	static COLORREF	COL_REMAIN_BAR_RED;
	static COLORREF	COL_TITLE;
	static COLORREF	COL_FILE_ATTR_SYSTEM;
	static COLORREF	COL_FILE_ATTR_HIDDEN;
	static COLORREF	COL_FILE_ATTR_READONLY;
	static COLORREF	COL_FILE_ATTR_DIRECTORY;
	static COLORREF	COL_FILE_ATTR_NORMAL;
  static int      search_folder_num;
  static TCHAR    search_folders[ FOLDER_NUM ][ MAX_PATH ];

	typedef struct _KEYS_T {
		//
		int		vk;
		int		ctl;
		int		sft;
		int		alt;
		int		f_no;
	}
	KEYS;

	static KEYS	keys[ PT_MAX_FUNCS ];

private:
	void AnlFunc( const TCHAR *kind, const TCHAR *param );

};
// --------------------------------------------------------------------------

#endif//_IPT_H_
