# Field definitions for Panasonic Aquarea
# Maps field names to their protocol and default display name

FIELD_REGISTRY = {
    # Main protocol fields
    "heatpumpState": {
        "protocol": "main",
        "name": "Heat Pump State",
        "type": "binary_sensor",  # or "switch" for writable
    },
    "forceDhwState": {
        "protocol": "main",
        "name": "Force DHW",
        "type": "switch",
    },
    "quietModeSchedule": {
        "protocol": "main",
        "name": "Quiet Mode Schedule",
        "type": "binary_sensor",
    },
    "mainScheduleState": {
        "protocol": "main",
        "name": "Main Schedule",
        "type": "binary_sensor",
    },
    "holidayModeState": {
        "protocol": "main",
        "name": "Holiday Mode",
        "type": "select",
        "options": {
            0: "Off",
            1: "Scheduled",
            2: "Active",
        },
    },
    "forceHeaterState": {
        "protocol": "main",
        "name": "Force Heater",
        "type": "binary_sensor",
    },
    "operatingModeState": {
        "protocol": "main",
        "name": "Operating Mode",
        "type": "select",
        "options": {
            0: "Off",        # 0b0001 - 1
            1: "Heat",       # 0b0010 - 1
            2: "Cool",       # 0b0011 - 1
            8: "Auto(Heat)", # 0b1001 - 1
            9: "Auto(Cool)", # 0b1010 - 1
        },
    },
    "dhwModeState": {
        "protocol": "main",
        "name": "DHW Mode",
        "type": "switch",
    },
    "zonesState": {
        "protocol": "main",
        "name": "Zones State",
        "type": "select",
        "entity_category": "config",
        "options": {
            0: "Zone1",           # 0b01 - 1
            1: "Zone2",           # 0b10 - 1
            2: "Zone1+Zone2",     # 0b11 - 1
        },
    },
    "powerfulModeTime": {
        "protocol": "main",
        "name": "Powerful Mode Time",
        "type": "select",
        "options": {
            0: "Off",
            1: "30min",
            2: "60min",
            3: "90min",
        },
    },
    "quietModeLevel": {
        "protocol": "main",
        "name": "Quiet Mode Level",
        "type": "select",
        "options": {
            0: "Off",
            1: "Level 1",
            2: "Level 2",
            3: "Level 3",
        },
    },
    "dhwHeaterState": {
        "protocol": "main",
        "name": "DHW Heater",
        "type": "binary_sensor",
    },
    "roomHeaterState": {
        "protocol": "main",
        "name": "Room Heater",
        "type": "binary_sensor",
    },
    "liquidType": {
        "protocol": "main",
        "name": "Liquid Type",
        "type": "binary_sensor",
        "entity_category": "config",
        "options": {
            0: "Water",
            1: "Glycol",
        },
    },
    "altExternalSensor": {
        "protocol": "main",
        "name": "Alternative External Sensor",
        "type": "switch",
    },
    "antiFreeze": {
        "protocol": "main",
        "name": "Anti Freeze",
        "type": "binary_sensor",
    },
    "optionalPcb": {
        "protocol": "main",
        "name": "Optional PCB",
        "type": "binary_sensor",
    },
    "z1SensorSettings": {
        "protocol": "main",
        "name": "Zone 1 Sensor Settings",
        "type": "select",
        "entity_category": "config",
        "options": {
            1: "Water temp",
            2: "External thermostat",
            3: "Internal thermostat",
            4: "Thermistor",
        },
    },
    "z2SensorSettings": {
        "protocol": "main",
        "name": "Zone 2 Sensor Settings",
        "type": "select",
        "entity_category": "config",
        "options": {
            1: "Water temp",
            2: "External thermostat",
            3: "Internal thermostat",
            4: "Thermistor",
        },
    },
    "externalControl": {
        "protocol": "main",
        "name": "External Control",
        "type": "binary_sensor",
    },
    "externalHeatCoolControl": {
        "protocol": "main",
        "name": "External Heat/Cool Control",
        "type": "binary_sensor",
    },
    "externalErrorSignal": {
        "protocol": "main",
        "name": "External Error Signal",
        "type": "binary_sensor",
    },
    "externalCompressorControl": {
        "protocol": "main",
        "name": "External Compressor Control",
        "type": "binary_sensor",
    },
    "bufferInstalled": {
        "protocol": "main",
        "name": "Buffer Installed",
        "type": "switch",
    },
    "dhwInstalled": {
        "protocol": "main",
        "name": "DHW Installed",
        "type": "binary_sensor",
    },
    "solarMode": {
        "protocol": "main",
        "name": "Solar Mode",
        "type": "select",
        "entity_category": "config",
        "options": {
            1: "No solar",        # 0b01
            2: "Solar buffer",    # 0b10
            3: "Solar DHW",       # 0b11
        },
    },
    "externalPadHeater": {
        "protocol": "main",
        "name": "External Pad Heater",
        "type": "select",
        "entity_category": "config",
        "options": {
            1: "No pad heater",   # 0b01
            2: "Type A",          # 0b10
            3: "Type B",          # 0b11
        },
    },
    "bivalentControl": {
        "protocol": "main",
        "name": "Bivalent Control",
        "type": "switch",
    },
    "bivalentMode": {
        "protocol": "main",
        "name": "Bivalent Mode",
        "type": "select",
        "entity_category": "config",
        "options": {
            0: "Alternative",
            1: "Parallel",
            2: "Advanced",
        },
    },
    "bivalentAdvancedHeat": {
        "protocol": "main",
        "name": "Bivalent Advanced Heat",
        "type": "binary_sensor",
    },
    "bivalentAdvancedDhw": {
        "protocol": "main",
        "name": "Bivalent Advanced DHW",
        "type": "binary_sensor",
    },
    "heatingMode": {
        "protocol": "main",
        "name": "Heating Mode",
        "type": "select",
        "entity_category": "config",
        "options": {
            0: "Compensation Curve",  # 0b01 - 1
            1: "Direct",              # 0b10 - 1
        },
    },
    "coolingMode": {
        "protocol": "main",
        "name": "Cooling Mode",
        "type": "select",
        "entity_category": "config",
        "options": {
            0: "Compensation Curve",  # 0b01 - 1
            1: "Direct",              # 0b10 - 1
        },
    },
    "pumpFlowrateMode": {
        "protocol": "main",
        "name": "Pump Flowrate Mode",
        "type": "select",
        "entity_category": "config",
        "options": {
            0: "Delta T",         # 0b01 - 1
            1: "Max flow",        # 0b10 - 1
        },
    },
    "z1HeatRequestTemp": {
        "protocol": "main",
        "name": "Zone 1 Heat Request Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",

    },
    "z1CoolRequestTemp": {
        "protocol": "main",
        "name": "Zone 1 Cool Request Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
    },
    "z2HeatRequestTemp": {
        "protocol": "main",
        "name": "Zone 2 Heat Request Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
    },
    "z2CoolRequestTemp": {
        "protocol": "main",
        "name": "Zone 2 Cool Request Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
    },
    "dhwTargetTemp": {
        "protocol": "main",
        "name": "DHW Target Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
    },
    "pumpFlow": {
        "protocol": "main",
        "name": "Pump Flow",
        "type": "sensor",
        "unit_of_measurement": "l/h",
        "state_class": "measurement",
        "device_class": "volume_flow_rate",
        "accuracy_decimals": 2,
    },
}


def get_field(field_name):
    """Get field metadata by name."""
    if field_name not in FIELD_REGISTRY:
        raise ValueError(f"Unknown field: {field_name}")
    return FIELD_REGISTRY[field_name]


def validate_field(field_name, expected_type=None):
    """Validate that a field exists and optionally check its type."""
    field = get_field(field_name)
    if expected_type and field["type"] != expected_type:
        raise ValueError(
            f"Field {field_name} is of type {field['type']}, expected {expected_type}"
        )
    return field