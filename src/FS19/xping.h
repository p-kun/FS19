#ifndef _XPING_H_
#define _XPING_H_

/****************************************************************************
 * Included files
 ****************************************************************************/

//#include <stdio.h>
#include <memory.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iphlpapi.h>
//#include <icmpapi.h>
//#include <locale.h>
#include <tchar.h>

/****************************************************************************
 * Private pre-processor definitions
 ****************************************************************************/

#define ICM_BUF_SIZE  32
#define ICM_TIMEOUT   2000

/****************************************************************************
 * Private types
 ****************************************************************************/

/****************************************************************************
 * Public type declarations
 ****************************************************************************/

/****************************************************************************
 * Public function prototypes
 ****************************************************************************/

/****************************************************************************
 * Public data
 ****************************************************************************/

/****************************************************************************
 * Private data
 ****************************************************************************/

/****************************************************************************
 * Private functions
 ****************************************************************************/

class xping
{
public:
  xping(WORD version)
  {
    m_wsa_result = WSAStartup(version, &m_wsa_data);
  }

  xping()
  {
    m_wsa_result = -1;
  }

  ~xping()
  {
    if (m_wsa_result == 0)
      {
        WSACleanup();
      }
  }

  BOOL send(BYTE ip0, BYTE ip1, BYTE ip2, BYTE ip3)
  {
    HANDLE            handle;
    PICMP_ECHO_REPLY  p;
    DWORD             ret;
  	char              buf[ICM_BUF_SIZE];
  	DWORD             size = sizeof(ICMP_ECHO_REPLY) + sizeof(buf);
    union {
      IPAddr          addr;
      BYTE            u[4];
    }
    ip;

    ip.u[0] = ip0;
    ip.u[1] = ip1;
    ip.u[2] = ip2;
    ip.u[3] = ip3;

    p = (PICMP_ECHO_REPLY)alloca(sizeof(ICMP_ECHO_REPLY) + ICM_BUF_SIZE);

    handle = IcmpCreateFile();

    ret = IcmpSendEcho(handle, ip.addr, buf, ICM_BUF_SIZE, NULL, p, size, ICM_TIMEOUT);

    IcmpCloseHandle(handle);

    return (ret == 0);
  }

private:
  int       m_wsa_result;
  WSADATA   m_wsa_data;

};

#endif /* _XPING_H_ */
