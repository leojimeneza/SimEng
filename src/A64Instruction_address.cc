#include "A64Instruction.hh"

#include <iostream>

namespace simeng {

std::vector<std::pair<uint64_t, uint8_t>> A64Instruction::generateAddresses() {
  if (!isLoad() && !isStore()) {
    // Not a load or store
    return {};
  }

  switch (opcode) {
    case A64Opcode::STR_I:
    case A64Opcode::LDR_I: {
      if (metadata.wback) {
        exception = A64InstructionException::ExecutionNotYetImplemented;
        return {};
      }

      int baseOpIndex = 0;
      if (opcode == A64Opcode::STR_I) {
        baseOpIndex = 1;
      }

      auto address = operands[baseOpIndex].get<uint64_t>() + metadata.offset;
      setMemoryAddresses({{address, 1 << metadata.scale}});
      return memoryAddresses;
    }
    default:
      return {};
  }
}

}  // namespace simeng
