#include "tcpSocket.h"
bool TcpSocket::ready()
{
	return mCurrentStat;
}
TcpSocket::TcpSocket(const std::string &host,int port,std::string &buffer):mHostName(host),mPort(port),mCurrentStat(false),mBuffer(buffer)
{
}
bool TcpSocket::connectToHost()
{
	if(!bindByName())
	{
		return false;
	}
	mSocketFd = socket(AF_INET,SOCK_STREAM , 0);
	if(-1 == mSocketFd)
	{
		std::cerr<<"fail to create socketFd\n";
		return false;
	}
	int ret = connect(mSocketFd , reinterpret_cast<sockaddr *>(&mHostAddress),sizeof(mHostAddress));
	if(-1 == ret)
	{
		close(mSocketFd);
		std::cerr<<"fail to connect ToHost\n";
			return false;
	}
	mCurrentStat = true;
	start();
	return true;
}
void TcpSocket::disconnectToHost()
{
	mCurrentStat = false;
	close(mSocketFd);
}
bool TcpSocket::sendToHost(const std::string &str)
{
	if(!ready())
	{
		return false;
	}
	int res = write(mSocketFd , str.c_str(),str.length());
	if(res < 0)
	{
		disconnectToHost();
		return false;
	}
	else if(res >0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void TcpSocket::run()
{
	int res;
	char ch;
	mBuffer.clear();
	while(ready())
	{
		res = read(mSocketFd , &ch , 1);
		if(res <= 0)
		{
			std::cout << "connect has been disconnected\n";
			disconnectToHost();
			break;
		}
		mBuffer += ch;
	}
}
