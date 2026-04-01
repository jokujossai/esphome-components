import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    get_protocol,
)
from ..fields import get_field

CONF_FIELD = "field"

PanasonicAquareaSensor = panasonic_aquarea_ns.class_(
    "PanasonicAquareaSensor", sensor.Sensor
)

def validate_sensor_field(value):
    """Validate field is suitable for sensor."""
    if CONF_FIELD not in value:
        raise cv.Invalid(f"Field {CONF_FIELD} is required")

    field_name = value[CONF_FIELD]
    field = get_field(field_name)

    # Sensor can display any field except text
    if field["type"] == "text_sensor":
        raise cv.Invalid(f"Field {field_name} type {field['type']} is not compatible with sensor")

    # Set default name if not provided
    if "name" not in value and "id" not in value:
        value["name"] = field["name"]

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
    field_name = config[CONF_FIELD]
    field = get_field(field_name)
    protocol = field["protocol"]

    # Define namespaces dynamically
    field_ns = panasonic_aquarea_ns.namespace("fields")
    protocol_ns = field_ns.namespace(protocol)

    # Generate template instantiation
    template_args = cg.TemplateArguments(getattr(protocol_ns, field_name))
    var = cg.new_Pvariable(config[cv.CONF_ID], template_args)
    await sensor.register_sensor(var, config)

    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
    cg.add(var.set_parent(parent))

    protocol_id = get_protocol(config[CONF_PANASONIC_AQUAREA_ID], protocol)
    protocol_var = await cg.get_variable(protocol_id)
    cg.add(var.set_protocol(protocol_var))
    cg.add(protocol_var.add_child(var))

    return var