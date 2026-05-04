import esphome.codegen as cg
import esphome.config_validation as cv

from . import EpaperModel

CONF_FAST_UPDATE_EVERY = "fast_update_every"


class Uc8179BwrModel(EpaperModel):
    """Waveshare panels driven by the UC8179 controller in B/W/R mode.

    The init sequences (Init / Init_Fast / Init_Part) are emitted in C++
    because they need a busy-wait between the POWER ON command and the rest
    of the panel configuration, so the base-class init_sequence array stays
    empty.
    """

    def __init__(self, name, **defaults):
        super().__init__(
            name,
            "EPaperWaveshareUc8179Bwr",
            initsequence=(),
            **defaults,
        )

    def extra_schema(self) -> dict:
        # fast_update_every controls FAST vs PARTIAL pacing among non-full
        # updates. 1 = always FAST after the first FULL (no PARTIAL ever).
        # >1 = FAST every Nth non-full update, the rest are PARTIAL. 0 =
        # never FAST (only FULL + PARTIAL).
        return {
            cv.Optional(CONF_FAST_UPDATE_EVERY, default=1): cv.int_range(0, 255),
        }

    def configure(self, var, config) -> None:
        cg.add(var.set_fast_update_every(config[CONF_FAST_UPDATE_EVERY]))


Uc8179BwrModel(
    "waveshare-7.5in-bv2-bwr",
    width=800,
    height=480,
    data_rate=10_000_000,
)
