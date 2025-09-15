import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, mqtt
from esphome.const import (
  CONF_ID,
  CONF_NAME,
  CONF_UNIT_OF_MEASUREMENT,
  CONF_ACCURACY_DECIMALS,
  CONF_STATE_CLASS,
  CONF_MQTT_ID
)
from esphome.core import coroutine
from esphome.util import Registry

from .topics import (
  get_topic,
  generate_topic_mapping,
  TYPE_BINARY_SENSOR,
  TYPE_SENSOR,
  TYPE_SWITCH,
)

CONF_LISTEN_ONLY = "listen_only"
CONF_DECODERS = "decoders"
CONF_DECODER_ID = "decoder_id"
CONF_ENCODERS = "encoders"
CONF_ENCODER_ID = "encoder_id"
CONF_PANASONIC_AQUAREA_ID = "panasonic_aquarea_id"
CONF_TOPIC = "topic"
CONF_SEND_LOG_TOPIC = "send_log_topic"

DEPENDENCIES = ["uart"]

panasonic_aquarea_ns = cg.esphome_ns.namespace("panasonic_aquarea")
PanasonicAquareaComponent = panasonic_aquarea_ns.class_(
  "PanasonicAquareaComponent", cg.Component, uart.UARTDevice
)

PanasonicAquareaDecoder = panasonic_aquarea_ns.class_("PanasonicAquareaDecoder")
PanasonicAquareaDecoderMain = panasonic_aquarea_ns.class_(
  "PanasonicAquareaDecoderMain", PanasonicAquareaDecoder
)

PanasonicAquareaEncoder = panasonic_aquarea_ns.class_("PanasonicAquareaEncoder")
PanasonicAquareaEncoderMain = panasonic_aquarea_ns.class_(
  "PanasonicAquareaEncoderMain", PanasonicAquareaEncoder
)

PanasonicAquareaTopic = panasonic_aquarea_ns.enum("PanasonicAquareaTopic")

DECODER_REGISTRY = Registry({
  cv.GenerateID(CONF_DECODER_ID): cv.use_id(PanasonicAquareaDecoder),
})

ENCODER_REGISTRY = Registry({
  cv.GenerateID(CONF_ENCODER_ID): cv.use_id(PanasonicAquareaEncoder),
  # TODO: Remove on final version, required for testing with HeishaMon
  cv.GenerateID(CONF_MQTT_ID): cv.use_id(mqtt.MQTTClientComponent),
  cv.Optional(CONF_SEND_LOG_TOPIC): cv.publish_topic,
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

async def build_encoders(config):
  encoders = []
  for conf in config:
    encoder = await cg.build_registry_entry(ENCODER_REGISTRY, conf)

    # TODO: Remove on final version, required for testing with HeishaMon
    mqtt_client = await cg.get_variable(conf[CONF_MQTT_ID])
    cg.add(encoder.set_mqtt_client_component(mqtt_client))

    # TODO: Remove on final version, required for testing with HeishaMon
    if CONF_SEND_LOG_TOPIC in conf:
      cg.add(encoder.set_send_log_topic(conf[CONF_SEND_LOG_TOPIC]))

    encoders.append(encoder)
  return encoders

def validate_encoders(value):
  if isinstance(value, str) and value.lower() == "all":
    return validate_encoders(list(ENCODER_REGISTRY.keys()))
  return cv.validate_registry("encoder", ENCODER_REGISTRY)(value)

def register_decoder(name, type):
  registerer = DECODER_REGISTRY.register(name, type, {})

  def decorator(func):
    async def new_func(config, decoder_id):
      var = cg.new_Pvariable(decoder_id)
      await coroutine(func)(var, config)
      return var

    return registerer(new_func)

  return decorator

def register_encoder(name, type):
  registerer = ENCODER_REGISTRY.register(name, type, {})

  def decorator(func):
    async def new_func(config, encoder_id):
      var = cg.new_Pvariable(encoder_id)
      await coroutine(func)(var, config)
      return var

    return registerer(new_func)

  return decorator

DECODERS = {}
ENCODERS = {}

def collect_decoders(config):
  DECODERS[config[CONF_ID]] = config[CONF_DECODERS]
  return config

def collect_encoders(config):
  ENCODERS[config[CONF_ID]] = config.get(CONF_ENCODERS, ["main"])
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

def get_encoder(id, name):
  for k, encoders in ENCODERS.items():
    if k.id != id.id:
      continue
    for encoder in encoders:
      if name in encoder:
        return encoder[CONF_ENCODER_ID]
    raise cv.Invalid(f"Encoder {name} not found")
  raise cv.Invalid(f"Component {id.id} not found")

@register_decoder("main", PanasonicAquareaDecoderMain)
def main_decoder(var, config):
  pass

@register_encoder("main", PanasonicAquareaEncoderMain)
def main_encoder(var, config):
  pass


CONFIG_SCHEMA = cv.All(
  cv.Schema(
    {
      cv.GenerateID(): cv.declare_id(PanasonicAquareaComponent),
      cv.Optional(CONF_LISTEN_ONLY, default=False): cv.boolean,
      cv.Optional(CONF_DECODERS, default=["main"]): validate_decoders,
      cv.Optional(CONF_ENCODERS, default=["main"]): validate_encoders,
    }
  )
  .extend(cv.COMPONENT_SCHEMA)
  .extend(uart.UART_DEVICE_SCHEMA),
  collect_decoders,
  collect_encoders
)

def validate_topic(type):
  def validator(value):
    if CONF_TOPIC not in value:
      raise cv.Invalid(f"Topic {CONF_TOPIC} is required")

    topic = get_topic(value[CONF_TOPIC])

    # TODO: switch -> binary_sensor is allowed
    if topic.type != type:
      if type == TYPE_BINARY_SENSOR and topic.type == TYPE_SWITCH:
        # Allow switch -> binary_sensor
        pass
      else:
        raise cv.Invalid(f"Topic {topic.name} is not a {type} topic")

    if CONF_NAME not in value and CONF_ID not in value:
      value[CONF_NAME] = topic.name

    value[CONF_TOPIC] = topic.name

    # Default configuration
    if topic.type == TYPE_SENSOR:
      if CONF_UNIT_OF_MEASUREMENT not in value and topic.unit_of_measurement is not None:
        value[CONF_UNIT_OF_MEASUREMENT] = topic.unit_of_measurement
      if CONF_ACCURACY_DECIMALS not in value and topic.accuracy_decimals is not None:
        value[CONF_ACCURACY_DECIMALS] = topic.accuracy_decimals
      if CONF_STATE_CLASS not in value and topic.state_class is not None:
        value[CONF_STATE_CLASS] = topic.state_class

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

  encoders = await build_encoders(config[CONF_ENCODERS])
  for encoder in encoders:
    cg.add(encoder.set_parent(var))
    cg.add(var.add_encoder(encoder))

  await cg.register_component(var, config)
  await uart.register_uart_device(var, config)
  
  if CONF_LISTEN_ONLY in config:
    cg.add(var.set_listen_only(config[CONF_LISTEN_ONLY]))

  return var
