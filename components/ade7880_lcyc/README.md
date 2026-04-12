# ADE7880 (Line Cycle Mode)

ESPHome component for the ADE7880 3-phase energy metering IC as used in Shelly 3EM devices.

## Differences from the built-in ESPHome ADE7880 component

This component uses the ADE7880's **line cycle energy accumulation mode** (LCYCMODE) instead of continuous conversion mode. The chip counts a configurable number of AC half-cycles, accumulates energy over that window, then asserts IRQ0 when the period is complete and the energy registers are ready to read. At 50 Hz this gives a hardware-synchronized 1-second update. Energy is accumulated as integers (mWh) rather than floats, avoiding drift over long runtimes.

Additional differences:

- **`reactive_power` sensor** — not available in the built-in component
- **`frequency` sensor** — not available in the built-in component
- **`total_power_gain` calibration field** — additional gain correction factor
- **Calibration verification** — every calibration register is read back after writing to confirm it was accepted
- **Watchdog** — if the chip stops producing IRQ0 pulses within `watchdog_threshold`, the driver reinitializes the chip; `failure_threshold` controls how many consecutive failures cause the component to give up

## Configuration

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/daniellinjama/esphome-components

ade7880_lcyc:
  id: energy_meter
  frequency: 50Hz
  irq0_pin: GPIO13
  irq1_pin: GPIO36
  reset_pin: GPIO16
  watchdog_threshold: 5s
  failure_threshold: 5
  phase_a:
    name: Phase A
    voltage:
      name: Voltage
    current:
      name: Current
    active_power:
      name: Active Power
    apparent_power:
      name: Apparent Power
    reactive_power:
      name: Reactive Power
    power_factor:
      name: Power Factor
    frequency:
      name: Frequency
    forward_active_energy:
      name: Forward Energy
    reverse_active_energy:
      name: Reverse Energy
    calibration:
      voltage_gain: 0
      current_gain: 0
      power_gain: 0
      phase_angle: 0
      total_power_gain: 0
  phase_b:
    # ...
  phase_c:
    # ...
  neutral:
    name: Neutral
    current:
      name: Current
    calibration:
      current_gain: 0
```

### Options

| Option | Default | Description |
|--------|---------|-------------|
| `frequency` | `50Hz` | Mains frequency (45–66 Hz). Used to set the line cycle count: `LINECYC = frequency × 2`. |
| `irq0_pin` | required | Interrupt pin connected to ADE7880 IRQ0. Fires when energy accumulation period completes. |
| `irq1_pin` | required | Interrupt pin connected to ADE7880 IRQ1. Used for reset detection. |
| `reset_pin` | optional | Hardware reset pin. If omitted, software reset is used. |
| `watchdog_threshold` | `5s` | Time without an IRQ0 pulse before triggering reinitialization. |
| `failure_threshold` | `5` | Number of consecutive initialization failures before the component marks itself failed. |
