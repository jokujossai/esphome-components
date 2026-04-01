# Field definitions for Panasonic Aquarea
# Maps field names to their protocol and default display name

FIELD_REGISTRY = {
    # Main protocol fields
    # Byte 4
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
    # Byte 5
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
    # Byte 6
    "dhwModeState": {
        "protocol": "main",
        "name": "DHW Mode",
        "type": "switch",
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
    # Byte 7
    "quietModeSchedule": {
        "protocol": "main",
        "name": "Quiet Mode Schedule",
        "type": "binary_sensor",
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
    # Byte 8
    "forceDefrostCommand": {
        "protocol": "main",
        "name": "Force Defrost",
        "type": "switch",
    },
    "forceSterilizationCommand": {
        "protocol": "main",
        "name": "Force Sterilization",
        "type": "switch",
    },
    # Byte 9
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
    # Byte 20
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
    # Byte 22
    "z1SensorSettings": {
        "protocol": "main",
        "name": "Zone 1 Sensor Settings",
        "type": "select",
        "entity_category": "config",
        "options": {
            0: "Water temp",
            1: "External thermostat",
            2: "Internal thermostat",
            3: "Thermistor",
        },
    },
    "z2SensorSettings": {
        "protocol": "main",
        "name": "Zone 2 Sensor Settings",
        "type": "select",
        "entity_category": "config",
        "options": {
            0: "Water temp",
            1: "External thermostat",
            2: "Internal thermostat",
            3: "Thermistor",
        },
    },
    # Byte 23
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
    # Byte 24
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
            0: "No solar",        # 0b01 - 1
            1: "Solar buffer",    # 0b10 - 1
            2: "Solar DHW",       # 0b11 - 1
        },
    },
    # Byte 25
    "externalPadHeater": {
        "protocol": "main",
        "name": "External Pad Heater",
        "type": "select",
        "entity_category": "config",
        "options": {
            0: "No pad heater",   # 0b01 - 1
            1: "Type A",          # 0b10 - 1
            2: "Type B",          # 0b11 - 1
        },
    },
    # Byte 26
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
    # Byte 28
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
    # Byte 29
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
    # Byte 38-44
    "z1HeatRequestTemp": {
        "protocol": "main",
        "name": "Zone 1 Heat Request Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
        "min_value": -5, # Dynamic, compensation curve -5, direct 20-35 (depends on model)
        "max_value": 75, # Dynamic, compensation curve 5, direct 55-65 (depends on model)
        "step": 1,
    },
    "z1CoolRequestTemp": {
        "protocol": "main",
        "name": "Zone 1 Cool Request Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
        "min_value":-5, # Dynamic, compensation curve -5, direct 20
        "max_value":20, # Dynamic, compensation curve 5, direct 20
        "step": 1,
    },
    "z2HeatRequestTemp": {
        "protocol": "main",
        "name": "Zone 2 Heat Request Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
        "min_value":-5, # Dynamic, compensation curve -5, direct 20
        "max_value":75, # Dynamic, compensation curve 5, direct 75?
        "step": 1,
    },
    "z2CoolRequestTemp": {
        "protocol": "main",
        "name": "Zone 2 Cool Request Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
        "min_value":-5, # Dynamic, compensation curve -5, direct 5
        "max_value":20, # Dynamic, compensation curve 5, direct 20
        "step": 1,
    },
    "dhwTargetTemp": {
        "protocol": "main",
        "name": "DHW Target Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
        "min_value":40,
        "max_value":75,
        "step": 1,
    },
    "roomHolidayShiftTemp": {
        "protocol": "main",
        "name": "Room Holiday Shift Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
        "min_value":-15,
        "max_value":15,
        "step": 1,
    },
    "dhwHolidayShiftTemp": {
        "protocol": "main",
        "name": "DHW Holiday Shift Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "slider",
        "device_class": "temperature",
        "min_value":-15,
        "max_value":15,
        "step": 1,
    },
    # Byte 45
    "maxPumpDuty": {
        "protocol": "main",
        "name": "Max Pump Duty",
        "type": "number",
        "entity_category": "config",
        "mode": "number",
        "min_value":64,
        "max_value":254,
        "step": 1,
    },
    # Byte 58-70
    "bufferTankDelta": {
        "protocol": "main",
        "name": "Buffer Tank Delta",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "K",
        "mode": "number",
        "min_value":0,
        "max_value":10,
        "step": 1,
    },
    "solarOnDelta": {
        "protocol": "main",
        "name": "Solar On Delta",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°K",
        "mode": "number",
        "min_value":6,
        "max_value":15,
        "step": 1,
    },
    "solarOffDelta": {
        "protocol": "main",
        "name": "Solar Off Delta",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°K",
        "mode": "number",
        "min_value":2,
        "max_value":9,
        "step": 1,
    },
    "solarFrostProtection": {
        "protocol": "main",
        "name": "Solar Frost Protection",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-20,
        "max_value":10,
        "step": 1,
    },
    "solarHighLimit": {
        "protocol": "main",
        "name": "Solar High Limit",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":70,
        "max_value":90,
        "step": 1,
    },
    "bivalentStartTemp": {
        "protocol": "main",
        "name": "Bivalent Start Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-15,
        "max_value":35,
        "step": 1,
    },
    "bivalentAdvancedStartTemp": {
        "protocol": "main",
        "name": "Bivalent Advanced Start Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-10,
        "max_value":0,
        "step": 1,
    },
    "bivalentStartDelay": {
        "protocol": "main",
        "name": "Bivalent Start Delay",
        "type": "number",
        "entity_category": "config",
        "mode": "number",
        "unit_of_measurement": "min",
        "min_value":0,
        "max_value":90,
        "step": 5,
    },
    "bivalentAdvancedStopTemp": {
        "protocol": "main",
        "name": "Bivalent Advanced Stop Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-10,
        "max_value":0,
        "step": 1,
    },
    "bivalentStopDelay": {
        "protocol": "main",
        "name": "Bivalent Stop Delay",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "min",
        "mode": "number",
        "min_value":0,
        "max_value":90,
        "step": 5,
    },
    "bivalentAdvancedDhwDelay": {
        "protocol": "main",
        "name": "Bivalent Advanced DHW Delay",
        "type": "number",
        "entity_category": "config",
        "mode": "number",
        "unit_of_measurement": "min",
        "min_value":30,
        "max_value":90,
        "step": 5,
    },
    # Byte 75-98
    "z1HeatCurveTargetHighTemp": {
        "protocol": "main",
        "name": "Z1 Heat Curve Target High Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":20, # Depends on model
        "max_value":65, # Depends on model
        "step": 1,
    },
    "z1HeatCurveTargetLowTemp": {
        "protocol": "main",
        "name": "Z1 Heat Curve Target Low Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":20, # Depends on model
        "max_value":65, # Depends on model
        "step": 1,
    },
    "z1HeatCurveOutsideLowTemp": {
        "protocol": "main",
        "name": "Z1 Heat Curve Outside Low Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-15,
        "max_value":15,
        "step": 1,
    },
    "z1HeatCurveOutsideHighTemp": {
        "protocol": "main",
        "name": "Z1 Heat Curve Outside High Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-15,
        "max_value":15,
        "step": 1,
    },
    "z2HeatCurveTargetHighTemp": {
        "protocol": "main",
        "name": "Z2 Heat Curve Target High Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":20, # Depends on model
        "max_value":65, # Depends on model
        "step": 1,
    },
    "z2HeatCurveTargetLowTemp": {
        "protocol": "main",
        "name": "Z2 Heat Curve Target Low Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":20, # Depends on model
        "max_value":65, # Depends on model
        "step": 1,
    },
    "z2HeatCurveOutsideLowTemp": {
        "protocol": "main",
        "name": "Z2 Heat Curve Outside Low Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-15,
        "max_value":15,
        "step": 1,
    },
    "z2HeatCurveOutsideHighTemp": {
        "protocol": "main",
        "name": "Z2 Heat Curve Outside High Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-15,
        "max_value":15,
        "step": 1,
    },
    "heatingOffOutdoorTemp": {
        "protocol": "main",
        "name": "Heating Off Outdoor Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-5,
        "max_value":35,
        "step": 1,
    },
    "heatDelta": {
        "protocol": "main",
        "name": "Heat Delta",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "K",
        "mode": "number",
        "min_value":1,
        "max_value":15,
        "step": 1,
    },
    "heaterOnOutdoorTemp": {
        "protocol": "main",
        "name": "Heater On Outdoor Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":-15,
        "max_value":20,
        "step": 1,
    },
    "z1CoolCurveTargetHighTemp": {
        "protocol": "main",
        "name": "Z1 Cool Curve Target High Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":5,
        "max_value":20,
        "step": 1,
    },
    "z1CoolCurveTargetLowTemp": {
        "protocol": "main",
        "name": "Z1 Cool Curve Target Low Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":5,
        "max_value":20,
        "step": 1,
    },
    "z1CoolCurveOutsideLowTemp": {
        "protocol": "main",
        "name": "Z1 Cool Curve Outside Low Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":15,
        "max_value":30,
        "step": 1,
    },
    "z1CoolCurveOutsideHighTemp": {
        "protocol": "main",
        "name": "Z1 Cool Curve Outside High Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":15,
        "max_value":30,
        "step": 1,
    },
    "z2CoolCurveTargetHighTemp": {
        "protocol": "main",
        "name": "Z2 Cool Curve Target High Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":5,
        "max_value":20,
        "step": 1,
    },
    "z2CoolCurveTargetLowTemp": {
        "protocol": "main",
        "name": "Z2 Cool Curve Target Low Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":5,
        "max_value":20,
        "step": 1,
    },
    "z2CoolCurveOutsideLowTemp": {
        "protocol": "main",
        "name": "Z2 Cool Curve Outside Low Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":15,
        "max_value":30,
        "step": 1,
    },
    "z2CoolCurveOutsideHighTemp": {
        "protocol": "main",
        "name": "Z2 Cool Curve Outside High Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":15,
        "max_value":30,
        "step": 1,
    },
    "coolDelta": {
        "protocol": "main",
        "name": "Cool Delta",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "K",
        "mode": "number",
        "min_value":1,
        "max_value":15,
        "step": 1,
    },
    "heatToCoolTemp": {
        "protocol": "main",
        "name": "Heat To Cool Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":11,
        "max_value":25,
        "step": 1,
    },
    "coolToHeatTemp": {
        "protocol": "main",
        "name": "Cool To Heat Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":5,
        "max_value":14,
        "step": 1,
    },
    "dhwHeatDelta": {
        "protocol": "main",
        "name": "DHW Heat Delta",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "K",
        "mode": "number",
        "min_value":-12,
        "max_value":-2,
        "step": 1,
    },
    "sterilizationTemp": {
        "protocol": "main",
        "name": "Sterilization Temperature",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "°C",
        "mode": "number",
        "min_value":55,
        "max_value":65,
        "step": 1,
    },
    "sterilizationMaxTime": {
        "protocol": "main",
        "name": "Sterilization Max Time",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "min",
        "mode": "number",
        "min_value":5,
        "max_value":60,
        "step": 5,
    },
    "heaterDelayTime": {
        "protocol": "main",
        "name": "Heater Delay Time",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "min",
        "mode": "number",
        "min_value":1,
        "max_value":120,
        "step": 1,
    },
    "heaterStartDelta": {
        "protocol": "main",
        "name": "Heater Start Delta",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "K",
        "mode": "number",
        "min_value":-128,
        "max_value":127,
        "step": 1,
    },
    "heaterStopDelta": {
        "protocol": "main",
        "name": "Heater Stop Delta",
        "type": "number",
        "entity_category": "config",
        "unit_of_measurement": "K",
        "mode": "number",
        "min_value":-128,
        "max_value":127,
        "step": 1,
    },
    # Byte 110
    "quietModeActual": {
        "protocol": "main",
        "name": "Quiet Mode Actual",
        "type": "binary_sensor",
    },
    "powerfulModeActual": {
        "protocol": "main",
        "name": "Powerful Mode Actual",
        "type": "binary_sensor",
    },
    "heatCoolSwitchActual": {
        "protocol": "main",
        "name": "Heat Cool Switch Actual",
        "type": "text_sensor",
        "options": {
            0: "Heat",
            1: "Cool",
        }
    },
    "externalSwitchActual": {
        "protocol": "main",
        "name": "External Switch Actual",
        "type": "text_sensor",
        # TODO: Verify real values
        "options": {
            0: "Closed",
            1: "Open",
        }
    },
    # Byte 111
    "threeWayValveState": {
        "protocol": "main",
        "name": "Three Way Valve State",
        "type": "text_sensor",
        "options": {
            0: "Room",
            1: "DHW",
        }
    },
    "defrostingState": {
        "protocol": "main",
        "name": "Defrosting State",
        "type": "binary_sensor",
    },
    # Byte 112
    "internalHeaterState": {
        "protocol": "main",
        "name": "Internal Heater State",
        "type": "binary_sensor",
    },
    "externalHeaterState": {
        "protocol": "main",
        "name": "External Heater State",
        "type": "binary_sensor",
    },
    # Byte 113-114
    "errorCodeType": {
        "protocol": "main",
        "name": "Error Code Type",
        "type": "text_sensor",
        "options": {
            0x21: "No error (H)",
            0x31: "No error (F)",
            0xa1: "H",
            0xb1: "F",
        },
    },
    # TODO: Should be text_sensor
    "errorCodeNumber": {
        "protocol": "main",
        "name": "Error Code Number",
        "type": "sensor",
        "accuracy_decimals": 0,
    },
    "errorCode": {
        "protocol": "main",
        "name": "Error Code",
        "type": "text_sensor",
        "accuracy_decimals": 0,
        "options": {
            # H12
            0xa11d: "H12 Capacity mismatch",
            # H15
            0xa11f: "H15 Compressor sensor error",
            # H20
            0xa125: "H20 Pump error",
            # H23
            0xa128: "H23 Refrigerant sensor error",
            # H27
            0xa12c: "H27 Service valve error",
            # H28
            0xa12d: "H28 Solar sensor error",
            # H31
            0xa130: "H31 Pool sensor error",
            # H36
            0xa135: "H36 Buffer tank sensor error",
            # H38
            0xa137: "H38 Brand mismatch error",
            # H42
            0xa13b: "H42 Low pressure protection",
            # H43
            0xa13c: "H43 Zone 1 sensor error",
            # H44
            0xa13d: "H44 Zone 2 sensor error",
            # H62
            0xa14f: "H62 Water flow error",
            # H63
            0xa150: "H63 Low pressure sensor error",
            # H64
            0xa151: "H64 High pressure sensor error",
            # H65
            0xa152: "H65 Device water circulation error",
            # H67
            0xa154: "H67 External thermistor 1 error",
            # H68
            0xa155: "H68 External thermistor 2 error",
            # H70
            0xa157: "H70 Back-up heater OLP error",
            # H72
            0xa159: "H72 Tank sensor error",
            # H74
            0xa15b: "H74 OCB communication error",
            # H75
            0xa15c: "H75 Low water temp protection",
            # H76
            0xa15d: "H76 RC-Indoor communication error",
            # H90
            0xa16b: "H90 Indoor-Outdoor communication error",
            # H91
            0xa16c: "H91 Tank heater OLP error",
            # H95
            0xa170: "H95 Voltage connaction error",
            # H98
            0xa173: "H98 High pressure protection",
            # H99
            0xa174: "H99 Indoor freeze prevention",
            # F12
            0xb11d: "F12 Pressure switch activated",
            # F14
            0xb11f: "F14 Poor compressor rotation",
            # F15
            0xb120: "F15 Fan motor lock error",
            # F16
            0xb121: "F16 Current protection",
            # F20
            0xb125: "F20 Compressor overload protection",
            # F22
            0xb127: "F22 Transistor module overload protection",
            # F23
            0xb128: "F23 DC peak",
            # F24
            0xb129: "F24 Refrigerant cycle error",
            # F25
            0xb12a: "F25 Cool / heat cycle error",
            # F27
            0xb12c: "F27 Pressure switch error",
            # F29
            0xb12e: "F29 Low discharge super heat",
            # F30
            0xb12f: "F30 Water outlet sensor 2 error",
            # F32
            0xb131: "F32 Internal thermostat error",
            # F36
            0xb135: "F36 Outdoor ambient sensor error",
            # F40
            0xb139: "F40 Outdoor discharge sensor error",
            # F41
            0xb13a: "F41 Power factor correction error",
            # F42
            0xb13b: "F42 Outdoor heat exchanger sensor error",
            # F43
            0xb13c: "F43 Outdoor defrost sensor error",
            # F45
            0xb13e: "F45 Water outlet sensor error",
            # F46
            0xb13f: "F46 Current transformer disconnection",
            # F48
            0xb141: "F48 Evaporator outlet sensor error",
            # F49
            0xb142: "F49 Bypass outlet sensor error",
            # F95
            0xb170: "F95 Cooling high pressure error",
        },
    },

    # Byte 169-170
    "pumpFlow": {
        "protocol": "main",
        "name": "Pump Flow",
        "type": "sensor",
        "unit_of_measurement": "l/h",
        "state_class": "measurement",
        "device_class": "volume_flow_rate",
        "accuracy_decimals": 2,
    },
    # Optional PCB protocol fields
    # Read-only status fields (response packet 0x71/0x11/0x50)
    # Byte 4 - Water pumps and mixing valves
    "z1WaterPumpState": {
        "protocol": "optional",
        "name": "Z1 Water Pump",
        "type": "binary_sensor",
    },
    "z1MixingValveState": {
        "protocol": "optional",
        "name": "Z1 Mixing Valve",
        "type": "text_sensor",
        "options": {
            0: "Stop",
            1: "Close",
            2: "Open",
            3: "Unknown",
        },
    },
    "z2WaterPumpState": {
        "protocol": "optional",
        "name": "Z2 Water Pump",
        "type": "binary_sensor",
    },
    "z2MixingValveState": {
        "protocol": "optional",
        "name": "Z2 Mixing Valve",
        "type": "text_sensor",
        "options": {
            0: "Stop",
            1: "Close",
            2: "Open",
            3: "Unknown",
        },
    },
    "poolWaterPumpState": {
        "protocol": "optional",
        "name": "Pool Water Pump",
        "type": "binary_sensor",
    },
    "solarWaterPumpState": {
        "protocol": "optional",
        "name": "Solar Water Pump",
        "type": "binary_sensor",
    },
    # Byte 5 - Alarm
    "alarmState": {
        "protocol": "optional",
        "name": "Alarm",
        "type": "binary_sensor",
    },
    # Write-only command fields (command packet 0xF1/0x11/0x50)
    # Byte 6 - System control
    "heatCoolMode": {
        "protocol": "optional",
        "name": "Heat/Cool Mode",
        "type": "switch",
    },
    "compressorState": {
        "protocol": "optional",
        "name": "Compressor",
        "type": "switch",
    },
    "smartGridMode": {
        "protocol": "optional",
        "name": "Smart Grid Mode",
        "type": "select",
        "options": {
            0: "Normal",
            1: "Capacity 1",
            2: "Off",
            3: "Capacity 2",
        },
    },
    "externalThermostat1State": {
        "protocol": "optional",
        "name": "External Thermostat 1",
        "type": "select",
        "options": {
            0: "None",
            1: "Cool",
            2: "Heat",
            3: "Both",
        },
    },
    "externalThermostat2State": {
        "protocol": "optional",
        "name": "External Thermostat 2",
        "type": "select",
        "options": {
            0: "None",
            1: "Cool",
            2: "Heat",
            3: "Both",
        },
    },
    # Byte 7-16 - Temperature commands
    "demandControl": {
        "protocol": "optional",
        "name": "Demand Control",
        "type": "sensor",
        "accuracy_decimals": 0,
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