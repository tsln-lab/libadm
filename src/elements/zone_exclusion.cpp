#include "adm/elements/zone_exclusion.hpp"
#include <ostream>

namespace adm {
  namespace detail {
    template class RequiredParameter<MinX>;
    template class RequiredParameter<MaxX>;
    template class RequiredParameter<MinY>;
    template class RequiredParameter<MaxY>;
    template class RequiredParameter<MinZ>;
    template class RequiredParameter<MaxZ>;
    template class RequiredParameter<MinElevation>;
    template class RequiredParameter<MaxElevation>;
    template class RequiredParameter<MinAzimuth>;
    template class RequiredParameter<MaxAzimuth>;
    template class OptionalParameter<ZoneLabel>;
    template class VectorParameter<Zones>;
  }  // namespace detail

  void CartesianZone::print(std::ostream& os) const {
    os << "(minX=" << get<MinX>() << ", maxX=" << get<MaxX>()
       << ", minY=" << get<MinY>() << ", maxY=" << get<MaxY>()
       << ", minZ=" << get<MinZ>() << ", maxZ=" << get<MaxZ>();
    if (has<ZoneLabel>()) {
      os << ", label=" << get<ZoneLabel>();
    }
    os << ")";
  }

  void PolarZone::print(std::ostream& os) const {
    os << "(minElevation=" << get<MinElevation>()
       << ", maxElevation=" << get<MaxElevation>()
       << ", minAzimuth=" << get<MinAzimuth>()
       << ", maxAzimuth=" << get<MaxAzimuth>();
    if (has<ZoneLabel>()) {
      os << ", label=" << get<ZoneLabel>();
    }
    os << ")";
  }

  bool isCartesian(const Zone& zone) {
    return zone.type() == typeid(CartesianZone);
  }

  bool isPolar(const Zone& zone) { return zone.type() == typeid(PolarZone); }

  namespace {
    struct PrintZone : public boost::static_visitor<void> {
      explicit PrintZone(std::ostream& os) : os_(os) {}
      void operator()(const CartesianZone& zone) const {
        os_ << "cartesian";
        zone.print(os_);
      }
      void operator()(const PolarZone& zone) const {
        os_ << "polar";
        zone.print(os_);
      }
      std::ostream& os_;
    };
  }  // namespace

  void ZoneExclusion::print(std::ostream& os) const {
    os << "(zones=[";
    bool first = true;
    for (const auto& zone : get<Zones>()) {
      if (!first) os << ", ";
      first = false;
      boost::apply_visitor(PrintZone(os), zone);
    }
    os << "])";
  }

}  // namespace adm
