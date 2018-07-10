//
// Created by xwy on 2018/7/3.
//

#include "Config.h"
#include <iostream>
#include <fstream>
#include "../utils/Common.h"

Config* Config::instance = nullptr;

int Config::OK = 0;
int Config::ERROR = -1;

std::string* Config::operator[](const std::string& key) {
    auto it = confs.find(key);
    if (it == confs.end()) {
        return nullptr;
    }
    return &(it->second);
}

Config* Config::get_instance() {
    if (instance == nullptr) {
        instance = new Config();
        instance->load();
    }
    return instance;
}

int Config::load() {
    reset();
    read();

    int port = atoi(confs["port"].c_str());
    if (port <= 0 || port > 65535) {
        std::cerr << "ERROR: port " << port << " is out of range!";
        exit(-1);
    }
    if (port <= 1024) {
        std::cout<< "WARN: port " << port << " is smaller than 1024!";
    }
    this->port = port;

    std::string& is_require_pass = confs["is_require_pass"];
    if (is_require_pass == "yes") {
        this->is_require_pass = true;
    } else if (is_require_pass == "no") {
        this->is_require_pass = false;
    } else {
        std::cerr << "ERROR: is_require_pass must be \"yes\" or \"no\"!";
        exit(-1);
    }

    this->log_dir = confs["log_dir"];
    this->pid_file = confs["pid_file"];
    this->db_file = confs["db_file"];

    std::string log_level = confs["log_level"];

    if (log_level == "all") {
        this->log_level = LogLevel::ALL;
    } else if (log_level == "trace") {
        this->log_level = LogLevel::TRACE;
    } else if (log_level == "debug") {
        this->log_level = LogLevel::DEBUG;
    } else if (log_level == "info") {
        this->log_level = LogLevel::INFO;
    } else if (log_level == "warn") {
        this->log_level = LogLevel::WARN;
    } else if (log_level == "error") {
        this->log_level = LogLevel::ERROR;
    } else if (log_level == "fatal") {
        this->log_level = LogLevel::FATAL;
    } else if (log_level == "off") {
        this->log_level = LogLevel::OFF;
    } else {
        std::cerr<<"log_level must one of \"all\", \"trace\", \"debug\", \"info\", \"warn\", \"error\", \"fatal\", \"off\"";
        exit(-1);
    }
}

int Config::reset() {
    confs["port"] = DefaultConfig::port;
    confs["is_require_pass"] = DefaultConfig::is_require_pass;
    confs["password"] = DefaultConfig::password;
    confs["log_dir"] = DefaultConfig::log_dir;
    confs["pid_file"] = DefaultConfig::pid_file;
    confs["db_file"] = DefaultConfig::db_file;
    confs["log_level"] = DefaultConfig::log_level;
}

int Config::read() {
    std::ifstream ifs(conf_path);

    if (!ifs.is_open()) {
        std::cerr<<"ERROR: config file open error!"<<std::endl;
        exit(-1);
    }


    std::string line;

    while (ifs.peek() != EOF) {
        std::getline(ifs, line);
        Common::trim(line);

        //skip the line beginning with '#'
        if (line.length() == 0 || line[0] == '#') {
            line.clear();
            continue;
        }

        size_t pos = line.find(' ');
        std::string key(line.begin(), line.begin() + pos);
        std::string value(line.begin() + pos, line.end());
        Common::ltrim(value);

        if (key.length() <= 0 || value.length() <= 0) {
            std::cerr << "ERROR: config \"" << line << "\"  error!" << std::endl;
            exit(-1);
        }

//        if(confs.find(key) != confs.end()) {
//            std::cerr << "duplicate config: " << line << std::endl;
//            exit(-1);
//        }

        confs[key] = value;
    }
}