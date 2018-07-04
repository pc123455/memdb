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

Config* Config::getInstance() {
    if (instance == nullptr) {
        instance = new Config();
        instance->load();
    }
    return instance;
}

int Config::load() {
    read();
}

int Config::read() {
    std::ifstream ifs(conf_path);

    if (!ifs.is_open()) {
        std::cerr<<"config file open error!"<<std::endl;
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
            std::cerr << "config: " << line << "  error!" << std::endl;
            exit(-1);
        }

        if(confs.find(key) != confs.end()) {
            std::cerr << "duplicate config: " << line << std::endl;
            exit(-1);
        }

        confs[key] = value;
    }
}