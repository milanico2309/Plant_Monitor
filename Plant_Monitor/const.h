/************************************************************************/
/** Do NOT edit anything beyond this point (unless u know what you do) **/
/************************************************************************/

#ifndef SOILMANAGER_CONST_H
#define SOILMANAGER_CONST_H

enum sensorDataArrayOffsets{
 VALUE = 0,
 PIN = 1,
 CALIBRATED_MIN = 2,
 CALIBRATED_MAX = 3
};

 enum TimeConversionFactorMillis{
  F_INSTANT = 0,
  F_MILLIS = 1,
  F_SECONDS = 1000,
  F_MINUTES = 60000,
  F_HOURS = 3600000
 };

#endif //SOILMANAGER_CONST_H