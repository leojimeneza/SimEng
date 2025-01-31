#include <cmath>

#include "InstructionMetadata.hh"
#include "simeng/arch/riscv/Instruction.hh"

namespace simeng {
namespace arch {
namespace riscv {

span<const MemoryAccessTarget> Instruction::generateAddresses() {
  assert((isLoad() || isStoreAddress()) &&
         "generateAddresses called on non-load-or-store instruction");

  uint64_t address;
  if (isLoad() && isStoreAddress() && isAtomic()) {
    // Atomics
    address = operands[1].get<uint64_t>();
  } else if (isLoad()) {
    address = operands[0].get<uint64_t>() + metadata.operands[1].mem.disp;
  } else {
    address = operands[1].get<uint64_t>() + metadata.operands[1].mem.disp;
  }

  // Atomics
  if (Opcode::RISCV_AMOADD_D <= metadata.opcode &&
      metadata.opcode <= Opcode::RISCV_AMOXOR_W_RL) {  // Atomics
    // THIS IS DEPENDENT ON CAPSTONE ENCODING AND COULD BREAK IF CHANGED
    int size = ((metadata.opcode - 182) / 4) % 2;  // 1 = Word, 0 = Double
    if (size == 1) {
      // Word
      setMemoryAddresses({{address, 4}});
    } else {
      // Double
      setMemoryAddresses({{address, 8}});
    }
    return getGeneratedAddresses();
  }

  switch (metadata.opcode) {
    case Opcode::RISCV_SD:
      [[fallthrough]];
    case Opcode::RISCV_LD: {
      setMemoryAddresses({{address, 8}});
      break;
    }
    case Opcode::RISCV_SW:
      [[fallthrough]];
    case Opcode::RISCV_LW:
      [[fallthrough]];
    case Opcode::RISCV_LWU: {
      setMemoryAddresses({{address, 4}});
      break;
    }
    case Opcode::RISCV_SH:
      [[fallthrough]];
    case Opcode::RISCV_LH:
      [[fallthrough]];
    case Opcode::RISCV_LHU: {
      setMemoryAddresses({{address, 2}});
      break;
    }
    case Opcode::RISCV_SB:
      [[fallthrough]];
    case Opcode::RISCV_LB:
      [[fallthrough]];
    case Opcode::RISCV_LBU: {
      setMemoryAddresses({{address, 1}});
      break;
    }

    // Atomics
    case Opcode::RISCV_LR_W:
      [[fallthrough]];
    case Opcode::RISCV_LR_W_AQ:
      [[fallthrough]];
    case Opcode::RISCV_LR_W_RL:
      [[fallthrough]];
    case Opcode::RISCV_LR_W_AQ_RL: {
      setMemoryAddresses({{operands[0].get<uint64_t>(), 4}});
      break;
    }
    case Opcode::RISCV_LR_D:
      [[fallthrough]];
    case Opcode::RISCV_LR_D_AQ:
      [[fallthrough]];
    case Opcode::RISCV_LR_D_RL:
      [[fallthrough]];
    case Opcode::RISCV_LR_D_AQ_RL: {
      setMemoryAddresses({{operands[0].get<uint64_t>(), 8}});
      break;
    }
    case Opcode::RISCV_SC_W:
      [[fallthrough]];
    case Opcode::RISCV_SC_W_AQ:
      [[fallthrough]];
    case Opcode::RISCV_SC_W_RL:
      [[fallthrough]];
    case Opcode::RISCV_SC_W_AQ_RL: {
      setMemoryAddresses({{operands[1].get<uint64_t>(), 4}});
      break;
    }
    case Opcode::RISCV_SC_D:
      [[fallthrough]];
    case Opcode::RISCV_SC_D_AQ:
      [[fallthrough]];
    case Opcode::RISCV_SC_D_RL:
      [[fallthrough]];
    case Opcode::RISCV_SC_D_AQ_RL: {
      setMemoryAddresses({{operands[1].get<uint64_t>(), 8}});
      break;
    }
    default:
      exceptionEncountered_ = true;
      exception_ = InstructionException::ExecutionNotYetImplemented;
      break;
  }
  return getGeneratedAddresses();
}

}  // namespace riscv
}  // namespace arch
}  // namespace simeng