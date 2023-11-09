#include "ServerOp.h"
ServerOp::ServerOp(string json)
{
	//读取配置文件
	Value root;
	Reader rd;
	ifstream ifs(json);
	rd.parse(ifs, root);
	m_port = root["port"].asInt();
	m_ServerID = root["ServerID"].asString();
}
ServerOp::~ServerOp()
{

}
string ServerOp::seckeyAgree(RequestMsg* reqMsg)
{
	//把公钥写入到磁盘
	ofstream ofs("public.pem");
	ofs << reqMsg->data();
	ofs.close();
	//创建非对称加密的对象
	RespondInfo info;
	RsaCrypto rsa("public.pem",false);
	//创建哈希对象
	Hash sha(T_SHA1);
	sha.addData(reqMsg->data());
	//签名
	bool b=rsa.rsaVerify(sha.result(), reqMsg->sign());
	if (!b)
	{
		cout << "签名失败！" << endl;
		info.status = false;
	}
	else
	{
		cout << "签名成功" << endl;
		//生成随机字符串，对称加密
		string key=getRandKey(Len16);
		cout << "生成的密钥是:" << key << endl;
		//公钥加密
		cout << "aaaaaaaaaaaa" << endl;
		string seckey=rsa.rsaPubKeyEncrypt(key);
		cout << "加密后的密钥是" <<seckey<< endl;
		//初始化发送数据
		info.clientID = reqMsg->clientid();
		info.seckeyID = 12;//数据库操作
		info.data = seckey;
		info.status = true;
		info.serverID = m_ServerID;
	}
	//序列化
	CodecFactory* factory = new RespondFactory(&info);
	Codec* c = factory->createCodec();
	string senddata=c->encodeMsg();
	delete factory;
	delete c;
	return senddata;
}
string ServerOp::getRandKey(ServerOp::KeyLen len)
{
	int flag = 0;
	string randStr = string();
	srand((unsigned int)time(nullptr));
	char* cs = "~!@#$%^&*()_+}{|\';[]";
	for (int i = 0; i < len; ++i)
	{
		flag = rand() % 4;	// 4中字符类型
		switch (flag)
		{
		case 0:	// a-z
			randStr.append(1, 'a' + rand() % 26);
			break;
		case 1: // A-Z
			randStr.append(1, 'A' + rand() % 26);
			break;
		case 2: // 0-9
			randStr.append(1, '0' + rand() % 10);
			break;
		case 3: // 特殊字符
			randStr.append(1, cs[rand() % strlen(cs)]);
			break;
		default:
			break;
		}
	}
	return randStr;
}
 void* ServerOp::working(void* arg)
{
	 sleep(1);
	 ServerOp* op = (ServerOp*)arg;
	 //获取通信的Tcpsocket
	 TcpSocket* tcp = op->m_list[pthread_self()];
	 string data;
	 string msg=tcp->recvMsg();
	 //反序列化
	 CodecFactory* factory = new RequestFactory(msg);
	 Codec* c = factory->createCodec();
	 RequestMsg *req=(RequestMsg*)c->decodeMsg();
	 //获取操作类型
	 //1.密钥协商
	 //2.密钥校验
	 //3.密钥注销
	 //4.断开连接
	 //cout << "req->data" << req->data();
	 int cmd = req->cmdtype();
	 switch (cmd)
	 {
		case 1:
			data= op->seckeyAgree(req);
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
	 }
	 //发送数据
	 tcp->sendMsg(data);
	 //释放数据
	 delete factory;
	 delete c;
	 //断开连接
	 tcp->disConnect();
	 op->m_list.erase(pthread_self());
	 delete tcp;
	 return nullptr;
}

void ServerOp::startServer()//启动服务器
{
	//1.监听客户端
	m_server = new TcpServer();
	m_server->setListen(m_port);
	while (1)
	{
		cout << "等待客户端连接" << endl;
		TcpSocket* cli= m_server->acceptConn();
		if (cli == nullptr)
		{
			continue;
		}
		//与客户端连接成功
		cout << "连接成功" << endl;
		//创建线程工作
		pthread_t tid;
		pthread_create(&tid, nullptr, working, this);
		m_list.insert(make_pair(tid, cli));
	}
}