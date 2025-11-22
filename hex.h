/************************************************************************************
* NIU CSCI 463
* Final Project Version 1: Memory 
* Matt Warner - z2004200
*
* I certify that this is my own work, and where applicable an
* extension of the starter code for the assignment/exam
*
* Class definition for the hex utility class.
*
************************************************************************************/
#ifndef HEX_H
#define HEX_H
#include <string>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <iomanip>

class hex {
public:
    static std::string to_hex8(uint8_t i);
    static std::string to_hex32(uint32_t i);
    static std::string to_hex0x32(uint32_t i);
    static std::string to_hex0x20(uint32_t i);
    static std::string to_hex0x12(uint32_t);
private:
    static std::string to_hexn(uint32_t i, uint32_t n_nibs);
};
#endif
