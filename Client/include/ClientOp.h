#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "json/json.h"
#include "CodecFactory.h"
#include "RequestCodec.h"
#include "RequestFactory.h"
#include "RespondFactory.h"
#include "RsaCrypto.h"
#include "Hash.h"
#include "TcpSocket.h"
using namespace std;
using namespace Json;

struct m_info
{
	string ServerID;//������id
	string ClientID;//�ͻ���id
	string ServerIP;//������ip
	unsigned short port;//�˿�
};
class ClientOp
{
public:
	ClientOp();
	~ClientOp();
	int usage();//ѡ���������
	bool seckeyAgree();//��ԿЭ��
	bool seckeyCheck();//��ԿУ��
	bool seckeyLog_off();//��Կע��
private:
	m_info Myinfo;
};

