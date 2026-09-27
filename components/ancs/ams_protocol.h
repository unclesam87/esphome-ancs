#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

namespace esphome {
namespace ancs {
namespace ams {

enum class PlaybackState : uint8_t { PAUSED, PLAYING, REWINDING, FAST_FORWARDING };

struct PlaybackInfo {
  PlaybackState state{PlaybackState::PAUSED};
  float rate{0};
  float elapsed{0};
};

struct EntityUpdate {
  uint8_t entity{0};
  uint8_t attribute{0};
  bool truncated{false};
  std::string value;
};

bool parse_playback_info(const std::string &value, PlaybackInfo &out);
bool parse_entity_update(const uint8_t *data, size_t len, EntityUpdate &out);
const char *playback_state_name(PlaybackState state);

}  // namespace ams
}  // namespace ancs
}  // namespace esphome
