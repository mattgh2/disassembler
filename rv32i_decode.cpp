#include "rv32i_decode.h"

std::string rv32i_decode::decode(uint32_t addr, uint32_t insn) {
    std::ostringstream os;
    os << to_hex32(addr) << ": " << std::setw(10) << std::left << to_hex32(insn);
    std::string mnemoic;

    switch(get_opcode(insn)) {
        case opcode_lui:
            os << render_lui(insn);
            break;
        case opcode_alu_imm:
            os << "alu_imm";
            break;
        case opcode_auipc:
            os << render_auipc(insn);
            break;
        case opcode_btype: 
            switch (get_funct3(insn)) {
                case funct3_beq:
                    mnemoic = "beq";
                    break;
                case funct3_bge:
                    mnemoic = "bge";
                    break;
                case funct3_blt:
                    mnemoic = "blt";
                    break;
                case funct3_bne:
                    mnemoic = "bne";
                    break;
                case funct3_bgeu:
                    mnemoic = "bgeu";
                    break;
                case funct3_bltu:
                    mnemoic = "bltu";
                    break;
                default:
                    os << render_illegal_insn();
            }
            os << render_btype(addr,insn,mnemoic);
            break;
        case opcode_jal:
            os << render_jal(addr,insn);
            break;
        case opcode_jalr:
            os << render_jalr(insn);
            break;
        case opcode_load_imm:
            // Funct 3
            os << "load_imm";
            break;
        case opcode_rtype:
            // funct 7
            os << "rtype";
            break;
        case opcode_stype:
            os << "stype";
            break;
        case opcode_system:
            // Funct 3
            os << "system";
            break;
        default:
            os << render_illegal_insn();
    }
    return os.str();
}

uint32_t rv32i_decode::get_opcode(uint32_t insn) {
    // First 7 bits of the insn.
    uint8_t mask = 0x7f;
    return insn & mask;
}

uint32_t rv32i_decode::get_rd(uint32_t insn) {
    // bits [7:11] (5 bits)
    uint8_t mask = 0x1f;
    return (insn >> 7) & mask;
}
uint32_t rv32i_decode::get_rs(uint32_t insn) {
    // bits [15:19] (5 bits)
    uint8_t mask = 0x1f;
    return (insn >> 15) & mask;
}
uint32_t rv32i_decode::get_rs2(uint32_t insn) { 
    // bits [20:24] (5 bits)
    uint8_t mask = 0x1f;
    return (insn >> 20) & mask;
}
uint32_t rv32i_decode::get_funct3(uint32_t insn) {
    // bits [12:14] (3 bits)
    uint8_t mask = 0x7;
    return (insn >> 12) & mask;
}
uint32_t rv32i_decode::get_funct7(uint32_t insn) {
    // bits [25:31] (7 bits)
    uint8_t mask = 0x7f;
    return (insn >> 25) & mask;
}
int32_t rv32i_decode::get_imm_i(uint32_t insn) {
    uint16_t mask = 0xfff;
    uint32_t imm_i = (insn >> 20) & mask;
    return imm_i | (((imm_i >> 11) & 1) ? 0xfffff000 : 0);
}
int32_t rv32i_decode::get_imm_u(uint32_t insn) {
    uint32_t mask = 0xfffff;
    uint32_t imm_u = (insn >> 12) & mask;
    return imm_u << 12;
}
int32_t rv32i_decode::get_imm_b(uint32_t insn) {
    // bit 31 in insn is placed in bit 12 and extended to bit 31.
    uint32_t imm_b = (insn >> 31) << 12;
    imm_b = (imm_b == 0) ? 0 : imm_b | 0xfffff000;

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
int32_t rv32i_decode::get_imm_s(uint32_t insn) {
    int32_t imm_s = (insn & 0xfe000000) >> (25-5);
    imm_s |= (insn & 0x00000f80) >> (7-0);
    if (insn & 0x80000000)
        imm_s |= 0xfffff000;
    return imm_s;
}
int32_t rv32i_decode::get_imm_j(uint32_t insn) {

    // bit 31 -> 12 sign-extended.
    uint32_t imm_j = (insn >> 31) << 20;
    imm_j |= ((imm_j > 0) ? 0xfff00000 : 0) ;

    // insn[30:21] -> imm[10:1]
    uint32_t upper_bits = (insn >> 21) & 0x7fe;
    imm_j |= upper_bits << 1;

    // Insert insn[20] in imm[11].
    imm_j |= ((insn >> 20) & 1) << 11;

    // bits insn[19:12] -> imm[19:12]
    uint32_t lower_bits = (insn >> 12) & 0xff;
    imm_j |= lower_bits << 12;

    return imm_j;
}

std::string rv32i_decode::render_illegal_insn() {
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}
std::string rv32i_decode::render_utype(uint32_t insn, const std::string &mnemonic) {
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << ","
        << to_hex0x20((imm_u >> 12)&0x0fffff);
    return os.str();
}
std::string rv32i_decode::render_lui(uint32_t insn) {
    return render_utype(insn,"lui");
}
std::string rv32i_decode::render_auipc(uint32_t insn) {
    return render_utype(insn,"auipc");
}
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn) { 
    uint32_t rd = get_rd(insn);
    uint32_t imm_j = get_imm_j(insn);

    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << ","
        << to_hex0x32(imm_j + addr); 
    return os.str();
}
std::string rv32i_decode::render_jalr(uint32_t insn) {
    uint32_t rd = get_rd(insn);
    uint32_t rs = get_rs(insn);
    int32_t imm_i = get_imm_i(insn);

    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << "," << render_base_disp(rs, imm_i);
        return os.str();
}
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const std::string &mnemonic) {
    uint32_t rs1 = get_rs(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << to_hex0x32(imm_b + addr);
    return os.str();
}
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic) {

}
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic) {

}
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)  {

}
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic) {

}
std::string rv32i_decode::render_ecall(uint32_t insn) {

}
std::string rv32i_decode::render_ebreak(uint32_t insn) {

}
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic) {

}
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic) {

}
std::string rv32i_decode::render_reg(int r) {
    return "x" + std::to_string(r);
}
std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp) { 
    std::string reg = render_reg(base);
    return std::to_string(disp) + "(" + reg + ")";
}
std::string rv32i_decode::render_mnemonic(const std::string &m) {
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::left << m;
    return os.str();
}
