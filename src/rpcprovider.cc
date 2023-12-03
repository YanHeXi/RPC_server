#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <string>
#include <functional>
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
}

void RpcProvider::Run()
{
    std::string ip = MprpcApplication::
                         GetInstance()
                             .GetConfig()
                             .Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::
                             GetInstance()
                                 .GetConfig()
                                 .Load("rpcserverport")
                                 .c_str());
    muduo::net::InetAddress address(ip, port);
    std::unique_ptr<muduo::net::TcpServer> m_tcpserverPtr;

    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");

    server.setConnectionCallback(std::bind(&RpcProvider::
                                               OnConnection,
                                           this,
                                           std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2,
                                        std::placeholders::_3));
    server.setThreadNum(4);
    server.start();

    m_eventLoop.loop();
}
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn) {}
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &,
                            muduo::net::Buffer *,
                            muduo::Timestamp) {}