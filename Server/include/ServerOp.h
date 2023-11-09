#pragma once
#include <json/json.h>
#include <string>
#include <fstream>
#include <map>
#include <unistd.h>
#include <pthread.h>
#include "TcpSocket.h"
#include "TcpServer.h"
#include "CodecFactory.h"
#include "RequestFactory.h"
#include "RespondFactory.h"
#include "Codec.h"
#include "RequestCodec.h"
#include "RespondCodec.h"
#include "RsaCrypto.h"
#include "Hash.h"
using namespace std;
using namespace Json;
class ServerOp
{
public:
	enum KeyLen { Len16 = 16, Len24 = 24, Len32 = 32 };
	ServerOp(string json);
	~ServerOp();

	void startServer();//����������
	static void* working(void *arg);//�̹߳���

	string seckeyAgree(RequestMsg* reqMsg);//��ԿЭ��
	 string getRandKey(KeyLen len);
private:
	unsigned short m_port;
	string m_ServerID;
	map<pthread_t, TcpSocket*> m_list;
	TcpServer* m_server = nullptr;
};

