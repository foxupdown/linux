#include <iostream>
#include "tcpClient.h"
using namespace std;
int main()
{
	cout << TcpClient("www.baidu.com").get(5) << endl;
	return 0;
}
