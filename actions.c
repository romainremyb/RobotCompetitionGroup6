#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ev3.h"    // think about cleaning up sleep time...
#include "ev3_port.h"
#include "ev3_tacho.h"
#include <pthread.h>

#include "motors.h"
#include "sensors.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

// scan from position (findBall)

typedef struct scanStruct{
    int threshold;
    uint8_t *sensors;
}scanStruct;

void *detectAball(void *arguments)
{   
    scanStruct *args  = (scanStruct*)arguments;
    int threshold=args->threshold;
    int *Sensors=args->sensors;

    int lastPosition;
    int currentT=0;
    int currentPosition;
    float diff;
    diff=10000000.0;
    lastPosition=getDistance(Sensors[2]);
    while (diff>threshold || currentT==0){
        currentT=getTouch(Sensors[1]);
        currentPosition=getDistance(Sensors[2]);
        diff=lastPosition-currentPosition;
        sleep(0.1);
    }
}

void findBall(int threshold, uint8_t *sensors, uint8_t *motors)
{
    int max_speed;
    set_tacho_ramp_up_sp( motors[1], 100);
    set_tacho_ramp_down_sp( motors[1], 100);    
    set_tacho_ramp_up_sp( motors[2], 100);
    set_tacho_ramp_down_sp( motors[2], 100); 
    get_tacho_max_speed( motors[1], &max_speed );
    set_tacho_speed_sp( motors[1], max_speed * 0.1 ); // fix speed
    get_tacho_max_speed( motors[2], &max_speed );
    set_tacho_speed_sp( motors[2], max_speed * 0.1 ); // fix speed
    set_tacho_polarity(motors[1], "normal");  //fix later
    set_tacho_polarity(motors[2], "inverted");

    scanStruct toThread[2];
    toThread->threshold=threshold;
    toThread->sensors=sensors;
    pthread_t tid;
    set_tacho_command_inx( motors[1], TACHO_RUN_FOREVER );    
    set_tacho_command_inx( motors[2], TACHO_RUN_FOREVER );
    pthread_create(&tid, NULL, detectAball, (void *)&threshold);
    (void) pthread_join(&tid, NULL);
    stopTacho(motors[1]);
    stopTacho(motors[2]);

}

// run to Ball

typedef struct runStruct{
    int minstop;
    uint8_t *sensors;
}runStruct;

void *threadRunStraight(void *arguments)
{
    runStruct *args  = (runStruct*)arguments;
    int minStop=args->minstop;
    int *Sensors=args->sensors;
    int currentT=0;
    int currentD;
    currentD=getDistance(Sensors[2]);
    while (currentD<=minStop || currentT==0){
        currentT=getTouch(Sensors[1]);
        currentD=getDistance(Sensors[2]);
        sleep(0.1);
    }
}


void runToBall(float speed, int minstop, uint8_t *sensors, uint8_t *motors)
{
    int max_speed;
    set_tacho_ramp_up_sp( motors[1], 100);
    set_tacho_ramp_down_sp( motors[1], 100);    
    set_tacho_ramp_up_sp( motors[2], 100);
    set_tacho_ramp_down_sp( motors[2], 100); 
    // polarity?
    get_tacho_max_speed( motors[1], &max_speed );
    set_tacho_speed_sp( motors[1], max_speed * speed );
    get_tacho_max_speed( motors[2], &max_speed );
    set_tacho_speed_sp( motors[2], max_speed * speed );
    set_tacho_command_inx( motors[1], TACHO_RUN_FOREVER );    
    set_tacho_command_inx( motors[2], TACHO_RUN_FOREVER );    

    runStruct toThread[2];
    toThread->minstop=minstop;
    toThread->sensors=sensors;
    //thread
    pthread_t tid;
    pthread_create(&tid, NULL, threadRunStraight, (void *)&toThread); 
    (void) pthread_join(&tid, NULL);
    stopTacho(motors[1]);
    stopTacho(motors[2]);

}


// estimate position in pitch

// run to special positions (5 + first shoot)

