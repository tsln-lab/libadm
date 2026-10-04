#include <catch2/catch.hpp>
#include <sstream>
#include "adm/document.hpp"
#include "adm/elements/audio_channel_format.hpp"
#include "adm/elements/frequency.hpp"
#include "adm/parse.hpp"
#include "adm/errors.hpp"
#include "adm/write.hpp"
#include "helper/file_comparator.hpp"

using namespace adm;

TEST_CASE("xml/audio_block_format_objects") {
  auto document = parseXml("xml_parser/audio_block_format_objects.xml");
  auto channelFormat =
      document->lookup(parseAudioChannelFormatId("AC_00031001"));
  REQUIRE(channelFormat->get<AudioChannelFormatId>()
              .get<AudioChannelFormatIdValue>() == 0x1001u);
  REQUIRE(channelFormat->get<AudioChannelFormatId>().get<TypeDescriptor>() ==
          TypeDefinition::OBJECTS);
  REQUIRE(channelFormat->get<AudioChannelFormatName>() == "MyChannelFormat");
  REQUIRE(channelFormat->get<TypeDescriptor>() == TypeDefinition::OBJECTS);

  auto blocksIter =
      channelFormat->getElements<AudioBlockFormatObjects>().begin();

  auto firstBlockFormat = *blocksIter++;
  REQUIRE(firstBlockFormat.get<Width>().get() == Approx(45.0f));
  REQUIRE(firstBlockFormat.get<Height>() == Approx(20.0f));
  REQUIRE(firstBlockFormat.get<Depth>() == Approx(0.2f));
  REQUIRE(firstBlockFormat.get<Gain>().isLinear());
  REQUIRE(firstBlockFormat.get<Gain>().asLinear() == Approx(0.8f));
  REQUIRE(firstBlockFormat.get<Diffuse>() == Approx(0.5f));
  REQUIRE(firstBlockFormat.get<ChannelLock>().get<ChannelLockFlag>() == true);
  REQUIRE(firstBlockFormat.get<ChannelLock>().get<MaxDistance>() ==
          Approx(1.f));
  REQUIRE(firstBlockFormat.get<ObjectDivergence>().get<Divergence>() ==
          Approx(0.5f));
  REQUIRE(firstBlockFormat.get<ObjectDivergence>().get<AzimuthRange>() ==
          Approx(60.f));
  REQUIRE(firstBlockFormat.get<ObjectDivergence>().get<PositionRange>() ==
          Approx(0.25f));
  REQUIRE(firstBlockFormat.get<JumpPosition>().get<JumpPositionFlag>() == true);
  REQUIRE(
      firstBlockFormat.get<JumpPosition>().get<InterpolationLength>().get() ==
      std::chrono::milliseconds(200));
  REQUIRE(firstBlockFormat.get<ScreenRef>() == true);
  auto zones = firstBlockFormat.get<ZoneExclusion>().get<Zones>();
  REQUIRE(zones.size() == 2);
  REQUIRE(isCartesian(zones[0]));
  auto cartesianZone = boost::get<CartesianZone>(zones[0]);
  REQUIRE(cartesianZone.get<MinX>() == Approx(-1.f));
  REQUIRE(cartesianZone.get<MaxX>() == Approx(1.f));
  REQUIRE(cartesianZone.get<MinY>() == Approx(-1.f));
  REQUIRE(cartesianZone.get<MaxY>() == Approx(0.f));
  REQUIRE(cartesianZone.get<MinZ>() == Approx(-1.f));
  REQUIRE(cartesianZone.get<MaxZ>() == Approx(1.f));
  REQUIRE(cartesianZone.has<ZoneLabel>());
  REQUIRE(cartesianZone.get<ZoneLabel>() == "Rear");
  REQUIRE(isPolar(zones[1]));
  auto polarZone = boost::get<PolarZone>(zones[1]);
  REQUIRE(polarZone.get<MinElevation>() == Approx(30.f));
  REQUIRE(polarZone.get<MaxElevation>() == Approx(90.f));
  REQUIRE(polarZone.get<MinAzimuth>() == Approx(-180.f));
  REQUIRE(polarZone.get<MaxAzimuth>() == Approx(180.f));
  REQUIRE(!polarZone.has<ZoneLabel>());
  REQUIRE(firstBlockFormat.get<Importance>() == 10);
  REQUIRE(firstBlockFormat.get<HeadphoneVirtualise>().get<Bypass>() == false);
  REQUIRE(firstBlockFormat.get<HeadphoneVirtualise>()
              .get<DirectToReverberantRatio>() == -60);
  REQUIRE(firstBlockFormat.get<HeadLocked>() == false);

  auto secondBlockFormat = *blocksIter++;
  REQUIRE(secondBlockFormat.isDefault<ZoneExclusion>());
  REQUIRE(secondBlockFormat.get<ZoneExclusion>().get<Zones>().empty());
  REQUIRE(secondBlockFormat.get<ScreenRef>() == false);
  REQUIRE(secondBlockFormat.get<JumpPosition>().get<JumpPositionFlag>() ==
          false);
  REQUIRE(secondBlockFormat.get<Gain>().isDb());
  REQUIRE(secondBlockFormat.get<Gain>().asDb() == -6.0);
  REQUIRE(secondBlockFormat.get<HeadLocked>() == true);

  auto thirdBlockFormat = *blocksIter++;
  REQUIRE(thirdBlockFormat.get<Gain>().isLinear());
  REQUIRE(thirdBlockFormat.get<Gain>().asLinear() == 0.5);

  REQUIRE(secondBlockFormat.get<HeadphoneVirtualise>().get<Bypass>() == true);
  REQUIRE(secondBlockFormat.get<HeadphoneVirtualise>()
              .get<DirectToReverberantRatio>() == 60);

  SECTION("writer") {
    std::stringstream xml;
    writeXml(xml, document);

    CHECK_THAT(xml.str(), EqualsXmlFile("write_audio_block_format_objects"));
  }
}

TEST_CASE("xml_parser/audio_block_format_objects_gain_unit_error") {
  REQUIRE_THROWS_AS(
      parseXml("xml_parser/audio_block_format_objects_gain_unit_error.xml"),
      error::XmlParsingUnexpectedAttrError);
}

namespace {
  std::string objectsBlockWithZone(const std::string& zone) {
    return "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
           "<ebuCoreMain><coreMetadata><format><audioFormatExtended>"
           "<audioChannelFormat audioChannelFormatID=\"AC_00031001\" "
           "audioChannelFormatName=\"Zoned\" typeDefinition=\"Objects\">"
           "<audioBlockFormat audioBlockFormatID=\"AB_00031001_00000001\">"
           "<position coordinate=\"azimuth\">0.0</position>"
           "<position coordinate=\"elevation\">0.0</position>"
           "<zoneExclusion>" +
           zone +
           "</zoneExclusion>"
           "</audioBlockFormat></audioChannelFormat>"
           "</audioFormatExtended></format></coreMetadata></ebuCoreMain>";
  }
}  // namespace

TEST_CASE("xml_parser/audio_block_format_objects_zone_errors") {
  SECTION("both coordinate systems") {
    std::istringstream xml(objectsBlockWithZone(
        "<zone minX=\"-1\" maxX=\"1\" minY=\"-1\" maxY=\"1\" minZ=\"-1\" "
        "maxZ=\"1\" minElevation=\"0\" maxElevation=\"90\" minAzimuth=\"-180\" "
        "maxAzimuth=\"180\"/>"));
    REQUIRE_THROWS_AS(parseXml(xml), error::XmlParsingError);
  }
  SECTION("no coordinates") {
    std::istringstream xml(objectsBlockWithZone("<zone>Nowhere</zone>"));
    REQUIRE_THROWS_AS(parseXml(xml), error::XmlParsingError);
  }
  SECTION("incomplete Cartesian zone") {
    std::istringstream xml(objectsBlockWithZone(
        "<zone minX=\"-1\" maxX=\"1\" minY=\"-1\" maxY=\"1\"/>"));
    REQUIRE_THROWS(parseXml(xml));
  }
  SECTION("out of range") {
    std::istringstream xml(objectsBlockWithZone(
        "<zone minElevation=\"0\" maxElevation=\"91\" minAzimuth=\"-180\" "
        "maxAzimuth=\"180\"/>"));
    REQUIRE_THROWS_AS(parseXml(xml), OutOfRangeError);
  }
  SECTION("not a number") {
    std::istringstream xml(objectsBlockWithZone(
        "<zone minElevation=\"nan\" maxElevation=\"90\" minAzimuth=\"-180\" "
        "maxAzimuth=\"180\"/>"));
    REQUIRE_THROWS_AS(parseXml(xml), OutOfRangeError);
  }
}
