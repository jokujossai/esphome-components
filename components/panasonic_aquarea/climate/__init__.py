import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

from .. import (
    panasonic_aquarea_ns,
    CHILD_SCHEMA_BASE,
    CONF_PANASONIC_AQUAREA_ID,
    get_protocol,
)

CONF_ZONE = "zone"
CONF_DIRECT_MIN_VALUE = "direct_min_value"
CONF_DIRECT_MAX_VALUE = "direct_max_value"

PanasonicAquareaZoneClimate = panasonic_aquarea_ns.class_(
    "PanasonicAquareaZoneClimate", climate.Climate, cg.Component
)


def validate_zone(value):
    value = cv.int_(value)
    if value not in [1, 2]:
        raise cv.Invalid("Zone must be 1 or 2")
    return value


CONFIG_SCHEMA = (
    climate.climate_schema(PanasonicAquareaZoneClimate)
    .extend(CHILD_SCHEMA_BASE)
    .extend(
        {
            cv.Required(CONF_ZONE): validate_zone,
            # Direct mode temperature limits — depend on heat pump model.
            # WH-UD: 20-55, WH-UH: 25-65 or 35-65, WH-UX/UQ: 20-60.
            # Defaults cover outer bounds of all models.
            cv.Optional(CONF_DIRECT_MIN_VALUE, default=20.0): cv.float_,
            cv.Optional(CONF_DIRECT_MAX_VALUE, default=65.0): cv.float_,
        }
    )
)


async def to_code(config):
    var = await climate.new_climate(config)
    await cg.register_component(var, config)

    cg.add(var.set_zone(config[CONF_ZONE]))
    cg.add(var.set_direct_min_value(config[CONF_DIRECT_MIN_VALUE]))
    cg.add(var.set_direct_max_value(config[CONF_DIRECT_MAX_VALUE]))

    parent = await cg.get_variable(config[CONF_PANASONIC_AQUAREA_ID])
    cg.add(var.set_parent(parent))

    protocol_var = get_protocol(config[CONF_PANASONIC_AQUAREA_ID], "main")
    cg.add(var.set_protocol(protocol_var))
    cg.add(protocol_var.add_child(var))

    return var
