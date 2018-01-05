/*
*   ��������ĳ�ʼ��, ����״̬����, �Լ�������ѯ
*	������������
*
*/
#ifndef __GAMENETWORKNODE_H__
#define __GAMENETWORKNODE_H__

#include <functional>
#include "cocos2d.h"
#include "KxServer.h"

namespace KxCocos {

struct ServerConn
{
	bool						Ipv6;				// �Ƿ�IPV6
    std::string                 ConnIP;				// ���ӵ�ip��ַ
    int                         Port;               // ���ӵĶ˿�
    int                         ConnectingTimes;    // ���������еĴ���������10��ǿ�ƹر�������
    KxServer::KxTCPConnector*   Connector;          // ����
    std::function<void(bool)>   ConnectCallback;

    ServerConn() :Port(0)
        , ConnectingTimes(0)
        , Connector(nullptr)
        , ConnectCallback(nullptr)
    {
    }
};

class CGameNetworkNode : public cocos2d::Node
{
private:
    CGameNetworkNode(void);
    ~CGameNetworkNode(void);

public:
    static CGameNetworkNode *getInstance();
    static void destroy();

	// �ڵ�ص�
    bool init();
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags);

	// ���ӹ���
	bool connect(const char* ip, int port, int id = 0, bool ipv6 = false, std::function<void(bool)> callBack = nullptr);
	bool reconnect(int id, std::function<void(bool)> callBack = nullptr);
	void close(int id);
	void closeAll();

	int sendData(char* buffer, unsigned int len, int id = 0);
	KxServer::KxTCPConnector* getConnector(int id);
	ServerConn* getServerConn(int id);

	// Module����
	void setDefaultModule(KxServer::IKxModule* module);
	void setModuleById(int id, KxServer::IKxModule* module);

private:
	KxServer::IKxModule* getKxModule(int id);

private:
    KxServer::IKxCommPoller*            m_pPoller;                      // ��ѯ��
	KxServer::IKxModule*				m_pDefaultModule;				// Ĭ�ϴ���ģ��
	std::map<int, KxServer::IKxModule*> m_mapKxModule;					// ����ģ��
    std::map<int, ServerConn>           m_mapServerConns;               // ����������
	static CGameNetworkNode*            m_pInstance;
};
}

#endif 
