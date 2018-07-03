//
// Created by xwy on 2018/7/3.
//

#include "Config.h"

Config* Config::instance = nullptr;

Config* Config::getInstance() {
    if (instance == nullptr) {
        instance = new Config();
    }
    return instance;
}