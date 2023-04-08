#include "clientHost.h"
#define SERVER_PORT 6969
#define _WIN32_WINNT
clientHost::clientHost()
{

}
clientHost::clientHost(string IP, int port,Connection_Hadler ConnHandler,Recive_Hadler RecvHandler) 
    :serverIP(IP), port(port), ConnectionHandler(ConnHandler), recvHandler(RecvHandler)
{

}
clientHost::~clientHost()
{
    CleanUp();
}
bool clientHost::init()
{
    WSADATA WinAllow;
    WORD Ver = MAKEWORD(2, 2);
    int res=WSAStartup(Ver, &WinAllow);
    return res == 0;
}
void clientHost::start()
{
    mysocket = CreateSocket();
    if (mysocket != INVALID_SOCKET)
    {
        ISOK = true;
        IsRuning=true;
        ConnectionHandler(true);
        while (true)
        {
            Run();
            Sleep(10);
        }
    }
}
void clientHost::Run()
{
    char Buffer[SIZE_BUFFER_RECV];
    memset(Buffer, 0, SIZE_BUFFER_RECV);
    int ByteRes = recv(mysocket, Buffer, SIZE_BUFFER_RECV, 0);
    if (ByteRes <= 0)
    {
        ConnectionHandler(false);

    }
    else if (ByteRes >= 1)
    {
        recvHandler(Buffer, ByteRes);
    }
}
SOCKET clientHost::CreateSocket()
{
    SOCKET TempSock = socket(AF_INET, SOCK_STREAM, 0);
    if (TempSock != INVALID_SOCKET)
    {
        sockaddr_in myAddr;
        myAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        myAddr.sin_family = AF_INET;
        myAddr.sin_port = htons(6969);

        //myAddr.sin_addr.S_un.S_addr = inet_addr(serverIP.c_str());
        // int res = inet_pton(AF_INET, serverIP.c_str(), &myAddr.sin_addr);

            int res = connect(TempSock, (sockaddr*)&myAddr, sizeof(myAddr));
            if (res != SOCKET_ERROR)
            {
                return TempSock;
            }
            else
            {
                closesocket(TempSock);
                CleanUp();
                return -1;

            }

    }
    else
    {
        closesocket(TempSock);
        CleanUp();
        return -1;
    }
}
void clientHost::CleanUp()
{
    closesocket(mysocket);
    WSACleanup();
}
