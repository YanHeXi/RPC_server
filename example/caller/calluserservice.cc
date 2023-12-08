#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("yuan");
    request.set_pwd("123");
    fixbug::LoginResponse response;
    stub.Login(nullptr, &request, &response, nullptr);
    if (0 == response.result().errcode())
    {
        std::cout << "rpc login response success:"
                  << response.sucess() << std::endl;
    }
    else
    {

        std::cout << "rpc login response error : "
                  << response.result().errmsg() << std::endl;
    }
    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("hao");
    req.set_pwd("123");
    fixbug::RegisterResponse rsp;

    stub.Register(nullptr, &req, &rsp, nullptr);

    if (0 == rsp.result().errcode())
    {
        std::cout << "rpc register response success:" << rsp.sucess() << std::endl;
    }
    else
    {
        std::cout << "rpc register response error : " << rsp.result().errmsg() << std::endl;
    }

    return 0;
}