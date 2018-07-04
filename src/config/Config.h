//
// Created by xwy on 2018/7/3.
//

#ifndef MEMDB_CONFIG_H
#define MEMDB_CONFIG_H


#include <string>
#include <unordered_map>

enum LogLevel { ALL = 0, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF};

class Config {

    std::unordered_map<std::string, std::string> confs;

public:
    std::string conf_path;

    int port;

    bool is_require_pass;

    std::string password;

    std::string log_file;

    int log_level;

    static int OK;
    static int ERROR;

private:
    Config() = default;
    ~Config() = default;
    Config(const Config& conf) = default;
    Config& operator=(const Config& conf) = default;

    static Config* instance;

    int load();

    int read();

public:
    static Config* getInstance();

};


#endif //MEMDB_CONFIG_H
