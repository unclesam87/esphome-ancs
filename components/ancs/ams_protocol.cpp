#include "ams_protocol.h"
#include <cmath>
#include <cstdlib>
#include <utility>

namespace esphome {
namespace ancs {
namespace ams {

static bool parse_float(const std::string &value, float &out) {
  if (value.empty()) return false;
  char *end = nullptr;
  float parsed = std::strtof(value.c_str(), &end);
  if (end == value.c_str() || *end != '\0' || !std::isfinite(parsed)) return false;
  out = parsed;
  return true;
}

bool parse_playback_info(const std::string &value, PlaybackInfo &out) {
  size_t first = value.find(',');
  if (first == std::string::npos) return false;
  size_t second = value.find(',', first + 1);
  if (second == std::string::npos || value.find(',', second + 1) != std::string::npos) return false;
  std::string state = value.substr(0, first);
  if (state.size() != 1 || state[0] < '0' || state[0] > '3') return false;
  PlaybackInfo parsed;
  parsed.state = static_cast<PlaybackState>(state[0] - '0');
  if (!parse_float(value.substr(first + 1, second - first - 1), parsed.rate) ||
      !parse_float(value.substr(second + 1), parsed.elapsed)) return false;
  out = parsed;
  return true;
}

bool parse_entity_update(const uint8_t *data, size_t len, EntityUpdate &out) {
  if (data == nullptr || len < 3) return false;
  if (!((data[0] == 0 && data[1] <= 1) || (data[0] == 2 && data[1] <= 3))) return false;
  EntityUpdate parsed;
  parsed.entity = data[0];
  parsed.attribute = data[1];
  parsed.truncated = (data[2] & 1) != 0;
  parsed.value.assign(reinterpret_cast<const char *>(data + 3), len - 3);
  out = std::move(parsed);
  return true;
}

const char *playback_state_name(PlaybackState state) {
  switch (state) {
    case PlaybackState::PAUSED: return "paused";
    case PlaybackState::PLAYING: return "playing";
    case PlaybackState::REWINDING: return "rewinding";
    case PlaybackState::FAST_FORWARDING: return "fast_forwarding";
  }
  return "unknown";
}

}  // namespace ams
}  // namespace ancs
}  // namespace esphome
