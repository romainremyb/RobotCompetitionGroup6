mainToRun=maintest

all:
		gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensors.c -o f/sensors.o
		gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors.c -o f/motors.o
		gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c $(mainToRun).c -o f/$(mainToRun).o
		gcc f/sensors.o f/motors.o f/$(mainToRun).o -Wall -lm -lev3dev-c -lpthread -o f/main

main:
		gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c $(mainToRun).c -o $(mainToRun).o
		gcc f/sensors.o f/motors.o $(mainToRun).o -Wall -lm -lev3dev-c -lpthread -o f/main

motors:
		gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c motors.c -o motors.o
		gcc f/sensors.o f/motors.o $(mainToRun).o -Wall -lm -lev3dev-c -lpthread -o f/main

sensors:
		gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c sensors.c -o sensors.o
		gcc f/sensors.o f/motors.o $(mainToRun).o -Wall -lm -lev3dev-c -lpthread -o f/main