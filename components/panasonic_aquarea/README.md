# Panasonic Aquarea

ESPHome component for integrating Panasonic Aquarea heat pumps. Targets [HeishaMon](https://github.com/heishamon/HeishaMon)-compatible hardware and DIY builds that connect to the heat pump's CN-CNT serial port.

The serial protocol implementation is based on the protocol research done by the [HeishaMon](https://github.com/heishamon/HeishaMon) project.

## Requirements

- ESPHome 2024.x or later
- Hardware with a UART interface connected to the heat pump CN-CNT port (HeishaMon-compatible board or equivalent DIY build)

## Configuration

### Basic setup

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/daniellinjama/esphome-components

uart:
  tx_pin: GPIO1
  rx_pin: GPIO3
  baud_rate: 9600
  data_bits: 8
  parity: EVEN
  stop_bits: 1

panasonic_aquarea:
```

### Options

| Option | Default | Description |
|--------|---------|-------------|
| `listen_only` | `false` | When `true`, the component only reads data and never sends packets to the heat pump. |
| `protocols` | `[main, main_request]` | List of protocols to enable. See [Protocols](#protocols). |
| `data_source` | `uart` | How packets are received. See [Data sources](#data-sources). |
| `on_packet_send` | — | Automation trigger fired when a packet is about to be sent. Receives the packet as `std::vector<uint8_t>`. |

### Protocols

| Protocol | Description |
|----------|-------------|
| `main` | Receives status broadcast packets from the heat pump (temperatures, states, power, etc.). |
| `main_request` | Sends periodic query/control packets to the heat pump. Required when `main` is active and `listen_only` is `false`. Supports an optional `query_interval` (default `5s`). |
| `optional` | Receives packets from the optional PCB port, providing additional I/O states (zone pumps, mixing valves, solar pump, pool pump, alarm). |

```yaml
panasonic_aquarea:
  protocols:
    - main
    - main_request:
        query_interval: 10s
    - optional
```

### Data sources

The default data source is `uart`. The MQTT and UDP sources exist for development and testing purposes.

```yaml
# UART (default) — direct serial connection
panasonic_aquarea:
  data_source: uart

# MQTT — receives raw packets from an MQTT topic, publishes responses
panasonic_aquarea:
  data_source:
    type: mqtt
    mqtt_id: mqtt1
    subscribe_topic: panasonic_heat_pump/raw/data
    publish_topic: panasonic_heat_pump/raw/esphome_output

# UDP — receives raw packets over the network
panasonic_aquarea:
  data_source:
    type: udp
    udp_id: udp1
```

## Platforms

All entity platforms share a common `field` option that selects which heat pump data field the entity maps to.

### sensor

Exposes a numeric heat pump field as a read-only ESPHome sensor.

```yaml
sensor:
  - platform: panasonic_aquarea
    field: outsideTemp
    name: Outside Temperature

  - platform: panasonic_aquarea
    field: heatPowerConsumption
    name: Heat Power Consumption
```

### binary_sensor

Exposes a boolean field as a read-only binary sensor.

```yaml
binary_sensor:
  - platform: panasonic_aquarea
    field: defrostingState
    name: Defrosting
```

### switch

Exposes a boolean field as a controllable switch. Toggling the switch sends a control packet to the heat pump.

```yaml
switch:
  - platform: panasonic_aquarea
    field: heatpumpState
    name: Heat Pump

  - platform: panasonic_aquarea
    field: forceDhwState
    name: Force DHW
```

### select

Exposes an enumerated field as a controllable select entity. Options are defined per field (e.g. Off / Heat / Cool / Auto).

```yaml
select:
  - platform: panasonic_aquarea
    field: operatingModeState
    name: Operating Mode

  - platform: panasonic_aquarea
    field: quietModeLevel
    name: Quiet Mode Level
```

### number

Exposes a numeric field as a controllable number entity. Requires explicit range configuration.

```yaml
number:
  - platform: panasonic_aquarea
    field: dhwTargetTemp
    name: DHW Target Temperature
    min_value: 40
    max_value: 75
    step: 1

  - platform: panasonic_aquarea
    field: z1HeatRequestTemp
    name: Zone 1 Heat Request Temperature
    min_value: -5
    max_value: 5
    step: 1
```

### text_sensor

Exposes an enumerated field as a read-only text sensor, displaying the human-readable option name.

```yaml
text_sensor:
  - platform: panasonic_aquarea
    field: errorCode
    name: Error Code

  - platform: panasonic_aquarea
    field: threeWayValveState
    name: Three-Way Valve State
```

### climate

Exposes a heating/cooling zone as a climate entity, combining the zone's current water temperature with its heat and cool setpoint fields.

```yaml
climate:
  - platform: panasonic_aquarea
    zone: 1
    name: Zone 1

  - platform: panasonic_aquarea
    zone: 2
    name: Zone 2
```

## Automation

### Action: `panasonic_aquarea.handle_packet`

Injects a raw packet into the component for processing. Useful for testing or replaying captured packets.

```yaml
panasonic_aquarea.handle_packet:
  packet: [0x71, 0xC8, 0x01, 0x10, ...]
```

### Trigger: `on_packet_send`

Fires before a packet is sent to the heat pump. Receives the packet bytes as `packet` (`std::vector<uint8_t>`).

```yaml
panasonic_aquarea:
  on_packet_send:
    then:
      - logger.log:
          format: "Sending packet (%d bytes)"
          args: [packet.size()]
```

## Available fields

The table below lists commonly used fields. The full field registry is defined in [`fields.py`](fields.py).

### Main protocol — control

| Field | Default entity | Description |
|-------|---------------|-------------|
| `heatpumpState` | switch | Heat pump on/off |
| `forceDhwState` | switch | Force DHW heating |
| `dhwModeState` | switch | DHW mode |
| `operatingModeState` | select | Off / Heat / Cool / Auto(Heat) / Auto(Cool) |
| `powerfulModeTime` | select | Powerful mode duration: Off / 30min / 60min / 90min |
| `quietModeLevel` | select | Quiet mode: Off / Level 1 / Level 2 / Level 3 |
| `holidayModeState` | select | Holiday mode: Off / Scheduled / Active |
| `zonesState` | select | Active zones: Zone1 / Zone2 / Zone1+Zone2 |
| `dhwTargetTemp` | number | DHW target temperature |
| `z1HeatRequestTemp` | number | Zone 1 heat request temperature |
| `z1CoolRequestTemp` | number | Zone 1 cool request temperature |
| `z2HeatRequestTemp` | number | Zone 2 heat request temperature |
| `z2CoolRequestTemp` | number | Zone 2 cool request temperature |

### Main protocol — temperatures

| Field | Description |
|-------|-------------|
| `outsideTemp` | Outdoor temperature |
| `mainInletTemp` | Main circuit inlet temperature |
| `mainOutletTemp` | Main circuit outlet temperature |
| `mainTargetTemp` | Main circuit target temperature |
| `dhwTemp` | DHW tank temperature |
| `z1WaterTemp` | Zone 1 water temperature |
| `z2WaterTemp` | Zone 2 water temperature |
| `z1Temp` | Zone 1 room/thermostat temperature |
| `z2Temp` | Zone 2 room/thermostat temperature |
| `dischargeTemp` | Compressor discharge temperature |
| `outsidePipeTemp` | Outdoor pipe temperature |
| `defrostTemp` | Defrost temperature |
| `evaOutletTemp` | Evaporator outlet temperature |
| `ipmTemp` | IPM (inverter module) temperature |

### Main protocol — power and flow

| Field | Description |
|-------|-------------|
| `pumpFlow` | Water pump flow rate (L/min) |
| `pumpSpeed` | Water pump speed |
| `pumpDuty` | Water pump duty cycle |
| `compressorFreq` | Compressor frequency (Hz) |
| `compressorCurrent` | Compressor current (A) |
| `fan1MotorSpeed` | Fan 1 motor speed (RPM) |
| `fan2MotorSpeed` | Fan 2 motor speed (RPM) |
| `highPressure` | High-side refrigerant pressure |
| `lowPressure` | Low-side refrigerant pressure |
| `heatPowerProduction` | Heat power output (kW) |
| `heatPowerConsumption` | Heat power input (kW) |
| `coolPowerProduction` | Cooling power output (kW) |
| `coolPowerConsumption` | Cooling power input (kW) |
| `dhwPowerProduction` | DHW power output (kW) |
| `dhwPowerConsumption` | DHW power input (kW) |

### Main protocol — status

| Field | Default entity | Description |
|-------|---------------|-------------|
| `defrostingState` | binary_sensor | Defrost cycle active |
| `dhwHeaterState` | binary_sensor | DHW electric heater active |
| `roomHeaterState` | binary_sensor | Room electric heater active |
| `internalHeaterState` | binary_sensor | Internal heater active |
| `externalHeaterState` | binary_sensor | External heater active |
| `forceHeaterState` | binary_sensor | Force heater active |
| `sterilizationState` | binary_sensor | Sterilization cycle active |
| `z1PumpState` | binary_sensor | Zone 1 pump running |
| `z2PumpState` | binary_sensor | Zone 2 pump running |
| `threeWayValveState` | text_sensor | Three-way valve position |
| `twoWayValveState` | text_sensor | Two-way valve position |
| `errorCode` | text_sensor | Active error code and description |
| `heatingMode` | text_sensor | Heating mode (compensation curve / direct) |
| `coolingMode` | text_sensor | Cooling mode |
| `operationsHours` | sensor | Total compressor run hours |
| `operationsCounter` | sensor | Total compressor start count |

### Optional PCB protocol

| Field | Default entity | Description |
|-------|---------------|-------------|
| `z1MixingValveState` | text_sensor | Zone 1 mixing valve position |
| `z2MixingValveState` | text_sensor | Zone 2 mixing valve position |
| `poolWaterPumpState` | binary_sensor | Pool pump running |
| `solarWaterPumpState` | binary_sensor | Solar pump running |
| `alarmState` | binary_sensor | External alarm signal |

## Examples

See the [examples/panasonic_aquarea](../../examples/panasonic_aquarea) directory for ready-to-use configurations.
