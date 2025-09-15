import esphome.config_validation as cv

import re

TYPE_SENSOR = "sensor"
TYPE_BINARY_SENSOR = "binary_sensor"
TYPE_SWITCH = "switch"

class Topic:
  def __init__(self, type, name, decoder, encoder):
    self.type = type
    self.name = name
    self.decoder = decoder
    self.encoder = encoder

TOPICS = (
  Topic(TYPE_SWITCH, "Heatpump State", "main", "main"),
  Topic(TYPE_SENSOR, "Pump Flow", "main", "main"),
  Topic(TYPE_SWITCH, "Force DHW State", "main", "main"),
)

def get_topic(name):
  if isinstance(name, int):
    if name >= len(TOPICS):
      raise cv.Invalid(f"Topic {name} is not a valid topic")
    return TOPICS[name]
  elif isinstance(name, str):
    if re.match(r"^TOP\d+$", name):
      return get_topic(int(name[3:]))

    name_underline = name.replace(" ", "_")
    for topic in TOPICS:
      if topic.name == name or topic.name == name_underline:
        return topic
    raise cv.Invalid(f"Topic {name} is not a valid topic")
  else:
    raise cv.Invalid(f"Topic {name} is not a valid topic")


def generate_topic_mapping(enum):
  return {
    topic.name: getattr(enum, topic.name.replace(" ", ""))
    for topic in TOPICS
  }
