from . import EpaperModel


class Uc8179BwrModel(EpaperModel):
    """Waveshare panels driven by the UC8179 controller in B/W/R mode.

    The init sequence is handled in C++ (it requires a busy-wait between
    the POWER ON command and the rest of the panel configuration), so the
    base-class init_sequence array stays empty.
    """

    def __init__(self, name, **defaults):
        super().__init__(
            name,
            "EPaperWaveshareUc8179Bwr",
            initsequence=(),
            **defaults,
        )


Uc8179BwrModel(
    "waveshare-7.5in-bv2-bwr",
    width=800,
    height=480,
    data_rate=10_000_000,
)
