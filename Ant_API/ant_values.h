#ifndef BASE_ANT_VALUES_H_
#define BASE_ANT_VALUES_H_

#include <atomic>
#include <string>

#include "base/no_destructor.h"
#include "base/version.h"

namespace base {

// Plain C++ storage for Ant API values.
// No Blink, no Mojo, no WTF — safe to include from any layer:
// base/**, content/**, chrome/**, components/**
//
// Usage:
//   #include "base/ant_values.h"
//   int32_t cpu = base::AntValues::GetInstance()->cpu.load();
//   std::string gpu = base::AntValues::GetInstance()->gpu;

struct AntValues {
  static AntValues* GetInstance();

  // Integer fields — atomic for thread-safe reads
  std::atomic<int32_t> value{0};
  std::atomic<int32_t> cpu{0};
  std::atomic<int32_t> screen_h{0};
  std::atomic<int32_t> screen_w{0};

  // String fields
  std::string string_val;
  std::string gpu;
  std::string model;
  std::string aversion;
  std::string bversion;
  std::string vendor;
  std::string brand;

  // Parsed Version — for version_info.cc which returns const base::Version&
  // Must be set via ant_service_impl.cc when SetBversion() is called
  base::Version bversion_parsed;

  AntValues();
  ~AntValues();

 private:
  friend class base::NoDestructor<AntValues>;
};

}  // namespace base

#endif  // BASE_ANT_VALUES_H_
