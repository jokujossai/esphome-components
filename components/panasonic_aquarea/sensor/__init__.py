import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from .. import (
    panasonic_aquarea_ns,
    CHILD_SCHEMA_BASE,
    validate_topic,
    CONF_PANASONIC_AQUAREA_ID,
    get_decoder,
)

CONF_TOPIC = "topic"

PanasonicAquareaSensor = panasonic_aquarea_ns.class_(
    "PanasonicAquareaSensor", sensor.Sensor
)

CONFIG_SCHEMA = cv.All(
    validate_topic("sensor"),
    sensor.sensor_schema(PanasonicAquareaSensor).extend(CHILD_SCHEMA_BASE),
)


async def to_code(config):
    var = await sensor.new_sensor(config)

    cg.add(var.set_topic(config[CONF_TOPIC]))

    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
    cg.add(var.set_parent(parent))

    # Get the decoder for the topic
    decoder_id = get_decoder(config[CONF_PANASONIC_AQUAREA_ID], "main")
    decoder = await cg.get_variable(decoder_id)
    cg.add(var.set_decoder(decoder))
    cg.add(decoder.add_sensor(var))

    return var