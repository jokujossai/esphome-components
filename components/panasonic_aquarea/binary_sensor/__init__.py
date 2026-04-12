import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    get_protocol,
)
from ..fields import get_field, lookup_field

CONF_FIELD = "field"

PanasonicAquareaBinarySensor = panasonic_aquarea_ns.class_(
    "PanasonicAquareaBinarySensor", binary_sensor.BinarySensor
)

def validate_binary_sensor_field(value):
    """Validate field is suitable for binary_sensor."""
    field, value = lookup_field(value)

    # Binary sensor only supports boolean fields (switch and binary_sensor types)
    if field["type"] not in ["binary_sensor", "switch"]:
        raise cv.Invalid(f"Field {value['field']} type {field['type']} is not compatible with binary_sensor (only boolean fields supported)")

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
    field_name = config[CONF_FIELD]
    field = get_field(field_name)
    protocol = field["protocol"]

    # Define namespaces dynamically
    field_ns = panasonic_aquarea_ns.namespace("fields")
    protocol_ns = field_ns.namespace(protocol)

    # Generate template instantiation
    template_args = cg.TemplateArguments(getattr(protocol_ns, field_name))
    var = cg.new_Pvariable(config[cv.CONF_ID], template_args)
    await binary_sensor.register_binary_sensor(var, config)

    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
    cg.add(var.set_parent(parent))

    protocol_var = get_protocol(config[CONF_PANASONIC_AQUAREA_ID], protocol)
    cg.add(var.set_protocol(protocol_var))
    cg.add(protocol_var.add_child(var))

    return var