#pragma once

#include <unordered_map>
#include <string>

class MprpcConfig
{
public:
    void LoadConigFile(const char *config_file);
    std::string Load(const std::string &key);
    MprpcConfig() {}
    ~MprpcConfig() {}

private:
    std::unordered_map<std::string, std::string> m_configMap;
};
