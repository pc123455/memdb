//
// Created by python on 18-5-25.
//

#ifndef MEMDB_CLIENT_H
#define MEMDB_CLIENT_H


#include <string>
#include <vector>
#include "utils/types.h"

class Client {

    //client name
    std::string name;
    //client password
    std::string& password;
    //client flags
    uint32_t flags;
    //client input buffer
    std::vector<Byte> in_buff;
    //clinet output buffer
    std::vector<Byte> out_buff;
    //is client authenticated
    int auth;
    //created time
    int64_t ctime;
    //last interaction time
    int64_t last_interaction_time;

public:
    static int AUTHENTICATED;

    Client(const char* name, std::string& password);

    bool is_authenticate();

    bool authenticate(std::string pwd);

    void refresh_time();
};


#endif //MEMDB_CLIENT_H
