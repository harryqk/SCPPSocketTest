#include <iostream>
#include <library.h>
#include "NetManager.h"
using namespace scppsocket;

void WaitForCommand();
int main()
{
    NetManager Mgr;
    char buf[10];
    printf("please input command\n");
    int mode = 0;
    while (true)
    {
        if (std::cin.getline(buf, 20))
        {
            std:
            std::string str = buf;
            std::cout << "command is:" << buf << std::endl;
            //std::cout << str << std::endl;
            if (str.compare("startclient") == 0)
            {
                printf("startclient\n");
                //StartClient("192.168.1.9", 1500);
                Mgr.StartTCPClient("192.168.1.9", 1500);
                mode = 1;
            } else if (str.compare(("startserver")) == 0)
            {
                printf("startserver\n");
                Mgr.StartTCPServer(1500, 6);
                //StartServer(1500,6);
                mode = 2;
            } else if (str.compare("stopclient") == 0)
            {
                printf("stopclient\n");
                Mgr.StopTCPClient();
                //StopClient();
            } else if (str.compare("stopserver") == 0)
            {
                printf("stopserver\n");
                Mgr.StopTCPCServer();
                //StopServer();
            } else if (str.compare("quit") == 0)
            {
                printf("quit\n");
                break;
            } else
            {
                std::cout << "send message is:" << buf << std::endl;
                Mgr.TCPClientSendMessage(buf, strlen(buf));

            }
        }
    }
    printf("main exit la la la \n");
    return 0;
}

void WaitForCommand()
{
    NetManager Mgr;
    char buf[10];
    printf("please input command\n");
    int mode = 0;
    while (std::cin.getline(buf, 20))
    {
        std:std::string  str = buf;
        //std::cout << "command is:" << buf << std::endl;
        //std::cout << str << std::endl;
        if(str.compare("startclient") == 0)
        {
            printf("startclient\n");
            //StartClient("192.168.1.9", 1500);
            Mgr.StartTCPClient("192.168.1.9", 1500);
            mode = 1;
        } else if(str.compare(("startserver")) == 0)
        {
            printf("startserver\n");
            Mgr.StartTCPServer(1500, 6);
            //StartServer(1500,6);
            mode = 2;
        }
        else if(str.compare("stopclient") == 0)
        {
            printf("stopclient\n");
            Mgr.StopTCPClient();
            //StopClient();
        }
        else if(str.compare("stopserver") == 0)
        {
            printf("stopserver\n");
            Mgr.StopTCPCServer();
            //StopServer();
        }
        else if(str.compare("quit") == 0)
        {
            printf("quit\n");
            //Mgr.ReleaseServer();
            //Mgr.ReleaseClient();
//            if(mode == 1)
//            {
//                Mgr.StopTCPClient();
//            }
//            else if(mode == 2)
//            {
//                Mgr.StopTCPCServer();
//            }

            break;
        }
        else
        {
            std::cout << "send message is:" << buf << std::endl;
            Mgr.TCPClientSendMessage(buf, strlen(buf));

        }
    }
}
