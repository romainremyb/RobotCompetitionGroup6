#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ev3.h"
#include "ev3_sensor.h"
#include "ev3_port.h"

void searchSensors(uint8_t *sensorsIndex);
float getDistance(uint8_t sn);
float getCompass(uint8_t sn);
int getCompassAngle(uint8_t sn, int init_compass, int lat);
int getTouch(uint8_t sn);

