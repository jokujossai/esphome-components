import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_OPTIONS

from .. import (
    panasonic_aquarea_ns,
    CHILD_SCHEMA_BASE,
    validate_topic,
    CONF_PANASONIC_AQUAREA_ID,
    get_decoder,
    get_encoder,
)
from ..topics import get_topic

CONF_TOPIC = "topic"

PanasonicAquareaSelect = panasonic_aquarea_ns.class_(
    "PanasonicAquareaSelect", select.Select
)

def validate_select_options(config):
    """Validate and set options from topic definition"""
    topic = get_topic(config[CONF_TOPIC])
    
    if not hasattr(topic, 'options') or topic.options is None:
        raise cv.Invalid(f"Topic {topic.name} does not have select options defined")
    
    # Convert options dict to list of strings for ESPHome select
    if topic.options is None:
        raise cv.Invalid(f"Topic {topic.name} does not have select options defined")
    config[CONF_OPTIONS] = topic.options
    
    return config

CONFIG_SCHEMA = cv.All(
    validate_topic("select"),
    validate_select_options,
    select.select_schema(PanasonicAquareaSelect)
    .extend(CHILD_SCHEMA_BASE)
    .extend({
        cv.Required(CONF_OPTIONS): cv.All(
            cv.ensure_list(cv.string_strict),
            cv.Length(min=1),
        ),
    }),
)


async def to_code(config):
    var = await select.new_select(config, options=config[CONF_OPTIONS])

    cg.add(var.set_topic(config[CONF_TOPIC]))

    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
    cg.add(var.set_parent(parent))

    # Get the decoder for the topic
    decoder_id = get_decoder(config[CONF_PANASONIC_AQUAREA_ID], "main")
    decoder = await cg.get_variable(decoder_id)
    cg.add(var.set_decoder(decoder))
    cg.add(decoder.add_select(var))

    # Set up encoder for command handling
    encoder_id = get_encoder(config[CONF_PANASONIC_AQUAREA_ID], "main")
    encoder = await cg.get_variable(encoder_id)
    cg.add(var.set_encoder(encoder))
    cg.add(encoder.add_select(var))
    
    return var