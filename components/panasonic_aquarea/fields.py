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
            2: "Heat",        # 0b0010 (4 LSB of 0b010010)
            3: "Cool",        # 0b0011 (4 LSB of 0b010011)
            9: "Auto(Heat)",  # 0b1001 (4 LSB of 0b011001)
            10: "Auto(Cool)", # 0b1010 (4 LSB of 0b011010)
        },
    },
    "zonesState": {
        "protocol": "main",
        "name": "Zones State",
        "type": "select",
        "options": {
            1: "Zone1",           # 0b01
            2: "Zone2",           # 0b10
            3: "Zone1+Zone2",     # 0b11
        },
    },
    "powerfulModeTime": {
        "protocol": "main",
        "name": "Powerful Mode Time",
        "type": "select",
        "options": {
            1: "Off",
            2: "30min",
            3: "60min",
            4: "90min",
        },
    },
    "quietModeLevel": {
        "protocol": "main",
        "name": "Quiet Mode Level",
        "type": "select",
        "options": {
            9: "Off",        # 0b01001
            10: "Level 1",   # 0b01010
            11: "Level 2",   # 0b01011
            12: "Level 3",   # 0b01100
            17: "Scheduled", # 0b10001
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
        "options": {
            1: "Compensation Curve",  # 0b01
            2: "Direct",              # 0b10
        },
    },
    "coolingMode": {
        "protocol": "main",
        "name": "Cooling Mode",
        "type": "select",
        "options": {
            1: "Compensation Curve",  # 0b01
            2: "Direct",              # 0b10
        },
    },
    "pumpFlowrateMode": {
        "protocol": "main",
        "name": "Pump Flowrate Mode",
        "type": "select",
        "options": {
            1: "Delta T",         # 0b01
            2: "Max flow",        # 0b10
        },
    },
    "z1HeatRequestTemp": {
        "protocol": "main",
        "name": "Zone 1 Heat Request Temperature",
        "type": "sensor",
    },
    "z1CoolRequestTemp": {
        "protocol": "main",
        "name": "Zone 1 Cool Request Temperature",
        "type": "sensor",
    },
    "z2HeatRequestTemp": {
        "protocol": "main",
        "name": "Zone 2 Heat Request Temperature",
        "type": "sensor",
    },
    "z2CoolRequestTemp": {
        "protocol": "main",
        "name": "Zone 2 Cool Request Temperature",
        "type": "sensor",
    },
    "dhwTargetTemp": {
        "protocol": "main",
        "name": "DHW Target Temperature",
        "type": "sensor",
    },
    "pumpFlow": {
        "protocol": "main",
        "name": "Pump Flow",
        "type": "sensor",
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