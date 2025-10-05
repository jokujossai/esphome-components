import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation

from . import (
    panasonic_aquarea_ns,
    PanasonicAquareaComponent,
    HandlePacketAction,
    CONF_PANASONIC_AQUAREA_ID,
    CONF_PACKET,
)

@automation.register_action(
    "panasonic_aquarea.handle_packet",
    HandlePacketAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(PanasonicAquareaComponent),
            cv.Required(CONF_PACKET): cv.templatable(cv.ensure_list(cv.hex_uint8_t)),
        }
    ),
)
async def handle_packet_action_to_code(config, action_id, template_arg, args):
    parent = await cg.get_variable(config[cv.CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, parent)

    template_ = await cg.templatable(config[CONF_PACKET], args, cg.std_vector.template(cg.uint8))
    cg.add(var.set_packet(template_))

    return var
