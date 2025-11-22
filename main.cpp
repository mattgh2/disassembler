/************************************************************************************
* NIU CSCI 463
* Final Project Version 1: Memory 
* Matt Warner - z2004200
*
* I certify that this is my own work, and where applicable an
* extension of the starter code for the assignment/exam
*
* Main function for testing functionality of the memory and hex classes.
*
************************************************************************************/
#include "hex.h"
#include "rv32i_decode.h"
#include "memory.h"
#include <cstdint>
#include <iostream>
#include <sstream>
#include <unistd.h>

static void usage() {
  std::cerr << "Usage: rv32i [-m hex-mem-size] infile" << std::endl;
  std::cerr << "    -m specify memory size (default = 0x100)" << std::endl;
  exit(1);
}
/**
 * Decodes and prints each instruction in a loop.
 *
 * @param mem System memory
 */
static void disassemble(const memory &mem){
    // For each 32-bit word in the simulated memory:
    //      Call rv32i_decode::decode() to format/render one instruction mnemonic and return it as a std::string
    //      print the memory address, instruction hex value, and the instruction mnemonic.
    for (uint32_t i = 0; i < mem.get_size(); i+=4) {
        uint32_t insn = mem.get32(i);
        std::cout << rv32i_decode::decode(i,insn) << '\n';
    }
}


int main(int argc, char **argv) {
  uint32_t memory_limit = 0x100; // default memory size is 0x100

  int opt;
  while ((opt = getopt(argc, argv, " m:")) != -1) {

    switch (opt) {
    case 'm': {
      std::istringstream iss(optarg);
      iss >> std::hex >> memory_limit;
    } break;
    default: 
      usage();
    }
  }

  if (optind >= argc)
    usage(); // missing filename

  memory mem(memory_limit);

  if (!mem.load_file(argv[optind]))
    usage();

  disassemble(mem);
  // mem.dump();
  return 0;
}
