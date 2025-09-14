import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import (
  CONF_ID,
  CONF_NAME,
)
from esphome.core import coroutine
from esphome.util import Registry

from .topics import get_topic, generate_topic_mapping

CONF_LISTEN_ONLY = "listen_only"
CONF_DECODERS= "decoders"
CONF_DECODER_ID = "decoder_id"
CONF_PANASONIC_AQUAREA_ID = "panasonic_aquarea_id"
CONF_TOPIC = "topic"

DEPENDENCIES = ["uart"]

panasonic_aquarea_ns = cg.esphome_ns.namespace("panasonic_aquarea")
PanasonicAquareaComponent = panasonic_aquarea_ns.class_(
  "PanasonicAquareaComponent", cg.Component, uart.UARTDevice
)

PanasonicAquareaDecoder = panasonic_aquarea_ns.class_("PanasonicAquareaDecoder")
PanasonicAquareaMainDecoder = panasonic_aquarea_ns.class_(
  "PanasonicAquareaMainDecoder", PanasonicAquareaDecoder
)

PanasonicAquareaTopic = panasonic_aquarea_ns.enum("PanasonicAquareaTopic")

DECODER_REGISTRY = Registry({
  cv.GenerateID(CONF_DECODER_ID): cv.use_id(PanasonicAquareaDecoder),
})

async def build_decoders(config):
  decoders = []
  for conf in config:
    decoder = await cg.build_registry_entry(DECODER_REGISTRY, conf)
    decoders.append(decoder)
  return decoders

def validate_decoders(value):
  if isinstance(value, str) and value.lower() == "all":
    return validate_decoders(list(DECODER_REGISTRY.keys()))
  return cv.validate_registry("decoder", DECODER_REGISTRY)(value)

def register_decoder(name, type):
  registerer = DECODER_REGISTRY.register(name, type, {})

  def decorator(func):
    async def new_func(config, decoder_id):
      var = cg.new_Pvariable(decoder_id)
      await coroutine(func)(var, config)
      return var

    return registerer(new_func)

  return decorator

DECODERS = {}

def collect_decoders(config):
  DECODERS[config[CONF_ID]] = config[CONF_DECODERS]
  return config

def get_decoder(id, name):
  for k, decoders in DECODERS.items():
    if k.id != id.id:
      continue
    for decoder in decoders:
      if name in decoder:
        return decoder[CONF_DECODER_ID]
    raise cv.Invalid(f"Decoder {name} not found")
  raise cv.Invalid(f"Component {id.id} not found")

@register_decoder("main", PanasonicAquareaMainDecoder)
def main_decoder(var, config):
  pass


CONFIG_SCHEMA = cv.All(
  cv.Schema(
    {
      cv.GenerateID(): cv.declare_id(PanasonicAquareaComponent),
      cv.Optional(CONF_LISTEN_ONLY, default=False): cv.boolean,
      cv.Optional(CONF_DECODERS, default=["main"]): validate_decoders,
    }
  )
  .extend(cv.COMPONENT_SCHEMA)
  .extend(uart.UART_DEVICE_SCHEMA),
  collect_decoders
)

def validate_topic(type):
  def validator(value):
    if CONF_TOPIC not in value:
      raise cv.Invalid(f"Topic {CONF_TOPIC} is required")

    topic = get_topic(value[CONF_TOPIC])

    # TODO: switch -> binary_sensor is allowed
    if topic.type != type:
      raise cv.Invalid(f"Topic {topic.name} is not a {type} topic")

    if CONF_NAME not in value and CONF_ID not in value:
      value[CONF_NAME] = topic.name

    value[CONF_TOPIC] = topic.name

    return value

  return validator


CHILD_SCHEMA_BASE = (
  cv.Schema({
    cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquareaComponent),
    cv.Required(CONF_TOPIC): cv.enum(generate_topic_mapping(PanasonicAquareaTopic)),
  })
)

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])

  decoders = await build_decoders(config[CONF_DECODERS])
  for decoder in decoders:
    cg.add(var.add_decoder(decoder))

  await cg.register_component(var, config)
  await uart.register_uart_device(var, config)
  
  if CONF_LISTEN_ONLY in config:
    cg.add(var.set_listen_only(config[CONF_LISTEN_ONLY]))

  return var
