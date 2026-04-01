import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.core import coroutine

from .. import (
  panasonic_aquarea_ns,
  CONF_PANASONIC_AQUAREA_ID,
  get_protocol,
)
from ..fields import get_field, validate_field

CONF_FIELD = "field"

PanasonicAquareaSwitch = panasonic_aquarea_ns.class_("PanasonicAquareaSwitch", switch.Switch)

def validate_switch_field(value):
  """Validate field is suitable for switch."""
  if CONF_FIELD not in value:
    raise cv.Invalid(f"Field {CONF_FIELD} is required")

  field_name = value[CONF_FIELD]
  field = get_field(field_name)

  # Ensure field type is compatible with switch
  if field["type"] not in ["switch", "binary_sensor"]:
    raise cv.Invalid(f"Field {field_name} type {field['type']} is not compatible with switch")

  # Set default name if not provided
  if "name" not in value and "id" not in value:
    value["name"] = field["name"]

  return value

CONFIG_SCHEMA = cv.All(
  validate_switch_field,
  switch.switch_schema(PanasonicAquareaSwitch).extend(
    cv.Schema({
      cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(panasonic_aquarea_ns.class_("PanasonicAquareaComponent")),
      cv.Required(CONF_FIELD): cv.string,
    })
  )
)

async def to_code(config):
  field_name = config[CONF_FIELD]
  field = get_field(field_name)
  protocol = field["protocol"]

  # Define namespaces dynamically
  field_ns = panasonic_aquarea_ns.namespace("fields")
  protocol_ns = field_ns.namespace(protocol)

  # Generate template instantiation: new PanasonicAquareaSwitch<main::fieldName>()
  template_args = cg.TemplateArguments(getattr(protocol_ns, field_name))
  var = cg.new_Pvariable(config[cv.CONF_ID], template_args)
  await switch.register_switch(var, config)

  parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
  cg.add(var.set_parent(parent))

  protocol_id = get_protocol(config[CONF_PANASONIC_AQUAREA_ID], protocol)
  protocol_var = await cg.get_variable(protocol_id)
  cg.add(var.set_protocol(protocol_var))
  cg.add(protocol_var.add_child(var))

  return var