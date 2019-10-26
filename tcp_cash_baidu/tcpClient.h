#ifdef TCP_CLIENT_H
#define TCP_CLIENT_H
#include"tcpSocket.h"
class TcpClient{
	public:
		TcpClient(std::string url);
		std::string get(int secs);
	private:
		std::string getDomain();
		std::string getUri();
		string mUrl;
};

#endif
