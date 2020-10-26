/*
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 * Copyright(C) 2004 ****, Inc.
 *
 *  File Name: savedir.h
 *  Created  : 11/03/03(水) 09:46:44
 *
 *  Function
 *    パス名を保持する
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 *  $Revision: 1.1.1.1 $
 *  $Date: 2008/07/27 10:32:48 $
 *  $Author: poko $
 * --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --
 */
#ifndef _SAVEDIR_H_
#define _SAVEDIR_H_

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/

typedef enum
{
  SAVEDIR_INVALID = -1,
  SAVEDIR_SRCCOPY,
  SAVEDIR_SRCAND,
  SAVEDIR_SRCOR,
}
SAVEDIR_PATTERN;

/****************************************************************************
 * Public type declarations
 ****************************************************************************/

typedef struct DIR_NODE_T
{
  struct DIR_NODE_T*  next;
  struct DIR_NODE_T*  subd;
  struct DIR_NODE_T*  parent;
  DWORD               d_no;
  int                 exist;
  unsigned int        htime;
  unsigned int        ltime;
  HANDLE              hHandle;
  HANDLE              hHeap;
  DWORD               usr_data;
  TCHAR               d_name[1];
}
D_NODE;

/****************************************************************************
 * Public function prototypes
 ****************************************************************************/

void    delete_savedir(void);
D_NODE* node_search(D_NODE *p_parent, TCHAR *elem);
D_NODE* savedir(const TCHAR *path);
D_NODE* savedir(const TCHAR *path, DWORD user_data, SAVEDIR_PATTERN pat);
D_NODE *savedir(D_NODE         *p_node,
                const TCHAR    *path,
                DWORD           usr_data = 0,
                SAVEDIR_PATTERN pat      = SAVEDIR_INVALID);
D_NODE* savedir(void);

#endif // _SAVEDIR_H_
