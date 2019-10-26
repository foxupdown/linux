#include "tcpClient.h"
using namespace std;
TcpClient :: TcpClient(std::string url):mUrl(url){ 
}
std::string TcpClient::get(int secs)
{
	string recvBuffer;
	return recvBuffer;
}
std::string TcpClient::getDomain()
{
	int pos = mUrl.find('/');
	if(-1 == pos)
	{
		return mUrl;
	}
	else 
		return std::string(mUrl.begin(),mUrl.begin()+pos);
}
std::string TcpClient::getUri()
{
	int pos = mUrl.find('/');
	if(-1 == pos)
	{
		return ('/');
	}
	else
	{
		std::string ret(mUrl.begin()+pos , mUrl.end());
		ret.erase(0 , ret.find_first_not_of(" "));
		ret.erase(ret.find_last_not_of(" ")+1);
		if(ret.empty())
		{
			return('/');
		}
		else
		{
		return ret;	
		}
	}
}
