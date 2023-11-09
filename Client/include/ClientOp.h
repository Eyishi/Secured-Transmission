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
	string ServerID;//服务器id
	string ClientID;//客户端id
	string ServerIP;//服务器ip
	unsigned short port;//端口
};
class ClientOp
{
public:
	ClientOp();
	~ClientOp();
	int usage();//选择的主界面
	bool seckeyAgree();//密钥协商
	bool seckeyCheck();//密钥校验
	bool seckeyLog_off();//密钥注销
private:
	m_info Myinfo;
};

