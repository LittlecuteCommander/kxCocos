#ifndef __NETWORK_MODULE_H__
#define __NETWORK_MODULE_H__

#include "BaseModule.h"
#include "BufferData.h"

class CNetworkModule : public CBaseModule
{
public:
	CNetworkModule(void);
	~CNetworkModule(void);

    // �����յ�������
    virtual void processLogic(char* buffer, unsigned int len, KxServer::IKxComm *target);
    // ������ֵĴ���
    virtual void processError(KxServer::IKxComm *target);
    // �����¼�
    virtual void processEvent(int eventId, KxServer::IKxComm* target);

private:
	CBufferData *m_pBuffData;
};

#endif 
