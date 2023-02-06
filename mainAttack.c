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

void toDirection(uint8_t *motors, uint8_t *sensors)
{
    int max_speed;
    rotate(motors, 30,0); // go to left before scanning
    // init rotation
    set_tacho_ramp_up_sp(motors[1], 100);
    set_tacho_ramp_down_sp(motors[1], 100);    
    set_tacho_ramp_up_sp(motors[2], 100);
    set_tacho_ramp_down_sp(motors[2], 100); 
    get_tacho_max_speed(motors[1], &max_speed );
    set_tacho_speed_sp(motors[1], max_speed * 0.05 ); // fix speed
    get_tacho_max_speed(motors[2], &max_speed );
    set_tacho_speed_sp(motors[2], max_speed * 0.05 );
    set_tacho_polarity(motors[1], "inversed");  //droite in this config
    set_tacho_polarity(motors[2], "normal");
    set_tacho_command_inx(motors[1], TACHO_RUN_FOREVER );    
    set_tacho_command_inx(motors[2], TACHO_RUN_FOREVER );
    int breakk=0;
    float currentDist=getDistance(sensors[2]);
    float previousDist;
    float diff=0;
    while (breakk==0){
        sleep(0.1);
        previousDist=currentDist;
        currentDist=getDistance(sensors[2]);
        diff=previousDist-currentDist;
        if (diff>688128){
            set_tacho_command_inx(motors[1], TACHO_STOP );    
            set_tacho_command_inx(motors[2], TACHO_STOP );         
            breakk=1;
        }
    }
}

void goToBall(uint8_t *motors, uint8_t *sensors)
{
    // launch motors
    int max_speed;
    set_tacho_ramp_up_sp(motors[1], 100);
    set_tacho_ramp_down_sp(motors[1], 100);    
    set_tacho_ramp_up_sp(motors[2], 100);
    set_tacho_ramp_down_sp(motors[2], 100); 
    get_tacho_max_speed(motors[1], &max_speed );
    set_tacho_speed_sp(motors[1], max_speed * 0.05 ); // fix speed
    get_tacho_max_speed(motors[2], &max_speed );
    set_tacho_speed_sp(motors[2], max_speed * 0.05 );
    set_tacho_polarity(motors[1], "inversed");  //droite in this config
    set_tacho_polarity(motors[2], "normal");
    set_tacho_command_inx(motors[1], TACHO_RUN_FOREVER );    
    set_tacho_command_inx(motors[2], TACHO_RUN_FOREVER );
    float distance;
    int breakk=0;
    while (breakk==0){
        sleep(0.1);
        distance=getDistance(sensors[2]);
        if (distance<=1079721984){
            set_tacho_command_inx(motors[1], TACHO_STOP );    
            set_tacho_command_inx(motors[2], TACHO_STOP );         
            breakk=1;       
        }
    }
}

int main()
{
    if (ev3_init() == -1 ) return (1);          //Init ev3
    while (ev3_tacho_init() < 1) Sleep(1000);   //Init tacho motors
    searchMotors(motors);
    ev3_sensor_init();                          //Init sensors
    searchSensors(sensors);
    armToZero(motors[0],&p);
    sleep(2);
    //armUp(motors[0],&p,85);
    sleep(5);
    goDist(motors,115,0);
    sleep(6);
    //rotate(motors, 180,0);
    sleep(2);
    throwBall(motors[0],&p);
    sleep(2);
    rotate(motors, 30,1);
    sleep(2);
    goDist(motors,50,1);
    sleep(2);
    toDirection(motors, sensors);
    sleep(2);
    goToBall(motors, sensors);
    sleep(2);
    catchBall(motors[0], &p);
    // go shoot
    rotate(motors, 160,1);
    sleep(2);
    goDist(motors,50,1);
    throwBall(motors[0],&p);  

    return 0;

}