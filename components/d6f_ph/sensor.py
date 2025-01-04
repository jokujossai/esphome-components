import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_TEMPERATURE,
    CONF_PRESSURE,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_PRESSURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PASCAL,
)

CONF_RANGE_MODE = "range_mode"

DEPENDENCIES = ["i2c"]

d6f_ph_ns = cg.esphome_ns.namespace("d6f_ph")
D6fPh = d6f_ph_ns.class_("D6fPh", cg.PollingComponent, i2c.I2CDevice)
D6fPhRangeMode = d6f_ph_ns.enum("D6fPhRangeMode")

RANGE_MODE_OPTIONS = {
    # -50 to 50 Pa (D6F-PH0505AD3)
    100: D6fPhRangeMode.RangeMode100,
    # 0 to 250 Pa (D6F-PH0025AD1)
    250: D6fPhRangeMode.RangeMode250,
    # 0 to 500 Pa (D6F-PH5050AD3)
    1000: D6fPhRangeMode.RangeMode1000,
}

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(D6fPh),
            cv.Required(CONF_RANGE_MODE): cv.enum(RANGE_MODE_OPTIONS),
            cv.Optional(CONF_TEMPERATURE): cv.maybe_simple_value(
                sensor.sensor_schema(
                    unit_of_measurement=UNIT_CELSIUS,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_TEMPERATURE,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                key=CONF_NAME,
            ),
            cv.Optional(CONF_PRESSURE): cv.maybe_simple_value(
                sensor.sensor_schema(
                    unit_of_measurement=UNIT_PASCAL,
                    accuracy_decimals=1,
                    device_class=DEVICE_CLASS_PRESSURE,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
                key=CONF_NAME,
            )
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x6C))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_range_mode(config[CONF_RANGE_MODE]))

    if temperature := config.get(CONF_TEMPERATURE):
        sens = await sensor.new_sensor(temperature)
        cg.add(var.set_temperature_sensor(sens))

    if pressure := config.get(CONF_PRESSURE):
        sens = await sensor.new_sensor(pressure)
        cg.add(var.set_pressure_sensor(sens))

