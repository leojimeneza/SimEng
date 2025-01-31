#pragma once

#include <forward_list>
#include <unordered_map>

#include "simeng/arch/Architecture.hh"
#include "simeng/arch/riscv/ExceptionHandler.hh"
#include "simeng/arch/riscv/Instruction.hh"
#include "simeng/kernel/Linux.hh"

using csh = size_t;

namespace simeng {
namespace arch {
namespace riscv {

/* A basic RISC-V implementation of the `Architecture` interface. */
class Architecture : public arch::Architecture {
 public:
  Architecture(kernel::Linux& kernel, YAML::Node config);
  ~Architecture();
  /** Pre-decode instruction memory into a macro-op of `Instruction`
   * instances. Returns the number of bytes consumed to produce it (always 4),
   * and writes into the supplied macro-op vector. */
  uint8_t predecode(const void* ptr, uint8_t bytesAvailable,
                    uint64_t instructionAddress,
                    MacroOp& output) const override;

  /** Returns an RISC-V register file structure description. */
  std::vector<RegisterFileStructure> getRegisterFileStructures() const override;

  /** Returns a zero-indexed register tag for a system register encoding. */
  int32_t getSystemRegisterTag(uint16_t reg) const override;

  /** Returns the number of system registers that have a mapping. */
  uint16_t getNumSystemRegisters() const override;

  /** Create an exception handler for the exception generated by `instruction`,
   * providing the core model object and a reference to process memory.
   * Returns a smart pointer to an `ExceptionHandler` which may be ticked until
   * the exception is resolved, and results then obtained. */
  std::shared_ptr<arch::ExceptionHandler> handleException(
      const std::shared_ptr<simeng::Instruction>& instruction, const Core& core,
      MemoryInterface& memory) const override;

  /** Retrieve the initial process state. */
  ProcessStateChange getInitialState() const override;

  /** Returns the maximum size of a valid instruction in bytes. */
  uint8_t getMaxInstructionSize() const override;

  /** Updates System registers of any system-based timers. */
  void updateSystemTimerRegisters(RegisterFileSet* regFile,
                                  const uint64_t iterations) const override;

  /** Returns the physical register structure as defined within the config file
   */
  std::vector<RegisterFileStructure> getConfigPhysicalRegisterStructure(
      YAML::Node config) const override;

  /** Returns the physical register quantities as defined within the config file
   */
  std::vector<uint16_t> getConfigPhysicalRegisterQuantities(
      YAML::Node config) const override;

 private:
  /** Retrieve an executionInfo object for the requested instruction. If a
   * opcode-based override has been defined for the latency and/or
   * port information, return that instead of the group-defined execution
   * information. */
  executionInfo getExecutionInfo(Instruction& insn) const;

  /** A decoding cache, mapping an instruction word to a previously decoded
   * instruction. Instructions are added to the cache as they're decoded, to
   * reduce the overhead of future decoding. */
  static std::unordered_map<uint32_t, Instruction> decodeCache;
  /** A decoding metadata cache, mapping an instruction word to a previously
   * decoded instruction metadata bundle. Metadata is added to the cache as it's
   * decoded, to reduce the overhead of future decoding. */
  static std::forward_list<InstructionMetadata> metadataCache;

  /** A mapping from system register encoding to a zero-indexed tag. */
  std::unordered_map<uint16_t, uint16_t> systemRegisterMap_;

  /** A map to hold the relationship between aarch64 instruction groups and
   * user-defined execution information. */
  std::unordered_map<uint16_t, executionInfo> groupExecutionInfo_;

  /** A map to hold the relationship between aarch64 instruction opcode and
   * user-defined execution information. */
  std::unordered_map<uint16_t, executionInfo> opcodeExecutionInfo_;

  /** A Capstone decoding library handle, for decoding instructions. */
  csh capstoneHandle;

  /** A reference to a Linux kernel object to forward syscalls to. */
  kernel::Linux& linux_;
};

}  // namespace riscv
}  // namespace arch
}  // namespace simeng
