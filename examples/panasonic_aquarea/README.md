# Panasonic Aquarea — Examples

Ready-to-use ESPHome configurations for the `panasonic_aquarea` component.

## minimal.yaml

A minimal starting point for a direct UART connection to the heat pump using a D1 Mini (ESP8266). Includes a small representative set of entities: a few switches, a sensor, a binary sensor, and a select. Use this as a base and add entities from the [component documentation](../../components/panasonic_aquarea/README.md) as needed.

## heishamon.yaml + heishamon/

A full-featured modular configuration targeting [HeishaMon](https://github.com/heishamon/HeishaMon)-compatible hardware. The top-level `heishamon.yaml` composes the setup from a set of sub-files using ESPHome's `packages` / `!include` mechanism.

### Structure

| File | Description |
|------|-------------|
| `heishamon.yaml` | Top-level file. Defines substitutions and assembles packages. Edit this file to select which optional packages to enable. |
| `heishamon/base.yaml` | Core setup: UART pins (GPIO15/GPIO13), Wi-Fi, MQTT, OTA, and the `panasonic_aquarea` component. |
| `heishamon/common.yaml` | All main protocol entities — the complete set of sensors, binary sensors, switches, selects, numbers, and text sensors for standard heat pump operation. |
| `heishamon/zone1_cool.yaml` | Zone 1 cooling control entities. |
| `heishamon/zone2.yaml` | Zone 2 heating control entities. |
| `heishamon/zone2_cool.yaml` | Zone 2 cooling control entities. |
| `heishamon/bivalent.yaml` | Bivalent heating system configuration fields. |
| `heishamon/optional_solar.yaml` | Optional PCB: solar collector monitoring and control. |
| `heishamon/optional_buffer.yaml` | Optional PCB: buffer tank monitoring and control. |
| `heishamon/optional_external.yaml` | Optional PCB: external heater integration. |
| `heishamon/one_wire.yaml` | Enable a 1-Wire bus for external temperature probes. |
| `heishamon/one_wire_sensor.yaml` | Add a single 1-Wire temperature sensor. Include multiple times with different variables for multiple probes. |
| `heishamon/s0.yaml` | S0 pulse counter inputs for external energy meters. |

### Customizing

The `heishamon.yaml` substitutions block controls zone temperature ranges — adjust these to match your heat pump model and zone sensor type (compensation curve vs. direct mode). Optional packages (solar, buffer, external heater, 1-Wire, S0) are commented out by default; uncomment the relevant `!include` lines to enable them.

## openeth-mqtt.yaml

An ESP32 configuration for QEMU-based testing on a computer. Uses the OPENETH virtual Ethernet driver and receives raw packets over MQTT, allowing the component to be exercised without physical heat pump hardware.
