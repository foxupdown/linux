#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
#include "myThread.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class TcpSocket:public myThread
{
public:
	TcpSocket(const std::string &host,int port,std::string &buffer);
	~TcpSocket();
	bool ready();
	bool connectToHost();
	void disconnectToHost();
	bool sendToHost(const std::string &str);
private:
	bool bindByName();
	void run();
	std::string mHostName;
	short mPort;
	sockaddr_in mHostAddress;
	int mSocketFd;
	bool mCurrentStat;
	std::string &mBuffer;
};
#endif
