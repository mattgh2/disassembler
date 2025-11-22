/************************************************************************************
* NIU CSCI 463
* Final Project Version 1: Memory 
* Matt Warner - z2004200
*
* I certify that this is my own work, and where applicable an
* extension of the starter code for the assignment/exam
*
* Provides utility for conversion to hexadecimal.
*
************************************************************************************/
#include "hex.h"

/**
 * Converts an 8 bit number to its hexadecimal representation.
 *
 * @param i A 8 bit number.
 *
 * @return A string containing the hexadecimal representation of the 8 bit input number.
 */
std::string hex::to_hex8(uint8_t i) {
        return to_hexn(i,2);
}
/**
 * Converts an 32 bit number to its hexadecimal representation.
 *
 * @param i A 32 bit number.
 *
 * @return A string containing the hexadecimal representation of the 32 bit input number.
 */
std::string hex::to_hex32(uint32_t i) {
        return to_hexn(i,8);
}
/**
 * Converts an 32 bit number to its hexadecimal representation with 0x prepended.
 *
 * @param i A 32 bit number.
 *
 * @return A string containing the hexadecimal representation of the 32 bit input number.
 */
std::string hex::to_hex0x32(uint32_t i) {
        return "0x" + to_hex32(i);
}
/**
 * Helper function for converting a 32 bit number to hexadecimal using n nibbles.
 *
 * @param i A 32 bit number.
 * @param n_nibs Number of nibbles to use in the output.
 *
 * @return A string containing the hexadecimal representation of the 32 bit number.
 */
std::string hex::to_hexn(uint32_t i, uint32_t n_nibs) {
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(n_nibs) << i;
    return os.str();
}
/**
 * Get the 20 least significant bits of a 32 bit unsigned integer in hexadecimal.
 *
 * @param i A 32 bit number
 *
 * @return A string containing the hexadecimal representation of the 32 bit number.
 */
std::string hex::to_hex0x20(uint32_t i) {
    return "0x" + to_hexn(i,5);
}
/**
 * Get the 20 least significant bits of a 32 bit unsigned integer in hexadecimal.
 *
 * @param i A 32 bit number
 *
 * @return A string containing the hexadecimal representation of the 32 bit number.
 */
std::string hex::to_hex0x12(uint32_t i) {
    return "0x" + to_hexn(i,3);
}
