//
// Created by xwy on 2018/7/3.
//

#ifndef MEMDB_CONFIG_H
#define MEMDB_CONFIG_H


#include <string>

enum LogLevel { ALL = 0, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF};

class Config {
    int port;

    int is_require_pass;

    std::string password;

    std::string log_file;

    int log_level;

    Config() = default;
    ~Config() = default;
    Config(const Config& conf) = default;
    Config& operator=(const Config& conf) = default;

    static Config* instance;

public:
    static Config* getInstance();

    static int load();
};


#endif //MEMDB_CONFIG_H
