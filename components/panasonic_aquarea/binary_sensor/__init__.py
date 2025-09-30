import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    get_decoder,
)
from ..fields import get_field

CONF_FIELD = "field"

PanasonicAquareaBinarySensor = panasonic_aquarea_ns.class_(
    "PanasonicAquareaBinarySensor", binary_sensor.BinarySensor
)

def validate_binary_sensor_field(value):
    """Validate field is suitable for binary_sensor."""
    if CONF_FIELD not in value:
        raise cv.Invalid(f"Field {CONF_FIELD} is required")

    field_name = value[CONF_FIELD]
    field = get_field(field_name)

    # Binary sensor only supports boolean fields (switch and binary_sensor types)
    if field["type"] not in ["binary_sensor", "switch"]:
        raise cv.Invalid(f"Field {field_name} type {field['type']} is not compatible with binary_sensor (only boolean fields supported)")

    # Set default name if not provided
    if "name" not in value and "id" not in value:
        value["name"] = field["name"]

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

    decoder_id = get_decoder(config[CONF_PANASONIC_AQUAREA_ID], protocol)
    decoder = await cg.get_variable(decoder_id)
    cg.add(var.set_decoder(decoder))
    cg.add(decoder.add_child(var))

    return var