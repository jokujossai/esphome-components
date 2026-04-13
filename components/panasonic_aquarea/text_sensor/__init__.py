import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    CONF_FIELD,
    create_and_register_child,
)
from ..fields import get_field_options, lookup_field

PanasonicAquareaTextSensor = panasonic_aquarea_ns.class_(
    "PanasonicAquareaTextSensor", text_sensor.TextSensor
)

def validate_text_sensor_field(value):
    """Validate field is suitable for text sensor (must have options)."""
    field, value = lookup_field(value)

    # Ensure field has options defined
    if "options" not in field:
        raise cv.Invalid(f"Field {value['field']} does not have options defined (not a select field)")

    return value

CONFIG_SCHEMA = cv.All(
    validate_text_sensor_field,
    text_sensor.text_sensor_schema(PanasonicAquareaTextSensor).extend(
        cv.Schema({
            cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(panasonic_aquarea_ns.class_("PanasonicAquareaComponent")),
            cv.Required(CONF_FIELD): cv.string,
        })
    ),
)


async def to_code(config):
    var = await create_and_register_child(config)

    options_labels, options_values = get_field_options(config)

    await text_sensor.register_text_sensor(var, config)
    cg.add(var.set_options(options_labels))
    cg.add(var.set_options_values(options_values))