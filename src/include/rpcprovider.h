#pragma once

#include "google/protobuf/service.h"
class RpcProvider
{
private:
public:
    void NotifyService(google::protobuf::Service *service);
    void Run();
};
