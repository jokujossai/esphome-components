import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    CONF_FIELD,
    create_and_register_child,
)
from ..fields import apply_field_defaults, get_field_options, lookup_field

PanasonicAquareaSelect = panasonic_aquarea_ns.class_(
    "PanasonicAquareaSelect", select.Select
)

def validate_select_field(value):
    """Validate field is suitable for select."""
    field, value = lookup_field(value)

    # Ensure field has options defined
    if "options" not in field:
        raise cv.Invalid(f"Field {value['field']} does not have options defined (not a select field)")

    apply_field_defaults(field, value, "select")
    return value

CONFIG_SCHEMA = cv.All(
    validate_select_field,
    select.select_schema(PanasonicAquareaSelect).extend(
        cv.Schema({
            cv.GenerateID(CONF_PANASONIC_AQUAREA_ID): cv.use_id(panasonic_aquarea_ns.class_("PanasonicAquareaComponent")),
            cv.Required(CONF_FIELD): cv.string,
        })
    ),
)


async def to_code(config):
    var = await create_and_register_child(config)

    options_labels, options_values = get_field_options(config)

    await select.register_select(var, config, options=options_labels)
    cg.add(var.set_options_values(options_values))