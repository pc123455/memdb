//
// Created by python on 18-5-24.
//

#include "Logger.h"
#include <glog/logging.h>
#include <iostream>

void Logger::init() {
    google::InitGoogleLogging("memdb");
}

std::ostream& Logger::info(const char *msg) {
    return LOG(INFO) << msg;
}

std::ostream& Logger::error(const char *error) {
    return LOG(ERROR) << error;
}
