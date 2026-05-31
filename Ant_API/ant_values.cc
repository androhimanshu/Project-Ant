#include "base/ant_values.h"

#include "base/no_destructor.h"

namespace base {

// static
AntValues* AntValues::GetInstance() {
  static base::NoDestructor<AntValues> instance;
  return instance.get();
}

AntValues::AntValues() : bversion_parsed("0.0.0.0") {}
AntValues::~AntValues() = default;

}  // namespace base
