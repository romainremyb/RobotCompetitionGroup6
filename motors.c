#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ev3.h"    // think about cleaning up sleep time...
#include "ev3_port.h"
#include "ev3_tacho.h"
#include <pthread.h>


// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif


void searchMotors(uint8_t *motorsIndex)
{
    uint8_t sn;
    ( ev3_search_tacho_plugged_in( 68, 0, &sn, 0)) ? motorsIndex[0] = sn : printf("Arm Motor not attached");
    ( ev3_search_tacho_plugged_in( 66, 0, &sn, 0)) ? motorsIndex[1] = sn : printf("Left wheel not attached");
    ( ev3_search_tacho_plugged_in( 67, 0, &sn, 0)) ? motorsIndex[2] = sn : printf("Right wheel not attached");
    //Reset all the motors
    multi_set_tacho_command_inx(motorsIndex, TACHO_RESET);
    sleep(0.1);
    set_tacho_stop_action_inx( motorsIndex[0], TACHO_COAST);
    set_tacho_stop_action_inx( motorsIndex[1], TACHO_COAST);
    set_tacho_stop_action_inx( motorsIndex[2], TACHO_COAST);
    sleep(0.1);
}

int getMotorPosition(uint8_t sn, int *p)
{
    get_tacho_position_sp(sn,&p);
    sleep(0.05);
    return p;
}

void armPositionToZero(uint8_t sn, int *p)
{
    int max_speed;
    set_tacho_ramp_up_sp( sn, 100);
    set_tacho_ramp_down_sp( sn, 100);
    p=getMotorPosition(sn, &p);
    if (p>0){
        set_tacho_polarity(sn, "inverted");
        get_tacho_max_speed( sn, &max_speed );
        set_tacho_speed_sp( sn, max_speed * 1 / 8 );
        set_tacho_position_sp( sn, 0 );
        set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );
        sleep(1.5); // perhaps change it
    }
    else if (p>=0){
        set_tacho_polarity(sn, "normal");
        get_tacho_max_speed( sn, &max_speed );
        set_tacho_speed_sp( sn, max_speed * 1 / 8 );
        set_tacho_position_sp( sn, 0 );
        set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );
        sleep(1.5); // perhaps change it
    }
    get_tacho_position_sp(sn,&p);
}


void armUp(uint8_t sn, int *p, int level) // use motor index
{
    int max_speed;
    //get_tacho_position_sp(sn,&p);
    set_tacho_ramp_up_sp( sn, 0);
    set_tacho_ramp_down_sp( sn, 0);
    set_tacho_polarity(sn, "normal");
    get_tacho_max_speed( sn, &max_speed );
    set_tacho_speed_sp( sn, max_speed * 1.5 / 8 );
    set_tacho_position_sp( sn, level );  
    set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );
    sleep(1.5);
    get_tacho_position_sp(sn,&p);
}


void catchBall(uint8_t sn, int *p)
{
        int max_speed;
        set_tacho_ramp_up_sp( sn, 100);
        set_tacho_ramp_down_sp( sn, 100);
        set_tacho_polarity(sn, "inverted");
        get_tacho_max_speed( sn, &max_speed );
        set_tacho_speed_sp( sn, max_speed * 1 / 8 );
        set_tacho_position_sp( sn, -70 );//
        set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );//
        sleep(0.7);
        set_tacho_position_sp( sn, -95 );
        set_tacho_speed_sp( sn, max_speed * 5 / 8 );
        set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );
        sleep(0.5);
        set_tacho_position_sp( sn, -105 );
        set_tacho_speed_sp( sn, max_speed * 2 / 8 );
        set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );
        sleep(1);
        printf("position is %d \n",getMotorPosition(sn,&p));
        set_tacho_polarity(sn, "normal");
        set_tacho_speed_sp( sn, max_speed * 1.8 / 8 );
        set_tacho_position_sp( sn, 75 );
        set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );
        sleep(1.5);
        get_tacho_position_sp(sn,&p);
}


void throwBall(uint8_t sn, int *p) // middle zone
{
    int max_speed;
    set_tacho_ramp_up_sp( sn, 100);
    set_tacho_ramp_down_sp( sn, 100);
    set_tacho_polarity(sn, "inverted");
    get_tacho_max_speed( sn, &max_speed );
    set_tacho_speed_sp( sn, max_speed * 0.5 / 8 );
    set_tacho_position_sp( sn, -65 );
    sleep(1);
    set_tacho_command_inx( sn, TACHO_RUN_TO_ABS_POS );
    sleep(1.3);
    set_tacho_polarity(sn, "inverted");
    set_tacho_position_sp(sn, 40 );
    set_tacho_speed_sp(sn, max_speed * 6.8 / 8 );
    set_tacho_command_inx(sn, TACHO_RUN_TO_ABS_POS );
    sleep(2);
    get_tacho_position_sp(sn,&p);
}

// stop run forever
void stopTacho(uint8_t sn)
{
    set_tacho_command_inx( sn, TACHO_STOP );
}

void rotate(uint8_t *motors, int degree, int lat) // lat->0 turn left, 1 turns right
{
    int max_speed, rots;
    float predictRots;
    predictRots=12.5 +2.34*degree;
    rots=(int)predictRots;
    set_tacho_ramp_up_sp(motors[1], 100);
    set_tacho_ramp_down_sp(motors[1], 100);    
    set_tacho_ramp_up_sp(motors[2], 100);
    set_tacho_ramp_down_sp(motors[2], 100); 
    get_tacho_max_speed(motors[1], &max_speed );
    set_tacho_speed_sp(motors[1], max_speed * 0.15 ); // fix speed
    get_tacho_max_speed(motors[2], &max_speed );
    set_tacho_speed_sp(motors[2], max_speed * 0.15 );

    if (lat==0){
        set_tacho_polarity(motors[1], "normal");  //gauche in this config
        set_tacho_polarity(motors[2], "inversed");
    }
    else {
        set_tacho_polarity(motors[1], "inversed");  //droite in this config
        set_tacho_polarity(motors[2], "normal");
    }
    set_tacho_position_sp(motors[1], rots );
    set_tacho_position_sp(motors[2], rots );
    set_tacho_command_inx(motors[1], TACHO_RUN_TO_REL_POS );    
    set_tacho_command_inx(motors[2], TACHO_RUN_TO_REL_POS );
}

void goDist(uint8_t *motors, int distance, int i) //i->0 to go backwards and 1 forward
{
    int max_speed, rots;
    float predictRots;
    predictRots=-6.65 +20.94*distance;
    rots=(int)predictRots;
    set_tacho_ramp_up_sp(motors[1], 100);
    set_tacho_ramp_down_sp(motors[1], 100);    
    set_tacho_ramp_up_sp(motors[2], 100);
    set_tacho_ramp_down_sp(motors[2], 100); 
    get_tacho_max_speed(motors[1], &max_speed );
    set_tacho_speed_sp(motors[1], max_speed * 0.2005 ); // fix speed
    get_tacho_max_speed(motors[2], &max_speed );
    set_tacho_speed_sp(motors[2], max_speed * 0.2 );

    if (i==0){
        set_tacho_polarity(motors[1], "normal");  //gauche in this config
        set_tacho_polarity(motors[2], "normal");
    }
    else {
        set_tacho_polarity(motors[1], "inversed");  //droite in this config
        set_tacho_polarity(motors[2], "inversed");
    }
    set_tacho_position_sp(motors[1], rots );
    set_tacho_position_sp(motors[2], rots );
    set_tacho_command_inx(motors[1], TACHO_RUN_TO_REL_POS );    
    set_tacho_command_inx(motors[2], TACHO_RUN_TO_REL_POS );
}

