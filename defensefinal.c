#include <stdio.h>
#include "ev3.h"
#include "ev3_sensor.h"
#include <pthread.h>

#include "motors.h"
#include "sensors.h"
#include "actions.h"


// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

uint8_t motors[3], sensors[3];
int p;

void main()
{
    if (ev3_init() == -1 ) return (1);          //Init ev3
    while (ev3_tacho_init() < 1) Sleep(1000);   //Init tacho motors
    searchMotors(motors);
    ev3_sensor_init();                          //Init sensors
    searchSensors(sensors);
    goDist(motors,40,0);
    sleep(0.5);
    goDist(motors,80,1);
    int i=2;
    for (;;){
        if (i%2==0){
            goDist(motors,80,0);
            i=i+1;
        }
        else {
            goDist(motors,80,1);
            i=i+1;
        }
    }
}