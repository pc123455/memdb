//
// Created by python on 18-5-25.
//

#include "Client.h"

int Client::AUTHENTICATED = 1;

Client::Client(const char *name): name(name), flags(0), ctime(time(NULL)), auth(0) {
    last_interaction_time = ctime;
}

bool Client::is_authenticate() {
    return (auth & AUTHENTICATED) != 0;
}

bool Client::authenticate(std::string pwd) {
    bool authed = (password == pwd);
    //set auth flag if authenticated
    auth |= authed ? AUTHENTICATED : 0;
    return authed;
}