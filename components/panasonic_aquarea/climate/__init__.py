import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

from .. import (
    panasonic_aquarea_ns,
    CHILD_SCHEMA_BASE,
    validate_topic,
    CONF_PANASONIC_AQUAREA_ID,
    get_decoder,
    get_encoder,
)

CONF_TOPIC = "topic"

PanasonicAquareaZoneClimate = panasonic_aquarea_ns.class_(
    "PanasonicAquareaZoneClimate", climate.Climate
)

CONFIG_SCHEMA = cv.All(
    validate_topic("climate"),
    climate.CLIMATE_SCHEMA.extend(CHILD_SCHEMA_BASE).extend(
        {
            cv.GenerateID(): cv.declare_id(PanasonicAquareaZoneClimate),
        }
    ),
)


async def to_code(config):
    var = await climate.new_climate(config)

    cg.add(var.set_topic(config[CONF_TOPIC]))

    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
    cg.add(var.set_parent(parent))

    # Get the decoder for the topic
    decoder_id = get_decoder(config[CONF_PANASONIC_AQUAREA_ID], "main")
    decoder = await cg.get_variable(decoder_id)
    cg.add(var.set_decoder(decoder))
    cg.add(decoder.add_climate(var))

    # Get the encoder for the topic
    encoder_id = get_encoder(config[CONF_PANASONIC_AQUAREA_ID], "main")
    encoder = await cg.get_variable(encoder_id)
    cg.add(var.set_encoder(encoder))

    return var