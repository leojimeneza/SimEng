#include "RegisterFile.hh"

#include <iostream>

namespace simeng {

std::ostream& operator<<(std::ostream& os, Register const& reg) {
  return os << reg.tag;
}

bool Register::operator==(Register other) const {
  return (other.type == type && other.tag == tag);
}

RegisterFile::RegisterFile(std::vector<uint16_t> registerFileSizes) {
  registerFiles =
      std::vector<std::vector<RegisterValue>>(registerFileSizes.size());

  for (size_t type = 0; type < registerFileSizes.size(); type++) {
    auto registerCount = registerFileSizes[type];
    registerFiles[type] =
        std::vector<RegisterValue>(registerCount, RegisterValue(0, 8));
  }
}

RegisterValue RegisterFile::get(Register reg) const {
  return registerFiles[reg.type][reg.tag];
}

void RegisterFile::set(Register reg, const RegisterValue& value) {
  registerFiles[reg.type][reg.tag] = value;
}

}  // namespace simeng
