//
// Created by harryqk on 7/17/22.
//

#include "NetManager.h"
#include <iostream>
#include "Mac/SCPPSocketFactoryMac.h"
#include "Win/SCPPSocketFactoryWin.h"
#include "SCPPSocket.h"
#include "TCPConnection.h"
#include "SocketUtil.h"
namespace scppsocket
{
    NetManager::NetManager()
    {
        std::cout << "Construct NetManager" << std::endl;
    }

    NetManager::~NetManager()
    {
        std::cout << "Destruct NetManager" << std::endl;
    }

    void NetManager::Init()
    {
        #ifdef _WIN32
                // Initialize Winsock
                WSADATA wsaData;
                int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
                if (iResult != NO_ERROR) {
                    wprintf(L"WSAStartup function failed with error: %d\n", iResult);
                    return;
                }
        #elif __APPLE__

        #elif __ANDROID__
        #endif
    }

    void NetManager::Cleanup()
    {
        #ifdef _WIN32
                WSACleanup();
        #elif __APPLE__

        #elif __ANDROID__
        #endif
    }


    void NetManager::StartTCPClient(const char* Address, int Port)
    {
        SCPPSocketFactory* Factory;
        Init();
#ifdef _WIN32
        Factory = new SCPPSocketFactoryWin();
#elif __APPLE__
        Factory = new SCPPSocketFactoryMac();
#elif __ANDROID__
#endif
        Local = Factory->CreateSocket(SocketAddressFamily::IPv4, SocketType::SOCKTYPE_Streaming, SocketProtocol::TCP);

        if(Local->GetFileDescriptor() == SOCKET_ERROR)
        {
            std::printf("CreateSocket Error\n");
            Cleanup();
            return;
        }
        //connect
        sockaddr_in ServerAddress = Factory->CreateAddress(Address, Port);
        Local->SetPeerAddress(ServerAddress);
        //set NonBlockMode
        int ret = Local->SetNonBlockMode(true);
        if(ret != SocketBlockMode::NonBlock)
        {
            std::printf("Set NonBlock mode fail\n");
            Cleanup();
            return;
        }

        ClientWorker = Factory->CreateTCPClientNetMangerWorker(Local);
        ClientWorker->SetIsWorking(true);
        std::thread thd([&]()
                        {
                            ClientWorker->DoWork();
                        });
        thd.detach();
        delete Factory;
        Factory = nullptr;
    }

    void NetManager::StopTCPCServer()
    {
        if(ServerWorker != nullptr)
        {
            ServerWorker->SetIsWorking(false);
            ServerWorker->StopWork();
        }
    }

    void NetManager::StopTCPClient()
    {
        if(ClientWorker != nullptr)
        {
            ClientWorker->SetIsWorking(false);
            ClientWorker->StopWork();
        }
    }



    void NetManager::TCPClientSendMessage(const char *Msg, int Len)
    {
        byte* bytes = new byte[4];
        SocketUtil::IntToByte(Len, bytes);
        char* sendBuff = new char[Len + 4];//4 is byte length of data
        memcpy(sendBuff, bytes, 4);
        memcpy(sendBuff + 4, Msg, Len);
        ClientWorker->SendMessage(sendBuff, Len + 4);
    }

    void NetManager::StartTCPClientSubThread(const char* Address, int Port)
    {

    }

    bool NetManager::StartTCPServer(int Port, int MaxConnection)
    {
        SCPPSocketFactory* Factory;
        Init();
#ifdef _WIN32
        Factory = new SCPPSocketFactoryWin();
#elif __APPLE__
        Factory = new SCPPSocketFactoryMac();
#elif __ANDROID__
#endif
        Local = Factory->CreateSocket(SocketAddressFamily::IPv4, SocketType::SOCKTYPE_Streaming, SocketProtocol::TCP);

        //set NonBlockMode
        bool ret = Local->SetNonBlockMode(true);
        if(!ret)
        {
            std::printf("Set NonBlock mode fail");
            Cleanup();
            return false;
        }
        //bind
        int result;
        result = Local->Bind(Port);
        if(result == SOCKET_ERROR)
        {
            std::printf("Bind fail");
            Cleanup();
            return false;
        }
        //listen;
        result = Local->Listen(MaxConnection);
        if(result == SOCKET_ERROR)
        {
            std::printf("Listen fail");
            Cleanup();
            return false;
        }
        ServerWorker = Factory->CreateTCPServerNetMangerWorker(Local);
        ServerWorker->SetIsWorking(true);
        std::thread thd([&]()
                        {
                            ServerWorker->DoWork();
                        });

        thd.detach();
        delete Factory;
        Factory = nullptr;
        return true;
    }

    void NetManager::Clear()
    {
        if(Local != nullptr)
        {
            delete Local;
            Local= nullptr;
        }
        if(ClientWorker != nullptr)
        {
            delete ClientWorker;
            ClientWorker = nullptr;
        }
        if(ServerWorker != nullptr)
        {
            delete ServerWorker;
            ServerWorker = nullptr;
        }
        Cleanup();
    }
}


