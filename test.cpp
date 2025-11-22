#include <bits/stdc++.h>
using namespace std;
auto solve() -> void {

}
static int32_t get_imm_b(uint32_t insn) {
    // bit 31 in insn is placed in bit 12 and extended to bit 31.
    uint32_t imm_b = (insn >> 31) << 12;
    imm_b = (imm_b == 0) ? 0 : imm_b | 0xfffff;

    // get bits 30-25

    uint32_t upper_6_bits = ((insn >> 25) & 0x7e);
    imm_b |= upper_6_bits << 5;

    // add bit 7 to imm_b bit 11
    imm_b |= ((insn >> 7) & 1) << 11;

    // add bits [11:8]
    uint32_t lower_4 = (insn >> 8) & 0xf00;
    imm_b |= (lower_4 << 1);
    return imm_b;
}
int main() {
    std::cout << std::boolalpha << ((10) ? "yes" : "no"); 
    uint32_t x = 0xf0000000;
    std::cout << "should be: " << (1 << 11) << '\n';
    std::cout << "got: " << ((x >> 31) << 11) << '\n';
    std::cout << std::hex << ((x >> 31) << 11) << std::setw(8);
    uint32_t y = 0x0000080;
    std::cout << boolalpha << (y >> 7);


    return 0;
}
