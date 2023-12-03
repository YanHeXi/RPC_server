#include <iostream>
#include <string>
#include "user.pb.h"

class Userservice : public fixbug::UserServiceRpc
{

public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "Login :" << std::endl;
        std::cout << "name" << name << " pwd: " << pwd << std::endl;
    }
};

int main()
{

    return 0;
}