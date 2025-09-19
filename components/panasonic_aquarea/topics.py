import esphome.config_validation as cv

TYPE_SENSOR = "sensor"
TYPE_BINARY_SENSOR = "binary_sensor"
TYPE_SWITCH = "switch"
TYPE_SELECT = "select"
TYPE_NUMBER = "number"
TYPE_TEXT_SENSOR = "text_sensor"

class Topic:
  def __init__(self, type, name, decoder, encoder=None, *, unit_of_measurement=None, accuracy_decimals=None, state_class=None, options=None):
    self.type = type
    self.name = name
    self.decoder = decoder
    self.encoder = encoder
    self.unit_of_measurement = unit_of_measurement
    self.accuracy_decimals = accuracy_decimals
    self.state_class = state_class
    self.options = options

MAIN_TOPICS_START = 0
EXTRA_TOPICS_START = 9000
MAIN_TO_OPT_TOPICS_START = 10000
OPT_TO_MAIN_TOPICS_START = 11000
CUSTOM_TOPICS_START = 12000

OPERATING_MODE_OPTIONS = [
  "Heat only",
  "Cool only",
  "Auto(Heat)",
  "DHW only",
  "Heat+DHW",
  "Cool+DHW",
  "Auto(Heat)+DHW",
  "Auto(Cool)",
  "Auto(Cool)+DHW",
]

# Custom topic options following HeishaMon naming conventions
HEATING_MODE_OPTIONS = [
  "Off",
  "Heat",
  "Cool",
  "Auto",
]

TOPICS = {
  # State/Control Topics (TOP0-TOP7)
  "TOP0": Topic(TYPE_SWITCH, "Heatpump State", "main", "main"),
  "TOP1": Topic(TYPE_SENSOR, "Pump Flow", "main", unit_of_measurement="l/min", accuracy_decimals=2),
  "TOP2": Topic(TYPE_SWITCH, "Force DHW State", "main", "main"),
  "TOP3": Topic(TYPE_SWITCH, "Quiet Mode Schedule", "main", "main"),
  "TOP4": Topic(TYPE_SELECT, "Operation Mode State", "main", "main", options=OPERATING_MODE_OPTIONS),

  # Custom Topics (CUSTOM0-CUSTOM1) - Split from TOP4 for better control
  "CUSTOM0": Topic(TYPE_SWITCH, "DHW Mode State", "main", "main"),
  "CUSTOM1": Topic(TYPE_SELECT, "Heating Mode State", "main", "main", options=HEATING_MODE_OPTIONS),
}

def get_topic(name):
  if isinstance(name, int):
    if name >= CUSTOM_TOPICS_START:
      topic_name = "CUSTOM" + str(name - CUSTOM_TOPICS_START)
    elif name >= OPT_TO_MAIN_TOPICS_START:
      # Not defined on HeishaMon
      topic_name = "XOPT" + str(name - OPT_TO_MAIN_TOPICS_START)
    elif name >= MAIN_TO_OPT_TOPICS_START:
      topic_name = "OPT" + str(name - MAIN_TO_OPT_TOPICS_START)
    elif name >= EXTRA_TOPICS_START:
      topic_name = "XTOP" + str(name - EXTRA_TOPICS_START)
    else:
      topic_name = "TOP" + str(name)

    if topic_name not in TOPICS:
      raise cv.Invalid(f"Topic {topic_name} ({name}) is not a valid topic")
    return TOPICS[topic_name]
  elif isinstance(name, str):
    if name in TOPICS:
      return TOPICS[name]

    name_underline = name.replace(" ", "_")
    for topic in TOPICS.values():
      if topic.name == name or topic.name == name_underline:
        return topic
    raise cv.Invalid(f"Topic {name} is not a valid topic")
  else:
    raise cv.Invalid(f"Topic {name} is not a valid topic")


def generate_topic_mapping(enum):
  return {
    topic.name: getattr(enum, topic.name.replace(" ", ""))
    for topic in TOPICS.values()
  }
