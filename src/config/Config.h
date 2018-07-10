//
// Created by xwy on 2018/7/3.
//

#ifndef MEMDB_CONFIG_H
#define MEMDB_CONFIG_H


#include <string>
#include <unordered_map>

#define DEF_CONF(k, v) static std::string (k) = (#v)

enum LogLevel { ALL = 0, TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF};

struct DefaultConfig {
    DEF_CONF(port, 8088);
    DEF_CONF(is_require_pass, no);
    DEF_CONF(password, );
    DEF_CONF(log_dir, /tmp/error);
    DEF_CONF(pid_file, pid);
    DEF_CONF(db_file, db);
    DEF_CONF(log_level, info);
};

class Config {

    std::unordered_map<std::string, std::string> confs;

public:
    std::string conf_path;

    int port;

    bool is_require_pass;

    std::string password;

    std::string log_dir;

    std::string pid_file;

    std::string db_file;

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

    int reset();

    int read();

public:
    std::string* operator[](const std::string& key);

    static Config* get_instance();
};


#endif //MEMDB_CONFIG_H
