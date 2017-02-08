#pragma once
#include <ossia/detail/optional.hpp>
#include <ossia/editor/value/value.hpp>
#include <ossia_export.h>

#include <boost/any.hpp>
#include <hopscotch_map.h>
#include <vector>
#include <string>

/**
 * \file node_attributes.hpp
 *
 * This file defines default types and accessors for common extended
 * attributes.
 *
 * The attributes are defined as accessors to the
 * extended attributes container.
 *
 * The implementation of the functions is at the end of \ref node.cpp
 */
namespace ossia
{
namespace net
{
class node_base;
using any_map = tsl::hopscotch_map<std::string, boost::any>;
using extended_attributes = any_map;

struct OSSIA_EXPORT instance_bounds
{
  instance_bounds() = default;
  instance_bounds(const instance_bounds&) = default;
  instance_bounds(instance_bounds&&) = default;
  instance_bounds& operator=(const instance_bounds&) = default;
  instance_bounds& operator=(instance_bounds&&) = default;

  instance_bounds(int32_t min, int32_t max)
      : min_instances{min}
      , max_instances{max} {}

  int32_t min_instances = 0;
  int32_t max_instances = std::numeric_limits<int32_t>::max();
};

using tags = std::vector<std::string>;
using description = std::string;
using priority = int32_t;
using refresh_rate = int32_t;
using value_step_size = int32_t;
using critical = bool; //! Means that the node is very important, e.g. a "play" message
using app_name = std::string;
using app_version = std::string;
using app_creator = std::string;

using default_value = ossia::value;
using extended_type = std::string; //! How a low-level type should be interpreted.

OSSIA_EXPORT optional<instance_bounds> get_instance_bounds(const extended_attributes& n);
OSSIA_EXPORT void set_instance_bounds(extended_attributes& n, optional<instance_bounds>);

OSSIA_EXPORT optional<tags> get_tags(const extended_attributes& n);
OSSIA_EXPORT void set_tags(extended_attributes& n, optional<tags> v);

OSSIA_EXPORT optional<description> get_description(const extended_attributes& n);
OSSIA_EXPORT void set_description(extended_attributes& n, optional<description> v);
OSSIA_EXPORT void set_description(extended_attributes& n, const char* v);

OSSIA_EXPORT optional<priority> get_priority(const extended_attributes& n);
OSSIA_EXPORT void set_priority(extended_attributes& n, optional<priority> v);

OSSIA_EXPORT optional<refresh_rate> get_refresh_rate(const extended_attributes& n);
OSSIA_EXPORT void set_refresh_rate(extended_attributes& n, optional<refresh_rate> v);

OSSIA_EXPORT optional<value_step_size> get_value_step_size(const extended_attributes& n);
OSSIA_EXPORT void set_value_step_size(extended_attributes& n, optional<refresh_rate> v);

OSSIA_EXPORT optional<critical> get_critical(const extended_attributes& n);
OSSIA_EXPORT void set_critical(extended_attributes& n, optional<critical> v);

OSSIA_EXPORT optional<extended_type> get_extended_type(const ossia::net::node_base& n);
OSSIA_EXPORT void set_extended_type(extended_attributes& n, optional<extended_type> v);

OSSIA_EXPORT optional<app_name> get_app_name(const extended_attributes& n);
OSSIA_EXPORT void set_app_name(extended_attributes& n, optional<app_name> v);

OSSIA_EXPORT optional<app_version> get_app_version(const extended_attributes& n);
OSSIA_EXPORT void set_app_version(extended_attributes& n, optional<app_version> v);

OSSIA_EXPORT optional<app_creator> get_app_creator(const extended_attributes& n);
OSSIA_EXPORT void set_app_creator(extended_attributes& n, optional<app_creator> v);

OSSIA_EXPORT optional<ossia::value> get_default_value(const extended_attributes& n);
OSSIA_EXPORT void set_default_value(extended_attributes& n, optional<ossia::value> v);

//! Represents a "raw" data buffer, that should not be interpreted as a readable string.
//! Only meaningful for strings.
OSSIA_EXPORT extended_type generic_buffer_type();

//! Represents a filesystem path : "c:\windows\virus.exe", "/usr/share/pony.png", etc.
//! Only meaningful for strings.
OSSIA_EXPORT extended_type filesystem_path_type();

//! Means that the array should be interpreted as a fixed float array (e.g. vec2f, etc)
OSSIA_EXPORT extended_type float_array_type();

/**
 * @brief get_attribute Get an attribute of an extended_attributes map.
 *
 * Example :
 * \code
 * auto desc = get_attribute<description>(e, "description");
 * \endcode
 *
 * The value will be default-constructed if it is not found (e.g. empty string, 0...)
 */
template<typename T>
auto get_attribute(
    const extended_attributes& e,
    const std::string& name)
{
  auto it = e.find(name);
  if(it != e.cend())
  {
    auto val = boost::any_cast<T>(&it.value());
    if(val)
      return *val;
  }

  return T{};
}

/**
 * @brief get_optional_attribute Maybe get an attribute of an extended_attributes map.
 *
 * Example :
 * \code
 * auto desc = get_optional_attribute<description>(e, "description");
 * if(desc)
 *   std::cerr << "The description is: " << *desc << "\n";
 * \endcode
 *
 * The value will be an empty \ref optional if it is not found.
 */
template<typename T>
optional<T> get_optional_attribute(
    const extended_attributes& e,
    const std::string& name)
{
  auto it = e.find(name);
  if(it != e.cend())
  {
    auto val = boost::any_cast<T>(&it.value());
    if(val)
      return *val;
  }

  return ossia::none;
}

template<typename T>
void set_attribute(extended_attributes& e, const std::string& str, const T& val)
{ e[str] = val; }

template<typename T>
void set_attribute(extended_attributes& e, const std::string& str, T&& val)
{ e[str] = std::move(val); }

inline OSSIA_EXPORT void set_attribute(
    extended_attributes &e,
    const std::string& str,
    ossia::none_t)
{
  e.erase(str);
}

template<typename T>
void set_optional_attribute(
    extended_attributes &e,
    const std::string& str, const optional<T>& opt)
{
  if(opt)
    set_attribute(e, str, *opt);
  else
    set_attribute(e, str, ossia::none);
}

}}
