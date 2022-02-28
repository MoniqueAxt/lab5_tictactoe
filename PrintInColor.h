/**
    Lab 5
    Definition and implementation of functions used to print in color to standard output
    @file StringTrim.h
    @author Monique Axt
    @version 1.0
*/

#ifndef LAB5_TCP_PRINTINCOLOR_H
#define LAB5_TCP_PRINTINCOLOR_H

#include <iostream>

class PrintInColor {
public:
    static inline void red(const std::string &msg) {
        std::cout << "\x1B[31m" << msg << "\033[0m\t\t" << std::endl;
    }

    static inline void dullGreen(const std::string &msg) {
        std::cout << "\x1B[32m" << msg << "\033[0m" << std::endl;
    }

    static inline void yellow(const std::string &msg) {
        std::cout << "\x1B[93m" << msg << "\033[0m\n" << std::endl;
    }
};

#endif //LAB5_TCP_PRINTINCOLOR_H
