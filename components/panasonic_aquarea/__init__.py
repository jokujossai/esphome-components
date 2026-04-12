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

from .fields import get_field

CONF_LISTEN_ONLY = "listen_only"
CONF_PROTOCOLS = "protocols"
CONF_PANASONIC_AQUAREA_ID = "panasonic_aquarea_id"
CONF_TOPIC = "topic"
CONF_DATA_SOURCE = "data_source"
CONF_SUBSCRIBE_TOPIC = "subscribe_topic"
CONF_PUBLISH_TOPIC = "publish_topic"
CONF_UDP_ID = "udp_id"
CONF_ON_PACKET_SEND = "on_packet_send"
CONF_PACKET = "packet"
CONF_QUERY_INTERVAL = "query_interval"
CONF_FIELD = "field"

DEPENDENCIES = []

MULTI_CONF = True

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
PanasonicAquareaDataSourceNull = panasonic_aquarea_ns.class_(
  "PanasonicAquareaDataSourceNull"
)

# Actions and Triggers
HandlePacketAction = panasonic_aquarea_ns.class_("HandlePacketAction", automation.Action)
OnPacketSendTrigger = panasonic_aquarea_ns.class_(
  "OnPacketSendTrigger", automation.Trigger.template(cg.std_vector.template(cg.uint8))
)

PanasonicAquareaProtocol = panasonic_aquarea_ns.class_("PanasonicProtocolInterface", cg.Component)
PanasonicAquareaProtocolMain = panasonic_aquarea_ns.class_(
  "PanasonicAquareaProtocolMain", PanasonicAquareaProtocol
)
PanasonicAquareaProtocolOptional = panasonic_aquarea_ns.class_(
  "PanasonicAquareaProtocolOptional", PanasonicAquareaProtocol
)
PanasonicAquareaProtocolMainRequest = panasonic_aquarea_ns.class_(
  "PanasonicAquareaProtocolMainRequest", PanasonicAquareaProtocol
)

PROTOCOL_REGISTRY = Registry()
validate_protocols = cv.validate_registry("protocol", PROTOCOL_REGISTRY)

async def build_protocols(config, parent_id):
  protocols = []
  for conf in config:
    protocol = await cg.build_registry_entry(PROTOCOL_REGISTRY, conf)
    protocols.append(protocol)
    await cg.register_component(protocol, conf)
    # Store mapping: (parent_id, protocol_name) -> protocol_id
    protocol_name = list(conf.keys())[0]
    protocol_conf = conf[protocol_name]
    PROTOCOLS[(str(parent_id), protocol_name)] = protocol
  return protocols

PROTOCOLS = {}

def get_protocol(parent_id, name):
  key = (str(parent_id), name)
  if key in PROTOCOLS:
    return PROTOCOLS[key]
  raise cv.Invalid(f"Protocol '{name}' not found for component {parent_id}")

@PROTOCOL_REGISTRY.register("main", PanasonicAquareaProtocolMain, {})
async def main_protocol_to_code(config, protocol_id):
  return cg.new_Pvariable(protocol_id)

@PROTOCOL_REGISTRY.register("optional", PanasonicAquareaProtocolOptional, {})
async def optional_protocol_to_code(config, protocol_id):
  return cg.new_Pvariable(protocol_id)

@PROTOCOL_REGISTRY.register("main_request", PanasonicAquareaProtocolMainRequest, {
  cv.Optional(CONF_QUERY_INTERVAL, default="5s"): cv.positive_time_period_milliseconds,
})
async def main_request_protocol_to_code(config, protocol_id):
  var = cg.new_Pvariable(protocol_id)
  cg.add(var.set_query_interval(config[CONF_QUERY_INTERVAL]))
  return var


DATA_SOURCE_REGISTRY = Registry({
  cv.GenerateID("data_source_id"): cv.declare_id(panasonic_aquarea_ns.class_("PanasonicAquareaDataSource")),
})

@DATA_SOURCE_REGISTRY.register(DATA_SOURCE_UART, PanasonicAquareaUARTDataSource, uart.UART_DEVICE_SCHEMA)
async def uart_data_source(config, data_source_id):
  var = cg.new_Pvariable(data_source_id)
  await uart.register_uart_device(var, config)
  return var

@DATA_SOURCE_REGISTRY.register(DATA_SOURCE_MQTT, PanasonicAquareaMQTTDataSource, {
  cv.Required(CONF_MQTT_ID): cv.use_id(mqtt.MQTTClientComponent),
  cv.Required(CONF_SUBSCRIBE_TOPIC): cv.subscribe_topic,
  cv.Required(CONF_PUBLISH_TOPIC): cv.publish_topic,
})
async def mqtt_data_source(config, data_source_id):
  var = cg.new_Pvariable(data_source_id)
  mqtt_client = await cg.get_variable(config[CONF_MQTT_ID])
  cg.add(var.set_mqtt_client(mqtt_client))
  cg.add(var.set_subscribe_topic(config[CONF_SUBSCRIBE_TOPIC]))
  cg.add(var.set_publish_topic(config[CONF_PUBLISH_TOPIC]))
  return var

@DATA_SOURCE_REGISTRY.register(DATA_SOURCE_UDP, PanasonicAquareaUDPDataSource, {
  cv.Required(CONF_UDP_ID): cv.use_id(udp.UDPComponent),
})
async def udp_data_source(config, data_source_id):
  var = cg.new_Pvariable(data_source_id)
  udp_component = await cg.get_variable(config[CONF_UDP_ID])
  cg.add(var.set_udp_component(udp_component))
  return var

@DATA_SOURCE_REGISTRY.register("null", PanasonicAquareaDataSourceNull, {})
async def null_data_source(config, data_source_id):
  return cg.new_Pvariable(data_source_id)


def validate_data_source(value):
  """Validate and normalize data_source configuration."""
  # Allow None/null for custom control mode
  if value is None:
    value = {"null": {}}

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


def _validate_protocol_dependencies(config):
  if config.get(CONF_LISTEN_ONLY, False):
    return config
  protocol_names = {list(p.keys())[0] for p in config.get(CONF_PROTOCOLS, [])}
  if "main" in protocol_names and "main_request" not in protocol_names:
    raise cv.Invalid("main_request protocol is required when main protocol is active and listen_only is false")
  return config

CONFIG_SCHEMA = cv.All(
  cv.Schema(
    {
      cv.GenerateID(): cv.declare_id(PanasonicAquareaComponent),
      cv.Optional(CONF_LISTEN_ONLY, default=False): cv.boolean,
      cv.Optional(CONF_PROTOCOLS, default=["main", "main_request"]): validate_protocols,
      cv.Optional(CONF_DATA_SOURCE, default=DATA_SOURCE_UART): validate_data_source,
      cv.Optional(CONF_ON_PACKET_SEND): automation.validate_automation(
        {
          cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnPacketSendTrigger),
        }
      ),
    }
  )
  .extend(cv.COMPONENT_SCHEMA),
  _validate_protocol_dependencies,
)

CHILD_SCHEMA_BASE = (
  cv.Schema({
    cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(PanasonicAquareaComponent),
  })
)


async def create_and_register_child(config):
  """Create a field entity variable and wire up parent/protocol/child. Returns var."""
  field_name = config[CONF_FIELD]
  field = get_field(field_name)
  protocol = field["protocol"]

  field_ns = panasonic_aquarea_ns.namespace("fields")
  protocol_ns = field_ns.namespace(protocol)

  template_args = cg.TemplateArguments(getattr(protocol_ns, field_name))
  var = cg.new_Pvariable(config[cv.CONF_ID], template_args)

  parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
  cg.add(var.set_parent(parent))

  protocol_var = get_protocol(config[CONF_PANASONIC_AQUAREA_ID], protocol)
  cg.add(var.set_protocol(protocol_var))
  cg.add(protocol_var.add_child(var))

  return var

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])

  # Setup data source
  data_source_config = config[CONF_DATA_SOURCE]
  data_source = await cg.build_registry_entry(DATA_SOURCE_REGISTRY, data_source_config)
  cg.add(var.set_data_source(data_source))

  protocols = await build_protocols(config[CONF_PROTOCOLS], config[CONF_ID])
  for protocol in protocols:
    cg.add(var.add_protocol(protocol))

  await cg.register_component(var, config)

  if CONF_LISTEN_ONLY in config:
    cg.add(var.set_listen_only(config[CONF_LISTEN_ONLY]))

  # Setup on_packet_send trigger
  for conf in config.get(CONF_ON_PACKET_SEND, []):
    trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
    await automation.build_automation(trigger, [(cg.std_vector.template(cg.uint8), "packet")], conf)

  return var


@automation.register_action(
    "panasonic_aquarea.handle_packet",
    HandlePacketAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(PanasonicAquareaComponent),
            cv.Required(CONF_PACKET): cv.templatable(cv.ensure_list(cv.hex_uint8_t)),
        }
    ),
)
async def handle_packet_action_to_code(config, action_id, template_arg, args):
    parent = await cg.get_variable(config[cv.CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, parent)

    template_ = await cg.templatable(config[CONF_PACKET], args, cg.std_vector.template(cg.uint8))
    if isinstance(template_, list):
        # Wrap list in explicit std::vector so C++ can deduce the template
        # parameter V in TEMPLATABLE_VALUE's set_packet(V)
        hex_items = ", ".join(f"0x{b:02X}" for b in template_)
        template_ = cg.RawExpression(f"std::vector<uint8_t>{{{hex_items}}}")
    cg.add(var.set_packet(template_))

    return var
