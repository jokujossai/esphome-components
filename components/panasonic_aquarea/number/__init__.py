import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.components.number import number_schema
from esphome.const import (
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_STEP,
)

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    get_protocol,
)
from ..fields import get_field, lookup_field

CONF_FIELD = "field"

PanasonicAquareaNumber = panasonic_aquarea_ns.class_(
    "PanasonicAquareaNumber", number.Number
)


def validate_number_field(value):
    """Validate field is suitable for number and apply registry defaults."""
    field, value = lookup_field(value)

    if field["type"] != "number":
        raise cv.Invalid(
            f"Field {value['field']} type {field['type']} is not compatible with number"
        )

    # Apply defaults from field registry for attributes the user hasn't set.
    for k, v in field.items():
        if k in ("protocol", "name", "type"):
            continue
        if k not in value:
            value[k] = v

    return value


CONFIG_SCHEMA = cv.All(
    validate_number_field,
    number_schema(PanasonicAquareaNumber).extend(
        cv.Schema(
            {
                cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(
                    panasonic_aquarea_ns.class_("PanasonicAquareaComponent")
                ),
                cv.Required(CONF_FIELD): cv.string,
                cv.Required(CONF_MIN_VALUE): cv.float_,
                cv.Required(CONF_MAX_VALUE): cv.float_,
                cv.Required(CONF_STEP): cv.positive_float,
            }
        )
    ),
)


async def to_code(config):
    field_name = config[CONF_FIELD]
    field = get_field(field_name)
    protocol = field["protocol"]

    # Define namespaces dynamically
    field_ns = panasonic_aquarea_ns.namespace("fields")
    protocol_ns = field_ns.namespace(protocol)

    # Generate template instantiation: new PanasonicAquareaNumber<main::fieldName>()
    template_args = cg.TemplateArguments(getattr(protocol_ns, field_name))
    var = cg.new_Pvariable(config[cv.CONF_ID], template_args)
    await number.register_number(
        var,
        config,
        min_value=config[CONF_MIN_VALUE],
        max_value=config[CONF_MAX_VALUE],
        step=config[CONF_STEP],
    )

    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
    cg.add(var.set_parent(parent))

    protocol_var = get_protocol(config[CONF_PANASONIC_AQUAREA_ID], protocol)
    cg.add(var.set_protocol(protocol_var))
    cg.add(protocol_var.add_child(var))

    return var
