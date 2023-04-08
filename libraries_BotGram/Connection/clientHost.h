#include <iostream>
#include <ws2tcpip.h>
#include "packet.h"
#pragma comment  (lib,"ws2_32.lib")//important
using namespace std;
typedef void (*Connection_Hadler)(bool flag);
typedef void (*Recive_Hadler)(char* buffer, int dataLen);


class clientHost
{
public:

	bool IsRecv = true;
    bool IsRuning=false;
	clientHost();
	clientHost(string IP,int port,Connection_Hadler connHandler,Recive_Hadler RecvHandler);
	~clientHost();
	bool init();
	void start();
	SOCKET mysocket;
	bool ISOK = false;
private:
	
	int port;
	string serverIP;
	Connection_Hadler ConnectionHandler;
	Recive_Hadler recvHandler;

	SOCKET CreateSocket();
	void Run();
	void CleanUp();


};


