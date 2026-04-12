# ESPHome Components

A collection of custom [ESPHome](https://esphome.io) components.

To use this repository, configure it inside your ESPHome YAML:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/daniellinjama/esphome-components
```

You can also find ready-to-use configuration examples in the [examples](examples) folder.

## [Panasonic Aquarea](components/panasonic_aquarea)

Integrates Panasonic Aquarea heat pumps with ESPHome via UART, targeting [HeishaMon](https://github.com/heishamon/HeishaMon)-compatible and DIY hardware. Exposes the full range of heat pump telemetry and settings as standard ESPHome entities — sensors, binary sensors, switches, selects, numbers, text sensors, and climate — enabling full monitoring and control from Home Assistant.

The serial protocol implementation is based on the protocol research done by the [HeishaMon](https://github.com/heishamon/HeishaMon) project.

## [ADE7880](components/ade7880)

Energy metering component for the ADE7880 IC as used in Shelly 3EM devices. Provides per-phase measurements of voltage, current, active power, reactive power, apparent power, power factor, frequency, and accumulated energy over I2C.

Compared to the built-in ESPHome ADE7880 component, this version uses the chip's **line cycle energy accumulation mode** (LCYCMODE) — the hardware counts a configurable number of AC half-cycles, asserts an interrupt when the period completes, and resets the energy registers. This gives hardware-synchronized 1-second energy snapshots with integer accumulation, avoiding floating-point drift over time. It also adds `reactive_power` and `frequency` sensors, a `total_power_gain` calibration field, and a watchdog that automatically reinitializes the chip on I2C failures.

## [D6F-PH](components/d6f_ph)

Differential pressure sensor for Omron D6F-PH series devices over I2C. Supports pressure and temperature readings across multiple range modes (±50 Pa, 0–250 Pa, 0–500 Pa).
