#pragma once

#include "arch/Architecture.hh"
#include "gmock/gmock.h"

namespace simeng {

/** Mock implementation of the `Architecture` interface. */
class MockArchitecture : public arch::Architecture {
 public:
  MOCK_CONST_METHOD5(predecode,
                     uint8_t(const void* ptr, uint8_t bytesAvailable,
                             uint64_t instructionAddress,
                             BranchPrediction prediction, MacroOp& output));
  MOCK_CONST_METHOD0(getRegisterFileStructures,
                     std::vector<RegisterFileStructure>());
  MOCK_CONST_METHOD1(canRename, bool(Register reg));
  MOCK_CONST_METHOD3(handleException,
                     std::shared_ptr<arch::ExceptionHandler>(
                         const std::shared_ptr<Instruction>& instruction,
                         const ArchitecturalRegisterFileSet& registerFileSet,
                         MemoryInterface& memory));
  MOCK_CONST_METHOD0(getInitialState, arch::ProcessStateChange());
};

}  // namespace simeng
