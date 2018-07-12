//
// Created by xwy on 2018/7/12.
//

#ifndef MEMDB_TIME_H
#define MEMDB_TIME_H

#include <sys/time.h>

inline int64_t time_ms(){
    struct timeval now;
    gettimeofday(&now, NULL);
    return (int64_t)now.tv_sec * 1000 + (int64_t)now.tv_usec / 1000;
}

#endif //MEMDB_TIME_H
