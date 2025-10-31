import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, mqtt, udp
from esphome.const import (
  CONF_ID,
  CONF_NAME,
  CONF_UNIT_OF_MEASUREMENT,
  CONF_ACCURACY_DECIMALS,
  CONF_STATE_CLASS,
  CONF_MQTT_ID,
  CONF_TYPE,
  CONF_PORT,
  CONF_TRIGGER_ID,
)
from esphome import automation
from esphome.core import coroutine
from esphome.util import Registry

CONF_LISTEN_ONLY = "listen_only"
CONF_DECODERS = "decoders"
CONF_DECODER_ID = "decoder_id"
CONF_ENCODERS = "encoders"
CONF_ENCODER_ID = "encoder_id"
CONF_PANASONIC_AQUAREA_ID = "panasonic_aquarea_id"
CONF_TOPIC = "topic"
CONF_DATA_SOURCE = "data_source"
CONF_SUBSCRIBE_TOPIC = "subscribe_topic"
CONF_PUBLISH_TOPIC = "publish_topic"
CONF_UDP_ID = "udp_id"
CONF_ON_PACKET_SEND = "on_packet_send"
CONF_PACKET = "packet"

DEPENDENCIES = []

DATA_SOURCE_UART = "uart"
DATA_SOURCE_MQTT = "mqtt"
DATA_SOURCE_UDP = "udp"

panasonic_aquarea_ns = cg.esphome_ns.namespace("panasonic_aquarea")
PanasonicAquareaComponent = panasonic_aquarea_ns.class_(
  "PanasonicAquareaComponent", cg.Component
)
PanasonicAquareaUARTDataSource = panasonic_aquarea_ns.class_(
  "PanasonicAquareaUARTDataSource", uart.UARTDevice
)
PanasonicAquareaMQTTDataSource = panasonic_aquarea_ns.class_(
  "PanasonicAquareaMQTTDataSource"
)
PanasonicAquareaUDPDataSource = panasonic_aquarea_ns.class_(
  "PanasonicAquareaUDPDataSource"
)

# Actions and Triggers
HandlePacketAction = panasonic_aquarea_ns.class_("HandlePacketAction", automation.Action)
OnPacketSendTrigger = panasonic_aquarea_ns.class_(
  "OnPacketSendTrigger", automation.Trigger.template(cg.std_vector.template(cg.uint8))
)

PanasonicAquareaDecoder = panasonic_aquarea_ns.class_("PanasonicAquareaDecoder")
PanasonicAquareaDecoderMain = panasonic_aquarea_ns.class_(
  "PanasonicAquareaDecoderMain", PanasonicAquareaDecoder, cg.Component
)

PanasonicAquareaEncoder = panasonic_aquarea_ns.class_("PanasonicAquareaEncoder")
PanasonicAquareaEncoderMain = panasonic_aquarea_ns.class_(
  "PanasonicAquareaEncoderMain", PanasonicAquareaEncoder, cg.Component
)

DECODER_REGISTRY = Registry({
  cv.GenerateID(CONF_DECODER_ID): cv.use_id(PanasonicAquareaDecoder),
})

ENCODER_REGISTRY = Registry({
  cv.GenerateID(CONF_ENCODER_ID): cv.use_id(PanasonicAquareaEncoder),
})

async def build_decoders(config):
  decoders = []
  for conf in config:
    decoder = await cg.build_registry_entry(DECODER_REGISTRY, conf)
    decoders.append(decoder)
    await cg.register_component(decoder, conf)
  return decoders

def validate_decoders(value):
  if isinstance(value, str) and value.lower() == "all":
    return validate_decoders(list(DECODER_REGISTRY.keys()))
  return cv.validate_registry("decoder", DECODER_REGISTRY)(value)

async def build_encoders(config):
  encoders = []
  for conf in config:
    encoder = await cg.build_registry_entry(ENCODER_REGISTRY, conf)
    encoders.append(encoder)
    await cg.register_component(encoder, conf)
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


DATA_SOURCE_REGISTRY = Registry({
  cv.GenerateID("data_source_id"): cv.declare_id(panasonic_aquarea_ns.class_("PanasonicAquareaDataSource")),
})

def register_data_source(name, type, schema):
  registerer = DATA_SOURCE_REGISTRY.register(name, type, schema)

  def decorator(func):
    async def new_func(config, data_source_id):
      var = cg.new_Pvariable(data_source_id)
      await coroutine(func)(var, config)
      return var

    return registerer(new_func)

  return decorator

@register_data_source(DATA_SOURCE_UART, PanasonicAquareaUARTDataSource, uart.UART_DEVICE_SCHEMA)
async def uart_data_source(var, config):
  await uart.register_uart_device(var, config)

@register_data_source(DATA_SOURCE_MQTT, PanasonicAquareaMQTTDataSource, {
  cv.Required(CONF_MQTT_ID): cv.use_id(mqtt.MQTTClientComponent),
  cv.Required(CONF_SUBSCRIBE_TOPIC): cv.subscribe_topic,
  cv.Required(CONF_PUBLISH_TOPIC): cv.publish_topic,
})
async def mqtt_data_source(var, config):
  mqtt_client = await cg.get_variable(config[CONF_MQTT_ID])
  cg.add(var.set_mqtt_client(mqtt_client))
  cg.add(var.set_subscribe_topic(config[CONF_SUBSCRIBE_TOPIC]))
  cg.add(var.set_publish_topic(config[CONF_PUBLISH_TOPIC]))

@register_data_source(DATA_SOURCE_UDP, PanasonicAquareaUDPDataSource, {
  cv.Required(CONF_UDP_ID): cv.use_id(udp.UDPComponent),
})
async def udp_data_source(var, config):
  udp_component = await cg.get_variable(config[CONF_UDP_ID])
  cg.add(var.set_udp_component(udp_component))


def validate_data_source(value):
  """Validate and normalize data_source configuration."""
  # Allow None/null for custom control mode
  if value is None:
    return None

  if isinstance(value, str):
    # Simple string format: "uart", "mqtt", or "udp"
    value = {value: {}}
  elif isinstance(value, dict):
    # If value has 'type' key, convert to registry format: {type: {config}}
    if CONF_TYPE in value:
      source_type = value.pop(CONF_TYPE)
      value = {source_type: value}

  if not isinstance(value, dict):
    raise cv.Invalid("data_source must be a string, dictionary, or null")

  # Should have exactly one key which is the data source type
  if len(value) != 1:
    raise cv.Invalid("data_source must have exactly one type specified")

  # Use the registry validator
  return cv.validate_registry_entry("data_source", DATA_SOURCE_REGISTRY)(value)


CONFIG_SCHEMA = cv.All(
  cv.Schema(
    {
      cv.GenerateID(): cv.declare_id(PanasonicAquareaComponent),
      cv.Optional(CONF_LISTEN_ONLY, default=False): cv.boolean,
      cv.Optional(CONF_DECODERS, default=["main"]): validate_decoders,
      cv.Optional(CONF_ENCODERS, default=["main"]): validate_encoders,
      cv.Optional(CONF_DATA_SOURCE, default=DATA_SOURCE_UART): validate_data_source,
      cv.Optional(CONF_ON_PACKET_SEND): automation.validate_automation(
        {
          cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnPacketSendTrigger),
        }
      ),
    }
  )
  .extend(cv.COMPONENT_SCHEMA),
  collect_decoders,
  collect_encoders
)

CHILD_SCHEMA_BASE = (
  cv.Schema({
    cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquareaComponent),
  })
)

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])

  # Setup data source using registry (if provided and not null)
  if CONF_DATA_SOURCE in config and config[CONF_DATA_SOURCE] is not None:
    data_source_config = config[CONF_DATA_SOURCE]
    data_source = await cg.build_registry_entry(DATA_SOURCE_REGISTRY, data_source_config)
    cg.add(var.set_data_source(data_source))

  decoders = await build_decoders(config[CONF_DECODERS])
  for decoder in decoders:
    cg.add(var.add_decoder(decoder))

  encoders = await build_encoders(config[CONF_ENCODERS])
  for encoder in encoders:
    cg.add(encoder.set_parent(var))
    cg.add(var.add_encoder(encoder))

  await cg.register_component(var, config)

  if CONF_LISTEN_ONLY in config:
    cg.add(var.set_listen_only(config[CONF_LISTEN_ONLY]))

  # Setup on_packet_send trigger
  for conf in config.get(CONF_ON_PACKET_SEND, []):
    trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
    await automation.build_automation(trigger, [(cg.std_vector.template(cg.uint8), "packet")], conf)

  return var
