import esphome.config_validation as cv
from esphome.components import switch

from .. import (
  panasonic_aquarea_ns,
  CONF_PANASONIC_AQUAREA_ID,
  CONF_FIELD,
  create_and_register_child,
)
from ..fields import apply_field_defaults, lookup_field

PanasonicAquareaSwitch = panasonic_aquarea_ns.class_("PanasonicAquareaSwitch", switch.Switch)

def validate_switch_field(value):
  """Validate field is suitable for switch."""
  field, value = lookup_field(value)

  # Ensure field type is compatible with switch
  if field["type"] not in ["switch", "binary_sensor"]:
    raise cv.Invalid(f"Field {value['field']} type {field['type']} is not compatible with switch")

  apply_field_defaults(field, value, "switch")
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
  var = await create_and_register_child(config)
  await switch.register_switch(var, config)