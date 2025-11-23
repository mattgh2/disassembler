/************************************************************************************
* NIU CSCI 463
* Final Project Version 2: Disassembler 
* Matt Warner - z2004200
*
* I certify that this is my own work, and where applicable an
* extension of the starter code for the assignment/exam
*
************************************************************************************/
#include "rv32i_decode.h"

/**
 * Decodes an rv32i encoded instruction.
 *
 * @param addr
 * @param insn
 *K
 * @return A string containing the decoded instruction.
 */
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn) {
    std::ostringstream os;
    os << to_hex32(addr) << ": " << std::setw(10) << std::left << to_hex32(insn);
    std::string mnemoic;

    if (insn == insn_ebreak || insn == insn_ecall) {
        os << ((insn == insn_ecall) ? render_ecall() : render_ebreak());
    }
    switch(get_opcode(insn)) {
        case opcode_lui:
            os << render_lui(insn);
            break;

        case opcode_alu_imm:
            switch (get_funct3(insn)) {
                case funct3_add:
                    os << render_itype_alu(insn, "addi", get_imm_i(insn));
                    break;
                case funct3_slti:
                    os << render_itype_alu(insn, "slti", get_imm_i(insn));
                    break;
                case funct3_sltiu:
                    os << render_itype_alu(insn, "sltiu", get_imm_i(insn));
                    break;
                case funct3_xori: 
                    os << render_itype_alu(insn, "xori", get_imm_i(insn));
                    break;
                case funct3_ori:
                    os << render_itype_alu(insn, "ori", get_imm_i(insn));
                    break;
                case funct3_andi:
                    os << render_itype_alu(insn, "andi", get_imm_i(insn));
                    break;
                case funct3_slli:
                    os << render_itype_alu(insn, "slli", get_imm_i(insn));
                    break;
                case funct3_srl:
                    switch (get_funct7(insn)) {
                        case funct7_srl:
                            os << render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
                            break;
                        case funct7_sra:
                            os << render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
                            break;
                        default: os << render_illegal_insn();
                    }
                    break;
                default:
                    os << render_illegal_insn();
            }
            return os.str();

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
            switch (get_funct3(insn)) {
                case funct3_lb:   
                    mnemoic = "lb";
                    break;
                case funct3_lbu:   
                    mnemoic = "lbu";
                    break;
                case funct3_lh:   
                    mnemoic = "lh";
                    break;
                case funct3_lhu:   
                    mnemoic = "lhu";
                    break;
                case funct3_lw:   
                    mnemoic = "lw";
                    break;
                default:
                    os << render_illegal_insn();
            }
            os << render_itype_load(insn, mnemoic);
            break;

        case opcode_rtype:
            switch (get_funct3(insn)) {
                case funct3_add:
                    switch (get_funct7(insn)) {
                        case funct7_add: 
                            mnemoic = "add";
                            break;
                        case funct7_sub:
                            mnemoic = "sub";
                            break;
                        default: os << render_illegal_insn();
                    }
                    break;
                case funct3_srl:
                    switch (get_funct7(insn)) {
                        case funct7_srl:
                            mnemoic = "srl";
                            break;
                        case funct7_sra:
                            mnemoic = "sra";
                            break;
                        default: os << render_illegal_insn();
                    }
                    break;
                case funct3_sll:
                    mnemoic = "sll";
                    break;
                case funct3_slt:
                    mnemoic = "slt";
                    break;
                case funct3_sltu:
                    mnemoic = "sltu";
                    break;
                case funct3_xor:
                    mnemoic = "xor";
                    break;
                case funct3_or:
                    mnemoic = "or";
                    break;
                case funct3_and:
                    mnemoic = "and";
                    break;
                default: os << render_illegal_insn();
            }
            os << render_rtype(insn,mnemoic);
            break;

        case opcode_stype:
            switch (get_funct3(insn)) {
                case funct3_sb:
                    mnemoic = "sb";
                    break;
                case funct3_sh:
                    mnemoic = "sh";
                    break;
                case funct3_sw:
                    mnemoic = "sw";
                    break;
                default:
                    os << render_illegal_insn();
            }
            os << render_stype(insn,mnemoic);
            break;
        case opcode_system:
            switch (get_funct3(insn)) {
                case funct3_csrrw:
                    os << render_csrrx(insn,"csrrw");
                    break;
                case funct3_csrrs:
                    os << render_csrrx(insn, "csrrs");
                    break;
                case funct3_csrrc:
                    os << render_csrrx(insn,"csrrc");
                    break;

                case funct3_csrrwi:
                    os << render_csrrxi(insn,"csrrwi");
                    break;
                case funct3_csrrsi:
                    os << render_csrrxi(insn,"csrrsi");
                    break;
                case funct3_csrrci:
                    os << render_csrrxi(insn, "csrrci");
                    break;
            }
            break;
        default:
            os << render_illegal_insn();
    }
    return os.str();
}

/**
 * Gets the opcode from an instruction
 *
 * @param insn The encoded instruction.
 *
 * @return The opcode
 */
uint32_t rv32i_decode::get_opcode(uint32_t insn) {
    // First 7 bits of the insn.
    uint8_t mask = 0x7f;
    return insn & mask;
}

/**
 * Gets the destination register from the encoded instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The destination register.
 */
uint32_t rv32i_decode::get_rd(uint32_t insn) {
    // bits [7:11] (5 bits)
    uint8_t mask = 0x1f;
    return (insn >> 7) & mask;
}
/**
 * Gets the first source register, rs1, from the instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return the source register.
 */
uint32_t rv32i_decode::get_rs(uint32_t insn) {
    // bits [15:19] (5 bits)
    uint8_t mask = 0x1f;
    return (insn >> 15) & mask;
}
/**
 * Gets the second source register, rs2, from the instruction.
 *
 * @param the instruction 
 *
 * @return rs2
 */
uint32_t rv32i_decode::get_rs2(uint32_t insn) { 
    // bits [20:24] (5 bits)
    uint8_t mask = 0x1f;
    return (insn >> 20) & mask;
}
/**
 * gets the funct3 value from the instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The funct3 value.
 */
uint32_t rv32i_decode::get_funct3(uint32_t insn) {
    // bits [12:14] (3 bits)
    uint8_t mask = 0x7;
    return (insn >> 12) & mask;
}
/**
 * gets the funct7 value from the instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The funct7 value.
 */
uint32_t rv32i_decode::get_funct7(uint32_t insn) {
    // bits [25:31] (7 bits)
    uint8_t mask = 0x7f;
    return (insn >> 25) & mask;
}
/**
 * Gets the immediate value from an I type instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The immediate value encoded in the instruction.
 */
int32_t rv32i_decode::get_imm_i(uint32_t insn) {
    uint16_t mask = 0xfff;
    uint32_t imm_i = (insn >> 20) & mask;
    return imm_i | (((imm_i >> 11) & 1) ? 0xfffff000 : 0);
}
/**
 * Gets the immediate value from an I type instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The immediate value encoded in the instruction.
 */
int32_t rv32i_decode::get_imm_u(uint32_t insn) {
    uint32_t mask = 0xfffff;
    uint32_t imm_u = (insn >> 12) & mask;
    return imm_u << 12;
}
/**
 * Gets the csr from the encoded instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The csr.
 */
uint16_t rv32i_decode::get_csr(uint32_t insn) {
    return (insn >> 20) & 0xfff;
}
/**
 * Gets the zimm from the encoded instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The zimmm.
 */
uint32_t rv32i_decode::get_zimm(uint32_t insn) {
    return (insn >> 15) & 0x1f;
}
/**
 * Gets the immediate value from a b type instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The immediate value encoded in the instruction.
 */
int32_t rv32i_decode::get_imm_b(uint32_t insn) {
    // bit 31 in insn is placed in bit 12 and extended to bit 31.
    uint32_t imm_b = ((insn >> 31) & 1) << 12;
    imm_b = (imm_b == 0) ? 0 : imm_b | 0xfffff000;

    // get bits 30-25
    uint32_t upper_6_bits = ((insn >> 25) & 0x7f);
    imm_b |= upper_6_bits << 5;

    // add bit 7 to imm_b bit 11
    imm_b |= ((insn >> 7) & 1) << 11;

    // add bits [11:8]
    uint32_t lower_4 = (insn >> 8) & 0xf;
    imm_b |= (lower_4 << 1);
    return imm_b;
}
/**
 * Gets the immediate value from an s type instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The immediate value encoded in the instruction.
 */
int32_t rv32i_decode::get_imm_s(uint32_t insn) {
    int32_t imm_s = (insn & 0xfe000000) >> (25-5);
    imm_s |= (insn & 0x00000f80) >> (7-0);
    if (insn & 0x80000000)
        imm_s |= 0xfffff000;
    return imm_s;
}
/**
 * Gets the immediate value from a j type instruction.
 *
 * @param insn The encoded instruction.
 *
 * @return The immediate value encoded in the instruction.
 */
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

/**
 * Renders an illegal instruction.
 *
 * @return A string containing text for printing illegal instructions.
 */
std::string rv32i_decode::render_illegal_insn() {
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}
/**
 * Renders a U type instruction
 *
 * @param insn The encoded instruction.
 * @param mnemonic the mnemonic for the instruction.
 *
 * @return The rendered text for a U type instruction.
 */
std::string rv32i_decode::render_utype(uint32_t insn, const std::string &mnemonic) {
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << ","
        << to_hex0x20((imm_u >> 12)&0x0fffff);
    return os.str();
}
/**
 * Renders a lui instruction
 *
 * @param insn The encoded instruction.
 *
 * @return The rendered text for lui instruction.
 */
std::string rv32i_decode::render_lui(uint32_t insn) {
    return render_utype(insn,"lui");
}
/**
 * Renders a auipc instruction
 *
 * @param insn The encoded instruction.
 *
 * @return The rendered text for auipc instruction.
 */
std::string rv32i_decode::render_auipc(uint32_t insn) {
    return render_utype(insn,"auipc");
}
/**
 * Renders a jal instruction
 *
 * @param insn The encoded instruction.
 *
 * @return The rendered text for jal instruction.
 */
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn) { 
    uint32_t rd = get_rd(insn);
    uint32_t imm_j = get_imm_j(insn);

    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << ","
        << to_hex0x32(imm_j + addr); 
    return os.str();
}
/**
 * Renders a jalr instruction
 *
 * @param insn The encoded instruction.
 *
 * @return The rendered text for jalr instruction.
 */
std::string rv32i_decode::render_jalr(uint32_t insn) {
    uint32_t rd = get_rd(insn);
    uint32_t rs = get_rs(insn);
    int32_t imm_i = get_imm_i(insn);

    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << "," << render_base_disp(rs, imm_i);
        return os.str();
}
/**
 * Renders a B type instruction
 *
 * @param insn The encoded instruction.
 * @param mnemonic the mnemonic for the instruction.
 *

 * @return The rendered text for a B type instruction.
 */
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const std::string &mnemonic) {
    uint32_t rs1 = get_rs(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << to_hex0x32(imm_b + addr);
    return os.str();
}
/**
 * Renders a i type load instruction
 *
 * @param insn The encoded instruction.
 * @param mnemonic the mnemonic for the instruction.
 *

 * @return The rendered text for an i type load instruction.
 */
std::string rv32i_decode::render_itype_load(uint32_t insn, const std::string &mnemonic) {
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs(insn);
    int32_t imm_i = get_imm_i(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_base_disp(rs1, imm_i);
    return os.str();
}
/**
 * Renders a S type load instruction
 *
 * @param insn The encoded instruction.
 * @param mnemonic the mnemonic for the instruction.
 *

 * @return The rendered text for an S type load instruction.
 */
std::string rv32i_decode::render_stype(uint32_t insn, const std::string &mnemonic) {
    uint32_t rs1 = get_rs(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << render_base_disp(rs1,imm_s);
    return os.str();
}
/**
 * Renders a i alu load instruction
 *
 * @param insn The encoded instruction.
 * @param mnemonic the mnemonic for the instruction.
 * @param imm_i the immediate value for the instruction.
 *

 * @return The rendered text for an i type alu instruction.
 */
std::string rv32i_decode::render_itype_alu(uint32_t insn, const std::string &mnemonic, int32_t imm_i)  {
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << imm_i;
    return os.str();
}
/**
 * Renders a r type instruction
 *
 * @param insn The encoded instruction.
 * @param mnemonic the mnemonic for the instruction.
 *
 * @return The rendered text for an r type alu instruction.
 */
std::string rv32i_decode::render_rtype(uint32_t insn, const std::string &mnemonic) {
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs(insn);
    uint32_t rs2 = get_rs2(insn);
    std::ostringstream os;

    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
    return os.str();
}
/**
 * Renders a ecall instruction
 *
 * @return The rendered text for an ecall instruction.
 */
std::string rv32i_decode::render_ecall() {
    return "ecall";
}
/**
 * Renders a ebreak instruction
 *
 * @return The rendered text for an ebreak instruction.
 */
std::string rv32i_decode::render_ebreak() {
    return "ebreak";
}
/**
 * Renders a csrrX instruction
 *
 * @param insn The encoded instruction.
 * @param mnemonic the mnemonic for the instruction.
 *
 * @return The rendered text for the instruction.
 */
std::string rv32i_decode::render_csrrx(uint32_t insn, const std::string &mnemonic) {
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs(insn);
    uint16_t csr = get_csr(insn);
    
    std::ostringstream os;

    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(csr) << "," << render_reg(rs1);
    return os.str();
}
/**
 * Renders a csrrXI instruction
 *
 * @param insn The encoded instruction.
 * @param mnemonic the mnemonic for the instruction.
 *
 * @return The rendered text for the instruction.
 */
std::string rv32i_decode::render_csrrxi(uint32_t insn, const std::string &mnemonic) {
    uint32_t rd = get_rd(insn);
    uint16_t csr = get_csr(insn);
    uint32_t zimm = get_zimm(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(csr) << "," << zimm;
    return os.str();
}
/**
 * Renders a register.
 *
 */
std::string rv32i_decode::render_reg(int r) {
    return "x" + std::to_string(r);
}
/**
 * Renders the displacement(base register) for an instruction.
 */
std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp) { 
    std::string reg = render_reg(base);
    return std::to_string(disp) + "(" + reg + ")";
}
/**
 * Renders the mnemonic for an instruction.
 */
std::string rv32i_decode::render_mnemonic(const std::string &m) {
    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::left << m;
    return os.str();
}
