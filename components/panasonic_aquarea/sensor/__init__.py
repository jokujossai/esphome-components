import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    CONF_FIELD,
    create_and_register_child,
)
from ..fields import lookup_field

PanasonicAquareaSensor = panasonic_aquarea_ns.class_(
    "PanasonicAquareaSensor", sensor.Sensor
)

def validate_sensor_field(value):
    """Validate field is suitable for sensor."""
    field, value = lookup_field(value)

    # Sensor can display any field except text
    if field["type"] == "text_sensor":
        raise cv.Invalid(f"Field {value['field']} type {field['type']} is not compatible with sensor")

    # Apply default values, but skip attributes that are not valid for sensors
    # Number-specific attributes that should be ignored for sensors
    number_only_attrs = {"mode", "min_value", "max_value", "step"}

    for k, v in field.items():
        if k in ("protocol", "name", "type"):
            continue
        if k in number_only_attrs:
            continue
        # Skip entity_category "config" for sensors (only "" and "diagnostic" are supported)
        if k == "entity_category" and v == "config":
            continue
        if k not in value:
            value[k] = v

    return value

CONFIG_SCHEMA = cv.All(
    validate_sensor_field,
    sensor.sensor_schema(PanasonicAquareaSensor).extend(
        cv.Schema({
            cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(panasonic_aquarea_ns.class_("PanasonicAquareaComponent")),
            cv.Required(CONF_FIELD): cv.string,
        })
    ),
)


async def to_code(config):
    var = await create_and_register_child(config)
    await sensor.register_sensor(var, config)