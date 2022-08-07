//
// Created by WIN10 on 2022/8/1.
//

#include "TCPServerNetManagerWorkerWin.h"
#include "../TCPConnection.h"
namespace scppsocket
{
    void TCPServerNetManagerWorkerWin::DoWork()
    {
        int server_sockfd;
        int result;
        server_sockfd = Local->GetFileDescriptor();
        FD_ZERO(&readfds);
        //Add server fd to set
        FD_SET(server_sockfd, &readfds);
        while(IsWorking)
        {
            //Copy readfds to testfds, select will modify testfds
            testfds = readfds;
            std::printf("server waiting\n");

            //Block until certain fd readable or error return, FD_SETSIZE is max system fd value
            result = select(FD_SETSIZE, &testfds, (fd_set *) nullptr,(fd_set *)nullptr, (struct timeval *) nullptr);
            if(result < 1)
            {
                printf("Win server select return errno=%d\n", errno);
                perror("Win server select return error");
                Local->Close();
                break;
            }

            if(FD_ISSET(server_sockfd, &testfds))
            {
                HandleAccept();
            }
            else
            {
                HandleRead();
            }
        }
        printf("server finish\n");
    }

    int TCPServerNetManagerWorkerWin::GetMacConnection() const
    {
        return MacConnection;
    }

    void TCPServerNetManagerWorkerWin::SetMacConnection(int macConnection)
    {
        MacConnection = macConnection;
    }

    void TCPServerNetManagerWorkerWin::HandleAccept()
    {
        int client_sockfd;
        struct sockaddr_in client_address;
        client_sockfd = Local->Accept((struct sockaddr *)&client_address);
        SCPPSocket* NewSock = Local->Clone(client_sockfd, client_address);
        Connection* Conn = new TCPConnection(NewSock);
        ConnectionsToClient.push_back(Conn);
        FD_SET(client_sockfd, &readfds);
        printf("adding client on fd %d\n", client_sockfd);
    }

    void TCPServerNetManagerWorkerWin::HandleRead()
    {
        int nread;

        std::list<Connection*>::iterator it = ConnectionsToClient.begin();
        while (it != ConnectionsToClient.end())
        {
            Connection* conn = *it;
            SSocket fd = conn->GetSSock()->GetFileDescriptor();
            if(FD_ISSET(fd,&testfds))
            {
                int ret =conn->Read(LenBuf,4);

                if(ret==SOCKET_ERROR)
                {
                    int err =WSAGetLastError();
                    if(err==WSAEWOULDBLOCK)
                    {
                        continue;
                    }
                    else if(err==WSAETIMEDOUT)//timeout
                    {
                        Local->Close();
                        printf("timeout client close socket on fd %d\n", fd);
                        break;
                    }
                    else if(err==WSAENETDOWN)//disconnect
                    {
                        Local->Close();
                        printf("disconnect client close socket on fd %d\n", fd);
                        break;
                    }
                    else
                    {
                        Local->Close();
                        printf("error client close socket on fd %d\n", fd);
                        break;
                    }
                }
                else
                {
                    //ConnectionToServer->Read(LenBuf, 4);
                    int len = SocketUtil::BytesToInt((byte *)LenBuf);
                    conn->Read(ReadBuf, len);
                    char* msg = new char[len];
                    memcpy(msg, ReadBuf, len);
                    printf("client read  %d\n", len);
                    printf("client read  %s\n", msg);
                }
            } else
            {
                it++;
            }
        }
    }

    TCPServerNetManagerWorkerWin::TCPServerNetManagerWorkerWin()
    {
        LenBuf = new char[4];
        ReadBuf = new char[1024];
        std::printf("construct TCPServerNetManagerWorkerWin\n");
    }

    TCPServerNetManagerWorkerWin::~TCPServerNetManagerWorkerWin()
    {
        delete LenBuf;
        LenBuf = nullptr;
        delete ReadBuf;
        ReadBuf = nullptr;
        while(!ConnectionsToClient.empty()){
            Connection* Conn = ConnectionsToClient.front();
            ConnectionsToClient.pop_front();
            delete Conn;
            Conn = nullptr;
        }
        std::printf("destruct TCPServerNetManagerWorkerWin\n");
    }

    void TCPServerNetManagerWorkerWin::SendMessage(const char *Msg, int Len)
    {

    }

    SCPPSocket *scppsocket::TCPServerNetManagerWorkerWin::GetLocal() const
    {
        return Local;
    }

    void TCPServerNetManagerWorkerWin::SetLocal(scppsocket::SCPPSocket *local)
    {
        Local = local;
    }

    void TCPServerNetManagerWorkerWin::StopWork()
    {
        delete LenBuf;
        LenBuf = nullptr;
        delete ReadBuf;
        ReadBuf = nullptr;
        std::list<Connection*>::iterator p1;
        for(p1=ConnectionsToClient.begin();p1!=ConnectionsToClient.end();p1++)
        {
            Connection* Conn = (Connection*)*p1;
            Conn->GetSSock()->ShutDown();
            Conn->GetSSock()->Close();
        }
        if(Local != nullptr)
        {
            Local->ShutDown();
            Local->Close();
        }
        std::printf("TCPServerNetManagerWorkerWin StopWork\n");
    }
}