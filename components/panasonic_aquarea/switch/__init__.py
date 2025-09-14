import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.core import coroutine

from .. import (
  panasonic_aquarea_ns,
  CHILD_SCHEMA_BASE,
  validate_topic,
  CONF_PANASONIC_AQUAREA_ID,
  get_decoder
)

CONF_TOPIC = "topic"

PanasonicAquareaSwitch = panasonic_aquarea_ns.class_("PanasonicAquareaSwitch", switch.Switch)

CONFIG_SCHEMA = cv.All(
  validate_topic("switch"),
  switch.switch_schema(PanasonicAquareaSwitch)
  .extend(CHILD_SCHEMA_BASE)
)

async def to_code(config):
  var = await switch.new_switch(config)

  cg.add(var.set_topic(config[CONF_TOPIC]))

  parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
  cg.add(var.set_parent(parent))

  decoder_id = get_decoder(config[CONF_PANASONIC_AQUAREA_ID], "main")
  decoder = await cg.get_variable(decoder_id)
  cg.add(var.set_decoder(decoder))
  cg.add(decoder.add_switch(var))

  # TODO: Encoder

  return var