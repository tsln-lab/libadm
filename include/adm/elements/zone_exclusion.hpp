/// @file zone_exclusion.hpp
#pragma once

#include <boost/variant.hpp>
#include <iosfwd>
#include <string>
#include <vector>
#include "adm/detail/auto_base.hpp"
#include "adm/detail/named_option_helper.hpp"
#include "adm/detail/named_type.hpp"
#include "adm/detail/optional_comparison.hpp"
#include "adm/detail/type_traits.hpp"
#include "adm/export.h"

namespace adm {

  /// @brief Tag for NamedType ::ZoneLabel
  struct ZoneLabelTag {};
  /// @brief NamedType for the value of a zone element, a name for the zone
  using ZoneLabel = detail::NamedType<std::string, ZoneLabelTag>;

  /// @brief Tag for NamedType ::MinX
  struct MinXTag {};
  /// @brief NamedType for the minX zone attribute, in the range [-1, 1]
  using MinX = detail::NamedType<float, MinXTag, detail::RangeValidator<-1, 1>>;
  /// @brief Tag for NamedType ::MaxX
  struct MaxXTag {};
  /// @brief NamedType for the maxX zone attribute, in the range [-1, 1]
  using MaxX = detail::NamedType<float, MaxXTag, detail::RangeValidator<-1, 1>>;
  /// @brief Tag for NamedType ::MinY
  struct MinYTag {};
  /// @brief NamedType for the minY zone attribute, in the range [-1, 1]
  using MinY = detail::NamedType<float, MinYTag, detail::RangeValidator<-1, 1>>;
  /// @brief Tag for NamedType ::MaxY
  struct MaxYTag {};
  /// @brief NamedType for the maxY zone attribute, in the range [-1, 1]
  using MaxY = detail::NamedType<float, MaxYTag, detail::RangeValidator<-1, 1>>;
  /// @brief Tag for NamedType ::MinZ
  struct MinZTag {};
  /// @brief NamedType for the minZ zone attribute, in the range [-1, 1]
  using MinZ = detail::NamedType<float, MinZTag, detail::RangeValidator<-1, 1>>;
  /// @brief Tag for NamedType ::MaxZ
  struct MaxZTag {};
  /// @brief NamedType for the maxZ zone attribute, in the range [-1, 1]
  using MaxZ = detail::NamedType<float, MaxZTag, detail::RangeValidator<-1, 1>>;

  /// @brief Tag for NamedType ::MinElevation
  struct MinElevationTag {};
  /// @brief NamedType for the minElevation zone attribute, in the range
  /// [-90, 90]
  using MinElevation = detail::NamedType<float, MinElevationTag,
                                         detail::RangeValidator<-90, 90>>;
  /// @brief Tag for NamedType ::MaxElevation
  struct MaxElevationTag {};
  /// @brief NamedType for the maxElevation zone attribute, in the range
  /// [-90, 90]
  using MaxElevation = detail::NamedType<float, MaxElevationTag,
                                         detail::RangeValidator<-90, 90>>;
  /// @brief Tag for NamedType ::MinAzimuth
  struct MinAzimuthTag {};
  /// @brief NamedType for the minAzimuth zone attribute, in the range
  /// [-180, 180]
  using MinAzimuth = detail::NamedType<float, MinAzimuthTag,
                                       detail::RangeValidator<-180, 180>>;
  /// @brief Tag for NamedType ::MaxAzimuth
  struct MaxAzimuthTag {};
  /// @brief NamedType for the maxAzimuth zone attribute, in the range
  /// [-180, 180]
  using MaxAzimuth = detail::NamedType<float, MaxAzimuthTag,
                                       detail::RangeValidator<-180, 180>>;

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS RequiredParameter<MinX>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS RequiredParameter<MaxX>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS RequiredParameter<MinY>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS RequiredParameter<MaxY>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS RequiredParameter<MinZ>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS RequiredParameter<MaxZ>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<MinElevation>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<MaxElevation>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<MinAzimuth>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        RequiredParameter<MaxAzimuth>;
    extern template class ADM_EXPORT_TEMPLATE_METHODS
        OptionalParameter<ZoneLabel>;

    using CartesianZoneBase =
        HasParameters<RequiredParameter<MinX>, RequiredParameter<MaxX>,
                      RequiredParameter<MinY>, RequiredParameter<MaxY>,
                      RequiredParameter<MinZ>, RequiredParameter<MaxZ>,
                      OptionalParameter<ZoneLabel>>;

    using PolarZoneBase = HasParameters<
        RequiredParameter<MinElevation>, RequiredParameter<MaxElevation>,
        RequiredParameter<MinAzimuth>, RequiredParameter<MaxAzimuth>,
        OptionalParameter<ZoneLabel>>;
  }  // namespace detail

  /// @brief Tag for CartesianZone
  struct CartesianZoneTag {};
  /**
   * @brief An exclusion zone given in Cartesian coordinates: a zone element
   * with the attributes minX, maxX, minY, maxY, minZ and maxZ
   *
   * Supported parameters are as follows:
   *
   * \rst
   * +---------------+----------------------+----------------------------+
   * | ADM Parameter | Parameter Type       | Pattern Type               |
   * +===============+======================+============================+
   * | minX          | :type:`MinX`         | :class:`RequiredParameter` |
   * +---------------+----------------------+----------------------------+
   * | maxX          | :type:`MaxX`         | :class:`RequiredParameter` |
   * +---------------+----------------------+----------------------------+
   * | minY          | :type:`MinY`         | :class:`RequiredParameter` |
   * +---------------+----------------------+----------------------------+
   * | maxY          | :type:`MaxY`         | :class:`RequiredParameter` |
   * +---------------+----------------------+----------------------------+
   * | minZ          | :type:`MinZ`         | :class:`RequiredParameter` |
   * +---------------+----------------------+----------------------------+
   * | maxZ          | :type:`MaxZ`         | :class:`RequiredParameter` |
   * +---------------+----------------------+----------------------------+
   * | (value)       | :type:`ZoneLabel`    | :class:`OptionalParameter` |
   * +---------------+----------------------+----------------------------+
   * \endrst
   */
  class CartesianZone : private detail::CartesianZoneBase,
                        private detail::AddWrapperMethods<CartesianZone> {
   public:
    using tag = CartesianZoneTag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    CartesianZone(MinX minX, MaxX maxX, MinY minY, MaxY maxY, MinZ minZ,
                  MaxZ maxZ, Parameters... optionalNamedArgs) {
      set(minX);
      set(maxX);
      set(minY);
      set(maxY);
      set(minZ);
      set(maxZ);
      detail::setNamedOptionHelper(this, std::move(optionalNamedArgs)...);
    }

    /// @brief Print overview to ostream
    ADM_EXPORT void print(std::ostream& os) const;

    using detail::CartesianZoneBase::set;
    using detail::AddWrapperMethods<CartesianZone>::get;
    using detail::AddWrapperMethods<CartesianZone>::has;
    using detail::AddWrapperMethods<CartesianZone>::isDefault;
    using detail::AddWrapperMethods<CartesianZone>::unset;

   private:
    using detail::CartesianZoneBase::get;
    using detail::CartesianZoneBase::has;
    using detail::CartesianZoneBase::isDefault;
    using detail::CartesianZoneBase::unset;

    friend class detail::AddWrapperMethods<CartesianZone>;
  };

  /// @brief Tag for PolarZone
  struct PolarZoneTag {};
  /**
   * @brief An exclusion zone given in polar coordinates: a zone element with
   * the attributes minElevation, maxElevation, minAzimuth and maxAzimuth
   *
   * Supported parameters are as follows:
   *
   * \rst
   * +---------------+-----------------------+----------------------------+
   * | ADM Parameter | Parameter Type        | Pattern Type               |
   * +===============+=======================+============================+
   * | minElevation  | :type:`MinElevation`  | :class:`RequiredParameter` |
   * +---------------+-----------------------+----------------------------+
   * | maxElevation  | :type:`MaxElevation`  | :class:`RequiredParameter` |
   * +---------------+-----------------------+----------------------------+
   * | minAzimuth    | :type:`MinAzimuth`    | :class:`RequiredParameter` |
   * +---------------+-----------------------+----------------------------+
   * | maxAzimuth    | :type:`MaxAzimuth`    | :class:`RequiredParameter` |
   * +---------------+-----------------------+----------------------------+
   * | (value)       | :type:`ZoneLabel`     | :class:`OptionalParameter` |
   * +---------------+-----------------------+----------------------------+
   * \endrst
   */
  class PolarZone : private detail::PolarZoneBase,
                    private detail::AddWrapperMethods<PolarZone> {
   public:
    using tag = PolarZoneTag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order after the mandatory ADM parameters.
     */
    template <typename... Parameters>
    PolarZone(MinElevation minElevation, MaxElevation maxElevation,
              MinAzimuth minAzimuth, MaxAzimuth maxAzimuth,
              Parameters... optionalNamedArgs) {
      set(minElevation);
      set(maxElevation);
      set(minAzimuth);
      set(maxAzimuth);
      detail::setNamedOptionHelper(this, std::move(optionalNamedArgs)...);
    }

    /// @brief Print overview to ostream
    ADM_EXPORT void print(std::ostream& os) const;

    using detail::PolarZoneBase::set;
    using detail::AddWrapperMethods<PolarZone>::get;
    using detail::AddWrapperMethods<PolarZone>::has;
    using detail::AddWrapperMethods<PolarZone>::isDefault;
    using detail::AddWrapperMethods<PolarZone>::unset;

   private:
    using detail::PolarZoneBase::get;
    using detail::PolarZoneBase::has;
    using detail::PolarZoneBase::isDefault;
    using detail::PolarZoneBase::unset;

    friend class detail::AddWrapperMethods<PolarZone>;
  };

  inline std::ostream& operator<<(std::ostream& os, const CartesianZone& zone) {
    zone.print(os);
    return os;
  }
  inline std::ostream& operator<<(std::ostream& os, const PolarZone& zone) {
    zone.print(os);
    return os;
  }

  inline bool operator==(const CartesianZone& lhs, const CartesianZone& rhs) {
    return detail::optionalsEqual<MinX, MaxX, MinY, MaxY, MinZ, MaxZ,
                                  ZoneLabel>(lhs, rhs);
  }
  inline bool operator!=(const CartesianZone& lhs, const CartesianZone& rhs) {
    return !(lhs == rhs);
  }
  inline bool operator==(const PolarZone& lhs, const PolarZone& rhs) {
    return detail::optionalsEqual<MinElevation, MaxElevation, MinAzimuth,
                                  MaxAzimuth, ZoneLabel>(lhs, rhs);
  }
  inline bool operator!=(const PolarZone& lhs, const PolarZone& rhs) {
    return !(lhs == rhs);
  }

  /// @brief Tag for Zone
  struct ZoneTag {};
  /// @brief A zone element of a zoneExclusion, either Cartesian or polar
  using Zone = boost::variant<CartesianZone, PolarZone>;
  ADD_TRAIT(Zone, ZoneTag);

  /// @brief Tag for Zones
  struct ZonesTag {};
  /// @brief The zone elements of a zoneExclusion
  using Zones = std::vector<Zone>;
  ADD_TRAIT(Zones, ZonesTag);

  /// @brief Whether a zone is given in Cartesian coordinates
  ADM_EXPORT bool isCartesian(const Zone& zone);
  /// @brief Whether a zone is given in polar coordinates
  ADM_EXPORT bool isPolar(const Zone& zone);

  namespace detail {
    extern template class ADM_EXPORT_TEMPLATE_METHODS VectorParameter<Zones>;

    using ZoneExclusionBase = HasParameters<VectorParameter<Zones>>;
  }  // namespace detail

  /// @brief Tag for ZoneExclusion
  struct ZoneExclusionTag {};
  /**
   * @brief ADM parameter class to specify the zone exclusion of an Objects
   * audioBlockFormat
   *
   * The zones are the zone sub-elements, each a :type:`Zone`:
   *
   * \rst
   * +---------------+----------------------+----------------------------+
   * | ADM Parameter | Parameter Type       | Pattern Type               |
   * +===============+======================+============================+
   * | zone          | :type:`Zones`        | :class:`VectorParameter`   |
   * +---------------+----------------------+----------------------------+
   * \endrst
   */
  class ZoneExclusion : private detail::ZoneExclusionBase,
                        private detail::AddWrapperMethods<ZoneExclusion> {
   public:
    using tag = ZoneExclusionTag;

    /**
     * @brief Constructor template
     *
     * Templated constructor which accepts a variable number of ADM parameters
     * in random order.
     */
    template <typename... Parameters>
    explicit ZoneExclusion(Parameters... optionalNamedArgs) {
      detail::setNamedOptionHelper(this, std::move(optionalNamedArgs)...);
    }

    /// @brief Print overview to ostream
    ADM_EXPORT void print(std::ostream& os) const;

    using detail::ZoneExclusionBase::add;
    using detail::ZoneExclusionBase::remove;
    using detail::ZoneExclusionBase::set;
    using detail::AddWrapperMethods<ZoneExclusion>::get;
    using detail::AddWrapperMethods<ZoneExclusion>::has;
    using detail::AddWrapperMethods<ZoneExclusion>::isDefault;
    using detail::AddWrapperMethods<ZoneExclusion>::unset;

   private:
    using detail::ZoneExclusionBase::get;
    using detail::ZoneExclusionBase::has;
    using detail::ZoneExclusionBase::isDefault;
    using detail::ZoneExclusionBase::unset;

    friend class detail::AddWrapperMethods<ZoneExclusion>;
  };

  inline std::ostream& operator<<(std::ostream& os,
                                  const ZoneExclusion& zoneExclusion) {
    zoneExclusion.print(os);
    return os;
  }

  inline bool operator==(const ZoneExclusion& lhs, const ZoneExclusion& rhs) {
    return lhs.get<Zones>() == rhs.get<Zones>();
  }
  inline bool operator!=(const ZoneExclusion& lhs, const ZoneExclusion& rhs) {
    return !(lhs == rhs);
  }

}  // namespace adm
