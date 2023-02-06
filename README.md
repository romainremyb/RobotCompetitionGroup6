# RobotCompetitionGroup6

This presents the code of the robot competition made in Eurecom for the OS course. Group 6 was a group of 3 people. Ali ALQETAITI built the robot. Yossi TAPIERO did the website and Romain Bourgeois did the code.

Our robot both grabs and throws the ball with its arm. A sonar sensor is placed on the side of the arm, this is the reason why the robot must move with the arm up. A touch sensor was also placed above the sonar one. The aim of this setup was to be able to detect if a close object (inference made from sonar) was a ball or a wall. A compass sensor was also placed on the robot to evaluate the robot’s position on the pitch as well as to control the angle while scanning the field to search for a ball. Unfortunately, that sensor was extremely unreliable, the ways the scanning function changed will be explained later.

The code is divided into the following files. “sensors.c” is made up of the primitive sensor related functions. “motors.c” brings together the functions using motors solely. “responseFunctions.ipynb” calculates the response functions of the positional attributes to both degree rotation and distance. “actions.c” functions are built on top of sensors.c and motors.c and contain the advanced routines that are going to be called in the main files. The building of the main attack and defense files will be explained later. 

“sensors.c”. 
These are the ones for saving sensor addresses (searchSensors), evaluating distance ahead (getDistance), getting the touch value (getTouch) and getting the compass value (getCompass). A final function (getCompassAngle) was responsible for extracting the angle that the robot makes in comparison to another compass value (aimed at being the value at ignition). 

“motors.c”. 
Before going to the rest of this file’s code it shall be noted that the motor actions could be controllers using position (rotational unit), time or stopping commands. Except when scanning (in action.c) the repo relies on position to control movements. Position commands could either be made in absolute terms, meaning the target position is reached at the end of the action. Or it could be made in “real terms”, meaning position at the end of the action equals initial position plus the target value placed in the command. 
Unfortunately, we ignored the restriction on the robot size, the code was hence built with the idea that the initial position of 0 of the arm would be when the arm touches the ground. Calibrations (in throwing and catching) could then only be made knowing that there was a consistency with the arm initial position…
“searchMotors” saves the addresses of the connected motors. getMotorPosition and armPositionToZero are self explanatory. armUp places the arm up with a certain amount, calibrated for catching the ball later. catchBall pushes down the arm to catch the ball and throwBall is self explanatory. stopTacho stops a motor from a “run forever” call. 
As a result of the unreliability of the compass motor to control rotational moves, multiple tests were made with different angles to estimate the position needed to accomplish a target angle with a linear regression. The regression and test data is available in “responseFunctions.ipynb”. The function is called rotate. 
We also thought that it was worth having a function that rides a desired distance (goDist). Tests were also completed and a linear model predicted in the jupyter notebook.

“actions.c”
The first function (findBall) rotates and asynchronously queries the distance and the touch sensor. The thread is a while loop of 0.1 seconds that calculates the difference between two successive sonar values. If the difference is above a threshold or the touch sensor detects a touch, the while loop breaks and the thread is finished. The “pthread_join” called breaks the loop and the motors are stopped. Unfortunately, the code could not be fixed on time, the compiler was not very explicit with the error, this issue is not a lock and does not seem to be struct parameterization of the thread…
This function aims at detecting a ball and is intended to be followed by runToBall. 
The latter function uses a thread in the same way as the above function. In this case, the while loop breaks when distance reaches a limit or a touch event. The limit is placed such that it is small enough to ensure that the object in the front is not a wall or another obstacle. This function aims to trigger the ball catch.
Because of the inability to compile and the lack of time remaining, the following functions could not be implemented. First, we wanted a function that uses the compass to estimate the position in the field. Another function was supposed to build on the inferred position to go to 5 different strategic positions in the pitch to start scanning with the above function. Other positions were planned to be used to throw the ball. function was responsible for going to the appropriate distance to trigger the catchBall function, it was meant to be called after runToBall.

Attacker main.
The above detailed reasons enabled the project to go as intended. The main file was supposed to go to the 5 positions saved in an array, findBall, runToBall, go to one of the shooting positions, throwBall, estimate position and so on…

The main file used to attack is named “mainAttack.c” and was modified quite a lot during the competitions and was made in a rush as attack.c could not be fixed. 

toDirection was aimed to replace findBall and goToBall instead of runToBall. We ignore if goToBall actually works because it was called after toDirection that seemed to rotate indefinitely and was unable to detect a ball. toDirection launches a “forever” rotation that breaks when the difference calculated in a while loop reaches a threshold. The issue observed during the competition could have been the selection of the threshold. goToBall ignites a “forever” move towards the target and breaks when detected distance reaches a threshold.

In the main, the robot goes to a hard-coded position and throws its initial ball. Then, it goes to a hard-coded position to scan the ball with (toDirection) and triggers (goToBall) which never happened during the contest. It then goes to a hard-coded throwing position to throw the ball…


Defender main.

The plan was to use two target positions to start scanning for the balls in the defensive area and throw them off. The robot would then either stay in the defensive area or walk back and forth in its zone. 

In the actual competition, many mains were used and we tried to make sure that the robot did not leave its zone. “defensefinal.c” displays one of the codes where the robot goes back and forth in its zone. 

Makefiles
“makefile” details the actual makefile used at the competition and “makefile2.txt” was the makefile with the “actions.c” file included.

