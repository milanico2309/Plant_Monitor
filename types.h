#include "Arduino.h"
#ifndef PLANTMONITOR_TYPES_H
#define PLANTMONITOR_TYPES_H

struct SensorValue{
  int id;
  String friendlyName;
  int value;
};

#endif //PLANTMONITOR_TYPES_H