#ifndef _IICON_H_
#define _IICON_H_

void GetTempDirectoryIconIndex( HANDLE *handle, DWORD *index );
void GetTempFileIconIndex( HANDLE *handle, DWORD *index );

#define MARK_CHECK	52
enum {
  MARK_GIT = 2,
  MARK_CVS,
  MARK_SVN,
  MARK_MODIFY,
  MARK_ADD,
  MARK_MARGE,
  MARK_CONF,
};

/* クラスにしてみる */

enum {
  IMG_INIT = -1,
  IMG_COMMON,
  IMG_COPYING,
  IMG_LOADING,
  IMG_MARK,
  IMG_STATUS,
  IMG_DIRVIEW,
  IMG_SCROLLVIEW,
  IMG_INFOVIEW,
  IMG_TREEVIEW,
  IMG_COPYDLG,
  IMG_MAX_NUM,
};

class iico
{
public:
  void draw(int x, int y, int idx = 0, BOOL isActive = TRUE);
  void draw(int x, int y, dirent *entry, BOOL isActive, BOOL isRemote = FALSE);
  void attach(HDC hDC = NULL, int idx = IMG_COMMON);

public:
  HANDLE get_instance(dirent *entry, DWORD *d_no, BOOL isRemotea);
  HANDLE get_instance(dirent *entry, DWORD *d_no);
  HANDLE get_remote_instance(dirent *entry, DWORD *d_no);

public:
  iico(int = IMG_COMMON);
  iico(HDC hDC, int idx = IMG_COMMON);
  ~iico();

public:
  operator void* () const;

private:
  void      initialization(void);
  void      destroy(void);

private:
  HDC       mhDC;
  int       mIndex;
  COLORREF  rgbBk;
  COLORREF  rgbFg;
  UINT      fStyle;
};

#endif
