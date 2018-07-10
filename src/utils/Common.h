//
// Created by xwy on 2018/7/4.
//

#ifndef MEMDB_COMMON_H
#define MEMDB_COMMON_H

#include <algorithm>
#include <cctype>
#include <locale>

class Common {
public:

    // trim from start (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    static inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    //upper string
    static inline std::string& string_toupper(std::string& s) {
        std::transform(s.begin(), s.end(), s.begin(), [](char c){ return std::toupper(c); });
        return s;
    }
};


#endif //MEMDB_COMMON_H
