#include <iostream>
#include <library.h>
#include <sstream>
#include "NetManager.h"
using namespace scppsocket;
using namespace std;

void WaitForCommand();
void OnServerMessageRead(int fd, const char* msg, int len);
void OnClientMessageRead(const char* msg, int len);

void OnClientMessageRead(const char *msg, int len)
{
    char* str = new char[len + 1];
    memcpy(str, msg, len);
    str[len] = '\0';
    printf("client read  len:%d\n", len);
    printf("client read  msg:%s\n", str);
    delete[] str;
}

void OnServerMessageRead(int fd, const char *msg, int len)
{
    char* str = new char[len + 1];
    memcpy(str, msg, len);
    str[len] = '\0';
    printf("server read  fd:%d\n", fd);
    printf("server read  len:%d\n", len);
    printf("server read  msg:%s\n", str);
    delete[] str;
}


std::vector<std::string> StringSplit(const std::string& str, char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

int main()
{
    NetManager Mgr;
    const int size = 50;
    char buf[size];
    printf("please input command\n");
    int mode = 0;
    Mgr.SetOnServerMessageRead(OnServerMessageRead);
    Mgr.SetOnClientMessageRead(OnClientMessageRead);
    while (true)
    {
        if (std::cin.getline(buf, size))
        {

            std::string str = buf;
            std::cout << "command is:" << buf << std::endl;

            //std::cout << str << std::endl;
            if (str.compare("startclient") == 0)
            {
                printf("startclient\n");
#ifdef _WIN32

                Mgr.StartTCPClient("192.168.1.104", 1500);
#elif __APPLE__
                Mgr.StartTCPClient("192.168.1.9", 1500);
#elif __linux__
                Mgr.StartTCPClient("10.0.2.15", 1500);
#endif
                mode = 0;
            } else if (str.compare(("startserver")) == 0)
            {
                printf("startserver\n");
                Mgr.StartTCPServer(1500, 6);
                //StartServer(1500,6);
                mode = 1;
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
                Mgr.StopTCPClient();
                Mgr.StopTCPCServer();
                Mgr.Clear();
                break;
            } else
            {
                vector<string> array = StringSplit(str, '|');
                if(array.size() ==  2)
                {

                    std::cout << "send message is:" << array[1] << std::endl;
                    if(mode == 0)
                    {
                        Mgr.TCPClientSendMessage(array[1].c_str(), array[1].length());
                    }
                    else
                    {
                        int intStr = atoi(array[0].c_str());
                        if(intStr <= 0)
                        {
                            Mgr.TCPServerBroadcast(array[1].c_str(), array[1].length());
                        } else
                        {
                            Mgr.TCPServerSendMessage(intStr, array[1].c_str(), array[1].length());
                        }
                    }
                }


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
            Mgr.StartTCPClient("192.168.1.104", 1500);
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
