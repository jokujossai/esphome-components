import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    CONF_FIELD,
    create_and_register_child,
)
from ..fields import apply_field_defaults, lookup_field

PanasonicAquareaBinarySensor = panasonic_aquarea_ns.class_(
    "PanasonicAquareaBinarySensor", binary_sensor.BinarySensor
)

def validate_binary_sensor_field(value):
    """Validate field is suitable for binary_sensor."""
    field, value = lookup_field(value)

    # Binary sensor only supports boolean fields (switch and binary_sensor types)
    if field["type"] not in ["binary_sensor", "switch"]:
        raise cv.Invalid(f"Field {value['field']} type {field['type']} is not compatible with binary_sensor (only boolean fields supported)")

    apply_field_defaults(field, value, "binary_sensor")
    return value

CONFIG_SCHEMA = cv.All(
    validate_binary_sensor_field,
    binary_sensor.binary_sensor_schema(PanasonicAquareaBinarySensor).extend(
        cv.Schema({
            cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(panasonic_aquarea_ns.class_("PanasonicAquareaComponent")),
            cv.Required(CONF_FIELD): cv.string,
        })
    ),
)


async def to_code(config):
    var = await create_and_register_child(config)
    await binary_sensor.register_binary_sensor(var, config)