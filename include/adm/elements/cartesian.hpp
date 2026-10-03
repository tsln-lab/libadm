/// @file cartesian.hpp
#pragma once

#include "adm/detail/named_type.hpp"

namespace adm {

  /// @brief Tag for NamedType ::Cartesian
  struct CartesianTag {};
  /// @brief NamedType for the cartesian flag of an audioBlockFormat
  using Cartesian = detail::NamedType<bool, CartesianTag>;

}  // namespace adm
