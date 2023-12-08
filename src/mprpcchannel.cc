#include "mprpcchannel.h"
#include "rpcheader.pb.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <error.h>
#include "mprpcapplication.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller,
                              const google::protobuf::Message *request,
                              google::protobuf::Message *response,
                              google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor *sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();
    uint32_t args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        std::cout << "serialize request error " << std::endl;
        return;
    }

    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }
    else
    {
        std::cout << "serialize rpc header error " << std::endl;
        return;
    }

    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char *)&header_size, 4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    std::cout << "-----------------------" << std::endl;
    std::cout << service_name << std::endl;
    std::cout << method_name << std::endl;
    std::cout << args_size << std::endl;
    std::cout << send_rpc_str << std::endl;
    std::cout << "-----------------------" << std::endl;

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create socket error" << errno << std::endl;

        exit(EXIT_FAILURE);
    }

    std::string ip = MprpcApplication::
                         GetInstance()
                             .GetConfig()
                             .Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::
                             GetInstance()
                                 .GetConfig()
                                 .Load("rpcserverport")
                                 .c_str());
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (-1 == connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        std::cout << "connect error erron:" << errno << std::endl;
        close(clientfd);
        exit(EXIT_FAILURE);
    }
    if (-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0))
    {
        std::cout << "send error erron:" << errno << std::endl;
        close(clientfd);

        return;
    }

    char recv_buf[1024] = {0};
    uint32_t recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0)))
    {
        std::cout << "recv error erron:" << errno << std::endl;
        close(clientfd);

        return;
    }

    // std::string response_str(recv_buf, 0, recv_size);
    if (!response->ParseFromArray(recv_buf, recv_size))
    {
        std::cout << "rpc login error recv_buf" << recv_buf << std::endl;
        close(clientfd);
        return;
    }

    close(clientfd);
}