#include "doctest.h"
#include "ams_protocol.h"

using namespace esphome::ancs::ams;

TEST_CASE("AMS playback info parses all three fields") {
  PlaybackInfo info{};
  CHECK(parse_playback_info("1,1.5,1234.7", info));
  CHECK(info.state == PlaybackState::PLAYING);
  CHECK(info.rate == doctest::Approx(1.5));
  CHECK(info.elapsed == doctest::Approx(1234.7));
  CHECK_FALSE(parse_playback_info("1,1.5", info));
  CHECK_FALSE(parse_playback_info("4,1,2", info));
  CHECK_FALSE(parse_playback_info("1,nan,2", info));
  CHECK_FALSE(parse_playback_info("1,1,2garbage", info));
}

TEST_CASE("AMS entity update validates IDs and length") {
  EntityUpdate update{};
  const uint8_t title[] = {2, 2, 0, 'T', 'e', 's', 't'};
  CHECK(parse_entity_update(title, sizeof(title), update));
  CHECK(update.entity == 2);
  CHECK(update.attribute == 2);
  CHECK(update.value == "Test");
  const uint8_t truncated[] = {2, 2, 1, 'X'};
  CHECK(parse_entity_update(truncated, sizeof(truncated), update));
  CHECK(update.truncated);
  CHECK_FALSE(parse_entity_update(title, 2, update));
  const uint8_t unknown[] = {1, 0, 0, 'X'};
  CHECK_FALSE(parse_entity_update(unknown, sizeof(unknown), update));
  const uint8_t unknown_attribute[] = {0, 3, 0};
  CHECK_FALSE(parse_entity_update(unknown_attribute, sizeof(unknown_attribute), update));
}
