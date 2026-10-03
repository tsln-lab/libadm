#include <catch2/catch.hpp>
#include <sstream>
#include "adm/elements/zone_exclusion.hpp"
#include "helper/parameter_checks.hpp"

using namespace adm;

TEST_CASE("zone_exclusion/cartesian_zone") {
  using namespace adm_test;
  auto zone = CartesianZone{MinX{-1.f}, MaxX{1.f},  MinY{-1.f},
                            MaxY{0.f},  MinZ{-1.f}, MaxZ{1.f}};
  check_required_param<MinX>(zone, hasDefaultOf(-1.f), canBeSetTo(-0.5f));
  check_required_param<MaxX>(zone, hasDefaultOf(1.f), canBeSetTo(0.5f));
  check_required_param<MinY>(zone, hasDefaultOf(-1.f), canBeSetTo(-0.5f));
  check_required_param<MaxY>(zone, hasDefaultOf(0.f), canBeSetTo(0.5f));
  check_required_param<MinZ>(zone, hasDefaultOf(-1.f), canBeSetTo(-0.5f));
  check_required_param<MaxZ>(zone, hasDefaultOf(1.f), canBeSetTo(0.5f));
  check_optional_param<ZoneLabel>(zone, canBeSetTo("Rear"));

  SECTION("label in the constructor") {
    auto labelled =
        CartesianZone{MinX{-1.f}, MaxX{1.f}, MinY{-1.f},       MaxY{0.f},
                      MinZ{-1.f}, MaxZ{1.f}, ZoneLabel{"Rear"}};
    REQUIRE(labelled.has<ZoneLabel>());
    REQUIRE(labelled.get<ZoneLabel>() == "Rear");
    REQUIRE(labelled != zone);
    labelled.unset<ZoneLabel>();
    REQUIRE(labelled == zone);
  }

  SECTION("range") {
    REQUIRE_THROWS_AS(MinX{-1.5f}, OutOfRangeError);
    REQUIRE_THROWS_AS(MaxZ{1.5f}, OutOfRangeError);
  }

  SECTION("print") {
    std::stringstream out;
    zone.print(out);
    REQUIRE(out.str() == "(minX=-1, maxX=1, minY=-1, maxY=0, minZ=-1, maxZ=1)");
  }
}

TEST_CASE("zone_exclusion/polar_zone") {
  using namespace adm_test;
  auto zone = PolarZone{MinElevation{-90.f}, MaxElevation{-30.f},
                        MinAzimuth{-180.f}, MaxAzimuth{180.f}};
  check_required_param<MinElevation>(zone, hasDefaultOf(-90.f),
                                     canBeSetTo(-45.f));
  check_required_param<MaxElevation>(zone, hasDefaultOf(-30.f),
                                     canBeSetTo(0.f));
  check_required_param<MinAzimuth>(zone, hasDefaultOf(-180.f),
                                   canBeSetTo(-90.f));
  check_required_param<MaxAzimuth>(zone, hasDefaultOf(180.f), canBeSetTo(90.f));
  check_optional_param<ZoneLabel>(zone, canBeSetTo("Below"));

  SECTION("range") {
    REQUIRE_THROWS_AS(MinElevation{-91.f}, OutOfRangeError);
    REQUIRE_THROWS_AS(MaxAzimuth{181.f}, OutOfRangeError);
  }

  SECTION("print") {
    std::stringstream out;
    PolarZone{MinElevation{-90.f}, MaxElevation{-30.f}, MinAzimuth{-180.f},
              MaxAzimuth{180.f}, ZoneLabel{"Below"}}
        .print(out);
    REQUIRE(out.str() ==
            "(minElevation=-90, maxElevation=-30, minAzimuth=-180, "
            "maxAzimuth=180, label=Below)");
  }
}

TEST_CASE("zone_exclusion/zones") {
  auto cartesian = Zone{CartesianZone{MinX{-1.f}, MaxX{1.f}, MinY{-1.f},
                                      MaxY{0.f}, MinZ{-1.f}, MaxZ{1.f}}};
  auto polar = Zone{PolarZone{MinElevation{30.f}, MaxElevation{90.f},
                              MinAzimuth{-180.f}, MaxAzimuth{180.f}}};
  REQUIRE(isCartesian(cartesian));
  REQUIRE(!isPolar(cartesian));
  REQUIRE(isPolar(polar));
  REQUIRE(!isCartesian(polar));
  REQUIRE(cartesian != polar);

  ZoneExclusion zoneExclusion;
  REQUIRE(!zoneExclusion.has<Zones>());
  REQUIRE(zoneExclusion.get<Zones>().empty());
  REQUIRE(zoneExclusion == ZoneExclusion{});

  SECTION("add and remove") {
    REQUIRE(zoneExclusion.add(cartesian));
    REQUIRE(zoneExclusion.add(polar));
    REQUIRE(!zoneExclusion.add(polar));
    REQUIRE(zoneExclusion.has<Zones>());
    REQUIRE(zoneExclusion.get<Zones>().size() == 2);
    REQUIRE(zoneExclusion.get<Zones>()[0] == cartesian);
    REQUIRE(zoneExclusion.get<Zones>()[1] == polar);
    REQUIRE(zoneExclusion != ZoneExclusion{});

    zoneExclusion.remove(cartesian);
    REQUIRE(zoneExclusion.get<Zones>() == Zones{polar});
    zoneExclusion.unset<Zones>();
    REQUIRE(zoneExclusion.get<Zones>().empty());
  }

  SECTION("set in the constructor") {
    auto fromZones = ZoneExclusion{Zones{cartesian, polar}};
    REQUIRE(fromZones.get<Zones>() == Zones{cartesian, polar});
    fromZones.set(Zones{polar});
    REQUIRE(fromZones.get<Zones>() == Zones{polar});
  }

  SECTION("print") {
    zoneExclusion.add(polar);
    std::stringstream out;
    zoneExclusion.print(out);
    REQUIRE(out.str() ==
            "(zones=[polar(minElevation=30, maxElevation=90, "
            "minAzimuth=-180, maxAzimuth=180)])");
  }
}
