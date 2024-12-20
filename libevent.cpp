#include "libevent.hpp"

ServerCore::~ServerCore(){

if(base)
event_base_free(base);

}

void ServerCore::start()
{
memset(&sock,0,sizeof(sock));
sock.sin_family=AF_INET;
sock.sin_port=htons(port);
sock.sin_addr.s_addr=htonl(INADDR_ANY);
base=event_base_new();
listen=evconnlistener_new_bind(base,connection,this,LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,-1,(sockaddr *)&sock,sizeof(sock));
event_base_dispatch(base);
}


void ServerCore::connection(evconnlistener *listen_t,evutil_socket_t fd,sockaddr* addr,int num,void *p){
cout<<"some body in!"<<endl;
auto server=static_cast<ServerCore *>(p);
server->levent=bufferevent_socket_new(server->base,fd,BEV_OPT_CLOSE_ON_FREE);
bufferevent_setcb(server->levent,reading,NULL,NULL,server);
bufferevent_enable(server->levent,EV_READ|EV_WRITE);


}



bool ServerCore::readByte(struct evbuffer* input,char *buf,size_t lnt){
size_t available=evbuffer_get_length(input);
	if(available<lnt)
	return false;
evbuffer_remove(input,buf,lnt);
return true;
}

void ServerCore::reading(bufferevent * in,void * p){
	unsigned char sig[2];
	char buff[1024]={0};
	auto server=static_cast<ServerCore *>(p);
	evbuffer *ev=bufferevent_get_input(in);
	if(evbuffer_get_length(ev)<=7)
	return;
	if(!server->readByte(ev,reinterpret_cast<char *>(sig),2))
	{
		cout<<"长度不够."<<endl;
		return;
	}
	if(!(sig[0]==0xaa&&sig[1]==0xf1))
	{
		cout<<"信号没对上."<<endl;
		return;
	}
	uint32_t netLength;
	if(!server->readByte(ev,reinterpret_cast<char *>(&netLength),4))
	{
		cout<<"长度不够"<<endl;
		return;
	}
	netLength=ntohl(netLength);
	cout<<"字节长度:"<<netLength<<endl;
	uint8_t t;
	if(!server->readByte(ev,reinterpret_cast<char*>(&t),1))
	{
		cout<<"长度不够"<<endl;
		return;
	}
	t=ntohl(t);
	cout<<"类型是"<<t<<"\n头部解析完毕"<<endl;
	if(evbuffer_get_length(ev)<netLength)
	{
		cout<<"消息长度不足支撑"<<endl;
		return;
	}
	vector<char> message(netLength);
	if(evbuffer_get_length(ev)<netLength)
	{
		cout<<"数据长度不够"<<endl;
		return;
	}
	if(!server->readByte(ev,message.data(),netLength))
	{
		cout<<"数据长度不够"<<endl;
		return;
	}
	string sMessage(message.begin(),message.end());
	cout<<sMessage<<endl;



//int value=bufferevent_read(in,buff,sizeof(buff));
//string one(buff,value);
//cout<<one<<endl;

}
