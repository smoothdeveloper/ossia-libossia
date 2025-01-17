#pragma once

#if __has_include(<dlfcn.h>)
#include <dlfcn.h>
#include <ossia/detail/fmt.hpp>

#include <stdexcept>

namespace ossia
{
class dylib_loader
{
public:
  explicit dylib_loader(const char* const so)
  {
    impl = dlopen(so, RTLD_LAZY | RTLD_LOCAL | RTLD_NODELETE);
    if(!impl)
    {
      throw std::runtime_error(fmt::format("{}: not found. ", so));
    }
  }

  dylib_loader(const dylib_loader&) noexcept = delete;
  dylib_loader& operator=(const dylib_loader&) noexcept = delete;
  dylib_loader(dylib_loader&& other)
  {
    impl = other.impl;
    other.impl = nullptr;
  }

  dylib_loader& operator=(dylib_loader&& other) noexcept
  {
    impl = other.impl;
    other.impl = nullptr;
    return *this;
  }

  ~dylib_loader()
  {
    if (impl)
    {
      dlclose(impl);
    }
  }

  template <typename T>
  T symbol(const char* const sym) const noexcept
  {
    return (T)dlsym(impl, sym);
  }

  operator bool() const { return bool(impl); }

private:
  void* impl{};
};
}
#endif
