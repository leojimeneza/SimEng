#ifndef __H_A64ARCHITECTURE
#define __H_A64ARCHITECTURE

#include "architecture.hh"

#include <unordered_map>

#include "A64Instruction.hh"

namespace simeng {

/* A basic ARMv8-a implementation of the `Architecture` interface. */
class A64Architecture : public Architecture {
 public:
  A64Architecture();

  /** Pre-decode instruction memory into a macro-op of `A64Instruction`
   * instances. Returns the macro-op generated and the number of bytes consumed
   * to produce it (always 4). */
  std::tuple<MacroOp, uint8_t> predecode(
      void* ptr, uint8_t bytesAvailable,
      uint64_t instructionAddress) const override;

  /** Returns an ARMv8-a register file structure description. */
  std::vector<std::pair<uint8_t, uint16_t>> getRegisterFileStructure()
      const override;
  bool canRename(Register reg) const override;

 private:
  /** A decoding cache, mapping an instruction word to a previously decoded
   * instruction. Instructions are added to the cache as they're decoded, to
   * reduce the overhead of future decoding. */
  static std::unordered_map<uint32_t, A64Instruction> decodeCache;
};

}  // namespace simeng

#endif