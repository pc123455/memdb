//
// Created by python on 18-5-25.
//

#include "Client.h"

Client::Client(const char *name): name(name), flags(0), ctime(time(NULL)), auth(0) {
    last_interaction_time = ctime;
}