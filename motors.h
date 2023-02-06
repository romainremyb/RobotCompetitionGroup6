#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"


void searchMotors(uint8_t *motorsIndex);
int getMotorPosition(uint8_t sn, int *p);
void armPositionToZero(uint8_t sn, int *p);
void armUp(uint8_t sn, int *p, int level);
void catchBall(uint8_t sn, int *p);
void throwBall(uint8_t sn, int *p);
void stopTacho(uint8_t sn);
void rotate(uint8_t *motors, int degree, int lat);
void goDist(uint8_t *motors, int distance, int i);

