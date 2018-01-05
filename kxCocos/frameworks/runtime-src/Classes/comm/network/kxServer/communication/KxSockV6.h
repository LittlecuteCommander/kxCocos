/*
 * ����SocketͨѶ����
 * �����˷����������ӳ٣����ͺͽ��սӿ�
 * ����Socket�����쳣
 *
 *  2013-04-12 By ��ү
 *  
 */
#ifndef __KX_SOCKETV6__
#define __KX_SOCKETV6__

#include "KxSock.h"

namespace KxServer {

class KxSockV6 : public KxSock
{
public:
	// ��ʼ��Socket
	bool init(KXSOCK_TYPE type);
	// ����ָ����IP�Ͷ˿�
	int connect(const char* addr, int port);
	// �󶨵�ָ����Ip�Ͷ˿�
	int bind(const char* addr, int port);
	// Accept����һ��Socket���Ӷ���
	KXCOMMID accept();
};

}

#endif
