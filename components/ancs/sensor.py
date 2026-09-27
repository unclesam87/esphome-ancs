# SPDX-License-Identifier: MIT
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import DEVICE_CLASS_DURATION, UNIT_SECOND

from . import AncsComponent

CONF_ANCS_ID = "ancs_id"
CONF_MEDIA_POSITION = "media_position"
CONF_MEDIA_DURATION = "media_duration"
CONF_MEDIA_PLAYBACK_RATE = "media_playback_rate"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ANCS_ID): cv.use_id(AncsComponent),
        cv.Optional(CONF_MEDIA_POSITION): sensor.sensor_schema(unit_of_measurement=UNIT_SECOND, accuracy_decimals=1),
        cv.Optional(CONF_MEDIA_DURATION): sensor.sensor_schema(
            unit_of_measurement=UNIT_SECOND, accuracy_decimals=1, device_class=DEVICE_CLASS_DURATION
        ),
        cv.Optional(CONF_MEDIA_PLAYBACK_RATE): sensor.sensor_schema(accuracy_decimals=2),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ANCS_ID])
    for key, setter in {
        CONF_MEDIA_POSITION: "set_media_position_sensor",
        CONF_MEDIA_DURATION: "set_media_duration_sensor",
        CONF_MEDIA_PLAYBACK_RATE: "set_media_playback_rate_sensor",
    }.items():
        if key in config:
            s = await sensor.new_sensor(config[key])
            cg.add(getattr(parent, setter)(s))
