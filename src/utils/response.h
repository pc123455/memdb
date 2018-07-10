//
// Created by xwy on 2018/7/2.
//

#ifndef MEMDB_RESPONSE_H
#define MEMDB_RESPONSE_H

#define NIL_RESPONSE                    "$-1\r\n"
#define OK_RESPONSE                     "+OK\r\n"
#define DB_ERROR_RESPONSE               "-database error\r\n"
#define PONG_RESPONSE                   "+PONG\r\n"
#define PASSWORD_WRONG                  "-Err invalid password\r\n"
#define PASSWORD_UNREQUIRED             "-Err requirepass is not setted\r\n"

#endif //MEMDB_RESPONSE_H
