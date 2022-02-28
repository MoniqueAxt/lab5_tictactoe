/**
    Lab 5
    Definition and implementation of string-trimming functions
    @file StringTrim.h
    @author Monique Axt
    @version 1.0
*/

#ifndef LAB5_TCP_STRINGTRIM_H
#define LAB5_TCP_STRINGTRIM_H

#include <algorithm>
#include <cctype>
#include <locale>

class StringTrim {
public:
// trim from left (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

// trim from right (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

// trim from both left and right (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
};

#endif //LAB5_TCP_STRINGTRIM_H
