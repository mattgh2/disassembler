/************************************************************************************
* NIU CSCI 463
* Final Project Version 1: Memory 
* Matt Warner - z2004200
*
* I certify that this is my own work, and where applicable an
* extension of the starter code for the assignment/exam
*
* Definitions for methods in the memory class.
*
************************************************************************************/
#include "memory.h"
#include "hex.h"

/**
 * Constructor. Initializes memory vector.
 * Note: Rounds memory size up to nearest integer divisible by 16.
 * 
 * @param siz An amount of memory in bytes.
 */
memory::memory(uint32_t siz) {
    // round the length up, mod-0x10
    siz = (siz + 15) & 0xfffffff0;
    mem.resize(siz,0xa5);
}
/**
 * Checks if a memory address is not a valid address.
 *
 * @param addr A memory address.
 *
 * @return true if the memory address is not valid, false otherwise.
 */
bool memory::check_illegal(uint32_t addr) const {
    bool valid_address = (addr < mem.size());
    if (valid_address == false)  {
        std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(addr) << std::endl;
    }
    return valid_address == false;
}
/**
 * Returns an 8 bit value in memory.
 * Note: Returns 0 if the memory address is invalid.
 *
 * @param addr A memory address.
 *
 * @return 8 bits from memory.
 */
uint8_t memory::get8(uint32_t addr) const {
    return (check_illegal(addr)) ? 0 : mem[addr];
}
/**
 * Returns a 16 bit value from memory in little-endian order.
 *
 * @param addr A memory address.
 *
 * @return 16 bits from memory, or 0 if the address is not valid.
 */
uint16_t memory::get16(uint32_t addr) const {
    uint8_t b1 = get8(addr);
    uint8_t b2 = get8(addr + 1);
    uint16_t res = b2;
    return (res << 8) | b1;
}
/**
 * Returns a 32 bit value from memory in little-endian order.
 *
 * @param addr A memory address.
 *
 * @return 32 bits from memory, or 0 if the address is not valid.
 */
uint32_t memory::get32(uint32_t addr) const {
    uint16_t b1 = get16(addr);
    uint16_t b2 = get16(addr + 2);
    uint32_t res = b2;
    return (res << 16) | b1;
}
/**
 * Sign extends a byte in memory to a 32 bit integer.
 *
 * @param addr An address in memory.
 *
 * @return The sign extended 32 bit signed integer, or zero if the address is not valid.
 */
int32_t memory::get8_sx(uint32_t addr) const {
    uint8_t x = get8(addr);
    uint32_t sign_bits = 0xffffff00;
    return ((x>>7) & 1) ? (x | sign_bits) : x;
}
/**
 * Sign extends a 16 bit value in memory to a 32 bit signed integer.
 *
 * @param addr An address in memory.
 *
 * @return the sign extended 32 bit signed integer, or zero if the address is not valid.
 */
int32_t memory::get16_sx(uint32_t addr) const {
    uint16_t x = get16(addr);
    uint32_t mask = 0xffff0000;
    return ((x>>15) & 1) ? (x | mask) : x;
}
/**
 * Converts a 32 bit value in memory to signed.
 *
 * @param addr A memory address.
 *
 * @return The converted 32 bit integer, or zero if the address is not valid.
 */
int32_t memory::get32_sx(uint32_t addr) const {
    return get32(addr);
}
/**
 * Stores a 8 bit unsigned integer in memory.
 *
 * @param addr A memory address.
 * @param val A value to place into memory.
 */
void memory::set8(uint32_t addr, uint8_t val) {
    if (check_illegal(addr)) return;
    mem[addr] = val;
}
/**
 * Stores a 16 bit unsigned integer in memory in little-endian order.
 *
 * @param addr A memory address.
 * @param val A value to place into memory.
 */
void memory::set16(uint32_t addr, uint16_t val) {
    set8(addr, (val & 0xff));
    set8(addr + 1, (val & 0xff00) >> 8);
}
/**
 * Stores a 32 bit unsigned integer in memory in little-endian order.
 *
 * @param addr A memory address.
 * @param val A value to place into memory.
 */
void memory::set32(uint32_t addr, uint32_t val) {
    set16(addr, (val & 0xffff));
    set16(addr + 2, (val & 0xffff0000) >> 16);
}
/**
 * Performs a memory dump.
 */
void memory::dump() const {

    int num_dump_lines = mem.size() / 16;
    int dump_line_starting_byte = 0;

    // Iterate through all (16 byte) dump lines, printing each byte.
    for (int row = 0; row < (num_dump_lines); ++row) {
        std::string ascii;

        std::cout << hex::to_hex32(dump_line_starting_byte) << ": ";

        // Print the next 16 bytes starting at dump_line_start.
        for (int i = dump_line_starting_byte, tmp = 1; i < dump_line_starting_byte + 16; ++i, ++tmp) {

            uint8_t current_byte = get8(i);
            ascii += isprint(current_byte) ? current_byte : '.';

            std::cout << hex::to_hex8(current_byte) << ' ';

            // Space between blocks of 8 bytes.
            if (tmp != 16 && tmp % 8 == 0) std::cout << ' ';
        }
        std::cout << "*" << ascii << "*\n";

        // Increment to the starting byte of the next dump line.
        dump_line_starting_byte += 16;
    }
}
/**
 * Loads contents of a file into memory.
 *
 * @param fname The name of the file.
 *
 * @return true if every byte in the file was read into memory, false otherwise.
 */
bool memory::load_file(const std::string &fname) {

    // Open file in binary mode.
    std::ifstream infile(fname, std::ios::in|std::ios::binary);

    if (infile.fail()) {
        std::cerr << "Can’t open file "<< fname << "for reading\n";
    }

    uint8_t i;
    infile >> std::noskipws;
    // Iterate through the file byte by byte.
    for (uint32_t addr = 0; infile >> i; ++addr) {

        if (check_illegal(addr) == true) {
            std::cout << "Program too big.\n";
            infile.close();
            return false;
        }
        // Store current byte in memory.
        set8(addr,i);
    }
    infile.close();
    return true;
}
