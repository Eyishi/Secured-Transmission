#include "ClientOp.h"
ClientOp::ClientOp()
{
	//1.读取json配置文件
	Value root;
	Reader rd;
	ifstream ifs("config.json");
	rd.parse(ifs, root);
	Myinfo.ServerID = root["ServerID"].asString();
	Myinfo.ClientID = root["ClientID"].asString();
	Myinfo.ServerIP = root["ServerIp"].asString();
	Myinfo.port = root["port"].asInt();
	cout << "ip" << Myinfo.ServerIP << endl;
	cout << "id" << Myinfo.ServerID << endl;
}
ClientOp::~ClientOp()
{

}
int ClientOp::usage()
{
	cout << "\t\t\t1.密钥协商\t\t\t" << endl;
	cout << "\t\t\t2.密钥校验\t\t\t" << endl;
	cout << "\t\t\t3.密钥注销\t\t\t" << endl;
	cout << "\t\t\t4.退出	 \t\t\t" << endl;
	int sel = -1;
	cin >> sel;
	while (getchar()!='\n');
	switch (sel)
	{
		case 1:
			seckeyAgree();//密钥协商
			break;
		case 2:
			seckeyCheck();//密钥校验
			break;
		case 3:
			seckeyLog_off();//密钥注销
			break;
		case 4:
			return 0;
		default:
			break;
	}
	return 1;
}
bool ClientOp::seckeyAgree()//密钥协商
{
	//1.组织数据 
	//生成密钥对
	RsaCrypto rsa;
	rsa.generateRsakey(1024);
	ifstream ifs("public.pem");
	stringstream str;
	str << ifs.rdbuf();
	//生成数据
	RequestInfo request;
	request.clientID = Myinfo.ClientID;
	request.serverID = Myinfo.ServerID;
	request.cmd = 1;
	request.data = str.str();
	//哈希运算
	Hash a(T_SHA1);
	a.addData(str.str());
	request.sign=rsa.rsaSign(a.result());
	cout << "签名完成" << endl;
	//创建工厂类
	CodecFactory* factory = new RequestFactory(&request);
	Codec* c = factory->createCodec();
	string encstr=c->encodeMsg();
	//释放数据
	delete factory;
	delete c;
	//套接字通信
	TcpSocket *tcp=new TcpSocket;

	int ret=tcp->connectToHost(Myinfo.ServerIP, Myinfo.port);
	if (ret != 0)
	{
		cout << "连接服务器失败" << endl;
		return false;
	}
	
		cout << "连接服务器成功" << endl;
		cout << Myinfo.ServerIP << "," << Myinfo.port << endl;
		//发送数据
		tcp->sendMsg(encstr);
		//等待服务器回复
		string recmsg = tcp->recvMsg();
		//进行反序列化
		factory = new RespondFactory(recmsg);
		c = factory->createCodec();
		RespondMsg* resData = (RespondMsg*)c->decodeMsg();
		if (!resData->status())
		{
			cout << "密钥协商失败" << endl;
			return false;
		}
	
	string key=rsa.rsaPriKeyDecrypt(resData->data());
	cout << "对称加密密钥是" << key << endl;
	tcp->disConnect();
	delete factory;
	delete c;
	delete tcp;
}
bool ClientOp::seckeyCheck()//密钥校验
{

}
bool ClientOp::seckeyLog_off()//密钥注销
{

}