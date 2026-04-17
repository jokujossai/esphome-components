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
    CONF_FIELD,
    create_and_register_child,
)
from ..fields import apply_field_defaults, lookup_field

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

    apply_field_defaults(field, value, "number")
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
                # Required here, but validate_number_field (first in cv.All)
                # fills these from the field registry when available. User
                # must supply them explicitly only if the field has no
                # registry defaults.
                cv.Required(CONF_MIN_VALUE): cv.float_,
                cv.Required(CONF_MAX_VALUE): cv.float_,
                cv.Required(CONF_STEP): cv.positive_float,
            }
        )
    ),
)


async def to_code(config):
    var = await create_and_register_child(config)
    await number.register_number(
        var,
        config,
        min_value=config[CONF_MIN_VALUE],
        max_value=config[CONF_MAX_VALUE],
        step=config[CONF_STEP],
    )
