
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ev3.h"
#include "ev3_sensor.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif


void searchSensors(uint8_t *sensorsIndex) //Save array with sensor addresses
{
    uint8_t sn;
    (ev3_search_sensor( LEGO_EV3_TOUCH, &sn, 0)) ? sensorsIndex[1] = sn : printf("Touch sensor (2) not attached");
    (ev3_search_sensor( LEGO_EV3_US, &sn, 0)) ? sensorsIndex[2] = sn : printf("Sonar sensor (3) not attached");
    (ev3_search_sensor( HT_NXT_COMPASS, &sn, 0)) ? sensorsIndex[0] = sn : printf("Compass sensor (1) not attached");

}

float getDistance(uint8_t sn)
{
    float value=-12345;
    if (!get_sensor_value0(sn, &value )) value = 0;
    return value;
}

float getCompass(uint8_t sn)
{
    float value;
    if ( !get_sensor_value0(sn, &value )) {
        value = 0;
      }
    return value;
}

int getCompassAngle(uint8_t sn, int init_compass, int lat){
        int val;
        if ( !get_sensor_value( 0, sn, &val ) ) {
            val = 0;
        }
        if (lat==0){  // 0 means turning to the left
            if (init_compass<val){
                return init_compass + 360 - val;
            }
            else {
                return init_compass-val;
            }
        }
        else {
            if (val<init_compass){
                return val + 360 - init_compass;
            }
            else {
                return val - init_compass;
            }
        }
}

int getTouch(uint8_t sn)
{
    uint32_t n, ii;
    int value;

    if ( get_sensor_num_values( sn, &n )) {
        for ( ii = 0; ii < n; ii++ ) {
        if ( !get_sensor_value( ii, sn, &value )) {
                value = 0;
        }
        }
    }
    return value;
}

