import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select

from .. import (
    panasonic_aquarea_ns,
    CONF_PANASONIC_AQUAREA_ID,
    get_protocol,
)
from ..fields import get_field

CONF_FIELD = "field"

PanasonicAquareaSelect = panasonic_aquarea_ns.class_(
    "PanasonicAquareaSelect", select.Select
)

def validate_select_field(value):
    """Validate field is suitable for select."""
    if CONF_FIELD not in value:
        raise cv.Invalid(f"Field {CONF_FIELD} is required")

    field_name = value[CONF_FIELD]
    field = get_field(field_name)

    # Ensure field has options defined
    if "options" not in field:
        raise cv.Invalid(f"Field {field_name} does not have options defined (not a select field)")

    # Set default name if not provided
    if "name" not in value and "id" not in value:
        value["name"] = field["name"]

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
    field_name = config[CONF_FIELD]
    field = get_field(field_name)
    protocol = field["protocol"]

    # Get options from field definition - convert dict {value: label} to list of labels
    options_dict = field["options"]
    options_list = [options_dict[k] for k in options_dict.keys()]
    options_values = [k for k in options_dict.keys()]

    # Define namespaces dynamically
    field_ns = panasonic_aquarea_ns.namespace("fields")
    protocol_ns = field_ns.namespace(protocol)

    # Generate template instantiation
    template_args = cg.TemplateArguments(getattr(protocol_ns, field_name))
    var = cg.new_Pvariable(config[cv.CONF_ID], template_args)
    await select.register_select(var, config, options=options_list)
    cg.add(var.set_options_values(options_values))

    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
    cg.add(var.set_parent(parent))

    protocol_var = get_protocol(config[CONF_PANASONIC_AQUAREA_ID], protocol)
    cg.add(var.set_protocol(protocol_var))
    cg.add(protocol_var.add_child(var))

    return var