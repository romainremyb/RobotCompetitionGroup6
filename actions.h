#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "sensors.h"
#include "motors.h"


void *threadRunStraight(void *arguments);
void runToBall(float speed, int minstop, uint8_t *sensors, uint8_t *motors);
void *detectAball(void *arguments);
void findBall(int threshold, uint8_t *sensors, uint8_t *motors);

